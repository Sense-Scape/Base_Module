#ifndef WAV_CHUNK
#define WAV_CHUNK

/*Standard Includes*/
#include <algorithm>
#include <sstream>

/*Custom Includes*/
#include "BaseChunk.h"
#include "ChunkToJSONConverter.h"

/**
 * @brief WAV header structure that is 32 bytes large
 */
typedef struct WAVHeader
{
    /* RIFF Chunk Descriptor */
    uint8_t RIFF[4] = { 'R', 'I', 'F', 'F' };           // RIFF Header Magic header - BigEndian
    uint32_t ChunkSize = 0;                                 // RIFF Chunk Size - LittleEndian  - size of whole file in bytes minus 8 bytes
    uint8_t WAVE[4] = { 'W', 'A', 'V', 'E' };           // WAVE Header - BigEndian
    /* "fmt" sub-chunk */
    uint8_t fmt[4] = { 'f', 'm', 't', ' ' };            // FMT header - BigEndian
    uint32_t Subchunk1Size = 16;                        // Size of the fmt chunk - BigEndian
    uint16_t AudioFormat = 1;                           // Audio format 1=PCM,6=mulaw,7=alaw,     257=IBM Mu-Law, 258=IBM A-Law, 259=ADPCM - LittleEndian - 1 as there is no quantization
    uint16_t NumOfChan = 0;                                 // Number of channels 1=Mono 2=Sterio - LittleEndian
    uint32_t SamplesPerSec = 0;                             // Sampling Frequency in Hz - LittleEndian
    uint32_t bytesPerSec = 0;                               // bytes per second - LittleEndian - " == SampleRate * NumChannels * BitsPerSample/8"
    uint16_t blockAlign = 0;                                // 2=16-bit mono, 4=16-bit stereo - LittleEndian
    uint16_t bitsPerSample = 0;                             // Number of bits per sample - LittleEndian
    /* "data" sub-chunk */
    uint8_t Subchunk2ID[4] = { 'd', 'a', 't', 'a' };    // "data"  string - BigEndian
    uint32_t Subchunk2Size = 0;                             // Sampled data length - LittleEndian
    // The following data - LittleEndian

    bool operator==(const WAVHeader& sWAVHeader) const {
        return std::memcmp(this, &sWAVHeader, sizeof(WAVHeader)) == 0;
    }

    std::shared_ptr<nlohmann::json> ToJSON() {
        auto JSONDocument = nlohmann::json();
        for (size_t RIFFIndex = 0; RIFFIndex < 4; RIFFIndex++)
            JSONDocument["RIFF"][RIFFIndex] = RIFF[RIFFIndex];
        JSONDocument["ChunkSize"] = ChunkSize;
        for (size_t WAVEIndex = 0; WAVEIndex < 4; WAVEIndex++)
            JSONDocument["WAVE"][WAVEIndex] = WAVE[WAVEIndex];
        for (size_t fmtIndex = 0; fmtIndex < 4; fmtIndex++)
            JSONDocument["fmt"][fmtIndex] = fmt[fmtIndex];
        JSONDocument["Subchunk1Size"] = Subchunk1Size;
        JSONDocument["AudioFormat"] = AudioFormat;
        JSONDocument["NumOfChan"] = NumOfChan;
        JSONDocument["SamplesPerSec"] = SamplesPerSec;
        JSONDocument["bytesPerSec"] = bytesPerSec;
        JSONDocument["blockAlign"] = blockAlign;
        JSONDocument["bitsPerSample"] = bitsPerSample;
        for (size_t Subchunk2IDIndex = 0; Subchunk2IDIndex < 4; Subchunk2IDIndex++)
            JSONDocument["Subchunk2ID"][Subchunk2IDIndex] = Subchunk2ID[Subchunk2IDIndex];
        JSONDocument["Subchunk2Size"] = Subchunk2Size;

        return std::make_shared<nlohmann::json>(JSONDocument);
    }
} WAVHeader;

/**
 * @brief Class that encapsulated time domain data with a WAV header
 */
class WAVChunk :
    public BaseChunk,
    public ChunkToJSONConverter
{

public:
    WAVHeader m_sWAVHeader;             ///< WAV header
    uint64_t m_i64TimeStamp;            ///< Starting timestamp of the wav chunk
    std::vector<int16_t> m_vi16Data;    ///< Time data corresponding to packed audio (One from each channel repeated)

    /**
    * @brief Defualt constructor
    */
    WAVChunk();

    /**
    * @brief Constructor
    * @param[in] sMACAddress MAC address to uniquely identify source of audio
    */
    WAVChunk(uint64_t i64TimeStamp);

    /**
    * @brief Copy constructor using pointer
    */
    WAVChunk(std::shared_ptr<WAVChunk> pWAVChunk);

    /**
    * @brief Copy constructor using reference
    */
    WAVChunk(WAVChunk &wavChunk);

    /**
    * @brief Returns the derived chunk type
    * @return ChunkType of type WAVChunk
    */
    ChunkType GetChunkType() override { return ChunkType::WAVChunk; };

    /**
    * @brief MOST PROBABLY A BROKEN FUNCTION - unpacks WAV data into indivudal vectors of channel data
    * @param[in] pvvi16UnpackedWAVData shared pointer to vector of vectors of int16
    */
    void UnpackWAVData(std::shared_ptr<std::vector<std::vector<int16_t>>> pvvi16UnpackedWAVData);

    /**
    * @brief Converts an array of 44 chars in to WAVHeader structure
    * @param[out] returns a WAVHeader structure
    */
    static WAVHeader BytesToWAVHeader(std::vector<char>& vcWAVHeader);

    /**
    * @brief Converts a WAV header into a array of 44 chars 
    * @return shared pointer to vector of byes
    */
    std::shared_ptr<std::vector<char>> WAVHeaderToBytes();

    /**
    * @brief Converts all little endian byte representation into partially big endian as per WAV header structure
    * @param[in] pointer to wav header bytes
    */
    static void FormatWAVHeaderBytes(std::shared_ptr<std::vector<char>> pvcWAVHeaderBytes);

    /**
    * @brief Prints out header data
    */
    std::string GetHeaderString();

    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetSize() override;

    /**
     * @brief Returns if the two classes are equal
     * param[in] wavChunk reference to WAVChunk
     * @return Reference to the class with which we want to compare
     */
    bool IsEqual(WAVChunk& wavChunk);

    /**
    * @brief Returns the JSON equivalent of this classes representation
    */
    std::shared_ptr<nlohmann::json> ToJSON() override;

private:
    /**
     * @brief Get the size of object in bytes
     * @return Size of object in bytes
     */
    unsigned GetInternalSize();
};


#endif
