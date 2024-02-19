#ifndef TIME_CHUNK_TESTS
#define TIME_CHUNK_TESTS

#include "doctest.h"
#include "TimeChunk.h"

TEST_CASE("TimeChunk Test") {

    // Constructor Parameters
    double dChunkSize = 512;
    double dSampleRate = 44100;
    uint64_t i64TimeStamp = 100000;
    unsigned uBits = 16;
    unsigned uNumBytes = 2;
    unsigned uNumChannels = 2;

    std::vector<int16_t> vu16ChannelOne;
    vu16ChannelOne.assign(dChunkSize, 1);
    std::vector<int16_t> vu16ChannelTwo;
    vu16ChannelTwo.assign(dChunkSize, 2);
    // All the above sum to bytes below - size of class
    BaseChunk baseChunk;
    // Size of header info, size of channels and size of base class
    unsigned uClassSize_bytes = 36 + dChunkSize * uNumChannels * uNumBytes + baseChunk.GetSize();

    // Lets just start by creating a Timechunk
    TimeChunk TimeChunkTestClass(dChunkSize, dSampleRate, i64TimeStamp, uBits, uNumBytes, uNumChannels);
    TimeChunkTestClass.m_vvi16TimeChunks[0] = vu16ChannelOne;
    TimeChunkTestClass.m_vvi16TimeChunks[1] = vu16ChannelTwo;
    
    TimeChunk TimeChunkTestClassCopy_0;
    
    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = TimeChunkTestClass.Serialise();
    TimeChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking base chunk functionality") {
        // And then that its type is basechunk
        CHECK(TimeChunkTestClass.GetChunkType() == ChunkType::TimeChunk);
        // Then lets ensure that the size of the data contained is 0 bytes
        CHECK(TimeChunkTestClass.GetSize() == uClassSize_bytes);
        // We can also check serialisation and deserialsaation
        CHECK(TimeChunkTestClass.IsEqual(TimeChunkTestClassCopy_0));
    }


    // With basic checks done, lets check the copy constructor works
    TimeChunk CopyOfTimeChunkTestClass = TimeChunkTestClass;


    SUBCASE("Checking copy constructors") {
        CHECK(TimeChunkTestClass.IsEqual(TimeChunkTestClass));
        // Now check if we change something in base class that the classes are no longer equal
        CopyOfTimeChunkTestClass.SetSourceIdentifier({ 1, 1 });
        CHECK(TimeChunkTestClass.IsEqual(CopyOfTimeChunkTestClass) == false);
    }
    

    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(ChunkType::TimeChunk);
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(0);
    JSONDocument[strChunkName]["SourceIdentifier"] = std::vector<uint8_t>();
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(i64TimeStamp);
    JSONDocument[strChunkName]["uBits"] = std::to_string(uBits);
    JSONDocument[strChunkName]["NumBytes"] = std::to_string(uNumBytes);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(uNumChannels);
    JSONDocument[strChunkName]["Channels"][std::to_string(0)] = vu16ChannelOne;
    JSONDocument[strChunkName]["Channels"][std::to_string(1)] = vu16ChannelTwo;

    SUBCASE("Checking ToJSON Converter") {
        CHECK(*(TimeChunkTestClass.ToJSON()) == JSONDocument);
    }
}

#endif