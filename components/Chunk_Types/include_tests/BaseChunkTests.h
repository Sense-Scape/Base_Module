#ifndef BASE_CHUNK_TESTS
#define BASE_CHUNK_TESTS

#include "doctest.h"
#include "BaseChunk.h"

TEST_CASE("BaseChunk Test") {

    // Lets just start by creating a vanilla BaseChunk
    BaseChunk BaseChunkTestClass;
    BaseChunk BaseChunkTestClassCopy_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = BaseChunkTestClass.Serialise();
    BaseChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking base chunk after using default constructor") {
        // And then that its type is basechunk
        CHECK(BaseChunkTestClass.GetChunkType() == ChunkType::ChunkBase);
        // Then lets ensure that the size of the data contained is 2 bytes
        CHECK(BaseChunkTestClass.GetSize() == 2);
        // lets check for equality to see if serialisation works
        CHECK(BaseChunkTestClass.IsEqual(BaseChunkTestClassCopy_0));
    }


    // Lets now give the device a unique identifier and see if the size if correct
    std::vector<uint8_t> vu8Indentidier = { 255, 255, 255 };
    BaseChunkTestClass.SetSourceIdentifier(vu8Indentidier);
    // We can also check serialisation again
    pvcBaseChunkBytes = BaseChunkTestClass.Serialise();
    BaseChunkTestClassCopy_0.Deserialise(pvcBaseChunkBytes);


    SUBCASE("Checking source identifier usage - size, member value and serialisation") {
        // Then lets ensure that the size of the data contained is 2 bytes
        CHECK(BaseChunkTestClass.GetSize() == 5);
        // We can also check if the vector was set correctly
        CHECK(BaseChunkTestClass.GetSourceIdentifier() == vu8Indentidier);
        // lets check for equality to see if serialisation works
        CHECK(BaseChunkTestClass.IsEqual(BaseChunkTestClassCopy_0));
    }

    SUBCASE("Checking Other functionality") {
        /// Lets check if we can convert the source identifier to something easily readable
        CHECK(BaseChunkTestClass.GetSourceIdentifierAsString() == "255:255:255");
    }

    // We can also go ahead and check if the copy constructor works
    BaseChunk BaseChunkTestClassCopy_1(BaseChunkTestClass);
    std::shared_ptr<BaseChunk> pBaseChunkTestClassCopy_2 = std::make_shared<BaseChunk>(BaseChunkTestClass);


    SUBCASE("Checking copy constructors using reference and pointer") {
        // checking copy by reference
        CHECK(BaseChunkTestClass.IsEqual(BaseChunkTestClassCopy_1));
        // and then by pointer
        CHECK(BaseChunkTestClass.IsEqual(*pBaseChunkTestClassCopy_2));
    }
}

#endif