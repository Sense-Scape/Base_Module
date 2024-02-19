#include "FFTMagnitudeChunk.h"

FFTMagnitudeChunk::FFTMagnitudeChunk() :
    BaseChunk(),
    m_dChunkSize(0),
    m_dSampleRate(0),
    m_i64TimeStamp(0),
    m_uNumChannels(0),
    m_vvfFFTMagnitudeChunks()
{
    InitialiseChannels();
}

FFTMagnitudeChunk::FFTMagnitudeChunk(double dChunkSize, double dSampleRate, uint64_t i64TimeStamp, unsigned uNumChannels) :
    BaseChunk(),
    m_dChunkSize(dChunkSize),
    m_dSampleRate(dSampleRate),
    m_i64TimeStamp(i64TimeStamp),
    m_uNumChannels(uNumChannels),
    m_vvfFFTMagnitudeChunks()
{
    InitialiseChannels();
}

FFTMagnitudeChunk::FFTMagnitudeChunk(std::shared_ptr<FFTMagnitudeChunk> pFFTMagnitudeChunk) :
    BaseChunk(pFFTMagnitudeChunk)
{
    // untested
    m_dChunkSize = pFFTMagnitudeChunk->m_dChunkSize;
    m_dSampleRate = pFFTMagnitudeChunk->m_dSampleRate;
    m_i64TimeStamp = pFFTMagnitudeChunk->m_i64TimeStamp;
    m_uNumChannels = pFFTMagnitudeChunk->m_uNumChannels;
    m_vvfFFTMagnitudeChunks = pFFTMagnitudeChunk->m_vvfFFTMagnitudeChunks;
}

FFTMagnitudeChunk::FFTMagnitudeChunk(const FFTMagnitudeChunk& FFTMagnitudeChunk) :
    BaseChunk(FFTMagnitudeChunk)
{
    // untested
    m_dChunkSize = FFTMagnitudeChunk.m_dChunkSize;
    m_dSampleRate = FFTMagnitudeChunk.m_dSampleRate;
    m_i64TimeStamp = FFTMagnitudeChunk.m_i64TimeStamp;
    m_uNumChannels = FFTMagnitudeChunk.m_uNumChannels;
    m_vvfFFTMagnitudeChunks = FFTMagnitudeChunk.m_vvfFFTMagnitudeChunks;
}

unsigned FFTMagnitudeChunk::GetSize()
{
    return GetInternalSize();
}


unsigned FFTMagnitudeChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += sizeof(m_dChunkSize);
    uByteSize += sizeof(m_dSampleRate);
    uByteSize += sizeof(m_i64TimeStamp);
    uByteSize += sizeof(m_uNumChannels);

    // Iterate over all elements of m_vvi16FFTMagnitudeChunks and infer type using auto
    for (const auto& vfFFTMagnitudeChunk : m_vvfFFTMagnitudeChunks)
        uByteSize += sizeof(vfFFTMagnitudeChunk[0]) * vfFFTMagnitudeChunk.size();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> FFTMagnitudeChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> FFTMagnitudeChunk::GetInternalSerialisation()
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

    memcpy(pcBytes, &m_uNumChannels, sizeof(m_uNumChannels));
    pcBytes += sizeof(m_uNumChannels);

    // Converting vector to bytes
    for (const auto& vi16FFTMagnitudeChunk : m_vvfFFTMagnitudeChunks)
    {
        unsigned uChunkSizeBytes = sizeof(vi16FFTMagnitudeChunk[0]) * vi16FFTMagnitudeChunk.size();
        memcpy(pcBytes, &vi16FFTMagnitudeChunk[0], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    return pvBytes;
}

void FFTMagnitudeChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Converting members to bytes
    memcpy(&m_dChunkSize, pcBytes, sizeof(m_dChunkSize));
    pcBytes += sizeof(m_dChunkSize);

    memcpy(&m_dSampleRate, pcBytes, sizeof(m_dSampleRate));
    pcBytes += sizeof(m_dSampleRate);

    memcpy(&m_i64TimeStamp, pcBytes, sizeof(m_i64TimeStamp));
    pcBytes += sizeof(m_i64TimeStamp);

    memcpy(&m_uNumChannels, pcBytes, sizeof(m_uNumChannels));
    pcBytes += sizeof(m_uNumChannels);

    // Reserving space for vector
    InitialiseChannels();

    // Filling data vector
    unsigned uSampleSize = sizeof(m_vvfFFTMagnitudeChunks[0][0]);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex) {
        for (unsigned uSampleIndex = 0; uSampleIndex < m_dChunkSize; uSampleIndex++)
        {
            memcpy(&m_vvfFFTMagnitudeChunks[uChannelIndex][uSampleIndex], pcBytes, uSampleSize);
            pcBytes += uSampleSize;
        }
    }
}

bool FFTMagnitudeChunk::IsEqual(FFTMagnitudeChunk& FFTMagnitudeChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(FFTMagnitudeChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare FFTMagnitudeChunk paramerters
    bool bIsEqual = (
        (m_dChunkSize == FFTMagnitudeChunk.m_dChunkSize) &&
        (m_dSampleRate == FFTMagnitudeChunk.m_dSampleRate) &&
        (m_i64TimeStamp == FFTMagnitudeChunk.m_i64TimeStamp) &&
        (m_uNumChannels == FFTMagnitudeChunk.m_uNumChannels) &&
        (m_vvfFFTMagnitudeChunks == FFTMagnitudeChunk.m_vvfFFTMagnitudeChunks)
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

void FFTMagnitudeChunk::InitialiseChannels()
{
    m_vvfFFTMagnitudeChunks.resize(m_uNumChannels);

    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex)
        m_vvfFFTMagnitudeChunks[uChannelIndex].resize(m_dChunkSize);
}

std::shared_ptr<nlohmann::json> FFTMagnitudeChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

    // Adding in FFTMagnitudeChunk fields
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(m_dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(m_dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(m_i64TimeStamp);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(m_uNumChannels);

    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; uChannelIndex++)
        JSONDocument[strChunkName]["Channels"][std::to_string(uChannelIndex)] = m_vvfFFTMagnitudeChunks[uChannelIndex];

    return std::make_shared<nlohmann::json>(JSONDocument);
}