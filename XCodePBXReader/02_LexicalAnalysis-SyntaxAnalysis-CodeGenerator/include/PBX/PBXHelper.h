#pragma once

#include "PBX/PBXDefines.h"
#include "PBX/PBXValue.h"
#include "PBX/PBXList.h"
#include "PBX/PBXObject.h"
#include "PBX/PBXTypeList.h"

class PBXHelper
{
	public:
		PBXHelper() = default;
		~PBXHelper() = default;

		static std::unique_ptr<PBXHelper>						m_pInstance;
		static PBXHelper*										getInstance();

		bool													isGuid(std::string sKey);
		std::string												getPBXType(std::string sGuid);
		PBXObject*												getObjectByGUID(std::string sGUID);

		std::string												getComment(std::string sGuid);
		std::string												getBuildFileReferenceInBuildPhase(std::string sGuid);

		std::string												getValueForKeyFromTypeForGuid(std::string sPBXType, std::string sGuid, std::string sKey);
		void													save(std::string sPBXProjectFileName, int& TAB_COUNT);
		void													populateReverseFileList();
		PBXTypeList*											GetTypeListByName(std::string sTypeName);
		PBXObject*												GetPBXObjectTypeByName(std::string sTypeName, std::string sName);

		std::unique_ptr<PBXObject>								m_pRootPBXObject = nullptr;
		std::map<std::string, std::unique_ptr<PBXTypeList>>		m_pTypeListMap;
		std::map<std::string, std::vector<std::string>>			m_pReverseFileList;
	protected:
	private:
};