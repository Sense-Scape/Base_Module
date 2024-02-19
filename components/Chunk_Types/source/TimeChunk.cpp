#include "TimeChunk.h"

TimeChunk::TimeChunk() :
    BaseChunk(),
    m_dChunkSize(0),
    m_dSampleRate(0),
    m_i64TimeStamp(0),
    m_uBits(0),
    m_uNumBytes(0),
    m_uNumChannels(0),
    m_vvi16TimeChunks()
{
    InitialiseChannels();
}

TimeChunk::TimeChunk(double dChunkSize, double dSampleRate, uint64_t i64TimeStamp, unsigned uBits, unsigned uNumBytes, unsigned uNumChannels) :
BaseChunk(),
m_dChunkSize(dChunkSize),
m_dSampleRate(dSampleRate),
m_i64TimeStamp(i64TimeStamp),
m_uBits(uBits),
m_uNumBytes(uNumBytes),
m_uNumChannels(uNumChannels),
m_vvi16TimeChunks()
{
    InitialiseChannels();
}

TimeChunk::TimeChunk(std::shared_ptr<TimeChunk> pTimeChunk) :
    BaseChunk(pTimeChunk)
{
    // untested
    m_dChunkSize = pTimeChunk->m_dChunkSize;
    m_dSampleRate = pTimeChunk->m_dSampleRate;
    m_i64TimeStamp = pTimeChunk->m_i64TimeStamp;
    m_uBits = pTimeChunk->m_uBits;
    m_uNumBytes = pTimeChunk->m_uNumBytes;
    m_uNumChannels = pTimeChunk->m_uNumChannels;
    m_vvi16TimeChunks = pTimeChunk->m_vvi16TimeChunks;
}

TimeChunk::TimeChunk(const TimeChunk& timeChunk) : 
    BaseChunk(timeChunk)
{
    // untested
    m_dChunkSize = timeChunk.m_dChunkSize;
    m_dSampleRate = timeChunk.m_dSampleRate;
    m_i64TimeStamp = timeChunk.m_i64TimeStamp;
    m_uBits = timeChunk.m_uBits;
    m_uNumBytes = timeChunk.m_uNumBytes;
    m_uNumChannels = timeChunk.m_uNumChannels;
    m_vvi16TimeChunks = timeChunk.m_vvi16TimeChunks;
}

unsigned TimeChunk::GetSize() 
{
    return GetInternalSize();
}


unsigned TimeChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += sizeof(m_dChunkSize);
    uByteSize += sizeof(m_dSampleRate);
    uByteSize += sizeof(m_i64TimeStamp);
    uByteSize += sizeof(m_uBits);
    uByteSize += sizeof(m_uNumBytes);
    uByteSize += sizeof(m_uNumChannels);

    // Iterate over all elements of m_vvi16TimeChunks and infer type using auto
    for (const auto& vfTimeChunk : m_vvi16TimeChunks)
        uByteSize += sizeof(vfTimeChunk[0]) * vfTimeChunk.size();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> TimeChunk::Serialise()
{  
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> TimeChunk::GetInternalSerialisation()
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
    memcpy(pcBytes, &m_dChunkSize, sizeof(m_dChunkSize));
    pcBytes += sizeof(m_dChunkSize);

    memcpy(pcBytes, &m_dSampleRate, sizeof(m_dSampleRate));
    pcBytes += sizeof(m_dSampleRate);

    memcpy(pcBytes, &m_i64TimeStamp, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(pcBytes, &m_uBits, sizeof(m_uBits));
    pcBytes += sizeof(m_uBits);

    memcpy(pcBytes, &m_uNumBytes, sizeof(m_uNumBytes));
    pcBytes += sizeof(m_uNumBytes);

    memcpy(pcBytes, &m_uNumChannels, sizeof(m_uNumChannels));
    pcBytes += sizeof(m_uNumChannels);

    // Converting vector to bytes
    for (const auto& vi16TimeChunk : m_vvi16TimeChunks)
    {
        unsigned uChunkSizeBytes = sizeof(vi16TimeChunk[0]) * vi16TimeChunk.size();
        memcpy(pcBytes, &vi16TimeChunk[0], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    return pvBytes;
}

void TimeChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Converting members to bytes
    memcpy(&m_dChunkSize, pcBytes, sizeof(m_dChunkSize));
    pcBytes += sizeof(m_dChunkSize);

    memcpy( &m_dSampleRate, pcBytes, sizeof(m_dSampleRate));
    pcBytes += sizeof(m_dSampleRate);

    memcpy(&m_i64TimeStamp, pcBytes, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(&m_uBits, pcBytes, sizeof(m_uBits));
    pcBytes += sizeof(m_uBits);

    memcpy(&m_uNumBytes, pcBytes, sizeof(m_uNumBytes));
    pcBytes += sizeof(m_uNumBytes);

    memcpy(&m_uNumChannels, pcBytes, sizeof(m_uNumChannels));
    pcBytes += sizeof(m_uNumChannels);

    // Reserving space for vector
    InitialiseChannels();

    // Filling data vector
    unsigned uSampleSize = sizeof(m_vvi16TimeChunks[0][0]);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex) {
        for (unsigned uSampleIndex = 0; uSampleIndex < m_dChunkSize; uSampleIndex++)
        {
            memcpy(&m_vvi16TimeChunks[uChannelIndex][uSampleIndex], pcBytes, uSampleSize);
            pcBytes += uSampleSize;
        }
    }
}

bool TimeChunk::IsEqual(TimeChunk& timeChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(timeChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);
    
    // We can then compare TimeChunk paramerters
    bool bIsEqual = (
        (m_dChunkSize == timeChunk.m_dChunkSize) &&
        (m_dSampleRate == timeChunk.m_dSampleRate) &&
        (m_i64TimeStamp == timeChunk.m_i64TimeStamp) &&
        (m_uBits == timeChunk.m_uBits) &&
        (m_uNumBytes == timeChunk.m_uNumBytes) &&
        (m_uNumChannels == timeChunk.m_uNumChannels) &&
        (m_vvi16TimeChunks == timeChunk.m_vvi16TimeChunks)
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}


void TimeChunk::InitialiseChannels()
{
    m_vvi16TimeChunks.resize(m_uNumChannels);

    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex)
        m_vvi16TimeChunks[uChannelIndex].resize(m_dChunkSize);
}

std::shared_ptr<nlohmann::json> TimeChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

    // Adding in Timechunk fields
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(m_dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(m_dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(m_i64TimeStamp);
    JSONDocument[strChunkName]["uBits"] = std::to_string(m_uBits);
    JSONDocument[strChunkName]["NumBytes"] = std::to_string(m_uNumBytes);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(m_uNumChannels);
 
    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; uChannelIndex++)
        JSONDocument[strChunkName]["Channels"][std::to_string(uChannelIndex)] = m_vvi16TimeChunks[uChannelIndex];

    return std::make_shared<nlohmann::json>(JSONDocument);
}