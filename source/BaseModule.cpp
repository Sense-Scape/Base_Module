#include "BaseModule.h"

BaseModule::BaseModule(unsigned uMaxInputBufferSize) : 
    m_uMaxInputBufferSize(uMaxInputBufferSize),
    m_cvDataInBuffer(),
    m_cbBaseChunkBuffer(uMaxInputBufferSize),
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
void BaseModule::Process(std::shared_ptr<BaseChunk> pBaseChunk)
{
    TryPassChunk(pBaseChunk);
}

void BaseModule::ContinuouslyTryProcess()
{
    while (!m_bShutDown)
    {
        std::shared_ptr<BaseChunk> pBaseChunk;
        if (TakeFromBuffer(pBaseChunk))
            Process(pBaseChunk);
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
        m_thread = std::thread([this]()
            { ContinuouslyTryProcess(); });
    else
    {
        // Log warning
        std::string strWarning = std::string(__FUNCTION__) + ": Processing thread already started";
        PLOG_WARNING << strWarning;
        // And force a crash if in debug and not release
        assert(true && m_bTestMode);
    }
}

void BaseModule::StopProcessing()
{
    m_bShutDown = true;
}

void BaseModule::SetNextModule(std::shared_ptr<BaseModule> pNextModule)
{
    m_pNextModule = pNextModule;
}

bool BaseModule::TryPassChunk(std::shared_ptr<BaseChunk> pBaseChunk)
{
    // Lets first check we are doing timing debugging
    if (m_bTrackProcessTime)
    {
        // if so print the last time since we passed a message. This should give us an estimate of how
        // long the module is taking to process
        auto duration = std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(m_CurrentTime - m_PreviousTime).count());
        std::string strInfo = std::string(__FUNCTION__) + m_sTrackerMessage + ": Time between passing chunks = " + duration + "us";
        PLOG_INFO << strInfo;

        m_PreviousTime = m_CurrentTime;
        m_CurrentTime = std::chrono::high_resolution_clock::now();
    }
    
    // Allow module that one is passing to to facilitate its own locking procedures
    bool bReturnSuccessful = false;

    if (m_pNextModule != nullptr)
        bReturnSuccessful = m_pNextModule->TakeChunkFromModule(pBaseChunk);
    else if (m_bTestMode)
        m_pTestChunkOutput = pBaseChunk;

    return bReturnSuccessful;
}

bool BaseModule::TakeChunkFromModule(std::shared_ptr<BaseChunk> pBaseChunk)
{
    // Maintain lock to prevent another module trying to access buffer
    // in the multi input buffer configuration
    bool bChunkPassed = false;
    std::unique_lock<std::mutex> BufferStateLock(m_BufferStateMutex);

    // Check if next module queue is full
    if (m_cbBaseChunkBuffer.size() < m_uMaxInputBufferSize)
    {
        m_cbBaseChunkBuffer.put(pBaseChunk);
        bChunkPassed = true;
    }

    // Then release and tell everyone we done
    BufferStateLock.unlock();
    m_cvDataInBuffer.notify_all();
    return bChunkPassed;
}

bool BaseModule::TakeFromBuffer(std::shared_ptr<BaseChunk>& pBaseChunk)
{
    // Maintain lock to prevent another module trying to access buffer
    // in the multi input buffer configuration
    std::lock_guard<std::mutex> BufferStateLock(m_BufferStateMutex);

    if (!m_cbBaseChunkBuffer.empty())
    {
        pBaseChunk = m_cbBaseChunkBuffer.get();
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
    m_PreviousTime = std::chrono::high_resolution_clock::now();
    m_CurrentTime = std::chrono::high_resolution_clock::now();
}

void BaseModule::TestProcess(std::shared_ptr<BaseChunk> pBaseChunk)
{
    Process(pBaseChunk);
}

std::shared_ptr<BaseChunk> BaseModule::GetTestOutput()
{
    if (m_pTestChunkOutput != nullptr)
        return std::move(m_pTestChunkOutput);
    else
    {
        std::string strError = std::string(__FUNCTION__) + " No test chunk output to return";
        PLOG_ERROR << strError;
        throw;
    }
}

void BaseModule::SetTestMode(bool bTestModeState)
{
    m_bTestMode = bTestModeState;
}