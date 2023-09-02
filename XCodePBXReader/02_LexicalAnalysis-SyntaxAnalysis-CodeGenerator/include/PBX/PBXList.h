#pragma once

#include "PBX/PBXDefines.h"
#include "PBX/PBXValue.h"

class PBXList
{
	public:
		PBXList() = default;
		~PBXList() = default;

		void												save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine = false);

		std::string											m_sKey;
		std::vector<std::unique_ptr<PBXValue>>				m_vList;
	protected:
	private:
};