#ifndef CHUNK_DUPLICATOR_UTILITY
#define CHUNK_DUPLICATOR_UTILITY

#include "BaseChunk.h"
#include "ByteChunk.h"   
#include "TimeChunk.h"
#include "WAVChunk.h"  
#include "WatchdogChunk.h"
#include "JSONChunk.h"

class ChunkDuplicatorUtility
{
public:

    static std::shared_ptr<BaseChunk> DuplicateDerivedChunk(std::shared_ptr<BaseChunk> pBaseChunk)
    {
        switch (pBaseChunk->GetChunkType())
        {
        case ChunkType::ChunkBase: return std::make_shared<BaseChunk>(pBaseChunk);
        case ChunkType::ByteChunk:  return std::make_shared<ByteChunk>(std::static_pointer_cast<ByteChunk>(pBaseChunk));
        case ChunkType::TimeChunk: return std::make_shared<TimeChunk>(std::static_pointer_cast<TimeChunk>(pBaseChunk));
        case ChunkType::WAVChunk:  return std::make_shared<WAVChunk>(std::static_pointer_cast<WAVChunk>(pBaseChunk));
        case ChunkType::WatchdogChunk:  return std::make_shared<WatchdogChunk>(std::static_pointer_cast<WatchdogChunk>(pBaseChunk));
        case ChunkType::JSONChunk:  return std::make_shared<JSONChunk>(std::static_pointer_cast<JSONChunk>(pBaseChunk));

        default:
        {
            std::cout << std::string(__FUNCTION__) + ": Unknown chunk type \n";
            return std::make_shared<BaseChunk>(BaseChunk(pBaseChunk));
        }
        }
    }

    static std::shared_ptr<BaseChunk> DeserialiseDerivedChunk(std::shared_ptr<std::vector<char>> pBytes, ChunkType eChunkType)
    {
        if (eChunkType == ChunkType::ChunkBase) {
            auto pChunk = std::make_shared<BaseChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::TimeChunk) {
            auto pChunk = std::make_shared<TimeChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::WAVChunk) {
            auto pChunk = std::make_shared<WAVChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::JSONChunk) {
            auto pChunk = std::make_shared<JSONChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else {
            std::cout << std::string(__FUNCTION__) + ": Unknown chunk type \n";
            auto pChunk = std::make_shared<BaseChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }

    }
};

#endif
