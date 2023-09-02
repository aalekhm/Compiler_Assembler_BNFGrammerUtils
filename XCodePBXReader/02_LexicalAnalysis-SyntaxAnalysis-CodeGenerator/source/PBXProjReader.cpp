#include "PBXProjReader.h"
#include "GrammerUtils.h"

#define SAVE_CURRENT_TOKEN											GrammerUtils::m_pSavedToken = GrammerUtils::m_pToken;
#define SAVE_PREV_TOKEN												GrammerUtils::m_pSavedToken = GrammerUtils::m_pPrevToken;

#define SAVED_TOKEN 												GrammerUtils::m_pSavedToken
#define SAVED_TOKEN_TEXT 											GrammerUtils::m_pSavedToken.getText()
#define PREV_TOKEN 													GrammerUtils::m_pPrevToken
#define PREV_TOKEN_TEXT 											GrammerUtils::m_pPrevToken.getText()

PBXProjReader::PBXProjReader()
{
	GrammerUtils::init();
}

void PBXProjReader::read(const char* sFile)
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


bool PBXProjReader::def() {
	if (!SECTION())
		return false;
	return true;

}

bool PBXProjReader::NAME_VALUE_PAIRS() {
	while (true) {
		if (NAME_AND_VALUE()) {
		}
		else
			break;
	}

	return true;

}

bool PBXProjReader::NAME_AND_VALUE() {

	PBXParseHelper* pPBXParseHelper = GetPBXParserHelper();

	if (!NAME_TYPES())
		return false;

	std::string sName = PREV_TOKEN_TEXT;
	pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey = PREV_TOKEN_TEXT;
	std::cout << PREV_TOKEN_TEXT;

	if (sName == "isa")
	{
		pPBXParseHelper->m_pCurrentPBXObject->m_iParseState = EPBXTYPE_PARSE_STATE::ISA_TYPE;
	}

	if (!GrammerUtils::match('=', MANDATORY_))
		return false;

	std::cout << " = ";

	if (!VALUE_TYPES()) {
	}
	else {
	}


	std::cout << PREV_TOKEN_TEXT << "\n";

	if (!GrammerUtils::match(';', MANDATORY_))
		return false;
	return true;

}

bool PBXProjReader::NAME_TYPES() {

	PBXParseHelper* pPBXParseHelper = GetPBXParserHelper();

	if (GrammerUtils::match(TokenType_::Type::TK_PBXNUMBER, OPTIONAL_)) {

		pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType = EPBXVALUE_TYPE::NUMBER;

		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_PBXIDENTIFIER, OPTIONAL_)) {

			pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType = EPBXVALUE_TYPE::IDENTIFIER;

			return true;
		}
		else
			if (GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {

				pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType = EPBXVALUE_TYPE::STRING;

				return true;
			}
			else
				return false;

	return true;

}

bool PBXProjReader::SECTION() {

	PBXParseHelper* pPBXParseHelper = GetPBXParserHelper();

	if (!GrammerUtils::match('{', MANDATORY_))
		return false;

	pPBXParseHelper->m_pCurrentPBXObjectStack.push_back(std::make_unique<PBXObject>());
	pPBXParseHelper->m_pCurrentPBXObject = pPBXParseHelper->m_pCurrentPBXObjectStack.back().get();

	std::cout << '{' << "\n";

	if (!NAME_VALUE_PAIRS()) {
	}
	else {
	}

	if (!GrammerUtils::match('}', MANDATORY_))
		return false;

	PBXObject* pCurrentPBXObject = nullptr;
	bool bFound = false;
	std::string sPBXType = "";

	if (pPBXParseHelper->m_pCurrentPBXObject->m_iParseState == EPBXTYPE_PARSE_STATE::END_OF_TYPE)
	{
		bFound = true;
		pCurrentPBXObject = pPBXParseHelper->m_pCurrentPBXObject;
		sPBXType = pPBXParseHelper->m_pCurrentPBXObject->m_sPBXType;
		std::cout << "PBX TYPE = " << sPBXType << "\n";
		pPBXParseHelper->m_pCurrentPBXObject->m_iParseState = EPBXTYPE_PARSE_STATE::START;
	}

	std::unique_ptr<PBXObject> pLastPBXObject = std::move(pPBXParseHelper->m_pCurrentPBXObjectStack.back());
	pPBXParseHelper->m_pCurrentPBXObjectStack.pop_back();

	if (pPBXParseHelper->m_pCurrentPBXObjectStack.size() > 0)
	{
		pPBXParseHelper->m_pCurrentPBXObject = pPBXParseHelper->m_pCurrentPBXObjectStack.back().get();
		std::cout << "KEY = " << pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey << "\n";

		pLastPBXObject->m_sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;
		pLastPBXObject->m_sKeyType = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType;

		pPBXParseHelper->m_pCurrentPBXObject->m_ChildMap[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey] = std::move(pLastPBXObject);
		PBXObject* pLastPBXObjectRaw = pPBXParseHelper->m_pCurrentPBXObject->m_ChildMap[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey].get();

		if (bFound)
		{
			std::string sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;

			if (m_pPBXHelper->m_pTypeListMap[sPBXType] == nullptr)
			{
				m_pPBXHelper->m_pTypeListMap[sPBXType] = std::make_unique<PBXTypeList>(sPBXType);
			}

			PBXTypeList* pPBXTypeList = m_pPBXHelper->m_pTypeListMap[sPBXType].get();
			if (pPBXTypeList != nullptr)
			{
				pCurrentPBXObject->m_sKey = sKey;
				pPBXTypeList->m_pObjectList[sKey] = pCurrentPBXObject;
			}
		}
	}
	else
	{
		m_pPBXHelper->m_pRootPBXObject = std::move(pLastPBXObject);
	}

	return true;

}

bool PBXProjReader::VALUE_TYPES() {

	PBXParseHelper* pPBXParseHelper = GetPBXParserHelper();

	if (SECTION()) {
		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {


			std::unique_ptr<PBXValue> pValue = std::make_unique<PBXValue>();
			{
				pValue.get()->m_sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;
				pValue.get()->m_ePbxKeyType = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType;
				pValue.get()->m_sValue = PREV_TOKEN_TEXT;
				pValue.get()->m_ePbxValueType = EPBXVALUE_TYPE::STRING;

				pPBXParseHelper->m_pCurrentPBXObject->m_PairMap[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey] = std::move(pValue);
				if (pPBXParseHelper->m_pCurrentPBXObject->m_iParseState == EPBXTYPE_PARSE_STATE::ISA_TYPE)
				{
					pPBXParseHelper->m_pCurrentPBXObject->m_iParseState = EPBXTYPE_PARSE_STATE::END_OF_TYPE;
					pPBXParseHelper->m_pCurrentPBXObject->m_sPBXType = PREV_TOKEN_TEXT;
				}
			}

			return true;
		}
		else
			if (GrammerUtils::match(TokenType_::Type::TK_PBXIDENTIFIER, OPTIONAL_)) {

				std::unique_ptr<PBXValue> pValue = std::make_unique<PBXValue>();
				{
					pValue.get()->m_sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;
					pValue.get()->m_ePbxKeyType = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType;
					pValue.get()->m_sValue = PREV_TOKEN_TEXT;
					pValue.get()->m_ePbxValueType = EPBXVALUE_TYPE::IDENTIFIER;

					pPBXParseHelper->m_pCurrentPBXObject->m_PairMap[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey] = std::move(pValue);
					if (pPBXParseHelper->m_pCurrentPBXObject->m_iParseState == EPBXTYPE_PARSE_STATE::ISA_TYPE)
					{
						pPBXParseHelper->m_pCurrentPBXObject->m_iParseState = EPBXTYPE_PARSE_STATE::END_OF_TYPE;
						pPBXParseHelper->m_pCurrentPBXObject->m_sPBXType = PREV_TOKEN_TEXT;
					}
				}

				return true;
			}
			else
				if (GrammerUtils::match(TokenType_::Type::TK_PBXNUMBER, OPTIONAL_)) {

					std::unique_ptr<PBXValue> pValue = std::make_unique<PBXValue>();
					{
						pValue.get()->m_sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;
						pValue.get()->m_ePbxKeyType = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKeyType;
						pValue.get()->m_sValue = PREV_TOKEN_TEXT;
						pValue.get()->m_ePbxValueType = EPBXVALUE_TYPE::NUMBER;

						pPBXParseHelper->m_pCurrentPBXObject->m_PairMap[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey] = std::move(pValue);
						if (pPBXParseHelper->m_pCurrentPBXObject->m_iParseState == EPBXTYPE_PARSE_STATE::ISA_TYPE)
						{
							pPBXParseHelper->m_pCurrentPBXObject->m_iParseState = EPBXTYPE_PARSE_STATE::END_OF_TYPE;
							pPBXParseHelper->m_pCurrentPBXObject->m_sPBXType = PREV_TOKEN_TEXT;
						}
					}

					return true;
				}
				else
					if (PBX_ARRAY()) {
						return true;
					}
					else
						return false;

	return true;

}

bool PBXProjReader::PBX_ARRAY_ELEMENTS() {
	if (GrammerUtils::match(TokenType_::Type::TK_PBXNUMBER, OPTIONAL_)) {
		return true;
	}
	else
		if (GrammerUtils::match(TokenType_::Type::TK_PBXIDENTIFIER, OPTIONAL_)) {
			return true;
		}
		else
			if (GrammerUtils::match(TokenType_::Type::TK_STRING, OPTIONAL_)) {
				return true;
			}
			else
				return false;

	return true;

}

bool PBXProjReader::PBX_ARRAY() {

	std::unique_ptr<PBXList> pPBXList = nullptr;
	PBXParseHelper* pPBXParseHelper = GetPBXParserHelper();

	if (!GrammerUtils::match('(', MANDATORY_))
		return false;

	pPBXList = std::make_unique<PBXList>();
	std::cout << '(' << "\n";

	while (true) {
		if (PBX_ARRAY_ELEMENTS()) {

			std::unique_ptr<PBXValue> pPBXValue = std::make_unique<PBXValue>();
			pPBXValue->m_bIsList = true;

			switch (PREV_TOKEN.getType())
			{
			case TokenType_::Type::TK_PBXNUMBER:
			{
				pPBXValue->m_ePbxValueType = EPBXVALUE_TYPE::NUMBER;
			}
			break;
			case TokenType_::Type::TK_PBXIDENTIFIER:
			{
				pPBXValue->m_ePbxValueType = EPBXVALUE_TYPE::IDENTIFIER;
			}
			break;
			case TokenType_::Type::TK_STRING:
			{
				pPBXValue->m_ePbxValueType = EPBXVALUE_TYPE::STRING;
			}
			break;
			}

			pPBXValue.get()->m_sValue = PREV_TOKEN_TEXT;

			pPBXList->m_vList.push_back(std::move(pPBXValue));
			std::cout << '\t' << PREV_TOKEN_TEXT << "\n";

			if (!GrammerUtils::match(',', OPTIONAL_)) {

			}

			else {

			}

		}
		else
			break;
	}

	if (!GrammerUtils::match(')', MANDATORY_))
		return false;

	if (pPBXList != nullptr)
	{
		pPBXList->m_sKey = pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey;
		pPBXParseHelper->m_pCurrentPBXObject->m_List[pPBXParseHelper->m_pCurrentPBXObject->m_sCurrentlyProcessingKey] = std::move(pPBXList);
	}

	return true;

}

