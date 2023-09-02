#pragma once

#include <windows.h>
#include <string>
#include <map>
#include "RandomAccessFile.h"
#include "StringTokenizer.h"
#include "Token.h"
#include <stack>
#include <memory>

#define OPTIONAL_			0
#define MANDATORY_			1

#define MAX_BYTECODE_SIZE	14 * 1024

class ByteArrayOutputStream;
class ByteArrayInputStream;

class GrammerUtils
{
	public:
		static Token								m_pToken;
		static Token								getNextToken();
		static Token								m_pPrevToken;
		static Token								m_pSavedToken;

		static bool									match(TokenType_::Type eTokenType, int TYPE);
		static bool									match(std::string sText, int TYPE);
		static bool									match(char ch, int TYPE);

		static void									init();
		static bool									read(const char* sFile);

		static bool									isOneOfTheKeywords(std::string sKeyword);

		static StringTokenizer*						m_pStrTok;
		static std::vector<std::string>				m_vKeywords;

		static int									iTabCount;
		static void									printTabs();
	private:
		static void*								m_HColor;
	public:
};