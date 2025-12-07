#pragma once

#include <stdio.h>
#include <string>
#include <fstream>

class BufferedStreamReader;
class BufferedStreamWriter;

enum class PERM
{
	PERM_READ = _S_IREAD,
	PERM_WRITE = _S_IWRITE,
	PERM_EXEC = _S_IEXEC,
	PERM_ALL = PERM_READ | PERM_WRITE | PERM_EXEC
};

class RandomAccessFile
{
	public:
								RandomAccessFile();
		virtual					~RandomAccessFile();

		bool					open(const char* sFileName, std::ios_base::openmode eMode);

		static bool				touch(const char* sFileNameToCreate);
		static bool				rename(const char* sOldFileName, const char* sNewFileName);
		static bool				deleteFile(const char* sFileName);
		static bool				deleteDirectory(const char* sFileName, bool bRecursive = false);
		static unsigned long	length(const char* sFileName);
		static bool				exists(const char* sFileName);

		void					seek(size_t iOffset);

		uint8_t					readByte();
		uint16_t				readShort();
		uint32_t				readInt();
		uint64_t				readLong();
		std::string				readLine();
		unsigned long			read(int8_t* pBuf, int iLength);

		unsigned long			readFully(int8_t* pBuf);
		unsigned long			read(int8_t* pBuf, int iOffset, int iLength);

		unsigned long			length();
		void					reset();
		////////////////////////////////////////////////////////////////
		bool					openForRead(const char* sFileName);
		bool					openForWrite(const char* sFileName);
		bool					openForAppend(const char* sFileName);

		bool					renameFile(const char* sNewFileName);
		
		unsigned long			readOld(char* pBuf, int iOffset, int iLength);
		char					readChar();
		std::string				readLineOld();

		size_t					write(const int8_t* sBuf, size_t iSizeInBytes);
		unsigned long 		write(const char* sBuf);
		unsigned long		write(const char* sBuf, unsigned long iOffset, unsigned long iLen);
		void				writeLine(const char* sBuf);

		void				writeByte(int byte0);
		void				writeShort(int shortValue);
		void				writeInt(unsigned long);
		void				writeLong(unsigned long long longValue);
		void				flush();

		bool				isEOF();

		void				close();
	protected:
	private:
		void				initBufferedReader(std::fstream& fStream, size_t iBufferSize);
		void				initBufferedWriter(std::ios_base::openmode eMode, std::fstream& fStream, size_t iBufferSize);

		FILE*				m_pFileObj;
		unsigned long long	m_iFileOffset;
		unsigned long long	m_iFileSize;
		std::string			m_sFileName;

		std::ios_base::openmode	m_eMode;
		std::fstream		m_fStream;

		std::unique_ptr<BufferedStreamReader>	m_pBufferedReader;
		std::unique_ptr<BufferedStreamWriter>	m_pBufferedWriter;
};