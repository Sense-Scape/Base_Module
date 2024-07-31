#include "QueueLengthChunk.h"

QueueLengthChunk::QueueLengthChunk(std::string strQueueName, unsigned uQueueLength) :
    m_strQueueName(strQueueName),
    m_u16QueueNameLength(strQueueName.length()),
    m_uQueueLength(uQueueLength),
    BaseChunk()
{
    // Get program path
    char filename[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", filename, PATH_MAX);

    // replace the end of the char array to become string
    if (count != -1)
        filename[count] = '\0';

    // Now extract just the executable name
    std::string strExecutableName = std::string(filename);
    size_t pos = strExecutableName.find_last_of("/\\");
    if (pos != std::string::npos)
        strExecutableName = std::string(filename).substr(pos + 1);
    else
        strExecutableName = "PathNameError";

    // And store it
    m_strProgramName = strExecutableName;
    m_u16ProgramNameLength = m_strProgramName.length();
}

QueueLengthChunk::QueueLengthChunk(std::shared_ptr<QueueLengthChunk> pQueueLengthChunk) :
    m_strQueueName(pQueueLengthChunk->m_strQueueName),
    m_u16QueueNameLength(pQueueLengthChunk->m_u16QueueNameLength),
    m_strProgramName(pQueueLengthChunk->m_strProgramName),
    m_u16ProgramNameLength(pQueueLengthChunk->m_u16ProgramNameLength),
    m_uQueueLength(pQueueLengthChunk->m_uQueueLength),
    BaseChunk(pQueueLengthChunk)
{

}

QueueLengthChunk::QueueLengthChunk() :
    m_strQueueName(""),
    m_u16QueueNameLength(0),
    m_strProgramName(""),
    m_u16ProgramNameLength(0),
    m_uQueueLength(0),
    BaseChunk()
{

}

unsigned QueueLengthChunk::GetSize()
{
    return GetInternalSize();
}

unsigned QueueLengthChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Then this class
    uByteSize += m_strQueueName.size();
    uByteSize += sizeof(m_u16QueueNameLength);
    uByteSize += m_strProgramName.size();
    uByteSize += sizeof(m_u16ProgramNameLength);
    uByteSize += sizeof(m_uQueueLength);

    return uByteSize;
}

std::shared_ptr<std::vector<char>> QueueLengthChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> QueueLengthChunk::GetInternalSerialisation()
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
    memcpy(pcBytes, &m_u16QueueNameLength, sizeof(m_u16QueueNameLength));
    pcBytes += sizeof(m_u16QueueNameLength);

    if(m_u16QueueNameLength)
    {
        memcpy(pcBytes, m_strQueueName.c_str(), m_u16QueueNameLength);
        pcBytes += m_u16QueueNameLength;
    }

    memcpy(pcBytes, &m_u16ProgramNameLength, sizeof(m_u16ProgramNameLength));
    pcBytes += sizeof(m_u16ProgramNameLength);

    if(m_u16ProgramNameLength)
    {
        memcpy(pcBytes, m_strProgramName.c_str(), m_u16ProgramNameLength);
        pcBytes += m_u16ProgramNameLength;
    }
     
    memcpy(pcBytes, &m_uQueueLength, sizeof(m_uQueueLength));
    pcBytes += sizeof(m_uQueueLength);

    return pvBytes;
}

void QueueLengthChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Deserializing members from bytes
    memcpy(&m_u16QueueNameLength, pcBytes, sizeof(m_u16QueueNameLength));
    pcBytes += sizeof(m_u16QueueNameLength);

    if(m_u16QueueNameLength)
    {
        m_strQueueName.assign(pcBytes, m_u16QueueNameLength);
        pcBytes += m_u16QueueNameLength;
    }
    else
        m_strQueueName = "";

    memcpy(&m_u16ProgramNameLength, pcBytes, sizeof(m_u16ProgramNameLength));
    pcBytes += sizeof(m_u16ProgramNameLength);

    if(m_u16ProgramNameLength)
    {
        m_strProgramName.assign(pcBytes, m_u16ProgramNameLength);
        pcBytes += m_u16ProgramNameLength;
    }
    else
        m_strQueueName = "";
        
    memcpy(&m_uQueueLength, pcBytes, sizeof(m_uQueueLength));
    pcBytes += sizeof(m_uQueueLength);
}

bool QueueLengthChunk::IsEqual(QueueLengthChunk& otherQueueLengthChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(otherQueueLengthChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare QueueLengthChunk parameters
    bool bIsEqual = (
        m_u16QueueNameLength == otherQueueLengthChunk.m_u16QueueNameLength &&
        m_strQueueName == otherQueueLengthChunk.m_strQueueName &&
        m_u16ProgramNameLength == otherQueueLengthChunk.m_u16ProgramNameLength &&
        m_strProgramName == otherQueueLengthChunk.m_strProgramName &&
        m_uQueueLength == otherQueueLengthChunk.m_uQueueLength
    );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}

std::shared_ptr<nlohmann::json> QueueLengthChunk::ToJSON()
{
    auto JSONDocument = nlohmann::json();
    auto strChunkName = "SystemInfo";

    // Adding in Basechunk fields
    JSONDocument[strChunkName]["StatEnvironment"] = std::string(m_strProgramName);
    JSONDocument[strChunkName]["StatName"] = std::string(m_strQueueName); 
    JSONDocument[strChunkName]["StatStaus"] = std::to_string(m_uQueueLength); 
    
    return std::make_shared<nlohmann::json>(JSONDocument);
}