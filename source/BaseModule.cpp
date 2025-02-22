#include "BaseModule.h"

BaseModule::BaseModule(unsigned uMaxInputBufferSize) : 
    m_uMaxInputBufferSize(uMaxInputBufferSize),
    m_cvDataInBuffer(),
    m_cbBaseChunkBuffer(),
    m_pNextModule(nullptr),
    m_bShutDown()
{
}

BaseModule::~BaseModule()
{
    // Stop all processing
    m_bShutDown = true;

    // Then we can try close the threads
    if(m_thread.joinable())
        m_thread.join();
}

void BaseModule::ContinuouslyTryProcess()
{

    while (!m_bShutDown)
    {
        std::shared_ptr<BaseChunk> pBaseChunk;
        if (TakeFromBuffer(pBaseChunk))
            CallChunkCallbackFunction(pBaseChunk);
        else
        {
            // Wait to be notified that there is data available
            std::unique_lock<std::mutex> BufferAccessLock(m_BufferStateMutex);
            m_cvDataInBuffer.wait_for(BufferAccessLock, std::chrono::milliseconds(1),  [this] {return (!m_cbBaseChunkBuffer.empty() || m_bShutDown);});
        }
    }
}

void BaseModule::StartProcessing()
{
    if (!m_thread.joinable())
    {
        m_thread = std::thread([this]()
            { ContinuouslyTryProcess(); });

        std::string strInfo = std::string(__FUNCTION__) + " " + GetModuleType() + ": Processing thread started";
        PLOG_WARNING << strInfo;
    }  
    else
    {
        // Log warning
        std::string strWarning = std::string(__FUNCTION__) + ": Processing thread already started";
        PLOG_WARNING << strWarning;
    }
}

void BaseModule::StopProcessing()
{
    m_bShutDown = true;

    while (!m_thread.joinable())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        PLOG_INFO << "Waiting for " + GetModuleType() + " thread to shut down";
    }

    // Now we close out the thread and prepare to restart it
    m_thread.join();
    m_bShutDown = false;
}

void BaseModule::SetNextModule(std::shared_ptr<BaseModule> pNextModule)
{
    m_pNextModule = pNextModule;
}

bool BaseModule::TryPassChunk(const std::shared_ptr<BaseChunk> &pBaseChunk)
{
    // Lets first check we are doing timing debugging
    if (m_bTrackProcessTime)
    {
        auto duration = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(m_CurrentTrackingTime - m_PreviousTimeTracking).count());
        m_PreviousTimeTracking = m_CurrentTrackingTime;
        m_CurrentTrackingTime = std::chrono::high_resolution_clock::now();

        std::string strInfo = std::string(__FUNCTION__) + m_sTrackerMessage + ": Time between passing chunks = " + duration + "us";
        PLOG_INFO << strInfo;  
    }
       
    // Allow module that one is passing to to facilitate its own locking procedures
    bool bReturnSuccessful = false;

    if (m_pNextModule != nullptr)
        bReturnSuccessful = m_pNextModule->TakeChunkFromModule(std::move(pBaseChunk));

    return bReturnSuccessful;
}

bool BaseModule::TakeChunkFromModule(const std::shared_ptr<BaseChunk> &pBaseChunk)
{
    // Maintain lock to prevent another module trying to access buffer
    // in the multi input buffer configuration
    std::unique_lock<std::mutex> BufferStateLock(m_BufferStateMutex);

   
    bool bBufferHasSpace = m_cbBaseChunkBuffer.size() < m_uMaxInputBufferSize;
    bool bChunkPassed = false;
    bool bLogBufferFull = !bBufferHasSpace && !m_bAlreadyLoggedBufferFull;

    // Check if next module queue is full
    if (bBufferHasSpace)
    {
        m_cbBaseChunkBuffer.push(std::move(pBaseChunk));
        
        m_bAlreadyLoggedBufferFull = false;
        bChunkPassed = true;
    }
    else if(bLogBufferFull)
    {
        m_bAlreadyLoggedBufferFull = true;
        PLOG_WARNING << GetModuleType() + " Unable to accept chunk, queue full";
    }
        
    // Then release and tell everyone we done
    BufferStateLock.unlock();
    m_cvDataInBuffer.notify_all();
    return bChunkPassed;
}

bool BaseModule::TakeFromBuffer(std::shared_ptr<BaseChunk> &pBaseChunk)
{
    // Maintain lock to prevent another module trying to access buffer
    // in the multi input buffer configuration
    std::lock_guard<std::mutex> BufferStateLock(m_BufferStateMutex);

    if (!m_cbBaseChunkBuffer.empty())
    {
        pBaseChunk = std::move(m_cbBaseChunkBuffer.front());
        m_cbBaseChunkBuffer.pop();
        return true;
    }

    return false;
}

void BaseModule::TrackProcessTime(bool bTrackTime, std::string sTrackerMessage)
{
    // Update tracking states
    m_sTrackerMessage = sTrackerMessage;
    m_bTrackProcessTime = bTrackTime;
    // Then start the timers
    m_PreviousTimeTracking = std::chrono::high_resolution_clock::now();
    m_CurrentTrackingTime = std::chrono::high_resolution_clock::now();
}

void BaseModule::StartReporting()
{
    m_QueueSizeReportingThread = std::thread([this]()
            { StartReportingLoop(); });
}

void BaseModule::SetReportingDescriptors(std::string strReportingJsonRoot, std::string strReportingJsonModuleAddition)
{
    m_strReportingJsonRoot = strReportingJsonRoot;
    m_strReportingJsonModuleAddition = strReportingJsonModuleAddition;
}

void BaseModule::StartReportingLoop()
{
    while (!m_bShutDown)
    {

        // Lets start by generating Queue stat
        std::unique_lock<std::mutex> BufferAccessLock(m_BufferStateMutex);
        uint16_t u16CurrentBufferSize = m_cbBaseChunkBuffer.size();
        auto strModuleName = GetModuleType();
        BufferAccessLock.unlock();

        nlohmann::json j = {
            {m_strReportingJsonRoot, {
                { strModuleName  + "_"+ m_strReportingJsonModuleAddition, {  // Extra `{}` around key-value pairs
                    {"QueueLength", std::to_string(u16CurrentBufferSize)}
                }}
            }}
        };

        // Then transmit
        auto pJSONChunk = std::make_shared<JSONChunk>();
        pJSONChunk->m_JSONDocument = j;
        CallChunkCallbackFunction(pJSONChunk);

        // And sleep as not to send too many
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void BaseModule::CallChunkCallbackFunction(std::shared_ptr<BaseChunk> pBaseChunk)
{
    std::unique_lock<std::mutex> BufferAccessLock(m_FunctionCallbackMapMutex);
    auto eChunkType = pBaseChunk->GetChunkType();
    
    // Check if we have registered a function to process the chunk
    if (m_FunctionCallbackMap.find(eChunkType) != m_FunctionCallbackMap.end())
        m_FunctionCallbackMap[eChunkType](pBaseChunk);
    else
        if(!TryPassChunk(pBaseChunk))
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
}

void BaseModule::CheckAndThrowJSON(const nlohmann::json_abi_v3_11_2::json& j, const std::string& key) {
    auto it = j.find(key);
    if (it == j.end()) {
        std::string strFatal = std::string(__FUNCTION__) + "Key '" + key + "' not found in JSON.";
        PLOG_FATAL << strFatal;
        throw std::runtime_error(strFatal);
    }
}