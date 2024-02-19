#ifndef JSON_CHUNK_TESTS
#define JSON_CHUNK_TESTS

#include "doctest.h"
#include "JSONChunk.h"

TEST_CASE("JSONChunk Test") {

    // Lets just start by creating a vanilla JSONChunk
    JSONChunk JSONChunkTestClass;
    JSONChunk JSONChunkTestClassCopy_0;

    // All the above sum to bytes below - size of class
    BaseChunk baseChunk;
    // Size of header info, size of channels and size of base class, 2 is for member json size
    unsigned uClassSize_bytes =  2 + baseChunk.GetSize();

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = JSONChunkTestClass.Serialise();
    JSONChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking JSON chunk after using default constructor") {
        // And then that its type is JSONchunk
        CHECK(JSONChunkTestClass.GetChunkType() == ChunkType::JSONChunk);
        // Then lets ensure that the size of the data contained is 2 bytes
        CHECK(JSONChunkTestClass.GetSize() == uClassSize_bytes);
        // We can also check serialisation and deserialsaation
        CHECK(JSONChunkTestClass.IsEqual(JSONChunkTestClassCopy_0));
    }


    JSONChunkTestClass.m_JSONDocument["test"] = "confirmed";

    // We can also check the basic serialisation functionality
    pvcBaseChunkBytes = JSONChunkTestClass.Serialise();
    JSONChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking JSON chunk after Adjusting JSON Document") {
        // And then that its type is JSONchunk
        CHECK(JSONChunkTestClass.GetChunkType() == ChunkType::JSONChunk);
        // We can also check serialisation and deserialsaation
        CHECK(JSONChunkTestClass.IsEqual(JSONChunkTestClassCopy_0));
    }
}

#endif