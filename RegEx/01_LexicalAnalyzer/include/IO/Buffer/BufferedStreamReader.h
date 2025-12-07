#pragma once
#include "IO/Buffer/BufferData.hpp"
#include <string>

class BufferedStreamReader
{
	public:
						BufferedStreamReader(BufferData& pBuffer);

		void			seek(size_t iOffset);
		size_t			read(int8_t* pDstBuffer);
		size_t			read(int8_t* pDstBuffer, size_t iLength);
		size_t			read(int8_t* pDstBuffer, size_t iOffset, size_t iLength);

		uint8_t			readByte();
		uint16_t		readShort();
		uint32_t		readInt();
		uint64_t		readLong();
		std::string		readLine();

		bool			isEOS();
		void			reset();
		void			close();
	protected:
	private:
		BufferData		m_pBuffer;
		size_t			m_iCurrentPointer;

		int8_t*			m_pBufferInternal;
		size_t			m_iSize;
};