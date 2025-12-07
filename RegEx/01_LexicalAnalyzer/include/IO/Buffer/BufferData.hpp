#pragma once
#include <array>
#include <memory>
#include <cstdint>

class BufferData
{
	public:
		BufferData()
		: m_pData(nullptr)
		, m_iSize(0)
		{
		}

		BufferData(size_t iSize)
		{
			m_pData = (int8_t*)std::calloc( iSize, sizeof(int8_t) );
			m_iSize = iSize;
		}

		BufferData(const BufferData& pCopy)
		{
			m_iSize = pCopy.m_iSize;
			m_pData = pCopy.m_pData;
		}

		BufferData(BufferData&& pCopy)
		{
			m_iSize = pCopy.m_iSize;
			m_pData = pCopy.m_pData;

			pCopy.m_iSize = 0;
			pCopy.m_pData = nullptr;
		}

		void operator=(const BufferData& pCopy) = delete;

		~BufferData()
		{
			if (m_pData != nullptr)
			{
				std::free(m_pData);
			}
		}

		void set(int8_t* pBuffer, size_t iSizeInBytes)
		{
			if (pBuffer != nullptr && iSizeInBytes > 0)
			{
				resize(iSizeInBytes);
				memcpy_s(m_pData, iSizeInBytes, pBuffer, iSizeInBytes);
				m_iSize = iSizeInBytes;
			}
		}

		bool resize(size_t iNewSize)
		{
			int8_t* pData = (int8_t*)std::calloc(iNewSize, sizeof(int8_t));
			if (pData != nullptr)
			{
				if (m_pData != nullptr)
				{
					if (iNewSize < m_iSize)
					{
						memcpy_s(pData, iNewSize * sizeof(int8_t), m_pData, iNewSize * sizeof(int8_t));
					}
					else
					{
						memcpy_s(pData, m_iSize * sizeof(int8_t), m_pData, m_iSize * sizeof(int8_t));
					}

					std::free(m_pData);
				}

				m_pData = pData;
				m_iSize = iNewSize;

				return true;
			}

			return false;
		}
		
		int8_t* get()
		{
			return m_pData;
		}

		size_t size()
		{
			return m_iSize;
		}
	protected:
	private:
		size_t		m_iSize;
		int8_t*		m_pData;
};