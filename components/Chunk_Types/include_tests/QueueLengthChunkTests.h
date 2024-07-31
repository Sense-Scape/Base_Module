#ifndef QUEUE_LENGTH_CHUNK_TESTS
#define QUEUE_LENGTH_CHUNK_TESTS

#include "doctest.h"
#include "QueueLengthChunk.h"

TEST_CASE("QueueLengthChunk Test")
{

    // Lets just start by creating a vanilla BaseChunk
    QueueLengthChunk cQueueLengthChunk_EmptyConstructor;
    QueueLengthChunk cQueueLengthChunk_EmptyConstructor_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes_EmptuConstructor = cQueueLengthChunk_EmptyConstructor.Serialise();
    cQueueLengthChunk_EmptyConstructor_0.Deserialise(pvcBaseChunkBytes_EmptuConstructor);

    SUBCASE("Checking base chunk after using default constructor")
    {
        // And then that its type is basechunk
        CHECK(cQueueLengthChunk_EmptyConstructor.GetChunkType() == ChunkType::QueueLengthChunk);
        // lets check for equality to see if serialisation works
        CHECK(cQueueLengthChunk_EmptyConstructor.IsEqual(cQueueLengthChunk_EmptyConstructor));
    }

    // Lets just start by creating a vanilla BaseChunk
    QueueLengthChunk cQueueLengthChunk("TestQueue", 10);
    QueueLengthChunk cQueueLengthChunk_0;

    // We can also check the basic serialisation functionality
    auto pvcBaseChunkBytes = cQueueLengthChunk.Serialise();
    cQueueLengthChunk_0.Deserialise(pvcBaseChunkBytes);

    SUBCASE("Checking base chunk after using default constructor")
    {
        // And then that its type is basechunk
        CHECK(cQueueLengthChunk.GetChunkType() == ChunkType::QueueLengthChunk);
        // lets check for equality to see if serialisation works
        CHECK(cQueueLengthChunk.IsEqual(cQueueLengthChunk_0));
    }

}

#endif