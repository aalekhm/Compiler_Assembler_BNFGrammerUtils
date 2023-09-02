#include "PBX/PBXTypeList.h"

void PBXTypeList::save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine)
{
	if (pRAFPBXFile != nullptr)
	{
		WRITE_STRING(pRAFPBXFile, std::string("/* Begin " + m_sISAType + " section */").c_str());
		WRITE_LINE(pRAFPBXFile, false);

		for (const std::pair<const std::string, PBXObject*>& pObjPair : m_pObjectList)
		{
			pObjPair.second->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
		}

		WRITE_STRING(pRAFPBXFile, std::string("/* End " + m_sISAType + " section */").c_str());
		WRITE_LINE(pRAFPBXFile, false);
		WRITE_LINE(pRAFPBXFile, false);
	}
}

PBXObject* PBXTypeList::GetObjectByKey(std::string sKey)
{
	auto pReturnPair = std::find_if(m_pObjectList.begin(),
									m_pObjectList.end(),
									[sKey](const std::pair<const std::string, PBXObject*>& pObj) -> bool
									{
										return (pObj.first == sKey);
									}
	);

	if (pReturnPair != m_pObjectList.end())
	{
		return pReturnPair->second;
	}

	return nullptr;
}