#ifndef QUEUE_LENGTH_CHUNK
#define	QUEUE_LENGTH_CHUNK

/* Custom Includes */
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Stores length of queue from particular module
 */
class QueueLengthChunk : public BaseChunk,
                         public ChunkToJSONConverter
{
public:
    std::string m_strQueueName;     ///< Module name
    uint16_t m_u16QueueNameLength;  ///< Length of QueueName
    std::string m_strProgramName;   ///< Program Name name
    uint16_t m_u16ProgramNameLength;///< Length of program name
    uint16_t m_uQueueLength;        ///< The length of the queue
    
    /**
     * @brief Construct a new Queue Length chunk
     * @param[in] strQueueName String of chunk/queue name
     * @param[in] uChunkLength The length of the queue
     */
    QueueLengthChunk(std::string strQueueName, unsigned uQueueLength);
    QueueLengthChunk(std::shared_ptr<QueueLengthChunk> pQueueLengthChunk);
    QueueLengthChunk();

    /**
     * @brief Get the Chunk Type object
     * @return ChunkType Chunk type of the chunk
     */
    ChunkType GetChunkType() override { return ChunkType::QueueLengthChunk; };

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    u_int64_t GetSize() override;

    /**
     * @brief Fill a byte array the represents this object
     * @param[in] pByteArray Shared pointer to byte vector containing byte data
     */
    std::shared_ptr<std::vector<char>> Serialise();

    /**
     * @brief Converts byte array to object members
     * @param[in] pvBytes Shared pointer to byte array that shall be used to construct memeber variables
     */
    void Deserialise(std::shared_ptr<std::vector<char>> pBytes);

    /**
     * @brief Returns if the two classes are equal
     * @param[in] GPSChunk Reference to the class with which we want to compare
     * @return whether the classes are equal or not
     */
    bool IsEqual(QueueLengthChunk &QueueLengthChunk);

    /**
     * @brief Returns the JSON equivalent of this classes representation
     */
    std::shared_ptr<nlohmann::json> ToJSON() override;

protected:
    /**
     * @brief Fill a byte array the represents this object
     * @param[in] pByteArray Shared pointer to byte vector containing byte data
     */
    std::shared_ptr<std::vector<char>> GetInternalSerialisation();

private:
    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    u_int64_t GetInternalSize();
};

#endif
