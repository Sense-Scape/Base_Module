#ifndef SESSION_CONTROLLER
#define SESSION_CONTROLLER

#include <utility>
#include "ByteChunk.h"

/*
* Base Session type used as inheritance mechanisim for derived session type
*/
class SessionController
{

public:
	char m_cTransmissionState;		///< Map of transmission state (byte position and value)
	uint32_t m_uSessionNumber;		///< Map of session number (byte position and value)
	uint32_t m_uSequenceNumber;		///< Map of sequence number (byte position and value)
	uint32_t m_u32uChunkType;		///< Map of contained chunk type (byte position and value)
	std::vector<uint8_t> m_usUID = std::vector<uint8_t>({ 0,0,0,0,0,0 });///< Map of unique unique identifier
	uint32_t m_uTransmissionSize;	///< Map of transmission data size (byte position and value, 0 - Transmitting; 1 - finished)

	/**
	* @brief Constructor for the session mode
	* @param[in] Enumnerated session type corresponding to sessionModeType
	*/
	SessionController() :
		m_cTransmissionState(),
		m_uSessionNumber(),
		m_uSequenceNumber(),
		m_u32uChunkType(),
		m_uTransmissionSize()
	{
	};


	/**
	* @brief Copy constructor
	*/
	SessionController(SessionController& reliableSessionSessionMode)
	{
		m_cTransmissionState = reliableSessionSessionMode.m_cTransmissionState;
		m_uSessionNumber = reliableSessionSessionMode.m_uSessionNumber;
		m_uSequenceNumber = reliableSessionSessionMode.m_uSequenceNumber;
		m_u32uChunkType = reliableSessionSessionMode.m_u32uChunkType;
		m_usUID = reliableSessionSessionMode.m_usUID;
		m_uTransmissionSize = reliableSessionSessionMode.m_uTransmissionSize;
	}

	/**
	* @brief Returns if the two classes are equal
	* @return Reference to the class with which we want to compare
	*/
	bool IsEqual(SessionController& reliableSessionSessionMode)
	{
		bool bIsEqual = (
			(m_cTransmissionState == reliableSessionSessionMode.m_cTransmissionState) &&
			(m_uSessionNumber == reliableSessionSessionMode.m_uSessionNumber) &&
			(m_uSequenceNumber == reliableSessionSessionMode.m_uSequenceNumber) &&
			(m_u32uChunkType == reliableSessionSessionMode.m_u32uChunkType) &&
			(m_usUID == reliableSessionSessionMode.m_usUID) &&
			(m_uTransmissionSize == reliableSessionSessionMode.m_uTransmissionSize)
			);

		return bIsEqual;
	}

	/**
	 * @brief Get the size of object in bytes
	 * @return Custom size of object in bytes
	 */
	unsigned GetSize()
	{
		unsigned uByteSize = 0;

		// Then this class
		uByteSize += sizeof(m_cTransmissionState);
		uByteSize += sizeof(m_uSessionNumber);
		uByteSize += sizeof(m_uSequenceNumber);
		uByteSize += sizeof(m_u32uChunkType);
		uByteSize += sizeof(m_usUID[0]) * 6;
		uByteSize += sizeof(m_uTransmissionSize);

		return uByteSize;
	}

	/**
	 * @brief Fill a byte array the represents this object
	 * @return pByteArray Shared pointer to byte vector containing byte data
	 */
	std::shared_ptr<std::vector<char>> Serialise()
	{
		auto pvBytes = std::make_shared<std::vector<char>>();
		unsigned uSize = GetSize();
		pvBytes->resize(uSize);
		char* pcBytes = pvBytes->data();

		memcpy(pcBytes, &m_cTransmissionState, sizeof(m_cTransmissionState));
		pcBytes += sizeof(m_cTransmissionState);

		memcpy(pcBytes, &m_uSessionNumber, sizeof(m_uSessionNumber));
		pcBytes += sizeof(m_uSessionNumber);

		memcpy(pcBytes, &m_uSequenceNumber, sizeof(m_uSequenceNumber));
		pcBytes += sizeof(m_uSequenceNumber);

		memcpy(pcBytes, &m_u32uChunkType, sizeof(m_u32uChunkType));
		pcBytes += sizeof(m_u32uChunkType);

		// Converting vector to bytes
		for (const auto& u8UIDElement : m_usUID)
		{
			unsigned uChunkSizeBytes = sizeof(u8UIDElement);
			memcpy(pcBytes, &u8UIDElement, uChunkSizeBytes);
			pcBytes += uChunkSizeBytes;
		}


		memcpy(pcBytes, &m_uTransmissionSize, sizeof(m_uTransmissionSize));
		pcBytes += sizeof(m_uTransmissionSize);

		return pvBytes;
	}

	/**
	 * @brief Converts byte array to object members
	 * @param[in] pvBytes Shared pointer to byte array that shall be used to construct memeber variables
	 */
	void Deserialise(std::shared_ptr<std::vector<char>> pvBytes)
	{
		char* pcBytes = pvBytes->data();

		// Converting members to bytes
		memcpy(&m_cTransmissionState, pcBytes, sizeof(m_cTransmissionState));
		pcBytes += sizeof(m_cTransmissionState);

		memcpy(&m_uSessionNumber, pcBytes, sizeof(m_uSessionNumber));
		pcBytes += sizeof(m_uSessionNumber);

		memcpy(&m_uSequenceNumber, pcBytes, sizeof(m_uSequenceNumber));
		pcBytes += sizeof(m_uSequenceNumber);

		memcpy(&m_u32uChunkType, pcBytes, sizeof(m_u32uChunkType));
		pcBytes += sizeof(m_u32uChunkType);

		auto uSampleSize = sizeof(m_usUID[0]);
		for (unsigned uUIDIndex = 0; uUIDIndex < 6; uUIDIndex++)
		{
			memcpy(&m_usUID[uUIDIndex], pcBytes, uSampleSize);
			pcBytes += uSampleSize;
		}

		memcpy(&m_uTransmissionSize, pcBytes, sizeof(m_uTransmissionSize));
		pcBytes += sizeof(m_uTransmissionSize);
	}

	/**
	 * @brief increment session number
	 */
	void IncrementSession()
	{
		m_uSessionNumber++;
		m_uSequenceNumber = 0;

		// And reset transmission state
		m_cTransmissionState = 0;
	}

	/**
	 * @brief increment session number
	 */
	void IncrementSequence()
	{
		m_uSequenceNumber++;
	}

};

#endif
