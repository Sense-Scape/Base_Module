#include "FFTChunk.h"

FFTChunk::FFTChunk() :
    BaseChunk(),
    m_dChunkSize(0),
    m_dSampleRate(0),
    m_i64TimeStamp(0),
    m_uNumChannels(0),
    m_vvcfFFTChunks()
{
    InitialiseChannels();
}

FFTChunk::FFTChunk(double dChunkSize, double dSampleRate, uint64_t i64TimeStamp, unsigned uNumChannels) :
    BaseChunk(),
    m_dChunkSize(dChunkSize),
    m_dSampleRate(dSampleRate),
    m_i64TimeStamp(i64TimeStamp),
    m_uNumChannels(uNumChannels),
    m_vvcfFFTChunks()
{
    InitialiseChannels();
}

FFTChunk::FFTChunk(std::shared_ptr<FFTChunk> pFFTChunk) :
    BaseChunk(pFFTChunk)
{
    // untested
    m_dChunkSize = pFFTChunk->m_dChunkSize;
    m_dSampleRate = pFFTChunk->m_dSampleRate;
    m_i64TimeStamp = pFFTChunk->m_i64TimeStamp;
    m_uNumChannels = pFFTChunk->m_uNumChannels;
    m_vvcfFFTChunks = pFFTChunk->m_vvcfFFTChunks;
}

FFTChunk::FFTChunk(const FFTChunk& FFTChunk) :
    BaseChunk(FFTChunk)
{
    // untested
    m_dChunkSize = FFTChunk.m_dChunkSize;
    m_dSampleRate = FFTChunk.m_dSampleRate;
    m_i64TimeStamp = FFTChunk.m_i64TimeStamp;
    m_uNumChannels = FFTChunk.m_uNumChannels;
    m_vvcfFFTChunks = FFTChunk.m_vvcfFFTChunks;
}

unsigned FFTChunk::GetSize()
{
    return GetInternalSize();
}


unsigned FFTChunk::GetInternalSize()
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

    // Iterate over all elements of m_vvi16FFTChunks and infer type using auto
    for (const auto& vfFFTChunk : m_vvcfFFTChunks)
        uByteSize += sizeof(vfFFTChunk[0]) * vfFFTChunk.size();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> FFTChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> FFTChunk::GetInternalSerialisation()
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
    for (const auto& vi16FFTChunk : m_vvcfFFTChunks)
    {
        unsigned uChunkSizeBytes = sizeof(vi16FFTChunk[0]) * vi16FFTChunk.size();
        memcpy(pcBytes, &vi16FFTChunk[0], uChunkSizeBytes);
        pcBytes += uChunkSizeBytes;
    }

    return pvBytes;
}

void FFTChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
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
    unsigned uSampleSize = sizeof(m_vvcfFFTChunks[0][0]);
    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex) {
        for (unsigned uSampleIndex = 0; uSampleIndex < m_dChunkSize; uSampleIndex++)
        {
            memcpy(&m_vvcfFFTChunks[uChannelIndex][uSampleIndex], pcBytes, uSampleSize);
            pcBytes += uSampleSize;
        }
    }
}

bool FFTChunk::IsEqual(FFTChunk& FFTChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(FFTChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare FFTChunk paramerters
    bool bIsEqual = (
        (m_dChunkSize == FFTChunk.m_dChunkSize) &&
        (m_dSampleRate == FFTChunk.m_dSampleRate) &&
        (m_i64TimeStamp == FFTChunk.m_i64TimeStamp) &&
        (m_uNumChannels == FFTChunk.m_uNumChannels) &&
        (m_vvcfFFTChunks == FFTChunk.m_vvcfFFTChunks)
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::shared_ptr<std::vector<float>> FFTChunk::GetChannelPower(unsigned uChannelIndex)
{
    auto pPowerData = std::make_shared<std::vector<float>>();

    for (unsigned uSampleIndex = 0; uSampleIndex < m_dChunkSize; uSampleIndex++)
        pPowerData->emplace_back(std::abs(m_vvcfFFTChunks[uChannelIndex][uSampleIndex]));

    return pPowerData;
}

void FFTChunk::InitialiseChannels()
{
    m_vvcfFFTChunks.resize(m_uNumChannels);

    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; ++uChannelIndex)
        m_vvcfFFTChunks[uChannelIndex].resize(m_dChunkSize);
}

std::string FFTChunk::ConvertComplexChannelDataToString(uint16_t uChannelIndex)
{
    std::ostringstream oss;

    // Add each elemement in channel to the string
    for (size_t i = 0; i < m_vvcfFFTChunks[uChannelIndex].size(); ++i) {

        // Add the sample
        const auto& cfChannelSample = m_vvcfFFTChunks[uChannelIndex][i];
        oss << cfChannelSample.real() << ' ' << cfChannelSample.imag();

        // Check if it's not the last iteration before adding space
        if (i != m_vvcfFFTChunks[uChannelIndex].size() - 1)
            oss << ',';
    }

    return oss.str();
}

std::shared_ptr<nlohmann::json> FFTChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

    // Adding in FFTChunk fields
    JSONDocument[strChunkName]["ChunkSize"] = std::to_string(m_dChunkSize);
    JSONDocument[strChunkName]["SampleRate"] = std::to_string(m_dSampleRate);
    JSONDocument[strChunkName]["TimeStamp"] = std::to_string(m_i64TimeStamp);
    JSONDocument[strChunkName]["NumChannels"] = std::to_string(m_uNumChannels);

    for (unsigned uChannelIndex = 0; uChannelIndex < m_uNumChannels; uChannelIndex++)
        JSONDocument[strChunkName]["Channels"][std::to_string(uChannelIndex)] = ConvertComplexChannelDataToString(uChannelIndex);

    return std::make_shared<nlohmann::json>(JSONDocument);
}