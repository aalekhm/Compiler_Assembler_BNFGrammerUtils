
#include <iostream>
#include <fstream>
#include "PBXProjReader.h"
#include "PBX/PBXHelper.h"
#include "ByteArrayOutputStream.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: CodeGenerator.exe filename.asm" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string sFilename = argv[1];
	PBXProjReader* pPBXProjReader = new PBXProjReader();
	pPBXProjReader->read(sFilename.c_str());

	system("copyMainDotO.bat");

	PBXHelper* pPBXHelper = PBXHelper::getInstance();
	{	
		pPBXHelper->populateReverseFileList();

		PBXObject* pObj = pPBXHelper->getObjectByGUID("4387D2E5221BFD35ACB32507");

		PBXObject* pPBXObject = nullptr;
		pPBXObject = pPBXHelper->GetPBXObjectTypeByName("PBXFileReference", "UnityEngine.ImageConversionModule_Attr.cpp");

		pPBXObject = pPBXHelper->GetPBXObjectTypeByName("PBXNativeTarget", "Unity-iPhone");

 		pPBXHelper->save("project.pbxproj", TAB_COUNT);
		bool b = true;
	}

	exit(EXIT_SUCCESS);
}