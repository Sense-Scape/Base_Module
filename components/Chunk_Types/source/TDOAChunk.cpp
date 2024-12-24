#include "TDOAChunk.h"

TDOAChunk::TDOAChunk(double dSampleRate, uint64_t i64TimeStamp_us,uint64_t u64ChunkSize) :
    BaseChunk(),
    m_dSampleRate(dSampleRate),
    m_i64TimeStamp_us(i64TimeStamp_us),
    m_u64ChunkSize(u64ChunkSize),
    m_u16NumberSources(0)
{

}

TDOAChunk::TDOAChunk(std::shared_ptr<TDOAChunk> pTDOAChunk) :
    BaseChunk(pTDOAChunk)
{
    m_dSampleRate = pTDOAChunk->m_dSampleRate;
    m_i64TimeStamp_us = pTDOAChunk->m_i64TimeStamp_us;
    m_u64ChunkSize = pTDOAChunk->m_u64ChunkSize;
    m_u16NumberSources = pTDOAChunk->m_u16NumberSources;
    m_vvvi16TimeChunks = pTDOAChunk->m_vvvi16TimeChunks;
    m_vvu8SourceIdentifiers = pTDOAChunk->m_vvu8SourceIdentifiers;
    m_vu8SourceIdentifierSizes = pTDOAChunk->m_vu8SourceIdentifierSizes;
    m_vu8ChannelsPerSource = pTDOAChunk->m_vu8ChannelsPerSource;
    m_vdLongitude = pTDOAChunk->m_vdLongitude;
    m_vdLatitude = pTDOAChunk->m_vdLatitude;
}

TDOAChunk::TDOAChunk(const TDOAChunk& TdoaChunk) : 
    BaseChunk(TdoaChunk)
{
    m_dSampleRate = TdoaChunk.m_dSampleRate;
    m_i64TimeStamp_us = TdoaChunk.m_i64TimeStamp_us;
    m_u64ChunkSize = TdoaChunk.m_u64ChunkSize;
    m_u16NumberSources = TdoaChunk.m_u16NumberSources;
    m_vvvi16TimeChunks = TdoaChunk.m_vvvi16TimeChunks;
    m_vvu8SourceIdentifiers = TdoaChunk.m_vvu8SourceIdentifiers;
    m_vu8SourceIdentifierSizes = TdoaChunk.m_vu8SourceIdentifierSizes;
    m_vu8ChannelsPerSource = TdoaChunk.m_vu8ChannelsPerSource;
    m_vdLongitude = TdoaChunk.m_vdLongitude;
    m_vdLatitude = TdoaChunk.m_vdLatitude;
}

u_int64_t  TDOAChunk::GetSize() 
{
    return GetInternalSize();
}

void TDOAChunk::AddData(double dLong, double dLat, std::vector<uint8_t> vu8SourceIdentifier, std::vector<std::vector<int16_t>> &vvi16Data)
{
    m_vvu8SourceIdentifiers.emplace_back(vu8SourceIdentifier);
    m_vu8SourceIdentifierSizes.emplace_back(vu8SourceIdentifier.size());

    m_vdLongitude.emplace_back(dLong);
    m_vdLatitude.emplace_back(dLat);
    m_vu8ChannelsPerSource.emplace_back(vvi16Data.size());
    m_vvvi16TimeChunks.emplace_back(vvi16Data);
    m_u16NumberSources++;
}

u_int64_t  TDOAChunk::GetInternalSize()
{
    u_int64_t  uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += sizeof(m_dSampleRate);
    uByteSize += sizeof(m_i64TimeStamp_us);
    uByteSize += sizeof(m_u64ChunkSize);
    uByteSize += sizeof(m_u16NumberSources);
    
    uByteSize += m_u16NumberSources*sizeof(m_vdLongitude[0]);
    uByteSize += m_u16NumberSources*sizeof(m_vdLatitude[0]);
    uByteSize += m_u16NumberSources*sizeof(m_vu8ChannelsPerSource[0]);
    uByteSize += m_u16NumberSources*sizeof(m_vu8SourceIdentifierSizes[0]);
    
    // Adding source identifiers
    for (size_t i = 0; i < m_u16NumberSources; i++)
        uByteSize += m_vvu8SourceIdentifiers[i].size()*sizeof(m_vvu8SourceIdentifiers[i][0]);

    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        auto numChans = m_vu8ChannelsPerSource[i];
        uByteSize += m_u64ChunkSize * numChans * sizeof(m_vvvi16TimeChunks[0][0][0]);
    }

    return uByteSize;
}

std::shared_ptr<std::vector<char>> TDOAChunk::Serialise()
{  
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> TDOAChunk::GetInternalSerialisation()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char* pcBytes = pvBytes->data();

    // Copy base data
    auto pcvBaseBytes = BaseChunk::Serialise();
    unsigned uBaseClassSize = BaseChunk::GetSize();
    memcpy(pcBytes, &pcvBaseBytes->front(), uBaseClassSize);
    pcBytes += uBaseClassSize;

    
    // Converting members to bytes
    memcpy(pcBytes, &m_dSampleRate, sizeof(m_dSampleRate));
    pcBytes += sizeof(m_dSampleRate);

    memcpy(pcBytes, &m_i64TimeStamp_us, sizeof(m_i64TimeStamp_us));
    pcBytes += sizeof(m_i64TimeStamp_us);

    memcpy(pcBytes, &m_u64ChunkSize, sizeof(m_u64ChunkSize));
    pcBytes += sizeof(m_u64ChunkSize);

    memcpy(pcBytes, &m_u16NumberSources, sizeof(m_u16NumberSources));
    pcBytes += sizeof(m_u16NumberSources);

    auto count = 0;
    for (size_t i = 0; i < m_vu8SourceIdentifierSizes.size(); i++)
    {
        unsigned uChunkSizeBytes = sizeof(m_vu8SourceIdentifierSizes[i]);
        memcpy(pcBytes, &m_vu8SourceIdentifierSizes[i], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;

        uChunkSizeBytes = sizeof(m_vvu8SourceIdentifiers[i])*m_vu8SourceIdentifierSizes[i];
        memcpy(pcBytes, &m_vvu8SourceIdentifiers[i], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }
    

    for (const auto& u8NumChannels : m_vu8ChannelsPerSource)
    {
        unsigned uChunkSizeBytes = sizeof(u8NumChannels);
        memcpy(pcBytes, &u8NumChannels, uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    for (const auto& dLong : m_vdLongitude)
    {
        unsigned uChunkSizeBytes = sizeof(dLong);
        memcpy(pcBytes, &dLong, uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    for (const auto& m_dLat : m_vdLatitude)
    {
        unsigned uChunkSizeBytes = sizeof(m_dLat);
        memcpy(pcBytes, &m_dLat, uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    for (const auto& vvi16TimeChunkData : m_vvvi16TimeChunks)
    {
        for (const auto& vi16ChannelData : vvi16TimeChunkData)
        {
            unsigned uChunkSizeBytes = sizeof(vi16ChannelData[0]) * vi16ChannelData.size();
            memcpy(pcBytes, &vi16ChannelData[0], uChunkSizeBytes);
            pcBytes += uChunkSizeBytes;
        }
    }

    return pvBytes;
}

void TDOAChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Read member variables in the same order they were written
    memcpy(&m_dSampleRate, pcBytes, sizeof(m_dSampleRate));
    pcBytes += sizeof(m_dSampleRate);

    memcpy(&m_i64TimeStamp_us, pcBytes, sizeof(m_i64TimeStamp_us));
    pcBytes += sizeof(m_i64TimeStamp_us);

    memcpy(&m_u64ChunkSize, pcBytes, sizeof(m_u64ChunkSize));
    pcBytes += sizeof(m_u64ChunkSize);

    memcpy(&m_u16NumberSources, pcBytes, sizeof(m_u16NumberSources));
    pcBytes += sizeof(m_u16NumberSources);

    // Read source identifiers
    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        uint8_t u8IdentifierSize;
        memcpy(&u8IdentifierSize, pcBytes, sizeof(u8IdentifierSize));
        pcBytes += sizeof(u8IdentifierSize);
        m_vu8SourceIdentifierSizes.push_back(u8IdentifierSize);

        std::vector<uint8_t> vu8Identifier(u8IdentifierSize);
        memcpy(vu8Identifier.data(), pcBytes, u8IdentifierSize * sizeof(uint8_t));
        pcBytes += u8IdentifierSize * sizeof(uint8_t);
        m_vvu8SourceIdentifiers.push_back(vu8Identifier);
    }

    // Read channels per source
    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        uint8_t u8NumChannels;
        memcpy(&u8NumChannels, pcBytes, sizeof(u8NumChannels));
        pcBytes += sizeof(u8NumChannels);
        m_vu8ChannelsPerSource.push_back(u8NumChannels);
    }

    // Read longitude values
    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        double dLong;
        memcpy(&dLong, pcBytes, sizeof(dLong));
        pcBytes += sizeof(dLong);
        m_vdLongitude.push_back(dLong);
    }

    // Read latitude values
    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        double dLat;
        memcpy(&dLat, pcBytes, sizeof(dLat));
        pcBytes += sizeof(dLat);
        m_vdLatitude.push_back(dLat);
    }

    // Read time chunks data
    for (size_t i = 0; i < m_u16NumberSources; i++)
    {
        std::vector<std::vector<int16_t>> vvi16TimeChunkData;
        for (size_t j = 0; j < m_vu8ChannelsPerSource[i]; j++)
        {
            std::vector<int16_t> vi16ChannelData(m_u64ChunkSize);
            memcpy(vi16ChannelData.data(), pcBytes, sizeof(int16_t) * m_u64ChunkSize);
            pcBytes += sizeof(int16_t) * m_u64ChunkSize;
            vvi16TimeChunkData.push_back(vi16ChannelData);
        }
        m_vvvi16TimeChunks.push_back(vvi16TimeChunkData);
    }
}

