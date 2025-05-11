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
#include <queue>
#include <functional>

/* Custom Includes */
#include "BaseChunk.h"
#include "ChunkTypesNamingUtility.h"
#include <plog/Log.h>
#include "plog/Initializers/RollingFileInitializer.h"
#include "JSONChunk.h"

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
    virtual std::string GetModuleType() { return "BaseModule"; };

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
     * @brief Set the Next Module object to pass chunks along to
     * @param[in] m_NextModule pointer to the next module to pass chunk to
     */
    virtual void SetReportingNextModule(std::shared_ptr<BaseModule> pNextModule);

    /*
     * @brief Recieves base chunk pointer from previous module
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully inserted into the buffer
     * @return False if message was unsucessfully inserted into the buffer
     */
    bool TakeChunkFromModule(const std::shared_ptr<BaseChunk> &pBaseChunk);

    /*
     *  @brief tacks time between consecutive chunk passes
     *  @param[in] Boolean as to whether to track and log the processing time
     *  @param[in] String log message
     */
    void TrackProcessTime(bool bTrackTime, std::string sTrackerMessage);

    /*
     * @brief Function to start the reporting thread
     */
    void StartReporting();

    /*
     * @brief Set additional fields for reporting
     * @param[in] strReportingJsonRoot string specifying the root of JSON doc
     * @param[in] strReportingJsonModuleAddition in the case we have multiple of the same module, this differentiates them
     */
    void SetReportingDescriptors(std::string strReportingJsonRoot, std::string strReportingJsonModuleAddition);

    /**
     * @brief Calls processing function for particular chunk type
     * @param[in] pBaseChunk pointer to chunk to be processed
     */
    void CallChunkCallbackFunction(std::shared_ptr<BaseChunk> pBaseChunk);

    /**
     * @brief Registers which function should be called when processing a chunk
     * @param[in] eChunkType pointer to chunk to be processed
     * @param[in] function class function (&,ClassName::FunctionName)
     */
    template <typename ClassType>
    void RegisterChunkCallbackFunction(ChunkType eChunkType, 
                                    void (ClassType::*function)(std::shared_ptr<BaseChunk>), 
                                    BaseModule* pDerivedModule)
    {
        std::unique_lock<std::mutex> BufferAccessLock(m_FunctionCallbackMapMutex);
        m_FunctionCallbackMap[eChunkType] = [function, pDerivedModule](std::shared_ptr<BaseChunk> chunk) {
            (static_cast<ClassType*>(pDerivedModule)->*function)(chunk);
        };
    }

private:
    size_t m_uMaxInputBufferSize;                                           ///< Max size of the class input buffer

    // Contolling Timing For Debugging
    std::atomic<bool> m_bTrackProcessTime = false;                          ///< Boolean as to whether to track and log the processing time
    std::string m_sTrackerMessage = "";                                     ///< Log message printed when logging chunk processing time
    std::chrono::high_resolution_clock::time_point m_CurrentTrackingTime;   ///< Initial time used to track time between consecutive chunk passes
    std::chrono::high_resolution_clock::time_point m_PreviousTimeTracking;  ///< Final time used to track time between consecutive chunk passes

protected:
    // Controlling Queues 
    std::condition_variable m_cvDataInBuffer;                               ///< Conditional variable to control data in circulat buffer
    std::queue<std::shared_ptr<BaseChunk>> m_cbBaseChunkBuffer;             ///< Input buffer of module
    std::shared_ptr<BaseModule> m_pNextModule;                              ///< Shared pointer to next module into which messages are passed
    std::shared_ptr<BaseModule> m_pNextReportingModule;                     ///< Shared pointer to next module into which messages are passed
    std::atomic<bool> m_bShutDown;                                          ///< Whether to try continuously process
    std::mutex m_BufferStateMutex;                                          ///< Mutex to facilitate multi module buffer size checking
    std::thread m_thread;                                                   ///< Thread object for module processing
    bool m_bAlreadyLoggedBufferFull;                                        ///< Boolean State machine tracking if we have logged whether queue is full

    // Controlling Reporting
    std::thread m_QueueSizeReportingThread;                                 ///< Thread to report current status of module
    std::string m_strReportingJsonRoot = "undefined";                       ///< Used to store the overall binary name (eg Sensor/ProcServ)
    std::string m_strReportingJsonModuleAddition = "";                      ///< Used to inform which module in binary this is

    // Controling Function Calls
    std::mutex m_FunctionCallbackMapMutex;                                                      ///< Lock to callback map     
    std::map<ChunkType,std::function<void(std::shared_ptr<BaseChunk>)>> m_FunctionCallbackMap;  ///< Given a chunktype, a different proc function will be called

    /**
     * @brief Passes base chunk pointer to next module
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully inserted into queue
     * @return False if message was unsucessfully inserted into queue
     */
    bool TryPassChunk(const std::shared_ptr<BaseChunk> &pBaseChunk);

    /**
     * @brief Passes base chunk pointer to next module
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully inserted into queue
     * @return False if message was unsucessfully inserted into queue
     */
    bool TryPassReportingChunk(const std::shared_ptr<BaseChunk> &pBaseChunk);

    /**
     * @brief Tries to extract a message from the input buffer
     * @param[in] pBaseChunk pointer to base chunk
     * @return True if message was sucessfully removed from the buffer
     * @return False if message was unsucessfully removed from the buffer
     */
    bool TakeFromBuffer(std::shared_ptr<BaseChunk> &pBaseChunk);

    /**
     * @brief Calls the infinite loop to report
     */
    virtual void StartReportingLoop();

    /**
     * @brief look for specified key and throw if not found
     * @param[in] jsonConfig JSON configuration of this module
     * @param[in] key Key for which on is looking
     */
    void CheckAndThrowJSON(const nlohmann::json_abi_v3_11_2::json& j, const std::string& key);
};

#endif
