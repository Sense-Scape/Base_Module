#ifndef COORDINATE_CHUNK
#define COORDINATE_CHUNK


/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Frequency Data Chunk used to store complex channel FFT data
 */
class CoordinateChunk : public BaseChunk,
    public ChunkToJSONConverter
{
public:
    uint64_t m_i64TimeStamp;    ///< Timestamp of when chunk was created
    double m_dLongitude;        ///< Longitude at point in time
    double m_dLatitude;         ///< Latitude at point in time

    /**
     * @brief Construct a new empty CoordinateChunk object
     */
    CoordinateChunk();

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::CoordinateChunk; };

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetSize() override;

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
    * @param[in] CoordinateChunk Reference to the class with which we want to compare
    * @return whether the classes are equal or not
    */
    bool IsEqual(CoordinateChunk& CoordinateChunk);


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
    unsigned GetInternalSize();

};

#endif