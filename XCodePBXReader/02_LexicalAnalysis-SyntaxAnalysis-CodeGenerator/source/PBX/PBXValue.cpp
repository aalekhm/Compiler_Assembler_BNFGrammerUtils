#pragma once

#include "PBX/PBXValue.h"
#include "PBX/PBXHelper.h"

void PBXValue::save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine)
{
	TAB_COUNT++;

	WRITE_TABS(pRAFPBXFile, TAB_COUNT, bSingleLine);
			
	if (pRAFPBXFile != nullptr)
	{
		if(m_ePbxKeyType != EPBXVALUE_TYPE::INVALID)
		{
			switch (m_ePbxKeyType)
			{
				case EPBXVALUE_TYPE::NUMBER:
				{
					WRITE_STRING(pRAFPBXFile, std::string(m_sKey + " = ").c_str());
				}
				break;
				case EPBXVALUE_TYPE::STRING:
				{
					WRITE_STRING(pRAFPBXFile, std::string("\"" + m_sKey + "\" = ").c_str());
				}
				break;
				case EPBXVALUE_TYPE::IDENTIFIER:
				{
					WRITE_STRING(pRAFPBXFile, std::string(m_sKey + " = ").c_str());
				}
				break;
			}
		}

		if(m_ePbxValueType != EPBXVALUE_TYPE::INVALID)
		{
			switch (m_ePbxValueType)
			{
				case EPBXVALUE_TYPE::NUMBER:
				{
					WRITE_STRING(pRAFPBXFile, std::string(m_sValue + PBXHelper::getInstance()->getComment(m_sValue)).c_str());
				}
				break;
				case EPBXVALUE_TYPE::STRING:
				{
					WRITE_STRING(pRAFPBXFile, std::string("\"" + m_sValue + "\"").c_str());
				}
				break;
				case EPBXVALUE_TYPE::IDENTIFIER:
				{
					WRITE_STRING(pRAFPBXFile, std::string(m_sValue + PBXHelper::getInstance()->getComment(m_sValue)).c_str());
				}
				break;
			}
		}
	}

	WRITE_STRING(pRAFPBXFile, std::string(m_bIsList ? "," : ";").c_str());
	WRITE_LINE(pRAFPBXFile, bSingleLine);

	TAB_COUNT--;
}