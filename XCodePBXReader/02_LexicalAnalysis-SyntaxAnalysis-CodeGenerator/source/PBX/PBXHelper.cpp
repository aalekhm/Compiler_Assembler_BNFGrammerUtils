#include "PBX/PBXHelper.h"

std::unique_ptr<PBXHelper> PBXHelper::m_pInstance = nullptr;

PBXHelper* PBXHelper::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = std::make_unique<PBXHelper>();
	}

	return m_pInstance.get();
}

bool PBXHelper::isGuid(std::string sKey)
{
	return std::regex_match(sKey, std::regex("[A-F0-9]{24}"));
}

std::string PBXHelper::getPBXType(std::string sGuid)
{
	PBXObject* pObject = getObjectByGUID(sGuid);
	if (pObject != nullptr)
	{
		return pObject->m_PairMap["isa"].get()->m_sValue;
	}
}

std::string PBXHelper::getBuildFileReferenceInBuildPhase(std::string sGuid)
{
	std::string sReturn = "";
	for (std::string sBuildPhaseType : BuildPhaseTypes)
	{
		PBXTypeList* pPBXTypeList = m_pTypeListMap[sBuildPhaseType].get();
		if (pPBXTypeList != nullptr)
		{
			for (const std::pair<std::string, PBXObject*>& pObjPair : pPBXTypeList->m_pObjectList)
			{
				PBXObject* pPBXObject = pObjPair.second;

				for (const std::pair<const std::string, std::unique_ptr<PBXList>>& pPBXListPair : pPBXObject->m_List)
				{
					std::vector<std::unique_ptr<PBXValue>>& pPBXValuePair = pPBXListPair.second->m_vList;
					
					auto pReturnMatch = std::find_if(	pPBXValuePair.begin(),
														pPBXValuePair.end(),
														[sGuid](std::unique_ptr<PBXValue>& pValue)
														{
															return (sGuid == pValue->m_sValue);
														});

					if (pReturnMatch != pPBXValuePair.end())
					{
						if (sBuildPhaseType == "PBXFrameworksBuildPhase")		sReturn = "Frameworks";
						else if (sBuildPhaseType == "PBXResourcesBuildPhase")	sReturn = "Resources";
						else if (sBuildPhaseType == "PBXSourcesBuildPhase")		sReturn = "Sources";
						else if (sBuildPhaseType == "PBXShellScriptBuildPhase") sReturn = "ShellScript";
						else if (sBuildPhaseType == "PBXCopyFilesBuildPhase")	sReturn = pPBXObject->m_PairMap["name"]->m_sValue;

						break;
					}
				}
			}
		}
	}

	return sReturn;
}

std::string PBXHelper::getComment(std::string sGuid)
{
	std::string sReturnComment = "";
	if (isGuid(sGuid))
	{
		PBXObject* pPBXObject = getObjectByGUID(sGuid);
		if (pPBXObject != nullptr)
		{
			std::string sPBXType = getPBXType(sGuid);

			if (sPBXType == "PBXProject")
			{
				sReturnComment = "Project object";
			}
			else if (sPBXType == "PBXBuildFile")
			{
				std::string sFileRefGuid = pPBXObject->getValueForKey("fileRef");

				std::string sBuildPhaseComment = getBuildFileReferenceInBuildPhase(sGuid);
				std::string sName = getValueForKeyFromTypeForGuid("PBXFileReference", sFileRefGuid, "name");
				if (sName.empty())
				{
					sName = getValueForKeyFromTypeForGuid("PBXFileReference", sFileRefGuid, "path");
				}

				if (NOT sBuildPhaseComment.empty())
				{
					sReturnComment = sName + " in " + sBuildPhaseComment;
				}
			}
			else if (sPBXType == "PBXFrameworksBuildPhase")		sReturnComment = "Frameworks";
			else if (sPBXType == "PBXResourcesBuildPhase")		sReturnComment = "Resources";
			else if (sPBXType == "PBXSourcesBuildPhase")		sReturnComment = "Sources";
			else if (sPBXType == "PBXShellScriptBuildPhase")	sReturnComment = "ShellScript";
			else if (sPBXType == "PBXCopyFilesBuildPhase")
			{
				sReturnComment = pPBXObject->getValueForKey("name");
				if (sReturnComment.empty())
				{
					sReturnComment = "CopyFiles";
				}
			}
			else if (sPBXType == "XCConfigurationList")
			{
				std::string sIsAType = "";
				std::string sPBXNativeTarget = "";

				PBXTypeList* pPBXTypeList = GetTypeListByName("PBXNativeTarget");
				if (pPBXTypeList != nullptr)
				{
					for (const std::pair<const std::string, PBXObject*>& pObjectPair : pPBXTypeList->m_pObjectList)
					{
						PBXObject* pPBXObject = pObjectPair.second;
						if (pPBXObject != nullptr)
						{
							PBXValue* pPBXValue = pPBXObject->m_PairMap["buildConfigurationList"].get();
							if (pPBXValue != nullptr)
							{
								if (sGuid == pPBXValue->m_sValue)
								{
									std::string sPBXNativeTarget = pPBXObject->m_PairMap["name"].get()->m_sValue;

									sReturnComment = "Build configuration list for PBXNativeTarget \"" + sPBXNativeTarget + "\"";
									break;
								}
							}
						}
					}
				}

				if (sReturnComment.empty())
				{
					sReturnComment = "Build configuration list for PBXProject \"Unity - iPhone\"";
				}
			}
			else
			{
				sReturnComment = pPBXObject->getValueForKey("name");
				if (sReturnComment.empty())
				{
					sReturnComment = pPBXObject->getValueForKey("path");
				}
			}

			if (NOT sReturnComment.empty())
			{
				sReturnComment = " /* " + sReturnComment + " */";
			}
		}
	}

	return sReturnComment;
}

std::string PBXHelper::getValueForKeyFromTypeForGuid(std::string sPBXType, std::string sGuid, std::string sKey)
{
	if (isGuid(sGuid))
	{
		PBXTypeList* pPBXFileReference = m_pTypeListMap[sPBXType].get();
		if (pPBXFileReference != nullptr)
		{
			PBXObject* pPBXObject = pPBXFileReference->GetObjectByKey(sGuid);
			if (pPBXObject != nullptr)
			{
				return pPBXObject->getValueForKey(sKey);
			}
		}
	}

	return "";
}

void PBXHelper::save(std::string sPBXProjectFileName, int& TAB_COUNT)
{
	if (NOT sPBXProjectFileName.empty())
	{
		RandomAccessFile* pRAFPBXFile = new RandomAccessFile();
		if (pRAFPBXFile->openForWrite(sPBXProjectFileName.c_str()))
		{
			WRITE_STRING(pRAFPBXFile, "// !$*UTF8*$!");
			WRITE_LINE(pRAFPBXFile, false);

			WRITE_STRING(pRAFPBXFile, "{");
			WRITE_LINE(pRAFPBXFile, false);
			{
				TAB_COUNT++;

				for (const std::pair<const std::string, std::unique_ptr<PBXValue>>& pObjPair : m_pRootPBXObject->m_PairMap)
				{
					pObjPair.second->save(pRAFPBXFile, TAB_COUNT);
				}

				m_pRootPBXObject->m_ChildMap["classes"]->save(pRAFPBXFile, TAB_COUNT);

				TAB_COUNT++;
				WRITE_TABS(pRAFPBXFile, TAB_COUNT, false);
				WRITE_STRING(pRAFPBXFile, "objects = {");
				WRITE_LINE(pRAFPBXFile, false);
				{
					bool bSingleLine = false;
					for (const std::pair<const std::string, std::unique_ptr<PBXTypeList>>& pTypeObjectPair : m_pTypeListMap)
					{
						bSingleLine = (pTypeObjectPair.first == "PBXBuildFile" || pTypeObjectPair.first == "PBXFileReference");
						pTypeObjectPair.second.get()->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
					}
				}
				WRITE_TABS(pRAFPBXFile, TAB_COUNT, false);
				WRITE_STRING(pRAFPBXFile, "};");
				WRITE_LINE(pRAFPBXFile, false);

				TAB_COUNT--;
			}
			WRITE_STRING(pRAFPBXFile, "}");

			pRAFPBXFile->close();
		}
	}
}

void PBXHelper::populateReverseFileList()
{
	for (std::string sBuildPhase : BuildPhaseTypes)
	{
		PBXTypeList* pPBXTypeList = GetTypeListByName(sBuildPhase);
		if (pPBXTypeList != nullptr)
		{
			for (std::pair<const std::string, PBXObject*>& pObjectPair : pPBXTypeList->m_pObjectList)
			{
				std::string sKey = pObjectPair.first;
				PBXObject* pObject = pObjectPair.second;
				if (pObject != nullptr)
				{
					auto fileList = pObject->m_List["files"].get();
					for (const std::unique_ptr<PBXValue>& pPBXValue : fileList->m_vList)
					{
						m_pReverseFileList[pPBXValue->m_sValue].push_back(sKey);
					}
				}
			}
		}
	}
}

PBXObject* PBXHelper::getObjectByGUID(std::string sGUID)
{
	PBXObject* pObject = (PBXObject*)m_pRootPBXObject->m_ChildMap["objects"].get();
	auto pReturnPair = std::find_if(pObject->m_ChildMap.begin(),
									pObject->m_ChildMap.end(),
									[sGUID](const std::pair<const std::string, std::unique_ptr<PBXObject>>& pObj) -> bool
									{
										return (pObj.first == sGUID);
									}
	);

	if (pReturnPair != pObject->m_ChildMap.end())
	{
		return (PBXObject*)pReturnPair->second.get();
	}

	return nullptr;
}

PBXTypeList* PBXHelper::GetTypeListByName(std::string sTypeName)
{
	if (m_pTypeListMap.find(sTypeName) != m_pTypeListMap.end())
	{
		return m_pTypeListMap[sTypeName].get();
	}

	return nullptr;
}

PBXObject* PBXHelper::GetPBXObjectTypeByName(std::string sTypeName, std::string sName)
{
	PBXObject* pReturnPBXObject = nullptr;
	PBXTypeList* pPBXTypeList = GetTypeListByName(sTypeName);
	if (pPBXTypeList != nullptr)
	{
		auto pReturnPair = std::find_if(pPBXTypeList->m_pObjectList.begin(),
										pPBXTypeList->m_pObjectList.end(),
										[sName](const std::pair<std::string, PBXObject*>& pObj) -> bool
										{
											return (pObj.second->m_PairMap["name"].get()->m_sValue == sName);
										}

		);

		return pReturnPair->second;
	}

	return nullptr;
}