#include "GrammerUtils.h"
#include "ByteArrayOutputStream.h"
#include "ByteArrayInputStream.h"
#include <assert.h>
#include "PBXProjReader.h"
#include <windows.h>

Token									GrammerUtils::m_pToken(TokenType_::Type::TK_UNKNOWN, "", -1, -1);
Token									GrammerUtils::m_pPrevToken(TokenType_::Type::TK_UNKNOWN, "", -1, -1);
Token									GrammerUtils::m_pSavedToken(TokenType_::Type::TK_UNKNOWN, "", -1, -1);

std::vector<std::string>				GrammerUtils::m_vKeywords;

StringTokenizer*						GrammerUtils::m_pStrTok = NULL;
int										GrammerUtils::iTabCount = 0;
HANDLE									GrammerUtils::m_HColor;

#define VERBOSE		1
#define COLORIZE	0

#if (COLORIZE == 1)
struct color
{
	color(WORD wAttribute)
		: m_colorAttribute(wAttribute)
	{};

	WORD m_colorAttribute;
};

color red(FOREGROUND_RED | FOREGROUND_INTENSITY);
color green(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
color blue(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
color white(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
color yellow(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
color magenta(FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

void colorize(const char* cStr)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (GrammerUtils::isABuiltInType(cStr))
	{
		SetConsoleTextAttribute(	hStdout,
									red.m_colorAttribute);
	}
	else
	if (GrammerUtils::isAKeyword(cStr))
	{
		SetConsoleTextAttribute(	hStdout,
									blue.m_colorAttribute);
	}
	else
	if (GrammerUtils::isAUserDefinedType(cStr) || GrammerUtils::isAUserDefinedInterface(cStr))
	{
		SetConsoleTextAttribute(	hStdout,
									yellow.m_colorAttribute);
	}
	else
	if (GrammerUtils::isAUserDefinedFunction(cStr))
	{
		SetConsoleTextAttribute(	hStdout,
									green.m_colorAttribute);
	}
	else
	if (GrammerUtils::isAUserDefinedVariable(cStr))
	{
		SetConsoleTextAttribute(	hStdout,
									magenta.m_colorAttribute);
	}
	else
	{
		SetConsoleTextAttribute(	hStdout,
									white.m_colorAttribute);
	}
}

std::ostream& operator<<(std::ostream& os, std::string sStr)
{
	colorize(sStr.c_str());
	printf("%s", sStr.c_str());

	return os;
}

std::ostream& operator<<(std::ostream& os, const char* cStr)
{
	colorize(cStr);
	printf("%s", cStr);

	return os;
}

std::ostream& operator<<(std::ostream& os, color c)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout,
		c.m_colorAttribute);

	return os;
}
#endif

void GrammerUtils::init()
{
	m_HColor = GetStdHandle(STD_OUTPUT_HANDLE); //just once
}

bool GrammerUtils::read(const char* sFile)
{
	bool bSuccess = false;
	RandomAccessFile* pRaf = new RandomAccessFile();
	bSuccess = pRaf->openForRead(sFile);
	if (bSuccess)
	{
		unsigned long iLength = pRaf->length();
		char* sBuff = new char[iLength + strlen("\r\n") + 1];
		memset(sBuff, 0, iLength);

		unsigned long iBytesRead = pRaf->read(sBuff);
		if (iBytesRead > 0)
		{
			m_pStrTok = StringTokenizer::create(sBuff);
			m_pStrTok->tokenize();
		}

		delete[] sBuff;
		sBuff = nullptr;
	}
	pRaf->close();

	return bSuccess;
}

Token GrammerUtils::getNextToken()
{
	m_pToken = m_pStrTok->nextToken();

	if (m_pToken.getType() == TokenType_::Type::TK_EOL
		||
		m_pToken.getType() == TokenType_::Type::TK_SINGLELINECOMMENT
		||
		m_pToken.getType() == TokenType_::Type::TK_MULTILINECOMMENT
		||
		m_pToken.getType() == TokenType_::Type::TK_WHITESPACE
	) {
		getNextToken();
	}

	return m_pToken;
}

bool GrammerUtils::match(TokenType_::Type eTokenType, int TYPE)
{
	TokenType_::Type eCurrentTokenType = m_pToken.getType();

	bool bIsAMatch = (eCurrentTokenType == eTokenType && !isOneOfTheKeywords(m_pToken.getText()));
	if (bIsAMatch)
	{
		m_pPrevToken = m_pToken;
		getNextToken();
	}

	return bIsAMatch;
}

bool GrammerUtils::match(std::string sText, int TYPE)
{
	std::string sCurrentTokenText = m_pToken.getText();

	bool bIsAMatch = (sCurrentTokenText == sText);
	if (bIsAMatch)
	{
		m_pPrevToken = m_pToken;
		getNextToken();
	}

	return bIsAMatch;
}

bool GrammerUtils::match(char ch, int TYPE)
{
	return match(std::string(1, ch), TYPE);
}

bool GrammerUtils::isOneOfTheKeywords(std::string sKeyword)
{
	bool bYes = false;
	for (std::string sInBuiltKeyword : m_vKeywords)
	{
		if (sInBuiltKeyword == sKeyword)
		{
			bYes = true;
			break;
		}
	}

	return bYes;
}

void GrammerUtils::printTabs()
{
	if (iTabCount > 0)
	{
		for (int i = 0; i < iTabCount; i++)
			std::cout << "\t";
	}
}
