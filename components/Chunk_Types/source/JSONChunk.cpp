#include "JSONChunk.h"

JSONChunk::JSONChunk() : BaseChunk(),
    m_JSONDocumentSize_bytes(0),
    m_JSONDocument()
{
}

JSONChunk::JSONChunk(std::shared_ptr<JSONChunk> pJSONChunk) : BaseChunk(pJSONChunk)
{
	m_JSONDocument = pJSONChunk->m_JSONDocument;
    m_JSONDocumentSize_bytes = pJSONChunk->m_JSONDocumentSize_bytes;
}

std::shared_ptr<std::vector<char>> JSONChunk::Serialise()
{
    return GetInternalSerialisation();
}

std::shared_ptr<std::vector<char>> JSONChunk::GetInternalSerialisation()
{
    auto pvBytes = std::make_shared<std::vector<char>>();
    pvBytes->resize(GetSize());
    char* pcBytes = pvBytes->data();

    // Copy base data
    auto pcvBaseBytes = BaseChunk::GetInternalSerialisation();
    unsigned uBaseClassSize = BaseChunk::GetSize();
    memcpy(pcBytes, &pcvBaseBytes->front(), uBaseClassSize);
    pcBytes += uBaseClassSize;

    // Serialising the JSON document
    auto pcJSONDocumentDump = m_JSONDocument.dump();
    m_JSONDocumentSize_bytes = pcJSONDocumentDump == "null" ? 0 : pcJSONDocumentDump.length();

    memcpy(pcBytes, &m_JSONDocumentSize_bytes, sizeof(m_JSONDocumentSize_bytes));
    pcBytes += sizeof(m_JSONDocumentSize_bytes);

    memcpy(pcBytes, &pcJSONDocumentDump.front(), m_JSONDocumentSize_bytes);
    pcBytes += m_JSONDocumentSize_bytes;

    return pvBytes;
}

void JSONChunk::Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
{
    // Fill in base data
    BaseChunk::Deserialise(pvBytes);

    char* pcBytes = pvBytes->data();
    pcBytes += BaseChunk::GetSize();

    // Converting members to bytes
    memcpy(&m_JSONDocumentSize_bytes, pcBytes, sizeof(m_JSONDocumentSize_bytes));
    pcBytes += sizeof(m_JSONDocumentSize_bytes);

    // Creating JSON document
    std::string strJSONDocument;
    strJSONDocument.reserve(m_JSONDocumentSize_bytes);
    strJSONDocument.assign(pcBytes, m_JSONDocumentSize_bytes);

    m_JSONDocument = m_JSONDocumentSize_bytes == 0 ? nlohmann::json() : nlohmann::json::parse(strJSONDocument);

    pcBytes += m_JSONDocumentSize_bytes;
}

bool JSONChunk::IsEqual(JSONChunk& jsonChunk)
{
    // We can compare base class
    auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
    auto comparatorBaseChunk = static_cast<BaseChunk&>(jsonChunk);
    bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

    // We can then compare TimeChunk paramerters
    bool bIsEqual = (
        m_JSONDocument == jsonChunk.m_JSONDocument &&
        m_JSONDocumentSize_bytes == jsonChunk.m_JSONDocumentSize_bytes
        );

    // Now we check base and derived classes are equal
    bIsEqual = (bBaseEqual && bIsEqual);

    return bIsEqual;
}


unsigned JSONChunk::GetSize()
{
    return GetInternalSize();
}


unsigned JSONChunk::GetInternalSize()
{
    unsigned uByteSize = 0;

    // First check baseclass
    BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
    uByteSize += SelfBaseChunk.GetSize();

    // Serialising the JSON document
    auto pcJSONDocumentDump = m_JSONDocument.dump();
    m_JSONDocumentSize_bytes = pcJSONDocumentDump == "null" ? 0 : pcJSONDocumentDump.length();

    // Then calcualte class size
    uByteSize += sizeof(m_JSONDocumentSize_bytes);
    uByteSize += m_JSONDocumentSize_bytes;

    return uByteSize;
}
