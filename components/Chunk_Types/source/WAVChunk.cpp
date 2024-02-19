#include "WAVChunk.h"

WAVChunk::WAVChunk() :
	BaseChunk(),
	m_sWAVHeader(),
	m_i64TimeStamp(),
	m_vi16Data()
{

}

WAVChunk::WAVChunk(uint64_t i64TimeStamp) : 
	BaseChunk(),
	m_sWAVHeader(),
	m_i64TimeStamp(i64TimeStamp),
	m_vi16Data()
{

}

WAVChunk::WAVChunk(std::shared_ptr<WAVChunk> pWAVChunk)
{
	m_sWAVHeader = pWAVChunk->m_sWAVHeader;
	m_vi16Data = pWAVChunk->m_vi16Data;
	m_i64TimeStamp = pWAVChunk->m_i64TimeStamp;
}

WAVChunk::WAVChunk(WAVChunk& wavChunk)
{
	m_sWAVHeader = wavChunk.m_sWAVHeader;
	m_vi16Data = wavChunk.m_vi16Data;
	m_i64TimeStamp = wavChunk.m_i64TimeStamp;
}

WAVHeader WAVChunk::BytesToWAVHeader(std::vector<char>& vcWAVHeader)
{
	WAVHeader sWAVHeader;

	// RIFF
	sWAVHeader.RIFF[0] =  *(reinterpret_cast<uint8_t*>(&vcWAVHeader[0]));
	sWAVHeader.RIFF[1] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[1]));
	sWAVHeader.RIFF[2] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[2]));
	sWAVHeader.RIFF[3] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[3]));
	// ChunkSize
	sWAVHeader.ChunkSize = *(reinterpret_cast<uint32_t*>(&vcWAVHeader[4]));
	// WAVE
	sWAVHeader.WAVE[0] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[8]));
	sWAVHeader.WAVE[1] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[9]));
	sWAVHeader.WAVE[2] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[10]));
	sWAVHeader.WAVE[3] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[11]));
	// fmt
	sWAVHeader.fmt[0] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[12]));
	sWAVHeader.fmt[1] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[13]));
	sWAVHeader.fmt[2] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[14]));
	sWAVHeader.fmt[3] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[15]));
	// Subchunk1Size
	sWAVHeader.Subchunk1Size = *(reinterpret_cast<uint32_t*>(&vcWAVHeader[16]));
	// AudioFormat 
	sWAVHeader.AudioFormat = *(reinterpret_cast<uint16_t*>(&vcWAVHeader[20]));
	// NumOfChan
	sWAVHeader.NumOfChan = *(reinterpret_cast<uint16_t*>(&vcWAVHeader[22]));
	// SamplePerSec
	sWAVHeader.SamplesPerSec = *(reinterpret_cast<uint32_t*>(&vcWAVHeader[24]));
	// bytesPerSec
	sWAVHeader.bytesPerSec = *(reinterpret_cast<uint32_t*>(&vcWAVHeader[28]));
	// blockAlign 
	sWAVHeader.blockAlign = *(reinterpret_cast<uint16_t*>(&vcWAVHeader[32]));
	// bitsPerSample
	sWAVHeader.bitsPerSample = *(reinterpret_cast<uint16_t*>(&vcWAVHeader[34]));
	// Subchunk2ID
	sWAVHeader.Subchunk2ID[0] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[36]));
	sWAVHeader.Subchunk2ID[1] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[37]));
	sWAVHeader.Subchunk2ID[2] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[38]));
	sWAVHeader.Subchunk2ID[3] = *(reinterpret_cast<uint8_t*>(&vcWAVHeader[39]));
	// Subchunk2Size;
	sWAVHeader.Subchunk2Size = *(reinterpret_cast<uint32_t*>(&vcWAVHeader[40]));

   return sWAVHeader;
}

std::shared_ptr<std::vector<char>> WAVChunk::WAVHeaderToBytes()
{
	auto pvcByteData = std::make_shared<std::vector<char>>();
	pvcByteData->resize(sizeof(m_sWAVHeader));

	char* pcWavHeaderBytes = reinterpret_cast<char*>(&m_sWAVHeader);
	std::memcpy(&pvcByteData->at(0), &m_sWAVHeader, sizeof(m_sWAVHeader));

	return pvcByteData;
}

void WAVChunk::FormatWAVHeaderBytes(std::shared_ptr<std::vector<char>> pvcWAVHeaderBytes)
{
	// Converting the following sets of bytes to big endian
	std::reverse(pvcWAVHeaderBytes->begin()+0, pvcWAVHeaderBytes->begin()+4);	// RIFF
	std::reverse(pvcWAVHeaderBytes->begin()+8, pvcWAVHeaderBytes->begin()+12);	// fmt
	std::reverse(pvcWAVHeaderBytes->begin()+12, pvcWAVHeaderBytes->begin()+16);	// subChunk1
	std::reverse(pvcWAVHeaderBytes->begin()+36, pvcWAVHeaderBytes->begin()+41);	// subChunk2
}


void WAVChunk::UnpackWAVData(std::shared_ptr<std::vector<std::vector<int16_t>>> pvvdUnpackedWAVData)
{
	// Resizing and reserving unpacked vector to increase speed
	pvvdUnpackedWAVData->resize(m_sWAVHeader.NumOfChan);
	for (unsigned uChannelIndex = 0; uChannelIndex < m_sWAVHeader.NumOfChan; uChannelIndex++)
		(*pvvdUnpackedWAVData)[uChannelIndex].reserve(m_sWAVHeader.Subchunk2Size/ m_sWAVHeader.NumOfChan);

	unsigned uChannelIndex = 0;

	// Unpacking data into individual channels
	for (unsigned uDataIndex = 0; uDataIndex < m_vi16Data.size(); uDataIndex++)
	{
		(*pvvdUnpackedWAVData)[uChannelIndex].emplace_back(m_vi16Data[uDataIndex]);
		// check if  the next channel count is greater than available channels
		// then either increment or reset channel index
		uChannelIndex = (uChannelIndex + 1 >= m_sWAVHeader.NumOfChan) ? 0 : (uChannelIndex + 1);
	}
}

std::string WAVChunk::GetHeaderString()
{
	std::string sHeaderData = "";
	
	sHeaderData += "ChunkSize: " + std::to_string(m_sWAVHeader.ChunkSize) + "\n";
	sHeaderData += "Subchunk1Size: " + std::to_string(m_sWAVHeader.Subchunk1Size) + "\n";
	sHeaderData += "AudioFormat: " + std::to_string(m_sWAVHeader.AudioFormat) + "\n";
	sHeaderData += "NumOfChan: " + std::to_string(m_sWAVHeader.NumOfChan) + "\n";
	sHeaderData += "SamplesPerSec: " + std::to_string(m_sWAVHeader.SamplesPerSec) + "\n";
	sHeaderData += "BytesPerSec: " + std::to_string(m_sWAVHeader.bytesPerSec) + "\n";
	sHeaderData += "BlockAlign: " + std::to_string(m_sWAVHeader.blockAlign) + "\n";
	sHeaderData += "BitsPerSample: " + std::to_string(m_sWAVHeader.bitsPerSample) + "\n";
	sHeaderData += "Subchunk2Size: " + std::to_string(m_sWAVHeader.Subchunk2Size) + "\n";

	return sHeaderData;
}

unsigned WAVChunk::GetSize()
{
	return GetInternalSize();
}

unsigned WAVChunk::GetInternalSize()
{
	unsigned uByteSize = 0;

	// First check baseclass
	BaseChunk SelfBaseChunk = static_cast<BaseChunk&>(*this);
	uByteSize += SelfBaseChunk.GetSize();

	// Then this class
	uByteSize += sizeof(m_sWAVHeader);
	uByteSize += sizeof(m_i64TimeStamp);

	// Iterate over all elements of m_vvi16TimeChunks and infer type using auto
	for (auto itDatum = m_vi16Data.begin(); itDatum != m_vi16Data.end(); ++itDatum)
		uByteSize += sizeof(*itDatum);

	return uByteSize;
}

bool WAVChunk::IsEqual(WAVChunk &wavChunk)
{
	// We can compare base class
	auto SelfBaseChunk = static_cast<BaseChunk&>(*this);
	auto comparatorBaseChunk = static_cast<BaseChunk&>(wavChunk);
	bool bBaseEqual = SelfBaseChunk.IsEqual(comparatorBaseChunk);

	// We can then compare TimeChunk paramerters
	bool bIsEqual = (
		(m_i64TimeStamp == wavChunk.m_i64TimeStamp) &&
		(m_sWAVHeader == wavChunk.m_sWAVHeader) &&
		(m_vi16Data == wavChunk.m_vi16Data)
		);

	// Now we check base and derived classes are equal
	bIsEqual = (bBaseEqual && bIsEqual);

	return bIsEqual;
}

std::shared_ptr<nlohmann::json> WAVChunk::ToJSON()
{
	auto JSONDocument = nlohmann::json();
	auto strChunkName = ChunkTypesNamingUtility::toString(GetChunkType());

	// Adding in Basechunk fields
	JSONDocument[strChunkName]["SourceIdentifierSize"] = std::to_string(m_u16SourceIdentifierSize);
	JSONDocument[strChunkName]["SourceIdentifier"] = m_vu8SourceIdentifier;

	// Adding in WAVChunk fields
	JSONDocument[strChunkName]["WAVHeader"] = *m_sWAVHeader.ToJSON().get();
	JSONDocument[strChunkName]["ChunkSize"] = std::to_string(GetSize());
	JSONDocument[strChunkName]["TimeStamp"] = std::to_string(m_i64TimeStamp);

	std::stringstream stream;

	for (size_t vi16DatumIndex = 0; vi16DatumIndex < m_vi16Data.size(); ++vi16DatumIndex)
		stream << std::to_string(m_vi16Data[vi16DatumIndex]);

	JSONDocument[strChunkName]["VI16Data"] = stream.str();

	return std::make_shared<nlohmann::json>(JSONDocument);
}