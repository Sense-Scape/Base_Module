#ifndef COORDINATE_CHUNK_TESTS
#define COORDINATE_CHUNK_TESTS

#include "doctest.h"
#include "CoordinateChunk.h"

TEST_CASE("CoordinateChunk Test") {

    // Lets just start by creating a vanilla BaseChunk
    CoordinateChunk CoordinateChunkTestClass;
    CoordinateChunk CoordinateChunkTestClassCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = CoordinateChunkTestClass.Serialise();
    CoordinateChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking base chunk after using default constructor") {
        // And then that its type is basechunk
        CHECK(CoordinateChunkTestClass.GetChunkType() == ChunkType::CoordinateChunk);
        // lets check for equality to see if serialisation works
        CHECK(CoordinateChunkTestClass.IsEqual(CoordinateChunkTestClassCopy_0));
        // Then lets ensure that the size of the data contained is 2 bytes
        CHECK(CoordinateChunkTestClass.GetSize() == 26);
    }


    // Lets now give the device a unique identifier and see if the size if correct
    std::vector<uint8_t> vu8Indentidier = { 255, 255, 255 };
    CoordinateChunkTestClass.SetSourceIdentifier(vu8Indentidier);

    CoordinateChunkTestClass.m_dLatitude = 100.005;
    CoordinateChunkTestClass.m_dLatitude = -140.78;
    CoordinateChunkTestClass.m_i64TimeStamp = 5000984;

    // We can also check serialisation again
    pvcBaseChunkBytes = CoordinateChunkTestClass.Serialise();
    CoordinateChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking source identifier usage - size, member value and serialisation") {
        
        // We can also check if the vector was set correctly
        CHECK(CoordinateChunkTestClass.GetSourceIdentifier() == vu8Indentidier);
        // lets check for equality to see if serialisation works
        CHECK(CoordinateChunkTestClass.IsEqual(CoordinateChunkTestClassCopy_0));
    }

    SUBCASE("Checking Other functionality") {
        /// Lets check if we can convert the source identifier to something easily readable
        CHECK(CoordinateChunkTestClass.GetSourceIdentifierAsString() == "255:255:255");
    }

    // We can also go ahead and check if the copy constructor works
    CoordinateChunk CoordinateChunkTestClassCopy_1(CoordinateChunkTestClass);
    std::shared_ptr<CoordinateChunk> pCoordinateChunkTestClassCopy_2 = std::make_shared<CoordinateChunk>(CoordinateChunkTestClass);


    SUBCASE("Checking copy constructors using reference and pointer") {
        // checking copy by reference
        CHECK(CoordinateChunkTestClass.IsEqual(CoordinateChunkTestClassCopy_1));
        // and then by pointer
        CHECK(CoordinateChunkTestClass.IsEqual(*pCoordinateChunkTestClassCopy_2));
    }
}

#endif