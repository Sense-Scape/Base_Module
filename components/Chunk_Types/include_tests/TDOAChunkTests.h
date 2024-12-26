#ifndef TDOA_CHUNK_TESTS
#define TDOA_CHUNK_TESTS

#include "doctest.h"
#include "TDOAChunk.h"

TEST_CASE("TDOA Test") {

    double dSampleRate = 16000;
    uint64_t i64TimeStamp_us = 0;
    uint64_t u64ChunkSize = 512;

    BaseChunk baseChunk;

    // sample rate
    // timestamps
    // chunk size
    // num sources
    uint32_t u32ChunkClassSize = 8 + 8 + 8 + 2 + baseChunk.GetSize();;

    auto pTDOAChunk = std::make_shared<TDOAChunk>(dSampleRate, i64TimeStamp_us, u64ChunkSize);

    SUBCASE("Checking constructors") {
        CHECK(pTDOAChunk->m_dSampleRate == dSampleRate);
        CHECK(pTDOAChunk->m_i64TimeStamp_us == i64TimeStamp_us);
        CHECK(pTDOAChunk->m_u64ChunkSize == u64ChunkSize);
        CHECK(pTDOAChunk->GetSize() == u32ChunkClassSize);
    }

    auto pvv16Data = std::make_shared<std::vector<std::vector<int16_t>>>();
    pvv16Data->resize(2);
    pvv16Data->at(0).resize(u64ChunkSize);
    pvv16Data->at(1).resize(u64ChunkSize);
    pTDOAChunk->AddData(1,1,{1,1},*pvv16Data);

    // () channel data size
    // () long lat size
    // () num source identifier size
    // () channels per source
    // () source identidier sizes size
    u32ChunkClassSize += (2*512*2) + (8 + 8) + (1) + (1) + (1*2);
    SUBCASE("Checking size") {
        CHECK(pTDOAChunk->GetSize() == u32ChunkClassSize);
    }

    auto pvv16Data2 = std::make_shared<std::vector<std::vector<int16_t>>>();
    pvv16Data2->resize(3);
    pvv16Data2->at(0).resize(u64ChunkSize);
    pvv16Data2->at(1).resize(u64ChunkSize);
    pvv16Data2->at(2).resize(u64ChunkSize);
    pTDOAChunk->AddData(1,2,{1,2,3},*pvv16Data2);

    // () channel data size
    // () long lat size
    // () num source identifier size
    // () channels per source
    // () source identidier sizes size
    u32ChunkClassSize += (2*512*3) + (8 + 8) + (1) + (1) + (1*3);
    SUBCASE("Checking size again") {
        CHECK(pTDOAChunk->GetSize() == u32ChunkClassSize);
    }

    SUBCASE("Checking IsEqual functionality") {
        // Test equality with itself
        CHECK(pTDOAChunk->IsEqual(*pTDOAChunk));

        // Test equality with a copy
        auto pTDOAChunkCopy = std::make_shared<TDOAChunk>(*pTDOAChunk);
        CHECK(pTDOAChunk->IsEqual(*pTDOAChunkCopy));

        // Test inequality when base class changes
        pTDOAChunkCopy->SetSourceIdentifier({1, 1});
        CHECK(pTDOAChunk->IsEqual(*pTDOAChunkCopy) == false);

        // Test inequality when TDOA specific members change
        pTDOAChunkCopy = std::make_shared<TDOAChunk>(*pTDOAChunk);
        pTDOAChunkCopy->m_dSampleRate = 48000;
        CHECK(pTDOAChunk->IsEqual(*pTDOAChunkCopy) == false);
    }

    SUBCASE("Checking serialisation and deserialisation") {
        // Create a new chunk to deserialize into
        auto pTDOAChunkDeserialized = std::make_shared<TDOAChunk>();
        
        // Serialize original chunk
        auto pvcBytes = pTDOAChunk->Serialise();
        
        // Deserialize into new chunk
        pTDOAChunkDeserialized->Deserialise(pvcBytes);
        
        // Check if the chunks are equal after serialization/deserialization
        CHECK(pTDOAChunk->IsEqual(*pTDOAChunkDeserialized));
    }
}

#endif