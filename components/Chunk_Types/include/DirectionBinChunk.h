#ifndef DIRECTION_BIN_CHUNK
#define DIRECTION_BIN_CHUNK

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Frequency Data Chunk used to store complex channel FFT data
 */
class DirectionBinChunk : public BaseChunk,
        public ChunkToJSONConverter
{
public:
       
    /**
     * @brief Construct a new empty DirectionBinChunk object
     */
    DirectionBinChunk();

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] DirectionBinChunk reference
     */
    DirectionBinChunk(const DirectionBinChunk &DirectionBinChunk);

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] Shared pointer to FFT chunk
     */
    DirectionBinChunk(std::shared_ptr<DirectionBinChunk> pDirectionBinChunk);

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::DirectionBinChunk; };

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
     * @param DirectionBinChunk Reference to the class with which we want to compare
     * @return whether the classes are equal or not
     */
    bool IsEqual(DirectionBinChunk &DirectionBinChunk);

    /**
    * @brief Returns the JSON equivalent of this classes representation
    */
    std::shared_ptr<nlohmann::json> ToJSON() override;

    /**
     * @brief Sets the direction data based on the provided detection information.
     * @param u16NumberOfDetections The number of detections.
     * @param vu16DetectionFrequencyIndicies Reference to a vector containing the detection frequency indices.
     * @param vvu16DetectionAngles_deg Reference to a vector containing the detection angles in degrees.
     * @param dSampleRate The sample rate used for the detections.
     */
    void SetDirectionData(uint16_t u16NumberOfDetections, std::vector<uint16_t> &vu16DetectionFrequencyIndicies, std::vector<float> &vvu16DetectionAngles_deg, double dSampleRate );

    /**
     * @brief Retrieves the number of detections.
     * @return The number of detections.
     */
    uint16_t GetNumberOfDetections() {return m_u16NumberOfDetections;}

    /**
     * @brief Retrieves the detection frequency indices.
     * @return A vector containing the detection frequency indices.
     */
    std::vector<uint16_t> GetDetectionFrequencyIndicies() {return m_vu16DetectionFrequencyIndicies;}

    /**
     * @brief Retrieves the detection angles in degrees.
     * @return A vector containing the detection angles in degrees.
     */
    std::vector<float> GetDetectionAngles() {return m_vfDetectionAngles_deg;}

    /**
     * @brief Retrieves the sample rate used for the detections.
     * @return The sample rate.
     */
    double GetSampleRate() {return m_dSampleRate;}

protected:
    /**
     * @brief Fill a byte array the represents this object
     * @param[in] pByteArray Shared pointer to byte vector containing byte data
     */
    std::shared_ptr<std::vector<char>> GetInternalSerialisation();

private:

    uint16_t m_u16NumberOfDetections;                       ///< Number of Detection angles in this chunk
    std::vector<uint16_t> m_vu16DetectionFrequencyIndicies; ///< Vector of vectors containing detection indicies per channel
    std::vector<float> m_vfDetectionAngles_deg;           ///< Vector of vectors containing detection indicies per channel
    double m_dSampleRate;                                   ///< Sample rate used to generate data

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetInternalSize();
};

#endif