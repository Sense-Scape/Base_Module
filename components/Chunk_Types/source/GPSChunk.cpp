#include "GPSChunk.h"

GPSChunk::GPSChunk() : BaseChunk(),
                       m_dLatitude(0),
                       m_dLongitude(0),
                       m_i64TimeStamp(0),
                       m_bIsNorth(true),
                       m_bIsWest(true)
{
}

GPSChunk::GPSChunk(std::shared_ptr<GPSChunk> pGPSChunk) : BaseChunk(pGPSChunk)
{
    // untested
    m_i64TimeStamp = pGPSChunk->m_i64TimeStamp;
    m_dLongitude = pGPSChunk->m_dLongitude;
    m_dLatitude = pGPSChunk->m_dLatitude;
    m_bIsNorth = pGPSChunk->m_bIsNorth;
    m_bIsWest = pGPSChunk->m_bIsWest;
}

GPSChunk::GPSChunk(const GPSChunk &GPSChunk) : BaseChunk(GPSChunk)
{
    // untested
    m_i64TimeStamp = GPSChunk.m_i64TimeStamp;
    m_dLongitude = GPSChunk.m_dLongitude;
    m_dLatitude = GPSChunk.m_dLatitude;
    m_bIsNorth = GPSChunk.m_bIsNorth;
    m_bIsWest = GPSChunk.m_bIsWest;
}

unsigned GPSChunk::GetSize()
{
    return GetInternalSize();
}

unsigned GPSChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk &>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    uByteSize += sizeof(m_i64TimeStamp);
    uByteSize += sizeof(m_dLongitude);
    uByteSize += sizeof(m_dLatitude);
    uByteSize += sizeof(m_bIsNorth);
    uByteSize += sizeof(m_bIsWest);

    return uByteSize;
}

std::shared_ptr<std::vector<char>> GPSChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> GPSChunk::GetInternalSerialisation()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char *pcBytes = pvBytes->data();

    // Copy base data
    auto pcvBaseBytes = BaseChunk::Serialise();
    unsigned uBaseClassSize = BaseChunk::GetSize();
    memcpy(pcBytes, &pcvBaseBytes->front(), uBaseClassSize);
    pcBytes += uBaseClassSize;

    // Converting members to bytes
    memcpy(pcBytes, &m_i64TimeStamp, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(pcBytes, &m_dLongitude, sizeof(m_dLongitude));
    pcBytes += sizeof(m_dLongitude);

    memcpy(pcBytes, &m_dLatitude, sizeof(m_dLatitude));
    pcBytes += sizeof(m_dLatitude);

    memcpy(pcBytes, &m_bIsNorth, sizeof(m_bIsNorth));
    pcBytes += sizeof(m_bIsNorth);

    memcpy(pcBytes, &m_bIsWest, sizeof(m_bIsWest));
    pcBytes += sizeof(m_bIsWest);
    return pvBytes;
}

void GPSChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char *pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Converting members to bytes
    memcpy(&m_i64TimeStamp, pcBytes, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(&m_dLongitude, pcBytes, sizeof(m_dLongitude));
    pcBytes += sizeof(m_dLongitude);

    memcpy(&m_dLatitude, pcBytes, sizeof(m_dLatitude));
    pcBytes += sizeof(m_dLongitude);

    memcpy(&m_bIsNorth, pcBytes, sizeof(m_bIsNorth));
    pcBytes += sizeof(m_bIsNorth);

    memcpy(&m_bIsWest, pcBytes, sizeof(m_bIsWest));
}

bool GPSChunk::IsEqual(GPSChunk &GPSChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk &>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk &>(GPSChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare GPSChunk paramerters
    bool bIsEqual = (m_i64TimeStamp == GPSChunk.m_i64TimeStamp &&
                     m_dLongitude == GPSChunk.m_dLongitude &&
                     m_dLatitude == GPSChunk.m_dLatitude &&
                     m_bIsNorth == GPSChunk.m_bIsNorth &&
                     m_bIsWest == GPSChunk.m_bIsWest);

    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::shared_ptr<nlohmann::json> GPSChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

    // Adding in FFTChunk fields
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(m_i64TimeStamp);
    JSONDocument[strChunkName]["Longitude"] = std::to_string(m_dLongitude);
    JSONDocument[strChunkName]["Latitude"] = std::to_string(m_dLatitude);
    JSONDocument[strChunkName]["IsNorth"] = std::to_string(m_bIsNorth);
    JSONDocument[strChunkName]["IsWest"] = std::to_string(m_bIsWest);

    return std::make_shared<nlohmann::json>(JSONDocument);
}