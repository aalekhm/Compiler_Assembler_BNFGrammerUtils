
#include "IO/RandomAccessFile.h"
#include "IO/Buffer/BufferData.hpp"
#include "Parser/StringTokenizer.h"

#define REGEX_GRAMMER "TestCases/RegEx.gr"

int main()
{
	{
		std::unique_ptr<RandomAccessFile> pRAF = std::make_unique<RandomAccessFile>();
		if (pRAF != nullptr)
		{
			bool b = pRAF->open(REGEX_GRAMMER, std::ios_base::in);
			if (b)
			{
				int32_t iLength = pRAF->length();
				BufferData buff(iLength);
				int32_t iReadCount = pRAF->readFully(buff.get());
				if (iReadCount > 0)
				{
					StringTokenizer* st = StringTokenizer::create((const char*)buff.get());
					st->tokenize();
					while (st->hasMoreTokens())
					{
						Token tok = st->nextToken();
						if (!(tok.getType() == TokenType_::Type::TK_EOL || tok.getType() == TokenType_::Type::TK_WHITESPACE))
							std::cout << "Token :: " << TokenType_::toString(tok.getType()) << "\t" << tok.getText() << std::endl;
					}
				}
			}

			pRAF->close();
		}
	}

	exit(EXIT_SUCCESS);
}