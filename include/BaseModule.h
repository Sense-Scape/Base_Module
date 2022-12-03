#ifndef BASEMODULE
#define BASEMODULE

/*Standard Includes*/
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <thread>
#include <queue>
#include <vector>

#include <stdint.h>

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkTypesUtility.h"
#include "ModuleTypes.h"
#include "CircularBuffer.h"
#include <condition_variable>
/**
 * @brief Base processing class containing a threaded process and thread safe input buffer.
 *
 */
class BaseModule
{
private:
    size_t m_uMaxInputBufferSize;               ///< Max size of the class input buffer
    bool m_bDataInBuffer;                       ///<
   

protected:
    std::condition_variable m_cvDataInBuffer;   ///<
    CircularBuffer<std::shared_ptr<BaseChunk>> m_cbBaseChunkBuffer; ///< Input buffer of module
    std::shared_ptr<BaseModule> m_pNextModule;                      ///< Shared pointer to next module into which messages are passed
    bool m_bShutDown;                                               ///< Whether to try continuously process
    std::mutex m_BufferStateMutex;                                  ///< Mutex to facilitate multi module buffer size checking                                   
    std::thread m_thread;                                           ///< Thread object for module processing
    std::mutex m_ProcessStateMutex;
    

    /**
     * @brief Returns true if a message pointer had been retrieved an passed on to next module.
     *          If no pointer in queue then returns false
     */
    virtual void Process(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Passes base chunk pointer to next module
     *
     * @param pBaseChunk
     * @return true if message was sucessfully inserted into queue
     * @return false if message was unsucessfully inserted into queue
     */
    bool TryPassChunk(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Tries to extract a message from the input buffer
     *
     * @return true if message was sucessfully removed from the buffer
     * @return false if message was unsucessfully removed from the buffer
     */
    bool TakeFromBuffer(std::shared_ptr<BaseChunk>& pBaseChunk);

public:
    /**
     * @brief Construct a new Base Module object
     * @param[in] uMaxInputBufferSize size of the module input buffer
     */
    BaseModule(unsigned uMaxInputBufferSize = 2);
    virtual ~BaseModule();

    /**
     * @brief Returns module type
     * @param[out] ModuleType of processing module
     */
    virtual ModuleType GetModuleType() { return ModuleType::ModuleBase; };

    /**
     * @brief Starts the  process on its own thread
     *
     */
    virtual void StartProcessing();

    /**
     * @brief Check input buffer and try process data
     *
     */
    virtual void ContinuouslyTryProcess();

    /**
     * @brief Set the Next Module object to pass chunks along to
     *
     * @param m_NextModule pointer to the next module to pass chunk to
     */
    virtual void SetNextModule(std::shared_ptr<BaseModule> pNextModule);

    /**
     * @brief Recieves base chunk pointer from previous module
     *
     * @param pBaseChunk
     * @return true if message was sucessfully inserted into the buffer
     * @return false if message was unsucessfully inserted into the buffer
     */
    bool TakeChunkFromModule(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Sets module input buffer size
     *
     * @param uSize size of uinput buffer
     */
    void SetBufferSize(unsigned uSize);
};

#endif