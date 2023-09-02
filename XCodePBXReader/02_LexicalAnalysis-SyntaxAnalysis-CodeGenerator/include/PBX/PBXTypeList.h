#pragma once

#include "PBX/PBXDefines.h"
#include "PBX/PBXValue.h"
#include "PBX/PBXList.h"
#include "PBX/PBXObject.h"

class PBXTypeList
{
	public:
		PBXTypeList() = default;
		~PBXTypeList() = default;
		PBXTypeList(std::string sISAType) :m_sISAType(sISAType) {}

		void								save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine = false);
		PBXObject*							GetObjectByKey(std::string sKey);

		std::string							m_sISAType;
		std::map<std::string, PBXObject*>	m_pObjectList;
	protected:
	private:
};