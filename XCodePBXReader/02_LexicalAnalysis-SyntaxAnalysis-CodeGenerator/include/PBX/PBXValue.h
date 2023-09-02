#pragma once

#include "PBX/PBXDefines.h"
#include "RandomAccessFile.h"

class PBXValue
{
	public:
		PBXValue() = default;
		~PBXValue() = default;

		void				save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine = false);

		std::string			m_sKey;
		EPBXVALUE_TYPE		m_ePbxKeyType = EPBXVALUE_TYPE::INVALID;
		std::string			m_sValue;
		EPBXVALUE_TYPE		m_ePbxValueType = EPBXVALUE_TYPE::INVALID;

		bool				m_bIsList = false;
	protected:
	private:
};