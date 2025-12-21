
#include <iostream>
#include <fstream>
#include "RegEx/RegExReader.h"

// Examples
// "a[bc]+d" "This is abcd and abd and add"
// "(cat|dog)s" "I have cats and a dog"
// "a(b|c)*d" "ad abd acbd abbccd"
// "hello" "hello world"
// "a.*b+c?" "axbybbc"
// "[a-zA-Z][0-9]+" "R2D2"
// "[a-zA-Z][0-9]+" "C3PO"
// "[a-zA-Z][0-9]+" "123"
// "reg(ex|ular)+(abc)?" "regexular"
// "reg(ex|ular)+(abc)?" "regularabc"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: \"Regex Pattern\" \"String to match\"" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string sRegExPattern = argv[1];
	std::string sStringToMatch = argv[2];

	Regex re(sRegExPattern);
	{
		bool bMatched = re.match(sStringToMatch);
		{
			std::cout << std::endl;
			std::cout << "***************" << std::endl;
			std::cout << "Regex Pattern: " << sRegExPattern << std::endl;
			std::cout << "String to match : " << sStringToMatch << std::endl;
			std::cout << "Match result: " << (bMatched ? "true" : "false") << std::endl;
		}

		auto vMatches = re.findAll(sStringToMatch);
		{
			std::cout << "Matches Found : ";
			for (const auto& sMatch : vMatches)
			{
				std::cout << sMatch << ", ";
			}

			std::cout << std::endl;
		}
	}

	exit(EXIT_SUCCESS);
}