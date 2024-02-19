#ifndef WATCH_DOG_CHUNK
#define	WATCH_DOG_CHUNK

/*Standard Includes*/
#include <sstream>

/* Custom Includes */
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Byte Data Chunk used to store Byte datagram data to pass between modules
 */
class WatchdogChunk :   public BaseChunk,
                        public ChunkToJSONConverter
{
public:
    std::vector<uint8_t> m_vu8MACAddress;        ///< MAC Address

    /**
     * @brief Construct a new Time Chunk object
     * @param[in] strMACAddress MAC of current device
     */
    WatchdogChunk(std::vector<uint8_t> &vu8MACAddress);
    WatchdogChunk(std::shared_ptr<WatchdogChunk> pWatchdogChunk);
    // ~WatchdogChunk() {};

    /**
     * @brief Get the Chunk Type object
     * @return ChunkType Chunk type of the chunk
     */
    virtual ChunkType GetChunkType() override { return ChunkType::WatchdogChunk; };

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetSize();

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
     * @return Reference to the class with which we want to compare
     */
    bool IsEqual(WatchdogChunk& watchdogChunk);

    /**
    * @brief Returns the JSON equivalent of this classes representation
    */
    std::shared_ptr<nlohmann::json> ToJSON() override;

private:
    std::string MACAddressToString(const std::vector<uint8_t>& macAddress);
};

#endif
