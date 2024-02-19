#include "WatchdogChunk.h"

WatchdogChunk::WatchdogChunk(std::vector<uint8_t> &vu8MACAddress):
    BaseChunk(),
    m_vu8MACAddress(vu8MACAddress)
{
}

WatchdogChunk::WatchdogChunk(std::shared_ptr<WatchdogChunk> pWatchdogChunk) 
{
    m_vu8MACAddress = pWatchdogChunk->m_vu8MACAddress;  
}

unsigned WatchdogChunk::GetSize() 
{
    unsigned uByteSize = 0;

    uByteSize += sizeof(m_vu8MACAddress[0])*m_vu8MACAddress.size();

    return uByteSize;
}

std::shared_ptr<std::vector<char>> WatchdogChunk::Serialise()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char* pcBytes = pvBytes->data();

    // Converting members to bytes
    memcpy(pcBytes, &m_vu8MACAddress[0], sizeof(m_vu8MACAddress[0])*m_vu8MACAddress.size());
    
    return pvBytes;
}

void WatchdogChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    char* pcBytes = pvBytes->data();

    // Converting members to bytes
    // Assumed 6 bytes as a MAC address is 6 bytes ...
    m_vu8MACAddress.resize(6);
    memcpy(&m_vu8MACAddress[0], pcBytes, 6);

}

bool WatchdogChunk::IsEqual(WatchdogChunk& watchdogChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(watchdogChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare TimeChunk paramerters
    bool bIsEqual = (m_vu8MACAddress == watchdogChunk.m_vu8MACAddress);

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::string WatchdogChunk::MACAddressToString(const std::vector<uint8_t>& macAddress) {
    std::stringstream stream;
    stream << std::setfill('0') << std::hex;

    for (size_t i = 0; i < macAddress.size(); ++i) {
        stream << std::setw(2) << static_cast<int>(macAddress[i]);
        if (i != macAddress.size() - 1) 
            stream << ":";
    }
    std::string upperCased = stream.str();
    for (char& c : upperCased)
        c = std::toupper(c);
    return upperCased;
};

std::shared_ptr<nlohmann::json> WatchdogChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
    JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

    // Adding in Timechunk fields
    JSONDocument[strChunkName]["MACAddress"] = MACAddressToString(m_vu8MACAddress);

    return std::make_shared<nlohmann::json>(JSONDocument);
}