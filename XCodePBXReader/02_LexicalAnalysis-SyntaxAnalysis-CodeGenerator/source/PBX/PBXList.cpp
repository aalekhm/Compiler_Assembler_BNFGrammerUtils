#include "PBX/PBXList.h"

void PBXList::save(RandomAccessFile* pRAFPBXFile, int& TAB_COUNT, bool bSingleLine)
{
	TAB_COUNT++;

	WRITE_TABS(pRAFPBXFile, TAB_COUNT, bSingleLine);
	{
		WRITE_STRING(pRAFPBXFile, std::string(m_sKey + " = (").c_str());
	}
	WRITE_LINE(pRAFPBXFile, bSingleLine);

	if (pRAFPBXFile != nullptr)
	{
		for (const std::unique_ptr<PBXValue>& pPBXValue : m_vList)
		{
			pPBXValue->save(pRAFPBXFile, TAB_COUNT, bSingleLine);
		}
	}

	WRITE_TABS(pRAFPBXFile, TAB_COUNT, bSingleLine);
	{
		WRITE_STRING(pRAFPBXFile, ");");
	}
	WRITE_LINE(pRAFPBXFile, bSingleLine);

	TAB_COUNT--;
}
