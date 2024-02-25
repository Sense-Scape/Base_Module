#include "CoordinateChunk.h"

CoordinateChunk::CoordinateChunk() :
    BaseChunk(),
    m_dLatitude(0),
    m_dLongitude(0),
    m_i64TimeStamp(0)
{
    
}

CoordinateChunk::CoordinateChunk(std::shared_ptr<CoordinateChunk> pCoordinateChunk) :
    BaseChunk(pCoordinateChunk)
{
    // untested
    m_i64TimeStamp = pCoordinateChunk->m_i64TimeStamp;
    m_dLongitude = pCoordinateChunk->m_dLongitude;
    m_dLatitude = pCoordinateChunk->m_dLatitude;
}

CoordinateChunk::CoordinateChunk(const CoordinateChunk &coordinateChunk) :
    BaseChunk(coordinateChunk)
{
    // untested
    m_i64TimeStamp = coordinateChunk.m_i64TimeStamp;
    m_dLongitude = coordinateChunk.m_dLongitude;
    m_dLatitude = coordinateChunk.m_dLatitude;
}


unsigned CoordinateChunk::GetSize()
{
    return GetInternalSize();
}


unsigned CoordinateChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> CoordinateChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> CoordinateChunk::GetInternalSerialisation()
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
    memcpy(pcBytes, &m_i64TimeStamp, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(pcBytes, &m_dLongitude, sizeof(m_dLongitude));
    pcBytes += sizeof(m_dLongitude);

    memcpy(pcBytes, &m_dLatitude, sizeof(m_dLatitude));
    pcBytes += sizeof(m_dLatitude);

    return pvBytes;
}

void CoordinateChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Converting members to bytes
    memcpy(&m_i64TimeStamp, pcBytes, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(&m_dLongitude, pcBytes, sizeof(m_dLongitude));
    pcBytes += sizeof(m_dLongitude);

    memcpy(&m_dLatitude, pcBytes, sizeof(m_dLatitude));
    pcBytes += sizeof(m_dLatitude);

}

bool CoordinateChunk::IsEqual(CoordinateChunk& CoordinateChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(CoordinateChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare CoordinateChunk paramerters
    bool bIsEqual = (
        m_i64TimeStamp == CoordinateChunk.m_i64TimeStamp &&
        m_dLongitude == CoordinateChunk.m_dLongitude &&
        m_dLatitude == CoordinateChunk.m_dLatitude
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}


std::shared_ptr<nlohmann::json> CoordinateChunk::ToJSON()
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

    return std::make_shared<nlohmann::json>(JSONDocument);
}