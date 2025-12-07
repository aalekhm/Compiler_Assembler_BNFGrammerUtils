#include "RegEx/RegExReader.h"
#include "GrammerUtils.h"

#define SAVE_CURRENT_TOKEN											GrammerUtils::m_pSavedToken = GrammerUtils::m_pToken;
#define SAVE_PREV_TOKEN												GrammerUtils::m_pSavedToken = GrammerUtils::m_pPrevToken;

#define SAVED_TOKEN 												GrammerUtils::m_pSavedToken
#define SAVED_TOKEN_TEXT 											GrammerUtils::m_pSavedToken.getText()
#define PREV_TOKEN 													GrammerUtils::m_pPrevToken
#define PREV_TOKEN_TEXT 											GrammerUtils::m_pPrevToken.getText()

RegExReader::RegExReader()
{
	GrammerUtils::init();
}

void RegExReader::read(const char* sFile)
{
	//////////////// THIS PIECE OF CODE WILL REMAIN COMMON FOR ALL //////////////////
	if (GrammerUtils::read(sFile))
	{
		addKeywords();
		GrammerUtils::getNextToken();

		def();
	}
	/////////////////////////////////////////////////////////////////////////////////
}


bool RegExReader::def() {
if(!PARSE_EXPRESSION())
return false;
return true;

}

bool RegExReader::MORE_EXPRESSION() {
if(!GrammerUtils::match('(', MANDATORY_))
return false;
if(!PARSE_EXPRESSION())
return false;
if(!GrammerUtils::match(')', MANDATORY_))
return false;
return true;

}

bool RegExReader::CHARACTER_CLASS() {
if(!GrammerUtils::match('[', MANDATORY_))
return false;
while(true) {
if(GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {

}
else
break;
}

if(!GrammerUtils::match(']', MANDATORY_))
return false;
return true;

}

bool RegExReader::PARSE_PRIMARY() {
if(GrammerUtils::match('.', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {
return true;
}
else
if(CHARACTER_CLASS()) {
return true;
}
else
if(MORE_EXPRESSION()) {
return true;
}
else
return false;

return true;

}

bool RegExReader::PARSE_QUANTIFIERS() {
if(GrammerUtils::match('*', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match('+', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match('?', OPTIONAL_)) {
return true;
}
else
return false;

return true;

}

bool RegExReader::PARSE_FACTOR() {
if(!PARSE_PRIMARY())
return false;
if(!PARSE_QUANTIFIERS()) {
}
else {
}

return true;

}

bool RegExReader::PARSE_TERM() {
while(true) {
if(PARSE_FACTOR()) {
}
else
break;
}

return true;

}

bool RegExReader::PARSE_EXPRESSION() {
if(!PARSE_TERM())
return false;
while(true) {
if(GrammerUtils::match('|', OPTIONAL_)) {

if(!PARSE_EXPRESSION())
return false;
}
else
break;
}

return true;

}

