#ifndef FFT_MAGNITUDE_CHUNK_TESTS
#define FFT_MAGNITUDE_CHUNK_TESTS

#include "doctest.h"
#include "FFTMagnitudeChunk.h"

TEST_CASE("FFTMagnitudeChunk Test") {

    // Constructor Parameters
    double dChunkSize = 512;
    double dSampleRate = 44100;
    uint64_t i64TimeStamp = 100000;
    unsigned uNumChannels = 2;

    std::vector<float> vfChannelOne;
    vfChannelOne.assign(dChunkSize, 1);
    std::vector<float> vfChannelTwo;
    vfChannelTwo.assign(dChunkSize, 2);
    // All the above sum to bytes below - size of class
    BaseChunk baseChunk;
    // Size of header info, size of channels and size of base class
    unsigned uClassSize_bytes = 36 + dChunkSize * uNumChannels* sizeof(float) + baseChunk.GetSize();

    // Lets just start by creating a FFTMagnitudeChunk
    FFTMagnitudeChunk FFTMagnitudeChunkTestClass(dChunkSize, dSampleRate, i64TimeStamp, uNumChannels);
    FFTMagnitudeChunkTestClass.m_vvfFFTMagnitudeChunks[0] = vfChannelOne;
    FFTMagnitudeChunkTestClass.m_vvfFFTMagnitudeChunks[1] = vfChannelTwo;

    FFTMagnitudeChunk FFTMagnitudeChunkTestClassCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = FFTMagnitudeChunkTestClass.Serialise();
    FFTMagnitudeChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking base chunk functionality") {
        // And then that its type is basechunk
        CHECK(FFTMagnitudeChunkTestClass.GetChunkType() == ChunkType::FFTMagnitudeChunk);
        // Then lets ensure that the size of the data contained is 0 bytes
        CHECK(FFTMagnitudeChunkTestClass.GetSize() == uClassSize_bytes);
        // We can also check serialisation and deserialsaation
        CHECK(FFTMagnitudeChunkTestClass.IsEqual(FFTMagnitudeChunkTestClassCopy_0));
    }


    // With basic checks done, lets check the copy constructor works
    FFTMagnitudeChunk CopyOfFFTMagnitudeChunkTestClass = FFTMagnitudeChunkTestClass;


    SUBCASE("Checking copy constructors") {
        CHECK(FFTMagnitudeChunkTestClass.IsEqual(FFTMagnitudeChunkTestClass));
        // Now check if we change something in base class that the classes are no longer equal
        CopyOfFFTMagnitudeChunkTestClass.SetSourceIdentifier({ 1, 1 });
        CHECK(FFTMagnitudeChunkTestClass.IsEqual(CopyOfFFTMagnitudeChunkTestClass) == false);
    }


    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(ChunkType::FFTMagnitudeChunk);
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(0);
    JSONDocument[strChunkName]["SourceIdentifier"] = std::vector<uint8_t>();
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(i64TimeStamp);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(uNumChannels);
    JSONDocument[strChunkName]["Channels"][std::to_string(0)] = vfChannelOne;
    JSONDocument[strChunkName]["Channels"][std::to_string(1)] = vfChannelTwo;

    SUBCASE("Checking ToJSON Converter") {
        CHECK(*(FFTMagnitudeChunkTestClass.ToJSON()) == JSONDocument);
    }
}

#endif