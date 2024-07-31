#ifndef CHUNK_TYPES_UTILITY
#define CHUNK_TYPES_UTILITY

/* Standard Includes */
#include <iostream>
#include <memory>
#include <string>

/* Custom Includes */

/**
 * @brief Class that facillitates conversion between emunerated chunk type and string
 */
class ChunkTypesNamingUtility
{
public:
    ChunkTypesNamingUtility(){};
    ~ChunkTypesNamingUtility(){};

    static std::string toString(ChunkType eChunkType)
    {
        switch (eChunkType)
        {
        case ChunkType::ChunkBase:
            return "ChunkBase";
        case ChunkType::ByteChunk:
            return "ByteChunk";
        case ChunkType::TimeChunk:
            return "TimeChunk";
        case ChunkType::WAVChunk:
            return "WAVChunk";
        case ChunkType::WatchdogChunk:
            return "WatchdogChunk";
        case ChunkType::FFTChunk:
            return "FFTChunk";
        case ChunkType::JSONChunk:
            return "JSONChunk";
        case ChunkType::FFTMagnitudeChunk:
            return "FFTMagnitudeChunk";
        case ChunkType::GPSChunk:
            return "GPSChunk";
        case ChunkType::DetectionBinChunk:
            return "DetectionBinChunk";
        case ChunkType::DirectionBinChunk:
            return "DirectionBinChunk";
        case ChunkType::QueueLengthChunk:
            return "QueueLengthChunk";
        default:
            return "Unknown Chunk";
        }
    }

    static uint32_t ToU32(ChunkType eChunkType)
    {
        return static_cast<uint32_t>(eChunkType);
    }

    static ChunkType FromU32(uint32_t u32ChunkType)
    {   
        return static_cast<ChunkType>(u32ChunkType);
    }
};

#endif
