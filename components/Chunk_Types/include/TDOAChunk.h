#ifndef TDOA_CHUNK
#define TDOA_CHUNK

/*Standard Includes*/
#include <memory>
#include <vector>
#include <complex>

/*Custom Includes*/
#include "BaseChunk.h"


/**
 * @brief 
 */
class TDOAChunk : public BaseChunk
{
public:
    double m_dSampleRate;                                               ///< Sample rate used to obtain data in chunk
    uint64_t m_i64TimeStamp_us;                                            ///< Timestamp of when chunk was taken
    uint64_t m_u64ChunkSize;
    uint16_t m_u16NumberSources;
    std::vector<std::vector<std::vector<int16_t>>> m_vvvi16TimeChunks;  ///< Vector of vectors corresponding to channel samples
    std::vector<std::vector<uint8_t>> m_vvu8SourceIdentifiers;
    std::vector<uint8_t> m_vu8SourceIdentifierSizes;
    std::vector<uint8_t> m_vu8ChannelsPerSource;                       
    std::vector<double> m_vdLongitude;
    std::vector<double> m_vdLatitude;


    /**
     * @brief Construct a new Base Chunk object
     * @param[in] dSampleRate The sample rate used to generate all data within the chunk
     * @param[in] i64TimeStamp The time the chunk was created
     */
    TDOAChunk(double dSampleRate, uint64_t i64TimeStamp_us, uint64_t u64ChunkSize);

    /**
     * @brief Copy constructor of a time chunk
     * @param[in] timeChunk reference
     */
    TDOAChunk(const TDOAChunk& timeChunk);
    
    /**
     * @brief Copy constructor of a time chunk
     * @param[in] timeChunk shared pointer
     */
    TDOAChunk(std::shared_ptr<TDOAChunk> pTimeChunk);

    void AddData(double dLong, double dLat, std::vector<uint8_t> vu8SourceIdentifier, std::vector<std::vector<int16_t>> &vvi16Data);

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::TDOAChunk; };

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    uint64_t GetSize() override;

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
    uint64_t GetInternalSize();

};

#endif