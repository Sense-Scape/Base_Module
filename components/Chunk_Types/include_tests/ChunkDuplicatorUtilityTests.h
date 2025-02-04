#ifndef CHUNK_DUPLICATOR_UTILITY_TESTS
#define CHUNK_DUPLICATOR_UTILITY_TESTS

#include "doctest.h"
#include "ChunkDuplicatorUtility.h"


TEST_CASE("Chunk Duplicator Utility Test") {

	/// ===========================================
	/// 			JSON Chunk Test
	/// ===========================================

	// Create a JSON Chunk
	auto pJSONChunk = std::make_shared<JSONChunk>();
	pJSONChunk->m_JSONDocument["test"] = "confirmed";
	auto pcJSONBytes = pJSONChunk->Serialise();

	// Try Duplicate it using the duplicator deserialisation utility
	auto pJSONDeserialisedDerivedChunk = ChunkDuplicatorUtility::DeserialiseDerivedChunk(pcJSONBytes, ChunkType::JSONChunk);
	pJSONDeserialisedDerivedChunk = std::static_pointer_cast<JSONChunk>(pJSONDeserialisedDerivedChunk);

	// Try Duplicate it using the duplicator utility
	auto pJSONDuplicatedDerivedChunk = ChunkDuplicatorUtility::DuplicateDerivedChunk(pJSONChunk);
	pJSONDuplicatedDerivedChunk = std::static_pointer_cast<JSONChunk>(pJSONDuplicatedDerivedChunk);

	SUBCASE("Testing duplication and deserialisation of JSON chunk") {
		CHECK(pJSONDeserialisedDerivedChunk->IsEqual(*pJSONChunk));
		CHECK(pJSONDuplicatedDerivedChunk->IsEqual(*pJSONChunk));
	}

	/// ===========================================
	/// 			Queue Length Chunk
	/// ===========================================

	auto pQueueLengthChunk = std::make_shared<QueueLengthChunk>("TestQueue", 10);

	// Try Duplicate it using the duplicator deserialisation utility
	auto pDeserialisedDerivedChunk_QueueLengthChunk = ChunkDuplicatorUtility::DeserialiseDerivedChunk(pcJSONBytes, ChunkType::QueueLengthChunk);
	pDeserialisedDerivedChunk_QueueLengthChunk = std::static_pointer_cast<JSONChunk>(pDeserialisedDerivedChunk_QueueLengthChunk);

	// Try Duplicate it using the duplicator utility
	auto pDuplicatedDerivedChunk_QueueLengthChunk = ChunkDuplicatorUtility::DuplicateDerivedChunk(pQueueLengthChunk);
	pDuplicatedDerivedChunk_QueueLengthChunk = std::static_pointer_cast<JSONChunk>(pDuplicatedDerivedChunk_QueueLengthChunk);

	SUBCASE("Testing duplication and deserialisation of QueueLengthC chunk") {
		CHECK(pDeserialisedDerivedChunk_QueueLengthChunk->IsEqual(*pQueueLengthChunk));
		CHECK(pDuplicatedDerivedChunk_QueueLengthChunk->IsEqual(*pQueueLengthChunk));
	}
}

#endif