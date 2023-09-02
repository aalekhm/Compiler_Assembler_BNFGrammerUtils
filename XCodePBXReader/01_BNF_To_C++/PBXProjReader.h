#pragma once

#include "PBX/PBXHelper.h"
#include "PBX/PBXParseHelper.h"

class PBXProjReader {
	public:
		PBXProjReader();

		void addKeywords() {
		}

		bool def();
		bool NAME_VALUE_PAIRS();
		bool NAME_AND_VALUE();
		bool NAME_TYPES();
		bool SECTION();
		bool VALUE_TYPES();
		bool PBX_ARRAY_ELEMENTS();
		bool PBX_ARRAY();

		
		void 													read(const char* sFile);
		
		PBXParseHelper*											GetPBXParserHelper()
		{
			return m_pPBXParseHelper.get();
		}
	protected:							
									
	private:							
		std::unique_ptr<PBXParseHelper>							m_pPBXParseHelper = std::make_unique<PBXParseHelper>();

		PBXHelper*												m_pPBXHelper = PBXHelper::getInstance();

};
