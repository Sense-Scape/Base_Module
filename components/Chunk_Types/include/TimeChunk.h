#ifndef TIME_CHUNK
#define TIME_CHUNK

/*Standard Includes*/
#include <memory>
#include <vector>
#include <complex>

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Chunk used to store multichannel time data
 */
class TimeChunk : public BaseChunk,
                  public ChunkToJSONConverter
{
public:
    double m_dChunkSize;                                            ///< Number of samples contained in a single chunk
    double m_dSampleRate;                                           ///< Sample rate used to obtain data in chunk
    uint64_t m_i64TimeStamp;                                        ///< Timestamp of when chunk was taken
    unsigned m_uBits;                                               ///< Bits of ADC used to produce chunk
	unsigned m_uNumBytes;                                           ///< Number of bytes in single sample
    unsigned m_uNumChannels;                                        ///< Number of audio channels in chunk
    std::vector<std::vector<int16_t>> m_vvi16TimeChunks;            ///< Vector of vectors corresponding to channel samples

    /**
     * @brief Empty construct a new Base Chunk object
     */
    TimeChunk();

    /**
     * @brief Construct a new Base Chunk object
     * @param[in] dChunkSize The number of samples contained in each ADC channel chunk
     * @param[in] dSampleRate The sample rate used to generate all data within the chunk
     * @param[in] i64TimeStamp The time the chunk was created
     * @param[in] uBits Bits of ADC used to produce chunk
     * @param[in] uNumBytes Number of bytes in single sample
     * @param[in] uNumChannels Number of audio channels in chunk
     */
    TimeChunk(double dChunkSize, double dSampleRate, uint64_t i64TimeStamp, unsigned uBits, unsigned uNumBytes, unsigned uNumChannels);

    /**
     * @brief Copy constructor of a time chunk
     * @param[in] timeChunk reference
     */
    TimeChunk(const TimeChunk& timeChunk);
    
    /**
     * @brief Copy constructor of a time chunk
     * @param[in] timeChunk shared pointer
     */
    TimeChunk(std::shared_ptr<TimeChunk> pTimeChunk);

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::TimeChunk; };

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
     * @return Reference to the class with which we want to compare
     */
    bool IsEqual(TimeChunk& timeChunk);
    
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

    /**
     * @brief Fills all channels with zeroed data
     */
    void InitialiseChannels();
};

#endif