#include "DirectionBinChunk.h"

DirectionBinChunk::DirectionBinChunk() :
    m_u16NumberOfDetections(),
    m_vu16DetectionFrequencyIndicies(),
    m_vu16DetectionAngles_deg(),
    m_dSampleRate(),
    BaseChunk()
{

}

DirectionBinChunk::DirectionBinChunk(std::shared_ptr<DirectionBinChunk> pDirectionBinChunk) :
    m_u16NumberOfDetections(pDirectionBinChunk->m_u16NumberOfDetections),
    m_vu16DetectionFrequencyIndicies(pDirectionBinChunk->m_vu16DetectionFrequencyIndicies),
    m_vu16DetectionAngles_deg(pDirectionBinChunk->m_vu16DetectionAngles_deg),
    m_dSampleRate(pDirectionBinChunk->m_dSampleRate),
    BaseChunk(pDirectionBinChunk)
{

}

DirectionBinChunk::DirectionBinChunk(const DirectionBinChunk& DirectionBinChunk) :
    m_u16NumberOfDetections(DirectionBinChunk.m_u16NumberOfDetections),
    m_vu16DetectionFrequencyIndicies(DirectionBinChunk.m_vu16DetectionFrequencyIndicies),
    m_vu16DetectionAngles_deg(DirectionBinChunk.m_vu16DetectionAngles_deg),
    m_dSampleRate(DirectionBinChunk.m_dSampleRate),
    BaseChunk(DirectionBinChunk)
{

}

unsigned DirectionBinChunk::GetSize()
{
    return GetInternalSize();
}

unsigned DirectionBinChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += sizeof(m_u16NumberOfDetections); 
    uByteSize += sizeof(m_vu16DetectionFrequencyIndicies[0])*m_u16NumberOfDetections;
    uByteSize += sizeof(m_vu16DetectionAngles_deg[0])*m_u16NumberOfDetections;
    uByteSize += sizeof(m_dSampleRate);

    return uByteSize;
}

void DirectionBinChunk::SetDirectionData(uint16_t u16NumberOfDetections, std::vector<float> &vu16DetectionFrequencyIndicies, std::vector<float> &vvu16DetectionAngles_deg, double dSampleRate )
{
    m_u16NumberOfDetections = u16NumberOfDetections;
    m_vu16DetectionFrequencyIndicies = vu16DetectionFrequencyIndicies;
    m_vu16DetectionAngles_deg = vvu16DetectionAngles_deg;
    m_dSampleRate = dSampleRate;

    assert(m_vu16DetectionFrequencyIndicies.size() == m_u16NumberOfDetections);
    assert(vvu16DetectionAngles_deg.size() == m_u16NumberOfDetections);
}

std::shared_ptr<std::vector<char>> DirectionBinChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> DirectionBinChunk::GetInternalSerialisation()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char* pcBytes = pvBytes->data();

    // Copy base data
    auto pcvBaseBytes = BaseChunk::Serialise();
    unsigned uBaseClassSize = BaseChunk::GetSize();
    memcpy(pcBytes, &pcvBaseBytes->front(), uBaseClassSize);
    pcBytes += uBaseClassSize;

    memcpy(pcBytes, &m_u16NumberOfDetections, sizeof(m_u16NumberOfDetections));
    pcBytes += sizeof(m_u16NumberOfDetections);

    unsigned uChunkSizeBytes = sizeof(m_vu16DetectionFrequencyIndicies[0]) * m_u16NumberOfDetections;
    memcpy(pcBytes, &m_vu16DetectionFrequencyIndicies[0], uChunkSizeBytes);
    pcBytes += uChunkSizeBytes;

    uChunkSizeBytes = sizeof(m_vu16DetectionAngles_deg[0]) * m_u16NumberOfDetections;
    memcpy(pcBytes, &m_vu16DetectionAngles_deg[0], uChunkSizeBytes);
    pcBytes += uChunkSizeBytes;

    return pvBytes;
}

void DirectionBinChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    memcpy(&m_u16NumberOfDetections, pcBytes, 2);
    pcBytes += sizeof(m_u16NumberOfDetections);

    unsigned uSampleSize = 4;
    m_vu16DetectionFrequencyIndicies.resize(m_u16NumberOfDetections);
    for (unsigned uDetectionIndex = 0; uDetectionIndex < m_u16NumberOfDetections; uDetectionIndex++)
    {
        memcpy(&m_vu16DetectionFrequencyIndicies[uDetectionIndex], pcBytes, uSampleSize);
        pcBytes += uSampleSize;
    }

    // Filling data vector
    m_vu16DetectionAngles_deg.resize(m_u16NumberOfDetections);
    for (unsigned uDetectionIndex = 0; uDetectionIndex < m_u16NumberOfDetections; uDetectionIndex++)
    {
        memcpy(&m_vu16DetectionAngles_deg[uDetectionIndex], pcBytes, uSampleSize);
        pcBytes += uSampleSize;
    }
    
    memcpy(&m_dSampleRate, pcBytes, sizeof(double));
    pcBytes += sizeof(m_dSampleRate);

}

bool DirectionBinChunk::IsEqual(DirectionBinChunk& DirectionBinChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(DirectionBinChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare DirectionBinChunk paramerters
    bool bIsEqual = (
            m_vu16DetectionAngles_deg == DirectionBinChunk.m_vu16DetectionAngles_deg ||
            m_vu16DetectionFrequencyIndicies == DirectionBinChunk.m_vu16DetectionFrequencyIndicies ||
            m_dSampleRate == DirectionBinChunk.m_dSampleRate        
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::shared_ptr<nlohmann::json> DirectionBinChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier; 

    JSONDocument[strChunkName]["SampleRate"] = std::to_string(m_dSampleRate);
    JSONDocument[strChunkName]["NumberOfDetections"] =  std::to_string(m_u16NumberOfDetections);

    JSONDocument[strChunkName]["DetectionFrequencyIndicies"] = m_vu16DetectionFrequencyIndicies;
    JSONDocument[strChunkName]["DetectionAngles_deg"] = m_vu16DetectionAngles_deg;

    return std::make_shared<nlohmann::json>(JSONDocument);
}