#ifndef DETECTION_BIN_CHUNK
#define DETECTION_BIN_CHUNK

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief Frequency Data Chunk used to store complex channel FFT data
 */
class DetectionBinChunk : public BaseChunk,
        public ChunkToJSONConverter
{
public:
       
    /**
     * @brief Construct a new empty DetectionBinChunk object
     */
    DetectionBinChunk();

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] DetectionBinChunk reference
     */
    DetectionBinChunk(const DetectionBinChunk &DetectionBinChunk);

    /**
     * @brief Copy constrtuctor for the FFT chunk
     * @param[in] Shared pointer to FFT chunk
     */
    DetectionBinChunk(std::shared_ptr<DetectionBinChunk> pDetectionBinChunk);

    /**
     * @brief Get the Chunk Type object
     * @return[in] ChunkType of chunk
     */
    ChunkType GetChunkType() override { return ChunkType::DetectionBinChunk; };

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
     * @param[in] DetectionBinChunk Reference to the class with which we want to compare
     * @return whether the classes are equal or not
     */
    bool IsEqual(DetectionBinChunk &DetectionBinChunk);

    /**
    * @brief Returns the JSON equivalent of this classes representation
    */
    std::shared_ptr<nlohmann::json> ToJSON() override;

    /**
     * @brief Get the number of channels.
     * 
     * @return The number of channels as an unsigned 16-bit integer.
     */
    u_int16_t GetNumChannels() { return m_u16NumChannels; }

    /**
     * @brief Get the detected bins per channel.
     * 
     * @return A vector of unsigned 16-bit integers representing the detected bins for each channel.
     */
    std::shared_ptr<std::vector<uint16_t>> GetDetectedBinsPerChannel() { return std::make_shared<std::vector<uint16_t>>(m_vu16DetectedBinsPerChannel); }

    /**
     * @brief Get the detection bins.
     * 
     * @return A 2D vector of unsigned 16-bit integers representing the detection bins.
     *         The outer vector corresponds to channels and the inner vectors correspond to bins within each channel.
     */
    std::shared_ptr<std::vector<std::vector<uint16_t>>> GetDetectionBins() { return std::make_shared<std::vector<std::vector<uint16_t>>>(m_vvu16DetectionBins); }

    /**
     * @brief Set the detection bins and update related member variables.
     * 
     * This function takes a 2D vector of unsigned 16-bit integers as input,
     * where each inner vector represents the detection bins for a channel.
     * It updates the number of channels, the detection bins, and the number of detected bins per channel.
     * 
     * @param vvu16DetectionBins A reference to a 2D vector of unsigned 16-bit integers representing the detection bins.
     */
    void SetDetectionBins(std::vector<std::vector<uint16_t>> &vvu16DetectionBins);

protected:
    /**
     * @brief Fill a byte array the represents this object
     * @param[in] pByteArray Shared pointer to byte vector containing byte data
     */
    std::shared_ptr<std::vector<char>> GetInternalSerialisation();

private:

    u_int16_t m_u16NumChannels;
    std::vector<uint16_t> m_vu16DetectedBinsPerChannel;
    std::vector<std::vector<uint16_t>> m_vvu16DetectionBins;

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetInternalSize();
};

#endif