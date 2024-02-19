#include "ByteChunk.h"

ByteChunk::ByteChunk(unsigned m_uChunkLength) : BaseChunk(),
												m_uChunkLength(m_uChunkLength),
												m_vcDataChunk()
{
	m_vcDataChunk.resize(m_uChunkLength);
}

ByteChunk::ByteChunk(std::shared_ptr<ByteChunk> pByteChunk): BaseChunk(pByteChunk)
{
	m_uChunkLength = pByteChunk->m_uChunkLength;
	m_vcDataChunk = pByteChunk->m_vcDataChunk;
}