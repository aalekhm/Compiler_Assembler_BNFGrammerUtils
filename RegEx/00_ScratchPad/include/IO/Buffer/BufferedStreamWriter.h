#pragma once
#include "IO/Buffer/BufferData.hpp"
#include <string>

class BufferedStreamWriter
{
	public:
						BufferedStreamWriter(size_t iSize);
						BufferedStreamWriter(BufferData& pBuffer);

		void			writeByte(uint8_t byte);
		void			writeShort(uint16_t shortValue);
		void			writeInt(uint32_t intValue);
		void			writeLong(uint64_t longValue);

		size_t			write(const int8_t* pSrc, size_t iSizeInBytes);
		size_t			write(const char* pSrc, unsigned long iOffset, unsigned long iSizeInBytes);
		void			writeLine(const std::string& sLine);
		void			flush(std::fstream& sfStream);

		void			seek(size_t iOffset);
		void			reset();
		void			close();

		int8_t*			get();
	protected:
	private:
		bool			peek(size_t iOffset, size_t iSizeInBytes);
		void			resizeInternalBuffer(size_t iNewSize);

		BufferData		m_pBuffer;
		size_t			m_iCurrentPointer;

		int8_t*			m_pBufferInternal;
		size_t			m_iSize;
};