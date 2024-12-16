#include "DetectionBinChunk.h"

DetectionBinChunk::DetectionBinChunk() :
    m_u16NumChannels(),
    m_vvu16DetectionBins(),
    BaseChunk()
{

}

DetectionBinChunk::DetectionBinChunk(std::shared_ptr<DetectionBinChunk> pDetectionBinChunk) :
    m_u16NumChannels(pDetectionBinChunk->m_u16NumChannels),
    m_vvu16DetectionBins(pDetectionBinChunk->m_vvu16DetectionBins),
    BaseChunk(pDetectionBinChunk)
{

}

DetectionBinChunk::DetectionBinChunk(const DetectionBinChunk& DetectionBinChunk) :
    m_u16NumChannels(DetectionBinChunk.m_u16NumChannels),
    m_vvu16DetectionBins(DetectionBinChunk.m_vvu16DetectionBins),
    BaseChunk(DetectionBinChunk)
{

}

u_int64_t  DetectionBinChunk::GetSize()
{
    return GetInternalSize();
}

void DetectionBinChunk::SetDetectionBins(std::vector<std::vector<uint16_t>> &vvu16DetectionBins)
{
    m_u16NumChannels = vvu16DetectionBins.size();
    m_vvu16DetectionBins = vvu16DetectionBins;
    
    m_vu16DetectedBinsPerChannel.resize(m_u16NumChannels);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_u16NumChannels; uChannelIndex++)
    {
        m_vu16DetectedBinsPerChannel[uChannelIndex] = vvu16DetectionBins[uChannelIndex].size();
    }
}


u_int64_t  DetectionBinChunk::GetInternalSize()
{
    u_int64_t  uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += sizeof(m_u16NumChannels); 
    uByteSize += sizeof(m_vu16DetectedBinsPerChannel[0])*m_vu16DetectedBinsPerChannel.size();

    // Iterate over all elements of m_vvi16FFTChunks and infer type using auto
    for (const auto& vu16DetectionBins : m_vvu16DetectionBins)
        uByteSize += sizeof(vu16DetectionBins[0]) * vu16DetectionBins.size();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> DetectionBinChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> DetectionBinChunk::GetInternalSerialisation()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char* pcBytes = pvBytes->data();

    // Copy base data
    auto pcvBaseBytes = BaseChunk::Serialise();
    unsigned uBaseClassSize = BaseChunk::GetSize();
    memcpy(pcBytes, &pcvBaseBytes->front(), uBaseClassSize);
    pcBytes += uBaseClassSize;

    memcpy(pcBytes, &m_u16NumChannels, sizeof(m_u16NumChannels));
    pcBytes += sizeof(m_u16NumChannels);

    unsigned uChunkSizeBytes = sizeof(m_vu16DetectedBinsPerChannel[0]) * m_u16NumChannels;
    memcpy(pcBytes, &m_vu16DetectedBinsPerChannel[0], uChunkSizeBytes);
    pcBytes += uChunkSizeBytes;

    // Converting vector to bytes
    for (const auto& vu16DetectionBins : m_vvu16DetectionBins)
    {
        unsigned uChunkSizeBytes = sizeof(vu16DetectionBins[0]) * vu16DetectionBins.size();
        memcpy(pcBytes, &vu16DetectionBins[0], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    return pvBytes;
}

void DetectionBinChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    memcpy(&m_u16NumChannels, pcBytes, 2);
    pcBytes += sizeof(m_u16NumChannels);

    unsigned uSampleSize = 2;
    m_vu16DetectedBinsPerChannel.resize(m_u16NumChannels);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_u16NumChannels; uChannelIndex++)
    {
        memcpy(&m_vu16DetectedBinsPerChannel[uChannelIndex], pcBytes, uSampleSize);
        pcBytes += uSampleSize;
    }

    // Filling data vector
    m_vvu16DetectionBins.resize(m_u16NumChannels);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_u16NumChannels; ++uChannelIndex) {

        m_vvu16DetectionBins[uChannelIndex].resize(m_vu16DetectedBinsPerChannel[uChannelIndex]);
        
        for (unsigned uSampleIndex = 0; uSampleIndex < m_vu16DetectedBinsPerChannel[uChannelIndex]; uSampleIndex++)
        {
            memcpy(&m_vvu16DetectionBins[uChannelIndex][uSampleIndex], pcBytes, uSampleSize);
            pcBytes += uSampleSize;
        }
    }    
}

bool DetectionBinChunk::IsEqual(DetectionBinChunk& DetectionBinChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(DetectionBinChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare DetectionBinChunk paramerters
    bool bIsEqual = (
            m_u16NumChannels == DetectionBinChunk.m_u16NumChannels ||
            m_vu16DetectedBinsPerChannel == DetectionBinChunk.m_vu16DetectedBinsPerChannel ||
            m_vvu16DetectionBins == DetectionBinChunk.m_vvu16DetectionBins        
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::shared_ptr<nlohmann::json> DetectionBinChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier; 

    JSONDocument[strChunkName]["NumChannels"] =  std::to_string(m_u16NumChannels); 
    JSONDocument[strChunkName]["DetectedBinsPerChannel"] =  m_vu16DetectedBinsPerChannel; ; 

    for (unsigned uChannelIndex = 0; uChannelIndex < m_u16NumChannels; uChannelIndex++)
        JSONDocument[strChunkName]["DetectionBins"][std::to_string(uChannelIndex)] = m_vvu16DetectionBins[uChannelIndex];
    
    return std::make_shared<nlohmann::json>(JSONDocument);
}