#ifndef GPS_CHUNK
#define GPS_CHUNK

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Frequency Data Chunk used to store complex channel FFT data
 */
class GPSChunk : public BaseChunk,
                 public ChunkToJSONConverter
{
public:
    uint64_t m_i64TimeStamp; ///< Timestamp of when chunk was created
    bool m_bIsNorth;         ///< Whether latitude is north
    double m_dLongitude;     ///< Longitude at point in time
    bool m_bIsWest;          ///< Whether latitude is north
    double m_dLatitude;      ///< Latitude at point in time

    /**
     * @brief Construct a new empty GPSChunk object
     */
    GPSChunk();

    /**
     * @brief Construct a new GPSChunk object with all parameters
     * 
     * @param timestamp Timestamp of when chunk was created
     * @param isNorth Whether latitude is north
     * @param longitude Longitude at point in time
     * @param isWest Whether longitude is west
     * @param latitude Latitude at point in time
     */
    GPSChunk(uint64_t timestamp, bool isNorth, double longitude, bool isWest, double latitude);

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] GPSChunk reference
     */
    GPSChunk(const GPSChunk &GPSChunk);

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] Shared pointer to FFT chunk
     */
    GPSChunk(std::shared_ptr<GPSChunk> pGPSChunk);

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::GPSChunk; };

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
    bool IsEqual(GPSChunk &GPSChunk);

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