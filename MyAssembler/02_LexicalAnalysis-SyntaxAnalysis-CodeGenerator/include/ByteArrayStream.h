#pragma once
#include <stdint.h>

enum class ENDIANNESS
{
	LITTLE,
	BIG
};

class ByteArrayStream
{
	public:
		ByteArrayStream();
		ByteArrayStream(uint64_t iArraySize);
		ByteArrayStream(int8_t* pByteArray, uint64_t iArraySize);

		void			setEndianness(ENDIANNESS eENDIANNESS);

		uint64_t		getCurrentOffset();
		void			setCurrentOffset(uint64_t iNewOffset);

		void			reset();

		int32_t			getSize();
		int8_t*			get() const;
	protected:
		void			resize(int32_t iSize);

		int8_t*			m_pByteArrayRO;
		int8_t*			m_pByteArray;
		uint64_t		m_iCurrentPos;
		uint64_t		m_iArraySize;
		ENDIANNESS		m_eEndianness;
	private:
		void			init(uint64_t iArraySize);
};