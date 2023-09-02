#pragma once

#include "PBX/PBXDefines.h"
#include "PBX/PBXValue.h"
#include "PBX/PBXList.h"

class PBXObject
{
	public:
		PBXObject() = default;
		~PBXObject() = default;
		
		std::string											m_sCurrentlyProcessingKey;
		EPBXVALUE_TYPE										m_sCurrentlyProcessingKeyType = EPBXVALUE_TYPE::INVALID;

		EPBXTYPE_PARSE_STATE								m_iParseState = EPBXTYPE_PARSE_STATE::START;

		std::string											m_sKey;
		EPBXVALUE_TYPE										m_sKeyType = EPBXVALUE_TYPE::INVALID;
		std::string											m_sPBXType;
		
		std::map<std::string, std::unique_ptr<PBXValue>>	m_PairMap;
		std::map<std::string, std::unique_ptr<PBXList>>		m_List;
		std::map<std::string, std::unique_ptr<PBXObject>>	m_ChildMap;

		void												save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine = false);

		PBXObject*											GetObjectByGUID(std::string sGUID);

		std::string											getValueForKey(std::string sKey);
	protected:
	private:
};