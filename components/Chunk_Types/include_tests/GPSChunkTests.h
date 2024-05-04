#ifndef GPS_CHUNK_TESTS
#define GPS_CHUNK_TESTS

#include "doctest.h"
#include "GPSChunk.h"

TEST_CASE("GPSChunk Test")
{

    // Lets just start by creating a vanilla BaseChunk
    GPSChunk GPSChunkTestClass;
    GPSChunk GPSChunkTestClassCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = GPSChunkTestClass.Serialise();
    GPSChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);

    SUBCASE("Checking base chunk after using default constructor")
    {
        // And then that its type is basechunk
        CHECK(GPSChunkTestClass.GetChunkType() == ChunkType::GPSChunk);
        // lets check for equality to see if serialisation works
        CHECK(GPSChunkTestClass.IsEqual(GPSChunkTestClassCopy_0));
        // Then lets ensure that the size of the data contained is 2 bytes
        CHECK(GPSChunkTestClass.GetSize() == 28);
    }

    // Lets now give the device a unique identifier and see if the size if correct
    std::vector<uint8_t> vu8Indentidier = {255, 255, 255};
    GPSChunkTestClass.SetSourceIdentifier(vu8Indentidier);

    GPSChunkTestClass.m_dLatitude = 100.005;
    GPSChunkTestClass.m_dLatitude = -140.78;
    GPSChunkTestClass.m_i64TimeStamp = 5000984;
    GPSChunkTestClass.m_bIsNorth = true;
    GPSChunkTestClass.m_bIsWest = false;

    // We can also check serialisation again
    pvcBaseChunkBytes = GPSChunkTestClass.Serialise();
    GPSChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);

    SUBCASE("Checking source identifier usage - size, member value and serialisation")
    {

        // We can also check if the vector was set correctly
        CHECK(GPSChunkTestClass.GetSourceIdentifier() == vu8Indentidier);
        // lets check for equality to see if serialisation works
        CHECK(GPSChunkTestClass.IsEqual(GPSChunkTestClassCopy_0));
    }

    SUBCASE("Checking Other functionality")
    {
        /// Lets check if we can convert the source identifier to something easily readable
        CHECK(GPSChunkTestClass.GetSourceIdentifierAsString() == "255:255:255");
    }

    // We can also go ahead and check if the copy constructor works
    GPSChunk GPSChunkTestClassCopy_1(GPSChunkTestClass);
    std::shared_ptr<GPSChunk> pGPSChunkTestClassCopy_2 = std::make_shared<GPSChunk>(GPSChunkTestClass);

    SUBCASE("Checking copy constructors using reference and pointer")
    {
        // checking copy by reference
        CHECK(GPSChunkTestClass.IsEqual(GPSChunkTestClassCopy_1));
        // and then by pointer
        CHECK(GPSChunkTestClass.IsEqual(*pGPSChunkTestClassCopy_2));
    }
}

#endif