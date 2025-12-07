
#include <iostream>
#include <fstream>
#include "RegEx/RegExReader.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: SyntaxAnalyzer.exe filename.c" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string sFilename = argv[1];
	RegExReader* pRegExReader = new RegExReader();
	pRegExReader->read(sFilename.c_str());

	system("copyMainDotO.bat");

	exit(EXIT_SUCCESS);
}