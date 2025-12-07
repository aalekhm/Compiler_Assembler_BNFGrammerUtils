#include "ByteArrayStream.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

ByteArrayStream::ByteArrayStream()
{
	init(1);
}

ByteArrayStream::ByteArrayStream(uint64_t iArraySize)
{
	init(iArraySize);
}

ByteArrayStream::ByteArrayStream(int8_t* pByteArray, uint64_t iArraySize)
{
	m_pByteArray = nullptr;
	m_iCurrentPos = 0;
	m_iArraySize = iArraySize;
	m_eEndianness = ENDIANNESS::LITTLE;
	m_pByteArrayRO = pByteArray;
}

void ByteArrayStream::init(uint64_t iArraySize)
{
	m_pByteArray = nullptr;
	m_iCurrentPos = 0;
	m_iArraySize = iArraySize;
	m_eEndianness = ENDIANNESS::LITTLE;
	m_pByteArray = (int8_t*)malloc(sizeof(int8_t) * iArraySize);
}

void ByteArrayStream::setEndianness(ENDIANNESS eENDIANNESS)
{
	m_eEndianness = eENDIANNESS;
}

uint64_t ByteArrayStream::getCurrentOffset()
{
	return m_iCurrentPos;
}

void ByteArrayStream::setCurrentOffset(uint64_t iNewOffset)
{
	assert(iNewOffset <= m_iArraySize);
	m_iCurrentPos = iNewOffset;
}

void ByteArrayStream::reset()
{
	if (m_pByteArray != nullptr)
	{
		free(m_pByteArray);
		m_iArraySize = 0;
		m_pByteArray = nullptr;
	}

	m_iCurrentPos = 0;
}

void ByteArrayStream::resize(int32_t iSize)
{
	int32_t iNewSize = (iSize <= 1) ? 4 : (iSize * iSize);

	int8_t* pByteArray = (int8_t*)malloc(sizeof(int8_t) * iNewSize);
	assert(pByteArray != nullptr);
	if (pByteArray != nullptr)
	{
		memset(pByteArray, 0, sizeof(int8_t) * iNewSize);
		memcpy_s(pByteArray, iNewSize, m_pByteArray, m_iArraySize);

		if (m_pByteArray != nullptr)
		{
			free(m_pByteArray);
		}
		m_pByteArray = pByteArray;
		m_iArraySize = iNewSize;
	}
}

int32_t ByteArrayStream::getSize()
{
	return m_iArraySize;
}

int8_t* ByteArrayStream::get() const
{
	return m_pByteArray;
}
