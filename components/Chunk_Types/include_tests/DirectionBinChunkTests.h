#ifndef DIRECTION_BIN_CHUNK_TESTS
#define DIRECTION_BIN_CHUNK_TESTS

#include "doctest.h"
#include "DirectionBinChunk.h"

TEST_CASE("DirectionBinChunk Test") {

    DirectionBinChunk cDirectionBinChunk = DirectionBinChunk();

    uint16_t u16NumberOfDetection = 2;
    std::vector<uint16_t> vfDetectionIndicies = {0,1};
    std::vector<float> vfDetectionAngles_deg = {0,2}; 
    double dSampleRate_hz = 100;
    cDirectionBinChunk.SetDirectionData(u16NumberOfDetection, vfDetectionIndicies, vfDetectionAngles_deg, dSampleRate_hz);

    BaseChunk baseChunk;

    // 2 for u16NumberOfDetection, 2 for detections per chan, 12 for each detection
    unsigned uClassSize_bytes = 2 + 2*2 + 4*2 + 8 + baseChunk.GetSize();

    SUBCASE("Checking General functionality") {
        CHECK(cDirectionBinChunk.GetChunkType() == ChunkType::DirectionBinChunk);
        CHECK(cDirectionBinChunk.GetSize() == uClassSize_bytes);
    }

    SUBCASE("Checking Getters functionality") {
        CHECK(cDirectionBinChunk.GetDetectionAngles() == vfDetectionAngles_deg);
        CHECK(cDirectionBinChunk.GetDetectionFrequencyIndicies() == vfDetectionIndicies);
        CHECK(cDirectionBinChunk.GetNumberOfDetections() == u16NumberOfDetection);
    }


    DirectionBinChunk cDirectionBinChunkCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = cDirectionBinChunk.Serialise();
    cDirectionBinChunkCopy_0.Deserialise(pvcBaseChunkBytes);

    SUBCASE("Checking copy constructors") {
        CHECK(cDirectionBinChunkCopy_0.IsEqual(cDirectionBinChunk));
        // Now check if we change something in base class that the classes are no longer equal
        cDirectionBinChunkCopy_0.SetSourceIdentifier({ 1, 1 });
        CHECK(cDirectionBinChunkCopy_0.IsEqual(cDirectionBinChunk) == false);
    }

    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(ChunkType::DirectionBinChunk);
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(0);
    JSONDocument[strChunkName]["SourceIdentifier"] = std::vector<uint8_t>();
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(cDirectionBinChunk.GetSampleRate());
    JSONDocument[strChunkName]["NumberOfDetections"] = std::to_string(cDirectionBinChunk.GetNumberOfDetections());
    JSONDocument[strChunkName]["DetectionFrequencyIndicies"] = cDirectionBinChunk.GetDetectionFrequencyIndicies();
    JSONDocument[strChunkName]["DetectionAngles_deg"] = cDirectionBinChunk.GetDetectionAngles();


    SUBCASE("Checking ToJSON Converter") {
        CHECK((*cDirectionBinChunk.ToJSON()) == JSONDocument);
    }
}

#endif