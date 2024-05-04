#ifndef GPS_CHUNK
#define GPS_CHUNK

/* Standard Includes */
#include <vector>

/* Custom Includes */
#include "BaseChunk.h"

/**
 * @brief Chunk that is intended to store GPS representation of other chunks.
 *          May also store generic GPS documents
 */
class GPSChunk : public BaseChunk
{
public:
    bool m_bDataIsNorth; ///< Whether data is nort or not
    double m_dLatitude;  ///< Latitude value
    bool m_bDataIsWest;  ///< Whether data is west or not
    double m_dLongitude; ///< Latitude value

    /**
     * @brief Construct a new Time Chunk object
     */
    GPSChunk();
    GPSChunk(std::shared_ptr<GPSChunk> pGPSChunk);
    ~GPSChunk(){};

    /**
     * @brief Get the Chunk Type object
     * @return ChunkType Chunk type of the chunk
     */
    virtual ChunkType GetChunkType() override { return ChunkType::GPSChunk; };

    /**
     * @brief Returns if the two classes are equal
     * @return Reference to the class with which we want to compare
     */
    bool IsEqual(GPSChunk &GPSChunk);

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetSize() override;

    /**
     * @brief Fill a byte array the represents this object
     * @return Shared pointer to byte vector containing byte data
     */
    std::shared_ptr<std::vector<char>> Serialise() override;

    /**
     * @brief Converts byte array to object members
     * @param pvBytes Shared pointer to byte array that shall be used to construct memeber variables
     */
    void Deserialise(std::shared_ptr<std::vector<char>> pBytes);

protected:
    /**
     * @brief Fill a byte array the represents this object
     * @return Shared pointer to byte vector containing byte data on parent class
     */
    std::shared_ptr<std::vector<char>> GetInternalSerialisation();

private:
    uint16_t m_GPSDocumentSize_bytes; ///< Size of GPS document only used during serialisation

    /**
     * @brief Get the size of object in bytes
     * @return Size of object parent object in bytes
     */
    unsigned GetInternalSize();
};

#endif
