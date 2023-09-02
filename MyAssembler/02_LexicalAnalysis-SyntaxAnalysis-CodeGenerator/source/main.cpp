
#include <iostream>
#include <fstream>
#include "TinyAssemberReader.h"
#include "ByteArrayOutputStream.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: CodeGenerator.exe filename.asm" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string sFilename = argv[1];
	TinyAssemberReader* pTinyAssemberReader = new TinyAssemberReader();
	pTinyAssemberReader->read(sFilename.c_str());

	system("copyMainDotO.bat");

	exit(EXIT_SUCCESS);
}