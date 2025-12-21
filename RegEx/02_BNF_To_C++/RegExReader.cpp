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

		def(m_pRootASTNode);
	}
	/////////////////////////////////////////////////////////////////////////////////
}

std::shared_ptr<ASTNode> RegExReader::parse(const std::string& sPattern)
{
	if (NOT sPattern.empty())
	{
		GrammerUtils::parse(sPattern);

		addKeywords();
		GrammerUtils::getNextToken();
		def(m_pRootASTNode);

		return m_pRootASTNode;
	}

	return nullptr;
}

Regex::Regex(const std::string& sPattern)
{
	RegExReader parser;
	m_pRootAST = parser.parse(sPattern);
}

bool Regex::match(const std::string& sText)
{
	size_t iPos = 0;

	for (size_t iStart = 0; iStart <= sText.length(); iStart++)
	{
		iPos = iStart;
		if (m_pRootAST->match(sText, iPos))
		{
			return true;
		}
	}

	return false;
}

std::vector<std::string> Regex::findAll(const std::string& sText)
{
	std::vector<std::string> vMatches;

	for (size_t iStart = 0; iStart < sText.length(); iStart++)
	{
		size_t iPos = iStart;
		size_t iOriginalPos = iPos;

		if (m_pRootAST->match(sText, iPos))
		{
			vMatches.push_back(sText.substr(iOriginalPos, iPos - iOriginalPos));

			// Skip to the end of this match for the next iteration
			// (but don't skip if we didn't actually consume anything, to avoid infinite loop)
			if (iPos > iOriginalPos)
			{
				iStart = iPos - 1;
			}
		}
	}

	return vMatches;
}


bool RegExReader::def(std::shared_ptr<ASTNode>& pReturnNode) {
	if (!PARSE_EXPRESSION(pReturnNode)) {

		return false;
	}
	return true;

}

bool RegExReader::PARSE_EXPRESSION(std::shared_ptr<ASTNode>& pReturnNode) {

	std::shared_ptr<ASTNode> pLeftNode;
	std::shared_ptr<ASTNode> pRightNode;

	if (!PARSE_TERM(pLeftNode)) {

		return false;
	}
	if (!ALTERNATE_EXPRESSION(pRightNode)) {
		pReturnNode = pLeftNode;
	}
	else {

		std::shared_ptr<ASTNode> pAlternateASTNode = std::make_shared<AlternationASTNode>(pLeftNode, pRightNode);
		pReturnNode = pAlternateASTNode;

	}

	return true;

}

bool RegExReader::ALTERNATE_EXPRESSION(std::shared_ptr<ASTNode>& pReturnNode) {
	if (!GrammerUtils::match('|', MANDATORY_))
		return false;
	if (!PARSE_EXPRESSION(pReturnNode)) {

		return false;
	}
	return true;

}

bool RegExReader::PARSE_TERM(std::shared_ptr<ASTNode>& pReturnNode) {

	std::shared_ptr<SequenceASTNode> pSequenceASTNode = std::make_shared<SequenceASTNode>();

	while (true) {

		std::shared_ptr<ASTNode> pFactorNode;

		if (PARSE_FACTOR(pFactorNode)) {
			pSequenceASTNode->addChild(pFactorNode);
		}
		else
			break;
	}


	pReturnNode = pSequenceASTNode;

	return true;

}

bool RegExReader::PARSE_FACTOR(std::shared_ptr<ASTNode>& pReturnNode) {

	std::shared_ptr<ASTNode> pPrimaryNode;

	if (!PARSE_PRIMARY(pPrimaryNode)) {

		return false;
	}
	if (!PARSE_QUANTIFIERS(pPrimaryNode)) {

	}
	else {
	}


	pReturnNode = pPrimaryNode;

	return true;

}

bool RegExReader::PARSE_PRIMARY(std::shared_ptr<ASTNode>& pReturnNode) {
	if (GrammerUtils::match('.', OPTIONAL_)) {

		pReturnNode = std::make_shared<DotASTNode>();

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {

			pReturnNode = std::make_shared<CharacterASTNode>(PREV_TOKEN_TEXT[0]);

			return true;
		}
		else
			if (PARSE_CHARACTER_SET(pReturnNode)) {

				return true;
			}
			else
				if (PARSE_GROUP_CAPTURES(pReturnNode)) {

					return true;
				}
				else
					return false;

	return true;

}

bool RegExReader::PARSE_CHARACTER_SET(std::shared_ptr<ASTNode>& pReturnNode) {
	if (!GrammerUtils::match('[', MANDATORY_))
		return false;

	std::shared_ptr<CharacterClassASTNode> pCharacterClassNode = std::make_shared<CharacterClassASTNode>();

	if (!GrammerUtils::match('^', OPTIONAL_)) {

	}

	else {


		pCharacterClassNode->setNegated(true);

	}

	while (true) {
		if (PARSE_CHARACTER_RANGE(pCharacterClassNode)) {

		}
		else
			break;
	}

	if (!GrammerUtils::match(']', MANDATORY_))
		return false;

	pReturnNode = pCharacterClassNode;

	return true;

}

bool RegExReader::PARSE_CHARACTER_RANGE(std::shared_ptr<CharacterClassASTNode>& pReturnNode) {

	char startChar, endChar;

	if (!PARSE_CHARACTER_OR_INTEGER())
		return false;

	startChar = PREV_TOKEN_TEXT[0];

	if (!PARSE_CHARACTER_END_RANGE(pReturnNode)) {
		pReturnNode->addChar(startChar);
	}
	else {

		endChar = PREV_TOKEN_TEXT[0];
		pReturnNode->addRange(startChar, endChar);

	}

	return true;

}

bool RegExReader::PARSE_CHARACTER_END_RANGE(std::shared_ptr<CharacterClassASTNode>& pReturnNode) {
	if (!GrammerUtils::match('-', MANDATORY_))
		return false;
	if (!PARSE_CHARACTER_OR_INTEGER())
		return false;
	return true;

}

bool RegExReader::PARSE_CHARACTER_OR_INTEGER() {
	if (GrammerUtils::match(TokenType_::Type::TK_CHARACTER, OPTIONAL_)) {
		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {
			return true;
		}
		else
			return false;

	return true;

}

bool RegExReader::PARSE_GROUP_CAPTURES(std::shared_ptr<ASTNode>& pReturnNode) {
	if (!GrammerUtils::match('(', MANDATORY_))
		return false;
	if (!PARSE_EXPRESSION(pReturnNode)) {

		return false;
	}
	if (!GrammerUtils::match(')', MANDATORY_))
		return false;
	return true;

}

bool RegExReader::PARSE_QUANTIFIERS(std::shared_ptr<ASTNode>& pReturnNode) {
	if (GrammerUtils::match('*', OPTIONAL_)) {

		std::shared_ptr<RepeatASTNode> pRepeatASTNode = std::make_shared<RepeatASTNode>(pReturnNode, 0, -1);
		pReturnNode = pRepeatASTNode;

		return true;
	}
	else
		if (GrammerUtils::match('+', OPTIONAL_)) {

			std::shared_ptr<RepeatASTNode> pRepeatASTNode = std::make_shared<RepeatASTNode>(pReturnNode, 1, -1);
			pReturnNode = pRepeatASTNode;

			return true;
		}
		else
			if (GrammerUtils::match('?', OPTIONAL_)) {

				std::shared_ptr<RepeatASTNode> pRepeatASTNode = std::make_shared<RepeatASTNode>(pReturnNode, 0, 1);
				pReturnNode = pRepeatASTNode;

				return true;
			}
			else
				if (PARSE_QUANTIFIERS_X_TIMES(pReturnNode)) {

					return true;
				}
				else
					return false;

	return true;

}

bool RegExReader::PARSE_QUANTIFIERS_X_TIMES(std::shared_ptr<ASTNode>& pReturnNode) {
	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	std::shared_ptr<RepeatASTNode> pRepeatASTNode = std::make_shared<RepeatASTNode>(pReturnNode, -1, -1);
	int iMin = 0, iMax = 0;

	if (!GrammerUtils::match(TokenType_::Type::TK_INTEGER, MANDATORY_))
		return false;

	iMin = atoi(PREV_TOKEN_TEXT);
	pRepeatASTNode->setMin(iMin);
	pRepeatASTNode->setExactMin(true);

	if (!GrammerUtils::match(',', OPTIONAL_)) {

	}

	else {


		pRepeatASTNode->setExactMin(false);

		if (!GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {

		}

		else {


			iMax = atoi(PREV_TOKEN_TEXT);
			pRepeatASTNode->setMax(iMax);
			pRepeatASTNode->setExactMin(false);

		}

	}

	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	pReturnNode = pRepeatASTNode;

	return true;

}

