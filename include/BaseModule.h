#ifndef BASE_MODULE
#define BASE_MODULE

/* Standard Includes */
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <stdint.h>
#include <thread>
#include <queue>
#include <vector>
#include <chrono>

/* Custom Includes */
#include "BaseChunk.h"
#include "ChunkTypesNamingUtility.h"
#include "CircularBuffer.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"

/*
 * @brief Base processing class containing a threaded process
 *          to pass on chunks and thread safe input buffer.
 */
class BaseModule
{

public:
    /*
     * @brief Construct a new Base Module object
     * @param[in] uMaxInputBufferSize size of the module input buffer
     */
    BaseModule(unsigned uMaxInputBufferSize = 1);
    virtual ~BaseModule();

    /*
     * @brief Returns module type
     * @param[out] ModuleType of processing module
     */
    virtual std::string GetModuleType(){return "BaseModule"};

    /*
     * @brief Starts the process on its own thread
     */
    virtual void StartProcessing();

    /*
     * @brief stops the process on its own thread
     */
    virtual void StopProcessing();

    /*
     * @brief Check input buffer and try process data
     */
    virtual void ContinuouslyTryProcess();

    /*
     * @brief Set the Next Module object to pass chunks along to
     * @param[in] m_NextModule pointer to the next module to pass chunk to
     */
    virtual void SetNextModule(std::shared_ptr<BaseModule> pNextModule);

    /*
     * @brief Recieves base chunk pointer from previous module
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully inserted into the buffer
     * @return False if message was unsucessfully inserted into the buffer
     */
    bool TakeChunkFromModule(std::shared_ptr<BaseChunk> pBaseChunk);

    /*
     *  @brief tacks time between consecutive chunk passes
     *  @param[in] Boolean as to whether to track and log the processing time
     *  @param[in] String log message
     */
    void TrackProcessTime(bool bTrackTime, std::string sTrackerMessage);

    /*
     * @brief used to test the processing of a particular module
     * @param[in] pBaseChunk Pointer to base chuk
     */
    void TestProcess(std::shared_ptr<BaseChunk> pBaseChunk);

    /*
     * @brief Puts the module into a test mode where a single chunk will be processed and returned when requested
     * @param[in] Boolean as to whethere the module should be in test mode
     */
    void SetTestMode(bool bTestModeState);

    /*
     * @brief If the mo
     * @param[in] Boolean as to whethere the module should be in test mode
     */
    std::shared_ptr<BaseChunk> GetTestOutput();

private:
    size_t m_uMaxInputBufferSize;                                  ///< Max size of the class input buffer
    std::atomic<bool> m_bTrackProcessTime = false;                 ///< Boolean as to whether to track and log the processing time
    std::string m_sTrackerMessage = "";                            ///< Log message printed when logging chunk processing time
    std::chrono::high_resolution_clock::time_point m_CurrentTime;  ///< Initial time used to track time between consecutive chunk passes
    std::chrono::high_resolution_clock::time_point m_PreviousTime; ///< Final time used to track time between consecutive chunk passes

    bool m_bTestMode;                              ///< Boolean as to whether the module is doing doing normal processing or test processing
    std::shared_ptr<BaseChunk> m_pTestChunkOutput; ///< Member used to store test outputs

protected:
    std::condition_variable m_cvDataInBuffer;                       ///< Conditional variable to control data in circulat buffer
    CircularBuffer<std::shared_ptr<BaseChunk>> m_cbBaseChunkBuffer; ///< Input buffer of module
    std::shared_ptr<BaseModule> m_pNextModule;                      ///< Shared pointer to next module into which messages are passed
    std::atomic<bool> m_bShutDown;                                  ///< Whether to try continuously process
    std::mutex m_BufferStateMutex;                                  ///< Mutex to facilitate multi module buffer size checking
    std::thread m_thread;                                           ///< Thread object for module processing

    /**
     * @brief Returns true if a message pointer had been retrieved an passed on to next module.
     *          If no pointer in queue then returns false
     * @param[in] pBaseChunk pointer to base chunk
     */
    virtual void Process(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Passes base chunk pointer to next module
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully inserted into queue
     * @return False if message was unsucessfully inserted into queue
     */
    bool TryPassChunk(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Tries to extract a message from the input buffer
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully removed from the buffer
     * @return False if message was unsucessfully removed from the buffer
     */
    bool TakeFromBuffer(std::shared_ptr<BaseChunk> &pBaseChunk);
};

#endif
