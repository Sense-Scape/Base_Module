#ifndef CHUNK_DUPLICATOR_UTILITY_TESTS
#define CHUNK_DUPLICATOR_UTILITY_TESTS

#include "doctest.h"
#include "ChunkDuplicatorUtility.h"

TEST_CASE("Chunk Duplicator Utility Test") {

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
}

#endif