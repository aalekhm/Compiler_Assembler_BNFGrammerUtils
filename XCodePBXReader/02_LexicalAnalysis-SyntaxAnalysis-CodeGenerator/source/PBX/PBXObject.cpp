#include "PBX/PBXObject.h"
#include "PBX/PBXHelper.h"

void PBXObject::save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine)
{
	TAB_COUNT++;

	bool bShouldTab = (m_sPBXType == "PBXBuildFile" || m_sPBXType == "PBXFileReference") ? false : bSingleLine;
	WRITE_TABS(pRAFPBXFile, TAB_COUNT, bShouldTab);
	{
		std::string sKey = "";
		
		if (m_sKey.empty())
		{
			sKey = "{";
		}
		else
		{
			if(m_sKeyType != EPBXVALUE_TYPE::INVALID)
			{
				switch (m_sKeyType)
				{
					case EPBXVALUE_TYPE::NUMBER:		sKey = m_sKey + " = ";
					break;
					case EPBXVALUE_TYPE::STRING:		sKey = "\"" + m_sKey + "\"";
					break;
					case EPBXVALUE_TYPE::IDENTIFIER:	sKey = m_sKey;
					break;
				}

				sKey = sKey + PBXHelper::getInstance()->getComment(m_sKey) + " = {";
			}
		}

		WRITE_STRING(pRAFPBXFile, sKey.c_str());
	}
	WRITE_LINE(pRAFPBXFile, bSingleLine);

	if (pRAFPBXFile != nullptr)
	{
		// Pairs
		for (const std::pair<const std::string, std::unique_ptr<PBXValue>>& pPairKeyValue : m_PairMap)
		{
			pPairKeyValue.second.get()->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
		}

		// Childs
		for (const std::pair<const std::string, std::unique_ptr<PBXObject>>& pChildKeyValue : m_ChildMap)
		{
			pChildKeyValue.second.get()->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
		}

		// List
		for (const std::pair<const std::string, std::unique_ptr<PBXList>>& pListKeyValue : m_List)
		{
			pListKeyValue.second.get()->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
		}
	}

	WRITE_TABS(pRAFPBXFile, TAB_COUNT, bSingleLine);
	{
		WRITE_STRING(pRAFPBXFile, "};");
	}
	WRITE_LINE(pRAFPBXFile, bShouldTab);

	TAB_COUNT--;
}

PBXObject* PBXObject::GetObjectByGUID(std::string sGUID)
{
	PBXObject* pObject = (PBXObject*)m_ChildMap["objects"].get();
	auto pReturnPair = std::find_if(	pObject->m_ChildMap.begin(),
										pObject->m_ChildMap.end(),
										[sGUID](const std::pair<const std::string, std::unique_ptr<PBXObject>>& pObj) -> bool
										{
											return (pObj.second->m_PairMap["name"].get()->m_sValue == sGUID);
										}
	);

	return nullptr;
}

std::string PBXObject::getValueForKey(std::string sKey)
{
	std::string sReturnValue = "";
	if (NOT sKey.empty())
	{
		if (m_PairMap.find(sKey) != m_PairMap.end())
		{
			sReturnValue = m_PairMap[sKey].get()->m_sValue;
		}
	}

	return sReturnValue;
}
