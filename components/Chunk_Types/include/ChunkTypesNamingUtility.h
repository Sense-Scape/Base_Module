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

    ChunkTypesNamingUtility() {};
    ~ChunkTypesNamingUtility() {};

    static std::string toString(ChunkType eChunkType)
    {
        switch (eChunkType)
        {
            case ChunkType::ChunkBase: return "ChunkBase";
            case ChunkType::ByteChunk:  return "ByteChunk";
            case ChunkType::TimeChunk: return "TimeChunk";
            case ChunkType::WAVChunk:  return "WAVChunk";
            case ChunkType::WatchdogChunk: return "WatchdogChunk";
            case ChunkType::FFTChunk: return "FFTChunk";
            case ChunkType::JSONChunk: return "JSONChunk";
            case ChunkType::FFTMagnitudeChunk: return "FFTMagnitudeChunk";

            default: return "Unknown Chunk";
        }
    }

    static uint32_t ToU32(ChunkType eChunkType)
    {
        switch (eChunkType)
        {
        case ChunkType::ChunkBase: return 1;
        case ChunkType::ByteChunk:  return 2;
        case ChunkType::TimeChunk: return 3;
        case ChunkType::WAVChunk:  return 4;
        case ChunkType::WatchdogChunk: return 5;
        case ChunkType::FFTChunk: return 6;
        case ChunkType::JSONChunk: return 7;
        case ChunkType::FFTMagnitudeChunk: return 8;

        default: return 0; // error
        }
    }

    static ChunkType FromU32(uint32_t u32ChunkType)
    {
        switch (u32ChunkType)
        {
        case 1: return ChunkType::ChunkBase;
        case 2: return ChunkType::ByteChunk;
        case 3: return ChunkType::TimeChunk;
        case 4: return ChunkType::WAVChunk;
        case 5: return ChunkType::WatchdogChunk;
        case 6: return ChunkType::FFTChunk;
        case 7: return ChunkType::JSONChunk;
        case 8: return ChunkType::FFTMagnitudeChunk;

        default: return ChunkType::ChunkBase; // error
        }
    }
};

#endif
