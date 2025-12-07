#include "IO/RandomAccessFile.h"
#include "IO/Buffer/BufferedStreamReader.h"
#include "IO/Buffer/BufferedStreamWriter.h"
#include <io.h>
#include <iostream>
#include <filesystem>

#define ACCESS_EXISTENCE_ONLY	0
#define ACCESS_WRITE_ONLY		2
#define ACCESS_READ_ONLY		4
#define ACCESS_READ_AND_WRITE	6

#define SUCCESS					0
#define NOT						!

RandomAccessFile::RandomAccessFile()
: m_pFileObj(nullptr)
, m_iFileOffset(0)
, m_sFileName("")
, m_iFileSize(-1)
, m_pBufferedReader(nullptr)
{
}

bool RandomAccessFile::touch(const char* sFileNameToCreate)
{
	if (NOT RandomAccessFile::exists(sFileNameToCreate))
	{
		std::fstream fileStream;

		fileStream.open(sFileNameToCreate, std::ios::out);
		if (NOT fileStream)
		{
			std::cout << "Specified file could not be created. " << sFileNameToCreate << std::endl;

			return false;
		}

		fileStream.close();
	}
	else
	{
		std::cout << "Specified file already exists!" << std::endl;
	}

	return true;
}

bool RandomAccessFile::exists(const char* sFileName)
{
//#if __cplusplus == 201703L
	return std::filesystem::exists(sFileName);
//#else
//	return (_access(sFileName, ACCESS_EXISTENCE_ONLY) == SUCCESS);
//#endif
}

void RandomAccessFile::seek(size_t iOffset)
{
	m_pBufferedReader->seek(iOffset);
	//if (iOffset >= 0 && iOffset < m_iFileSize)
	//{
	//	m_fStream.seekg(iOffset);
	//}
}

void RandomAccessFile::initBufferedReader(std::fstream& fStream, size_t iBufferSize)
{
	BufferData bufferData(iBufferSize);
	fStream.read((char*)bufferData.get(), iBufferSize);

	m_pBufferedReader = std::make_unique<BufferedStreamReader>(bufferData);
}

void RandomAccessFile::initBufferedWriter(std::ios_base::openmode eMode, std::fstream& fStream, size_t iBufferSize)
{
	BufferData bufferData(iBufferSize);
	m_pBufferedWriter = std::make_unique<BufferedStreamWriter>(iBufferSize);

	if (eMode == std::ios_base::app)
	{
		fStream.read((char*)bufferData.get(), iBufferSize);
		m_pBufferedWriter->write(bufferData.get(), iBufferSize);
	}
}

bool RandomAccessFile::open(const char* sFileName, std::ios_base::openmode eMode)
{
	bool bReturn = false;
	switch (eMode)
	{
		case std::ios_base::in:
		{
			if (exists(sFileName))
			{
				m_fStream.open(sFileName, eMode);
				if (m_fStream.good())
				{
					m_eMode = eMode;
					m_sFileName = sFileName;
					m_iFileSize = length(sFileName);

					initBufferedReader(m_fStream, m_iFileSize);

					bReturn = true;
				}
			}
		}
		break;
		case std::ios_base::out:
		case std::ios_base::app:
		{
			if (NOT exists(sFileName))
			{
				touch(sFileName);
			}

			m_fStream.open(sFileName, eMode);
			if (m_fStream.good())
			{
				m_eMode = eMode;
				m_sFileName = sFileName;
				m_iFileSize = length(sFileName);
				if (m_iFileSize == 0)
				{
					m_iFileSize = 8;
				}

				initBufferedWriter(eMode, m_fStream, m_iFileSize);

				bReturn = true;
			}
		}
		break;
	}

	return bReturn;
	//if (exists(sFileName))
	//{
	//	m_fStream.open(sFileName, eMode);
	//	if (m_fStream.good())
	//	{
	//		m_iFileOffset = 0;
	//		m_sFileName = sFileName;
	//
	//		if (eMode == std::ios_base::in)
	//		{
	//			m_iFileSize = length(sFileName);
	//			initBufferedReader(m_iFileSize);
	//		}
	//		else
	//		if (eMode == std::ios_base::out)
	//		{
	//
	//		}
	//
	//		return true;
	//	}
	//}
	//
	//return false;
}

bool RandomAccessFile::openForRead(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sFileName << std::endl;

		return false;
	}

	// Open file for reading
	fErr = fopen_s(&m_pFileObj, sFileName, "rb");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for reading." << std::endl;
		return false;
	}

	m_iFileOffset = 0;
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::openForWrite(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file can be written to!
	fErr = _access(sFileName, ACCESS_WRITE_ONLY);
	if (fErr == EACCES)
	{
		std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		return false;
	}

	// Open file for writing.
	fErr = fopen_s(&m_pFileObj, sFileName, "wb+");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for writing." << std::endl;
		return false;
	}

	m_iFileOffset = 0;
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::openForAppend(const char* sFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sFileName << std::endl;

		return false;
	}

	// Open file for appending.
	fErr = fopen_s(&m_pFileObj, sFileName, "a+");

	if (fErr != SUCCESS)
	{
		std::cout << "could not open File / Directory " << sFileName << " for appending." << std::endl;
		return false;
	}

	m_iFileOffset = RandomAccessFile::length(sFileName);
	m_sFileName = sFileName;

	return true;
}

bool RandomAccessFile::rename(const char* sOldFileName, const char* sNewFileName)
{
	return (std::rename(sOldFileName, sNewFileName) == 0);
}
bool RandomAccessFile::renameFile(const char* sNewFileName)
{
	errno_t fErr;

	// Check if specified file exists !
	fErr = _access(sNewFileName, ACCESS_EXISTENCE_ONLY);
	if (fErr != SUCCESS)
	{
		if (fErr == EACCES)
			std::cout << "Access denied : the file's permission setting does not allow specified access. " << sNewFileName << std::endl;

		if (fErr == ENOENT)
			std::cout << "File name or path not found. " << sNewFileName << std::endl;

		return false;
	}

	// try renaming the file.
	fErr = ::rename(m_sFileName.c_str(), sNewFileName);

	if (fErr != SUCCESS)
	{
		std::cout << "could not rename File / Directory " << m_sFileName << std::endl;
		return false;
	}

	m_sFileName = sNewFileName;

	return true;
}

bool RandomAccessFile::deleteFile(const char* sFileName)
{
//#if __cplusplus == 201703L
	return (std::filesystem::remove(sFileName) == 0);
//#else
//	return (::remove(sFileName) == SUCCESS);
//#endif
}

bool RandomAccessFile::deleteDirectory(const char* sFileName, bool bRecursive)
{
//#if __cplusplus == 201703L
	return bRecursive ? std::filesystem::remove_all(sFileName) : std::filesystem::remove(sFileName);
//#else
//	return false;
//#endif
}

unsigned long RandomAccessFile::length(const char* sFileName)
{
	if (RandomAccessFile::exists(sFileName))
	{
//#if __cplusplus == 201703L
		return std::filesystem::file_size(sFileName);
//#else
//#endif
	}

	return -1;
}

unsigned long RandomAccessFile::length()
{
	return m_iFileSize;
}

void RandomAccessFile::reset()
{
	return m_pBufferedReader->reset();
}

unsigned long RandomAccessFile::read(int8_t* pBuf, int iLength)
{
	return m_pBufferedReader->read(pBuf, iLength);
}

unsigned long RandomAccessFile::readFully(int8_t* pBuf)
{
	return m_pBufferedReader->read(pBuf);
}

unsigned long RandomAccessFile::read(int8_t* pBuf, int iOffset, int iLength)
{
	return m_pBufferedReader->read(pBuf, iOffset, iLength);
}

unsigned long RandomAccessFile::readOld(char* pBuf, int iOffset, int iLength)
{
	int iCount = 0;
	int iBytesToRead = iLength, iBytesRead = 0;

	if (iBytesToRead <= 0)
		return -1;

	if(m_pFileObj != nullptr)
	{	
		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		while (	NOT isEOF()
				&&
				iBytesRead < iBytesToRead
		) {
			iBytesRead = fread(pBuf + iOffset, sizeof(char), iBytesToRead, m_pFileObj);
			m_iFileOffset += iBytesRead;
		}

		pBuf[iLength] = '\0';
	}

	return iBytesRead;
}

char RandomAccessFile::readChar() 
{
	if (m_pFileObj != NULL) 
	{
		unsigned char ch;

		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		ch = fgetc(m_pFileObj);

		m_iFileOffset += 1;

		return ch;
	}

	return NULL;
}

uint8_t RandomAccessFile::readByte()
{
	return m_pBufferedReader->readByte();
}

uint16_t RandomAccessFile::readShort()
{
	return m_pBufferedReader->readShort();
}

uint32_t RandomAccessFile::readInt()
{
	return m_pBufferedReader->readInt();
}

uint64_t RandomAccessFile::readLong()
{
	return m_pBufferedReader->readLong();
}

std::string RandomAccessFile::readLine()
{
	return m_pBufferedReader->readLine();
}

std::string RandomAccessFile::readLineOld()
{
	if (NOT isEOF())
	{
		int iPrevFilePointer = m_iFileOffset;

		unsigned char c = readChar();
		while (NOT(c == '\r' || c == '\n') && NOT isEOF())
		{
			c = readChar();
		}

		int iLineLength = m_iFileOffset - iPrevFilePointer - 1;
		if (c == '\r')
		{
			m_iFileOffset++;
		}

		if (iLineLength > 0)
		{
			void* pLine = calloc(iLineLength, sizeof(pLine));

			fseek(m_pFileObj, iPrevFilePointer, SEEK_SET);

			int iRead = fread(pLine, sizeof(char), iLineLength, m_pFileObj);
			fseek(m_pFileObj, m_iFileOffset, SEEK_SET);

			std::string sLine = (char*)pLine;
			delete[] pLine;

			return sLine;
		}
	}

	return "";
}

size_t RandomAccessFile::write(const int8_t* sBuffer, size_t iSizeInBytes)
{
	return m_pBufferedWriter->write(sBuffer, iSizeInBytes);
}

unsigned long RandomAccessFile::write(const char* sBuf)
{
	int iBytesWritten = 0;
	if (m_pFileObj != nullptr)
	{
		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		iBytesWritten = fwrite(sBuf, sizeof(char), strlen(sBuf), m_pFileObj);

		m_iFileOffset += iBytesWritten;
	}

	return iBytesWritten;
}

unsigned long RandomAccessFile::write(const char* sBuf, unsigned long iOffset, unsigned long iLen)
{
	int iBytesWritten = 0;
	if (m_pFileObj != nullptr)
	{
		char* pStartBufOffset = (char*)sBuf + iOffset;

		fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
		iBytesWritten = fwrite(pStartBufOffset, sizeof(char), iLen, m_pFileObj);

		m_iFileOffset += iBytesWritten;
	}

	return iBytesWritten;
}

void RandomAccessFile::writeLine(const char* sBuf)
{
	m_pBufferedWriter->writeLine(sBuf);
	//if (m_pFileObj != nullptr)
	//{
	//	write(sBuf);
	//	write("\r\n");
	//}
}

bool RandomAccessFile::isEOF()
{
	return m_pBufferedReader->isEOS();
}

void RandomAccessFile::writeByte(int byte0) 
{
	m_pBufferedWriter->writeByte(byte0);
	//if (m_pFileObj != NULL)
	//{
	//	fseek(m_pFileObj, m_iFileOffset, SEEK_SET);
	//	fputc(byte0, m_pFileObj);
	//
	//	m_iFileOffset += 1;
	//}
}

void RandomAccessFile::writeShort(int shortValue) 
{
	m_pBufferedWriter->writeShort(shortValue);
	//if (m_pFileObj != NULL) 
	//{
	//	writeByte(shortValue & 0xff);
	//	writeByte((shortValue >> 8) & 0xff);
	//}
}

void RandomAccessFile::writeInt(unsigned long intValue)
{
	m_pBufferedWriter->writeInt(intValue);
	//if (m_pFileObj != NULL) 
	//{
	//	writeShort(intValue & 0xffff);
	//	writeShort((intValue >> 16) & 0xffff);
	//}
}

void RandomAccessFile::writeLong(unsigned long long longValue) 
{
	m_pBufferedWriter->writeLong(longValue);
	//if (m_pFileObj != NULL) 
	//{
	//	writeInt(longValue & 0xffffffff);
	//	writeInt((longValue >> 32) & 0xffffffff);
	//}
}

void RandomAccessFile::flush()
{
	if (m_eMode == std::ios_base::out)
	{
		if(m_fStream.good())
			m_pBufferedWriter->flush(m_fStream);
	}
}

void RandomAccessFile::close()
{
	if (m_eMode == std::ios_base::in)
	{
		if (m_pBufferedReader != nullptr)
		{
			m_pBufferedReader->close();
			m_pBufferedReader = nullptr;

			m_fStream.close();
		}
	}
	else
	if (m_eMode == std::ios_base::out)
	{
		if (m_pBufferedWriter != nullptr)
		{
			flush();
			m_pBufferedWriter->close();
			m_pBufferedWriter = nullptr;

			m_fStream.close();
		}
	}
	//if (m_pFileObj != nullptr)
	//{
	//	errno_t fErr;
	//	fErr = fclose(m_pFileObj);
	//
	//	if (fErr == 0)
	//	{
	//		std::cout << "File " << m_sFileName << " closed successfully\n" << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "Unable to close File \n" << m_sFileName << std::endl;
	//	}
	//}
	//
	//m_pFileObj = nullptr;
	//m_iFileOffset = 0;
	//m_sFileName = "";
}

RandomAccessFile::~RandomAccessFile()
{
	close();
}
