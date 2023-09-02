#pragma once

#include "PBX/PBXObject.h"

class PBXParseHelper
{
	public:
		PBXObject*												m_pCurrentPBXObject = nullptr;
		std::vector<std::unique_ptr<PBXObject>>					m_pCurrentPBXObjectStack;
};
