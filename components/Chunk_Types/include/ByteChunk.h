#ifndef Byte_CHUNK
#define	Byte_CHUNK

/* Standard Includes */
#include <vector>

/* Custom Includes */
#include "BaseChunk.h"

/**
 * @brief Byte Data Chunk used to store Byte datagram data to pass between modules
 */
class ByteChunk : public BaseChunk
{
public:
    unsigned m_uChunkLength;                ///< Length of Byte data
    std::vector<char> m_vcDataChunk;        ///< Vector of Byte data

    /**
     * @brief Construct a new Time Chunk object
     * @param[in] uChunkLength bytes to store in Byte Chunk
     */
    ByteChunk(unsigned uChunkLength);
    ByteChunk(std::shared_ptr<ByteChunk> pByteChunk);
    ~ByteChunk() {};

    /**
     * @brief Get the Chunk Type object
     * @return ChunkType Chunk type of the chunk
     */
    virtual ChunkType GetChunkType() override { return ChunkType::ByteChunk; };
};

#endif
