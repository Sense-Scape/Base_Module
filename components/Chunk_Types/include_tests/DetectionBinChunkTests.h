#ifndef DETECTION_BIN_CHUNK_TESTS
#define DETECTION_BIN_CHUNK_TESTS

#include "doctest.h"
#include "DetectionBinChunk.h"

TEST_CASE("DetectionBinChunk Test") {

    DetectionBinChunk cDetetionBinChunk = DetectionBinChunk();
    std::vector<std::vector<uint16_t>> DetectionBins = {{1,2,3},{3,4,5}};
    cDetetionBinChunk.SetDetectionBins(DetectionBins);

    BaseChunk baseChunk;
    // 2 for numChan, 2 for detections per chan, 12 for each detection
    unsigned uClassSize_bytes = 2 + 4 + 12 + baseChunk.GetSize();

    SUBCASE("Checking Getter and General functionality") {
        // And then that its type is basechunk
        std::vector<u_int16_t> vu16ExpectedetectionBinsPerChannel = {3,3};
        CHECK(cDetetionBinChunk.GetChunkType() == ChunkType::DetectionBinChunk);
        CHECK(cDetetionBinChunk.GetNumChannels() == 2);
        CHECK(*cDetetionBinChunk.GetDetectedBinsPerChannel() == vu16ExpectedetectionBinsPerChannel);
        CHECK(*cDetetionBinChunk.GetDetectionBins() == DetectionBins);
        CHECK(cDetetionBinChunk.GetSize() == uClassSize_bytes);
    }


    DetectionBinChunk DetetionBinChunkCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = cDetetionBinChunk.Serialise();
    DetetionBinChunkCopy_0.Deserialise(pvcBaseChunkBytes);

    SUBCASE("Checking copy constructors") {
        CHECK(DetetionBinChunkCopy_0.IsEqual(cDetetionBinChunk));
        // Now check if we change something in base class that the classes are no longer equal
        DetetionBinChunkCopy_0.SetSourceIdentifier({ 1, 1 });
        CHECK(DetetionBinChunkCopy_0.IsEqual(cDetetionBinChunk) == false);
    }


    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(ChunkType::DetectionBinChunk);
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(0);
    JSONDocument[strChunkName]["SourceIdentifier"] = std::vector<uint8_t>();
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(cDetetionBinChunk.GetNumChannels());
    JSONDocument[strChunkName]["DetectedBinsPerChannel"] = *cDetetionBinChunk.GetDetectedBinsPerChannel();
    JSONDocument[strChunkName]["DetectionBins"]["0"] = (*cDetetionBinChunk.GetDetectionBins())[0];
    JSONDocument[strChunkName]["DetectionBins"]["1"] = (*cDetetionBinChunk.GetDetectionBins())[1];


    SUBCASE("Checking ToJSON Converter") {
        CHECK((*cDetetionBinChunk.ToJSON()) == JSONDocument);
    }
}

#endif