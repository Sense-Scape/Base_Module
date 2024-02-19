#ifndef FFT_CHUNK_TESTS
#define FFT_CHUNK_TESTS

#include "doctest.h"
#include "FFTChunk.h"

TEST_CASE("FFTChunk Test") {

    // Constructor Parameters
    double dChunkSize = 2;
    double dSampleRate = 44100;
    uint64_t i64TimeStamp = 100000;
    unsigned uNumChannels = 3;

    std::vector<std::complex<float>> vu16ChannelOne;
    vu16ChannelOne.assign(dChunkSize, 1);
    auto strChannelOne = "1 0,1 0";
    std::vector<std::complex<float>> vu16ChannelTwo;
    vu16ChannelTwo.assign(dChunkSize, 2);
    auto strChannelTwo = "2 0,2 0";
    std::vector<std::complex<float>> vu16ChannelThree;
    vu16ChannelThree.assign(dChunkSize, std::complex<float>(2,2));
    auto strChannelThree = "2 2,2 2";

    // All the above sum to bytes below - size of class
    BaseChunk baseChunk;
    // Size of header info, size of channels and size of base class
    unsigned uClassSize_bytes = 28 + dChunkSize * uNumChannels * sizeof(std::complex<float>) + baseChunk.GetSize();

    // Lets just start by creating a FFTChunk
    FFTChunk FFTChunkTestClass(dChunkSize, dSampleRate, i64TimeStamp, uNumChannels);
    FFTChunkTestClass.m_vvcfFFTChunks[0] = vu16ChannelOne;
    FFTChunkTestClass.m_vvcfFFTChunks[1] = vu16ChannelTwo;
    FFTChunkTestClass.m_vvcfFFTChunks[2] = vu16ChannelThree;

    FFTChunk FFTChunkTestClassCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = FFTChunkTestClass.Serialise();
    FFTChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking base chunk functionality") {
        // And then that its type is basechunk
        CHECK(FFTChunkTestClass.GetChunkType() == ChunkType::FFTChunk);
        // Then lets ensure that the size of the data contained is 0 bytes
        CHECK(FFTChunkTestClass.GetSize() == uClassSize_bytes);
        // We can also check serialisation and deserialsaation
        CHECK(FFTChunkTestClass.IsEqual(FFTChunkTestClassCopy_0));
    }


    auto pvfChannelOnePower = FFTChunkTestClass.GetChannelPower(0);
    auto pvfChannelTwoPower = FFTChunkTestClass.GetChannelPower(1);
    auto pvfChannelThreePower = FFTChunkTestClass.GetChannelPower(2);


    SUBCASE("Checking Power Conversion") {
        CHECK((*pvfChannelOnePower)[0] - 1  <= 0.00001f);
        CHECK((*pvfChannelTwoPower)[0] - 2 <= 0.00001f);
        CHECK((*pvfChannelThreePower)[0] - std::sqrt(2*2 + 2*2) <= 0.00001f);
    }


    // With basic checks done, lets check the copy constructor works
    FFTChunk CopyOfFFTChunkTestClass = FFTChunkTestClass;


    SUBCASE("Checking copy constructors") {
        CHECK(FFTChunkTestClass.IsEqual(FFTChunkTestClass));
        // Now check if we change something in base class that the classes are no longer equal
        CopyOfFFTChunkTestClass.SetSourceIdentifier({ 1, 1 });
        CHECK(FFTChunkTestClass.IsEqual(CopyOfFFTChunkTestClass) == false);
    }


    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(ChunkType::FFTChunk);
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(0);
    JSONDocument[strChunkName]["SourceIdentifier"] = std::vector<uint8_t>();
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(i64TimeStamp);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(uNumChannels);
    JSONDocument[strChunkName]["Channels"][std::to_string(0)] = strChannelOne;
    JSONDocument[strChunkName]["Channels"][std::to_string(1)] = strChannelTwo;
    JSONDocument[strChunkName]["Channels"][std::to_string(2)] = strChannelThree;

    SUBCASE("Checking ToJSON Converter") {
        CHECK(*(FFTChunkTestClass.ToJSON()) == JSONDocument);
    }


}

#endif