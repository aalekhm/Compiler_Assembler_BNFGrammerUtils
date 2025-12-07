#include "IO/Buffer/BufferedStreamReader.h"
#include "Defines.h"

BufferedStreamReader::BufferedStreamReader(BufferData& pBuffer)
: m_iCurrentPointer(0)
, m_pBuffer(std::move(pBuffer))
{
	m_pBufferInternal = m_pBuffer.get();
	m_iSize = m_pBuffer.size();
}

void BufferedStreamReader::reset()
{
	m_iCurrentPointer = 0;
}

bool BufferedStreamReader::isEOS()
{
	return (m_iCurrentPointer >= m_iSize);
}

void BufferedStreamReader::seek(size_t iOffset)
{
	if (iOffset < m_iSize)
	{
		m_iCurrentPointer = iOffset;
	}
}

uint8_t BufferedStreamReader::readByte()
{
	if (m_iCurrentPointer < m_iSize)
	{
		uint8_t iByte = m_pBufferInternal[m_iCurrentPointer++];
		return (iByte & 0xff);
	}

	return -1;
}

uint16_t BufferedStreamReader::readShort()
{
	if ((m_iCurrentPointer + 1) < m_iSize)
	{
		return (readByte() | (uint16_t)readByte() << 8);
	}

	return -1;
}

uint32_t BufferedStreamReader::readInt()
{
	if ((m_iCurrentPointer + 3) < m_iSize)
	{
		return (readShort() | ((uint32_t)readShort() << 16));
	}

	return -1;
}

uint64_t BufferedStreamReader::readLong()
{
	if ((m_iCurrentPointer + 7) < m_iSize)
	{
		return (readInt() | (uint64_t)readInt() << 32);
	}

	return -1;
}

size_t BufferedStreamReader::read(int8_t* pDstBuffer)
{
	size_t iBytesRead = read(pDstBuffer, 0, m_iSize);
	if (iBytesRead > 0)
	{
		m_iCurrentPointer += iBytesRead;
	}

	return iBytesRead;
}

size_t BufferedStreamReader::read(int8_t* pDstBuffer, size_t iLength)
{
	size_t iBytesRead = read(pDstBuffer, m_iCurrentPointer, iLength);
	if (iBytesRead > 0)
	{
		m_iCurrentPointer += iBytesRead;
	}

	return iBytesRead;
}

size_t BufferedStreamReader::read(int8_t* pDstBuffer, size_t iOffset, size_t iLength)
{
	if (iOffset >= 0 && iOffset < m_iSize)
	{
		int32_t iMaxReadOffset = (iOffset + iLength);
		if (iMaxReadOffset > m_iSize)
		{
			iLength = m_iSize - iOffset;
		}

		memcpy_s(pDstBuffer, iLength, m_pBufferInternal + iOffset, iLength);

		return iLength;
	}

	return -1;
}

std::string BufferedStreamReader::readLine()
{
	if (NOT isEOS())
	{
		std::string sLine;
		unsigned char c = readByte();

		while (true)
		{
			if (NOT(c == '\r' || c == '\n' || c == 0))
			{
				if (c != 255)
				{
					sLine.push_back(c);
					c = readByte();
					//m_iCurrentPointer++;
				}
				else
					break;
			}
			else
			{	
				break;
			}
		}

		return sLine;
	}

	return "";
}

void BufferedStreamReader::close()
{
	m_iCurrentPointer = 0;
	m_pBufferInternal = nullptr;
	m_iSize = -1;
}