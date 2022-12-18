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
    // TODO: finish implementation
    if(m_thread.joinable())
    {
        m_thread.join();
    }
}
void BaseModule::Process(std::shared_ptr<BaseChunk> pBaseChunk)
{
    TryPassChunk(pBaseChunk);
}

void BaseModule::ContinuouslyTryProcess()
{
    
    std::unique_lock<std::mutex> ProcessLock(m_ProcessStateMutex);

    while (!m_bShutDown)
    {
        ProcessLock.unlock();

        std::shared_ptr<BaseChunk> pBaseChunk;
        if (TakeFromBuffer(pBaseChunk))
            Process(pBaseChunk);
        else
        {
            // Wait to be notified that there is data available
            std::unique_lock<std::mutex> BufferAccessLock(m_BufferStateMutex);
            m_cvDataInBuffer.wait(BufferAccessLock, [this] {return (!m_cbBaseChunkBuffer.empty() || m_bShutDown);});
        }

        ProcessLock.lock();
    }
}

void BaseModule::StartProcessing()
{
    m_thread = std::thread([this]()
        { ContinuouslyTryProcess(); });
}

void BaseModule::SetNextModule(std::shared_ptr<BaseModule> pNextModule)
{
    m_pNextModule = pNextModule;
}

bool BaseModule::TryPassChunk(std::shared_ptr<BaseChunk> pBaseChunk)
{
    // Allow module that one is passing to to facilitate its own locking procedures
    if (m_pNextModule != nullptr)
        return m_pNextModule->TakeChunkFromModule(pBaseChunk);
    else
        return false;
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
