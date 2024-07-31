#ifndef CHUNK_DUPLICATOR_UTILITY
#define CHUNK_DUPLICATOR_UTILITY

#include "BaseChunk.h"
#include "ByteChunk.h"   
#include "TimeChunk.h"
#include "WAVChunk.h"  
#include "WatchdogChunk.h"
#include "JSONChunk.h"
#include "GPSChunk.h"
#include "DirectionBinChunk.h"
#include "DetectionBinChunk.h"
#include "QueueLengthChunk.h"

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
        case ChunkType::GPSChunk:  return std::make_shared<GPSChunk>(std::static_pointer_cast<GPSChunk>(pBaseChunk));
        case ChunkType::DetectionBinChunk:  return std::make_shared<DetectionBinChunk>(std::static_pointer_cast<DetectionBinChunk>(pBaseChunk));
        case ChunkType::DirectionBinChunk:  return std::make_shared<DirectionBinChunk>(std::static_pointer_cast<DirectionBinChunk>(pBaseChunk));
        case ChunkType::QueueLengthChunk:  return std::make_shared<QueueLengthChunk>(std::static_pointer_cast<QueueLengthChunk>(pBaseChunk));

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
        else if (eChunkType == ChunkType::GPSChunk) {
            auto pChunk = std::make_shared<GPSChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::DetectionBinChunk) {
            auto pChunk = std::make_shared<DetectionBinChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::DirectionBinChunk) {
            auto pChunk = std::make_shared<DirectionBinChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else if (eChunkType == ChunkType::QueueLengthChunk) {
            auto pChunk = std::make_shared<QueueLengthChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }
        else {
            std::cout << std::string(__FUNCTION__) + ": Unknown chunk type: " + std::to_string(ChunkTypesNamingUtility::ToU32(eChunkType))+ " " + ChunkTypesNamingUtility::toString(eChunkType) +" \n";
            auto pChunk = std::make_shared<BaseChunk>();
            pChunk->Deserialise(pBytes);
            return pChunk;
        }

    }
};

#endif
