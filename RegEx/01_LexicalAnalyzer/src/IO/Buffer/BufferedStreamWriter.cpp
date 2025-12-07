#include "IO/Buffer/BufferedStreamWriter.h"
#include <fstream>

BufferedStreamWriter::BufferedStreamWriter(size_t iSize)
: m_iCurrentPointer(0)
{
	m_pBuffer.resize(iSize);

	m_pBufferInternal = m_pBuffer.get();
	m_iSize = m_pBuffer.size();
}

BufferedStreamWriter::BufferedStreamWriter(BufferData& pBuffer)
: m_iCurrentPointer(0)
, m_pBuffer(std::move(pBuffer))
{
	m_pBufferInternal = m_pBuffer.get();
	m_iSize = m_pBuffer.size();
}

void BufferedStreamWriter::reset()
{
	m_iCurrentPointer = 0;
}

void BufferedStreamWriter::seek(size_t iOffset)
{
	if (iOffset <= m_iSize)
	{
		m_iCurrentPointer = iOffset;
	}
}

void BufferedStreamWriter::resizeInternalBuffer(size_t iNewSize)
{
	m_pBuffer.resize(iNewSize << 1);

	m_pBufferInternal = m_pBuffer.get();
	m_iSize = m_pBuffer.size();
}

bool BufferedStreamWriter::peek(size_t iOffset, size_t iSizeInBytes)
{
	size_t iSizeNeeded = (iOffset + iSizeInBytes);
	if (iSizeNeeded >= m_iSize)
	{
		resizeInternalBuffer(iSizeNeeded);
	}

	return true;
}

void BufferedStreamWriter::writeByte(uint8_t byte)
{
	if (peek(m_iCurrentPointer, 0))
	{
		m_pBufferInternal[m_iCurrentPointer++] = byte;
	}
	//if (m_pBuffer != NULL)
	//{
	//	*m_pBuffer = byte;
	//	m_pBuffer++;
	//}
}

void BufferedStreamWriter::writeShort(uint16_t shortValue)
{
	if (peek(m_iCurrentPointer, 1))
	{
		writeByte(shortValue & 0xff);
		writeByte((shortValue >> 8) & 0xff);
	}

	//if (m_pBuffer != NULL)
	//{
	//	writeByte(shortValue & 0xff);
	//	writeByte((shortValue >> 8) & 0xff);
	//}
}

void BufferedStreamWriter::writeInt(uint32_t intValue)
{
	if (peek(m_iCurrentPointer, 3))
	{
		writeShort(intValue & 0xffff);
		writeShort((intValue >> 16) & 0xffff);
	}
	//if (m_pBuffer != nullptr)
	//{
	//	writeShort(intValue & 0xffff);
	//	writeShort((intValue >> 16) & 0xffff);
	//}
}

void BufferedStreamWriter::writeLong(uint64_t longValue)
{
	if (peek(m_iCurrentPointer, 7))
	{
		writeInt(longValue & 0xffffffff);
		writeInt((longValue >> 32) & 0xffffffff);
	}
	//if (m_pBuffer != nullptr)
	//{
	//	writeInt(longValue & 0xffffffff);
	//	writeInt((longValue >> 32) & 0xffffffff);
	//}
}

size_t BufferedStreamWriter::write(const int8_t* pSrc, size_t iSizeInBytes)
{
	if (peek(m_iCurrentPointer, iSizeInBytes))
	{
		int8_t* pDst = m_pBufferInternal + m_iCurrentPointer;
		memcpy_s(pDst, iSizeInBytes, pSrc, iSizeInBytes);

		m_iCurrentPointer += iSizeInBytes;
		return iSizeInBytes;
	}

	return -1;
}

size_t BufferedStreamWriter::write(const char* pSrc, unsigned long iOffset, unsigned long iSizeInBytes)
{
	if (peek(iOffset, iSizeInBytes))
	{
		int8_t* pDst = m_pBufferInternal + iOffset;
		memcpy_s(pDst, iSizeInBytes, pSrc, iSizeInBytes);

		return iSizeInBytes;
	}

	return -1;
}

void BufferedStreamWriter::writeLine(const std::string& sLine)
{
	size_t iLineSize = sLine.size();
	if (peek(m_iCurrentPointer, iLineSize))
	{
		write((const int8_t*)sLine.c_str(), iLineSize);
		write((const int8_t*)"\n", 1);
	}
}

void BufferedStreamWriter::flush(std::fstream& fStream)
{
	fStream.write((char*)m_pBufferInternal, m_iCurrentPointer);
}

void BufferedStreamWriter::close()
{
	m_iCurrentPointer = 0;
	m_pBufferInternal = nullptr;
	m_iSize = -1;
}