#include "BNFToCpp.h"
#include "NonTerminal.h"

#define ARGS_CONTENT_DELIM "##"
#define WRITE_LINE(__rafOut__, __line__) __rafOut__->writeLine(__line__);
#define WRITE_BUF(__rafOut__, __line__) __rafOut__->write(__line__);
#define WRITE_FORMATTED_LINE(__Line__, ...) \
sprintf_s(sLine, __Line__, __VA_ARGS__);	WRITE_LINE(rafOut, sLine); \

#define EXTRACT_ARGS_AND_CONTENT_FROM_TOKEN(__Token__) \
std::vector<std::string> vArgsWithContent	= StringTokenizer::splitString(__Token__.getText(), std::string(ARGS_CONTENT_DELIM)); \
std::string sArgs							= vArgsWithContent[0]; \
std::string sContent						= vArgsWithContent[1]; \

#define EXTRACT_ARGS_FROM_TOKEN(__Token__) \
std::string sArgList = __Token__.getText(); \
StringTokenizer::trimString(sArgList); \

BNFToCpp::BNFToCpp()
{
}

void BNFToCpp::process(const char* sGrammerFile, const char* sClassName)
{
	m_sClassName = sClassName;

	if (tokenize(sGrammerFile))
	{
		nonTerminalize();
		generateDotH();
		generateDotCPP();
	}
}

bool BNFToCpp::tokenize(const char* sGrammerFile)
{
	bool bSuccess = false;
	{
		RandomAccessFile pRafIn;
		bSuccess = pRafIn.openForRead(sGrammerFile);
		if (bSuccess)
		{
			unsigned long iLength = pRafIn.length();
			char* sBuff = new char[iLength + strlen("\r\n") + 1];
			memset(sBuff, 0, iLength);

			unsigned long iBytesRead = pRafIn.read(sBuff);
			if (iBytesRead > 0)
			{
				StringTokenizer* st = StringTokenizer::create(sBuff, false);
				st->tokenize();
				while (st->hasMoreTokens())
				{
					Token tok = st->nextToken();

					//if (!(tok.getType() == TokenType_::Type::TK_EOL || tok.getType() == TokenType_::Type::TK_WHITESPACE))
					//	std::cout << "BNFToCpp::tokenize() << Token :: " << TokenType_::toString(tok.getType()) << "\t" << tok.getText() << std::endl;

					m_vTokens.push_back(tok);
				}
			}

			delete[] sBuff;
			sBuff = nullptr;
		}

		pRafIn.close();
	}

	return bSuccess;
}

void BNFToCpp::nonTerminalize()
{
	NonTerminal* pCurrentNonTerminal = nullptr;
	bool bAddToken = true;
	for (Token tok : m_vTokens)
	{
		bAddToken = true;

		TokenType_::Type eTokenType = tok.getType();
		switch (eTokenType)
		{
		case TokenType_::Type::TK_BNFNONTERMINAL:
		{
			if (pCurrentNonTerminal == nullptr)
			{
				NonTerminal* pNonTerminal = NonTerminal::create(tok);
				pCurrentNonTerminal = pNonTerminal;
				m_vNonterminals.push_back(pNonTerminal);

				continue;
			}
		}
		break;
		case TokenType_::Type::TK_STRING:
		{
			if (strcmp(tok.getText(), "") == 0)
				continue;

			m_vKeywords.push_back(tok);
		}
		break;
		case TokenType_::Type::TK_WHITESPACE:
		case TokenType_::Type::TK_EOL:
		case TokenType_::Type::TK_EOI:
		case TokenType_::Type::TK_SINGLELINECOMMENT:
		case TokenType_::Type::TK_MULTILINECOMMENT:
		case TokenType_::Type::TK_BNFASSIGNMENT:
		{
			continue;
		}
		break;
		}

		if (bAddToken)
		{
			pCurrentNonTerminal->addToken(tok);
		}

		if (eTokenType == TokenType_::Type::TK_SEMICOL)
			pCurrentNonTerminal = nullptr;
	}
}

void BNFToCpp::generateDotH()
{
	std::unique_ptr<RandomAccessFile> rafOut = std::make_unique<RandomAccessFile>();
	std::string sHeaderFileName = m_sClassName + ".h";

	if (rafOut->openForWrite(sHeaderFileName.c_str()))
	{
		char sLine[255] = { 0 };
		{
			sprintf_s(sLine, "%sDotHIncludes.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut.get(), sLine);
		}

		sprintf_s(sLine, "class %s {", m_sClassName.c_str());	WRITE_LINE(rafOut, sLine);
		sprintf_s(sLine, "\tpublic:");						WRITE_LINE(rafOut, sLine);
		sprintf_s(sLine, "\t\t%s();", m_sClassName.c_str());	WRITE_LINE(rafOut, sLine);
		WRITE_LINE(rafOut, "");

		size_t iCount = m_vKeywords.size();
		// Keywords List
		{
			sprintf_s(sLine, "\t\tvoid addKeywords() {");		WRITE_LINE(rafOut, sLine);
			for (int i = 0; i < iCount; i++)
			{
				Token tok = m_vKeywords.at(i);
				sprintf_s(sLine, "\t\t\tGrammerUtils::m_vKeywords.push_back(\"%s\");", tok.getText());
				WRITE_LINE(rafOut, sLine);
			}
			WRITE_LINE(rafOut, "\t\t}\r\n");
		}

		iCount = m_vNonterminals.size();
		// NonTerminals as methods.
		{
			for (int i = 0; i < iCount; i++)
			{
				NonTerminal* pNonTerminal = m_vNonterminals.at(i);
				Token tok = pNonTerminal->get();

				Token firstToken = pNonTerminal->getTokenList()[0];
				std::string sArgList = firstToken.getText();
				StringTokenizer::trimString(sArgList);

				sprintf_s(sLine, "\t\tbool %s(%s);", tok.getText(), sArgList.c_str());
				WRITE_LINE(rafOut, sLine);
			}
			WRITE_LINE(rafOut, "");
		}

		{
			sprintf_s(sLine, "%sDotHDeclares.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut.get(), sLine);
		}

		sprintf_s(sLine, "};");		WRITE_LINE(rafOut, sLine);
	}

	rafOut->close();
}

void BNFToCpp::generateDotCPP()
{
	std::unique_ptr<RandomAccessFile> rafOut = std::make_unique<RandomAccessFile>();
	std::string sCppFileName = m_sClassName + ".cpp";

	if (rafOut->openForWrite(sCppFileName.c_str()))
	{
		char sLine[255] = { 0 };
		{
			sprintf_s(sLine, "%sDotCppDefinitions.txt\0", m_sClassName.c_str());
			writeFromFile(rafOut.get(), sLine);
			WRITE_LINE(rafOut, "");
		}

		size_t iCount = m_vNonterminals.size();
		for (int i = 0; i < iCount; i++)
		{
			NonTerminal* pNonTerminal = m_vNonterminals[i];
			const std::vector<Token>& vTokenList = pNonTerminal->getTokenList();

			m_FirstToken = vTokenList[0];
			onTokenCallback(rafOut.get(), m_sClassName.c_str(), pNonTerminal->get());

			size_t iTokenListSize = vTokenList.size();
			for (int j = 0; j < iTokenListSize; j++)
			{
				Token tok = vTokenList[j];
				onTokenCallback(rafOut.get(), m_sClassName.c_str(), tok);
			}
		}
	}

	rafOut->close();
}

void BNFToCpp::writeFromFile(RandomAccessFile* rafOutDest, const char* sSrcFileName)
{
	RandomAccessFile* rafIn = new RandomAccessFile();
	if (rafIn->openForRead(sSrcFileName))
	{
		char sLine[255] = { 0 };

		while (NOT rafIn->isEOF())
		{
			sprintf_s(sLine, rafIn->readLine().c_str());	WRITE_LINE(rafOutDest, sLine);
		}
	}

	rafIn->close();
}

void BNFToCpp::onTokenCallback(RandomAccessFile* rafOut, const char* sClassName, Token tok)
{
	char sLine[4096] = { 0 };
	TokenType_::Type eTokenType = tok.getType();
	std::string sValue = tok.getText();

	switch (eTokenType)
	{
		case TokenType_::Type::TK_BNFNONTERMINAL:
		{
			if (m_eGrammerState == EGrammerState::WRITING_FUNCTION_START)
			{
				if (m_FirstToken.getType() == TokenType_::Type::TK_FUNCARGLIST)
				{
					EXTRACT_ARGS_FROM_TOKEN(m_FirstToken);

					WRITE_FORMATTED_LINE("bool %s::%s(%s) {", sClassName, sValue.c_str(), sArgList.c_str());
				}
				else
				{
					WRITE_FORMATTED_LINE("bool %s::%s() {", sClassName, sValue.c_str());
				}
			
				m_eGrammerState = EGrammerState::WRITING_FUNCTION;
			}
			else
			if (m_eGrammerState == EGrammerState::WRITING_FUNCTION)
			{
				if (m_bOptional)
				{
					if (m_LastToken.getType() == TokenType_::Type::TK_CALLINGFUNCAARGSWITHCONTENT)
					{
						EXTRACT_ARGS_AND_CONTENT_FROM_TOKEN(m_LastToken);

						WRITE_FORMATTED_LINE("if(!%s(%s)) {",	sValue.c_str(), sArgs.c_str());
						WRITE_FORMATTED_LINE("%s",				sContent.c_str());
					}
					else
					{
						WRITE_FORMATTED_LINE("if(!%s()) {", sValue.c_str());
					}

					WRITE_FORMATTED_LINE("}");
					WRITE_FORMATTED_LINE("else {");

					m_bOptional = false;
				}
				else
				if (m_bWhile)
				{
					if (m_LastToken.getType() == TokenType_::Type::TK_CALLINGFUNCAARGSWITHCONTENT)
					{
						EXTRACT_ARGS_AND_CONTENT_FROM_TOKEN(m_LastToken);

						WRITE_FORMATTED_LINE("if(%s(%s)) {",	sValue.c_str(), sArgs.c_str());
						WRITE_FORMATTED_LINE("%s",				sContent.c_str());
					}
					else
					{
						WRITE_FORMATTED_LINE("if(%s()) {", sValue.c_str());
					}

					m_bWhile = false;
				}
				else
				if (m_bORed || m_bORedFirst)
				{
					if (m_bORedFirst)
					{
						m_bORedFirst = false;
					}
					else
					{
						WRITE_FORMATTED_LINE("return true;");
						WRITE_FORMATTED_LINE("}\r\nelse");
					}

					if (m_LastToken.getType() == TokenType_::Type::TK_CALLINGFUNCAARGSWITHCONTENT)
					{
						EXTRACT_ARGS_AND_CONTENT_FROM_TOKEN(m_LastToken);

						WRITE_FORMATTED_LINE("if(%s(%s)) {",	sValue.c_str(), sArgs.c_str());
						WRITE_FORMATTED_LINE("%s",				sContent.c_str());
					}
					else
					{
						WRITE_FORMATTED_LINE("if(%s()) {", sValue.c_str());
					}
				}
				else
				{
					if (m_LastToken.getType() == TokenType_::Type::TK_CALLINGFUNCAARGSWITHCONTENT)
					{
						EXTRACT_ARGS_AND_CONTENT_FROM_TOKEN(m_LastToken);

						WRITE_FORMATTED_LINE("if(!%s(%s)) {",	sValue.c_str(), sArgs.c_str());
						WRITE_FORMATTED_LINE("%s",				sContent.c_str());
						WRITE_FORMATTED_LINE("return false; }");
					}
					else
					{
						WRITE_FORMATTED_LINE("if(!%s())\r\nreturn false;", sValue.c_str());
					}
				}
			}
		}
		break;
		case TokenType_::Type::TK_BNFCODE:
		{
			sprintf_s(sLine, "%s", sValue.c_str());	WRITE_LINE(rafOut, sLine);
		}
		break;
		case TokenType_::Type::TK_STRING:
		case TokenType_::Type::TK_IDENTIFIER:
		{
			if (m_bOptional)
			{
				if (TokenType_::Type::TK_INVALID == TokenType_::fromString(sValue))
				{
					WRITE_FORMATTED_LINE("if(!GrammerUtils::match(\"%s\", OPTIONAL_)) {\r\n", sValue.c_str());
				}
				else
				{
					WRITE_FORMATTED_LINE("if(!GrammerUtils::match(TokenType_::Type::%s, OPTIONAL_)) {\r\n", sValue.c_str());
				}

				WRITE_FORMATTED_LINE("}\r\n");
				WRITE_FORMATTED_LINE("else {\r\n");

				m_bOptional = false;
			}
			else
			if (m_bWhile)
			{
				if (TokenType_::Type::TK_INVALID == TokenType_::fromString(sValue))
				{
					WRITE_FORMATTED_LINE("if(GrammerUtils::match(\"%s\", OPTIONAL_)) {\r\n", sValue.c_str());
				}
				else
				{
					WRITE_FORMATTED_LINE("if(GrammerUtils::match(TokenType_::Type::%s, OPTIONAL_)) {\r\n", sValue.c_str());
				}

				m_bWhile = false;
			}
			else
			if (m_bORed || m_bORedFirst)
			{
				if (m_bORedFirst)
				{
					m_bORedFirst = false;
				}
				else
				{
					WRITE_FORMATTED_LINE("return true;");
					WRITE_FORMATTED_LINE("}\r\nelse");
				}

				if (TokenType_::Type::TK_INVALID == TokenType_::fromString(sValue))
				{
					WRITE_FORMATTED_LINE("if(GrammerUtils::match(\"%s\", OPTIONAL_)) {", sValue.c_str());
				}
				else
				{
					WRITE_FORMATTED_LINE("if(GrammerUtils::match(TokenType_::Type::%s, OPTIONAL_)) {", sValue.c_str());
				}
			}
			else
			{
				if (TokenType_::Type::TK_INVALID == TokenType_::fromString(sValue))
				{
					WRITE_FORMATTED_LINE("if(!GrammerUtils::match(\"%s\", MANDATORY_))\r\nreturn false;", sValue.c_str());
				}
				else
				{
					WRITE_FORMATTED_LINE("if(!GrammerUtils::match(TokenType_::Type::%s, MANDATORY_))\r\nreturn false;", sValue.c_str());
				}
			}
		}
		break;
		case TokenType_::Type::TK_CHARACTER:
		{
			if (m_bOptional)
			{
				WRITE_FORMATTED_LINE("if(!GrammerUtils::match(\'%c\', OPTIONAL_)) {\r\n", sValue[0]);
				WRITE_FORMATTED_LINE("}\r\n");
				WRITE_FORMATTED_LINE("else {\r\n");

				m_bOptional = false;
			}
			else
			if (m_bWhile)
			{
				WRITE_FORMATTED_LINE("if(GrammerUtils::match(\'%c\', OPTIONAL_)) {\r\n", sValue[0]);
				m_bWhile = false;
			}
			else
			if (m_bORed || m_bORedFirst)
			{
				if (m_bORedFirst)
				{
					m_bORedFirst = false;
				}
				else
				{
					WRITE_FORMATTED_LINE("return true;");
					WRITE_FORMATTED_LINE("}\r\nelse");
				}

				WRITE_FORMATTED_LINE("if(GrammerUtils::match(\'%c\', OPTIONAL_)) {", sValue[0]);
			}
			else
			{
				WRITE_FORMATTED_LINE("if(!GrammerUtils::match(\'%c\', MANDATORY_))\r\nreturn false;", sValue[0]);
			}
		}
		break;
		case TokenType_::Type::TK_LBRACE:
			WRITE_LINE(rafOut, "while(true) {");
			m_bWhile = true;
		break;
		case TokenType_::Type::TK_SEMICOL:
			WRITE_LINE(rafOut, "return true;\r\n");
			WRITE_LINE(rafOut, "}\r\n");

			m_eGrammerState = EGrammerState::WRITING_FUNCTION_START;
		break;
		case TokenType_::Type::TK_LPAREN:
			m_bORedFirst = true;
		break;
		case TokenType_::Type::TK_RBRACE:
			WRITE_LINE(rafOut, "}");
			WRITE_LINE(rafOut, "else\r\nbreak;");
			WRITE_LINE(rafOut, "}\r\n");
			m_bWhile = false;
		break;
		case TokenType_::Type::TK_RPAREN:
			WRITE_LINE(rafOut, "return true;\r\n}");
			WRITE_LINE(rafOut, "else\r\nreturn false;\r\n");
			m_bORed = false;
		break;
		case TokenType_::Type::TK_BITWISEOR:
			m_bORed = true;
		break;
		case TokenType_::Type::TK_LSQBRACKET:
			m_bOptional = true;
		break;
		case TokenType_::Type::TK_RSQBRACKET:
			WRITE_LINE(rafOut, "}\r\n");
			m_bOptional = false;
		break;
	}

	m_LastToken = tok;
}

BNFToCpp::~BNFToCpp()
{
}