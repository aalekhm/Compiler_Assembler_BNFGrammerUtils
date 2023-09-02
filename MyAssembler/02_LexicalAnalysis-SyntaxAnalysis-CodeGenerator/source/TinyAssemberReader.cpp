#include "TinyAssemberReader.h"
#include "GrammerUtils.h"

#define __START_BLOCK_STRING__(_BLOCKNAME_) \
	updateBlockString(_BLOCKNAME_); \
	startBlockMarker(); \

#define __CURRENT_BLOCK_NODE__(__node__) \
	if(m_pASTCurrentBlockNode != nullptr) \
			m_pASTPrevBlockNode = m_pASTCurrentBlockNode; \
	m_pASTCurrentBlockNode = __node__; \

#define __RESET_BLOCK_NODE__ \
	m_pASTCurrentBlockNode = m_pASTPrevBlockNode; \
	m_pASTPrevBlockNode = nullptr; \

#define __END_CURRENT_BLOCK__ \
	removeLastFromBlockString(); \
	endBlockMarker(); \

#define __START_INTERFACE__ 										m_bInterfaceInProcess = true;
#define __END_INTERFACE__ 											m_bInterfaceInProcess = false;
										
#define __START_STRUCT__ 											m_bStructInProcess = true;
#define __END_STRUCT__ 												m_bStructInProcess = false;
									
#define __START_FUNCTION__ 											m_bFunctionInProcess = true;
#define __END_FUNCTION__ 											m_bFunctionInProcess = false;
									
#define SAVE_CURRENT_TOKEN											GrammerUtils::m_pSavedToken = GrammerUtils::m_pToken;
#define SAVE_PREV_TOKEN												GrammerUtils::m_pSavedToken = GrammerUtils::m_pPrevToken;

#define SAVED_TOKEN 												GrammerUtils::m_pSavedToken
#define SAVED_TOKEN_TEXT 											GrammerUtils::m_pSavedToken.getText()
#define PREV_TOKEN 													GrammerUtils::m_pPrevToken
#define PREV_TOKEN_TEXT 											GrammerUtils::m_pPrevToken.getText()

#define GET_INFO_FOR_KEY(__node__, __key__)							__node__->getAdditionalInfoFor(__key__)
#define SET_INFO_FOR_KEY(__node__, __key__, __info__)				__node__->setAdditionalInfo(__key__, __info__)
#define APPEND_INFO_FOR_KEY(__node__, __key__, __appendValue__)		__node__->appendAdditionalInfo(__key__, __appendValue__)
	
#define SET_OP_TYPE(__opNum__, __opType__, __opData__)				m_pCurrentStmt->setOperandType(__opNum__, __opType__, __opData__);
#define NEW_STMT(__newOpCode__)										pushNewStmt(__newOpCode__);
#define SET_CUR_OP_NUM(__opNum__)									m_eE_OPERAND_NUM = __opNum__;
#define SET_OPCODE(__opCode__)										m_pCurrentStmt->m_eOpcode = (E_OPCODEASM)__opCode__;

#define SET_OPCODE_BY_TYPE(__opType0__, __opType1__, __opType2__) \
	int32_t iOpcode = (int32_t)m_pCurrentStmt->m_eOpcode; \
	switch (m_eE_OPERAND_TYPE) \
	{ \
		case __opType0__: \
		{ \
			iOpcode += 0; \
		} \
		break; \
		case __opType1__: \
		{ \
			iOpcode += 1; \
		} \
		break; \
		case __opType2__: \
		{ \
			iOpcode += 2; \
		} \
		break; \
	} \
	SET_OPCODE(iOpcode); \

TinyAssemberReader::TinyAssemberReader()
	: m_bStructInProcess(false)
	, m_bFunctionInProcess(false)
	, m_bInterfaceInProcess(false)
	, m_pASTRootNode(nullptr)
	, m_pASTCurrentNode(nullptr)
	, m_pASTCurrentBlockNode(nullptr)
	, m_pASTPrevBlockNode(nullptr)
{
	GrammerUtils::init();
}

void TinyAssemberReader::read(const char* sFile)
{
	//////////////// THIS PIECE OF CODE WILL REMAIN COMMON FOR ALL //////////////////
	if (GrammerUtils::read(sFile))
	{
		addKeywords();
		GrammerUtils::getNextToken();

		m_pASTRootNode = new Tree();
		{
			m_pASTRootNode->m_eASTNodeType = ASTNodeType::ASTNode_ROOT;
			m_pASTCurrentNode = m_pASTRootNode;
		}

		def();

		GrammerUtils::processAssembly(m_vAsmStmts);
	}
	/////////////////////////////////////////////////////////////////////////////////
}

std::string TinyAssemberReader::popOperator()
{
	if(!m_vOperatorStack.empty())
	{
		std::string sTop = m_vOperatorStack.top();
		m_vOperatorStack.pop();
		
		return sTop;
	}
	
	return "";
}

ENUM_OP_PRECEDENCE TinyAssemberReader::opFromString(std::string sOperator)
{
	ENUM_OP_PRECEDENCE eOperator = ENUM_OP_PRECEDENCE::OP_INVALID;

	if (sOperator == "||")
		eOperator = ENUM_OP_PRECEDENCE::OP_LOGICALOR;
	else
	if (sOperator == "&&")
		eOperator = ENUM_OP_PRECEDENCE::OP_LOGICALAND;
	else
	if (sOperator == "|")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEOR;
	else
	if (sOperator == "&")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEAND;
	else
	if (sOperator == "^")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISEXOR;
	else
	if (sOperator == "~")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISENOT;
	else
	if (sOperator == "<<")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISELEFTSHIFT;
	else
	if (sOperator == ">>")
		eOperator = ENUM_OP_PRECEDENCE::OP_BITWISERIGHTSHIFT;
	else
	if (sOperator == "==")
		eOperator = ENUM_OP_PRECEDENCE::OP_EQ;
	else
	if (sOperator == "!=")
		eOperator = ENUM_OP_PRECEDENCE::OP_NEQ;
	else
	if (sOperator == "<")
		eOperator = ENUM_OP_PRECEDENCE::OP_LT;
	else
	if (sOperator == "<=")
		eOperator = ENUM_OP_PRECEDENCE::OP_LTEQ;
	else
	if (sOperator == ">")
		eOperator = ENUM_OP_PRECEDENCE::OP_GT;
	else
	if (sOperator == ">=")
		eOperator = ENUM_OP_PRECEDENCE::OP_GTEQ;
	else
	if (sOperator == "+")
		eOperator = ENUM_OP_PRECEDENCE::OP_ADD;
	else
	if (sOperator == "-")
		eOperator = ENUM_OP_PRECEDENCE::OP_SUB;
	else
	if (sOperator == "*")
		eOperator = ENUM_OP_PRECEDENCE::OP_MUL;
	else
	if (sOperator == "/")
		eOperator = ENUM_OP_PRECEDENCE::OP_DIV;
	else
	if (sOperator == "(")
		eOperator = ENUM_OP_PRECEDENCE::OP_LPAREN;
	else
	if (sOperator == ")")
		eOperator = ENUM_OP_PRECEDENCE::OP_RPAREN;
	else
	if (sOperator == "%")
		eOperator = ENUM_OP_PRECEDENCE::OP_MOD;
	else
	if (sOperator == "!")
		eOperator = ENUM_OP_PRECEDENCE::OP_NOT;
	else
	if (sOperator == "NEGATE")
		eOperator = ENUM_OP_PRECEDENCE::OP_NEGATE;

	return eOperator;
}

void TinyAssemberReader::checkOpPrecedenceAndPush(std::string sCurrentOperator)
{
	if(m_vOperatorStack.empty()) m_vOperatorStack.push(sCurrentOperator);
	else
	{
		if (sCurrentOperator == ")")
		{
			while (NOT m_vOperatorStack.empty())
			{
				std::string sOp = m_vOperatorStack.top();
				popOperator();

				if (sOp != "(")
				{	
					m_vPostFix.push_back(sOp);
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			if (sCurrentOperator == "(")
			{
				m_vOperatorStack.push(sCurrentOperator);
			}
			else
			{
				int eCurrOp = (int)opFromString(sCurrentOperator);

				std::string sTopOfStack = m_vOperatorStack.top();
				int eTopOfStack = (int)opFromString(sTopOfStack);

				if (eCurrOp > eTopOfStack)
					m_vOperatorStack.push(sCurrentOperator);
				else
				{
					popOperator();
					m_vPostFix.push_back(sTopOfStack);
					m_vOperatorStack.push(sCurrentOperator);
				}
			}
		}
	}
}

Tree* TinyAssemberReader::createPostFixExpr(Tree* pLeaf /* = nullptr*/)
{
	if (!m_vOperatorStack.empty())
	{
		std::string sTop = m_vOperatorStack.top();
		while (NOT(m_vOperatorStack.empty() || sTop == "_START_"))
		{
			m_vPostFix.push_back(sTop);
			m_vOperatorStack.pop();

			if (NOT m_vOperatorStack.empty())
				sTop = m_vOperatorStack.top();
		}

		if (sTop == "_START_")
			m_vOperatorStack.pop();
	}

	std::string sPostFixExpr = "";

	int iSize = m_vPostFix.size(), i = 0, iStartPos = 0;
	while (i < iSize)
	{
		if (m_vPostFix[i] == "_START_")
		{
			iStartPos = i + 1;
		}
		i++;
	}

	for (int i = iStartPos; i < iSize; i++)
	{
		sPostFixExpr += m_vPostFix[i];
		if (i < iSize - 1)
			sPostFixExpr += ", ";
	}

	if (iStartPos >= 1)
	{
		iStartPos--;
		while (iStartPos != m_vPostFix.size())
		{
			m_vPostFix.erase(m_vPostFix.begin() + iStartPos);
		}
	}
	else
		m_vPostFix.clear();

	if (pLeaf == nullptr)
		pLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, sPostFixExpr.c_str());
	else
		pLeaf->setAdditionalInfo("text", sPostFixExpr);

	return pLeaf;
}


Tree* TinyAssemberReader::makeLeaf(ASTNodeType eASTNodeType, const char* sText)
{
	Tree* pLeaf = new Tree();
	{
		pLeaf->m_eASTNodeType = eASTNodeType;
		pLeaf->setAdditionalInfo("text", sText);
	}

	return pLeaf;
}

Tree* TinyAssemberReader::makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight)
{
	Tree* pNode = new Tree();
	{
		pNode->m_eASTNodeType = eASTNodeType;
		pNode->m_pLeftNode = pLeft;
		pNode->m_pRightNode = pRight;
	}

	return pNode;
}

void TinyAssemberReader::updateBlockString(std::string sIdent)
{
	if (NOT sIdent.empty())
	{
		m_sBlockStringList.push_back(sIdent);
	}
}

std::string TinyAssemberReader::getBlockString()
{
	std::string sReturn;
	for (std::string sStr : m_sBlockStringList)
	{
		sReturn.append(sStr);
		sReturn.append("_");
	}

	return sReturn;
}

void TinyAssemberReader::removeLastFromBlockString()
{
	if (m_sBlockStringList.size() > 0)
	{
		m_sBlockStringList.erase(m_sBlockStringList.end()-1);
	}
}

std::string	TinyAssemberReader::getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable)
{
	std::string sFullyQualifiedName;
	bool bFound = false;

	if (NOT bFound)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			if (pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPE
				|
				pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPEARRAY
				|
				pChild->m_eASTNodeType == ASTNodeType::ASTNode_TYPESTRUCT
				) {
				if (pChild->m_sAdditionalInfo == sVariable)
				{
					sFullyQualifiedName = pChild->getAdditionalInfoFor("text");
					bFound = true;
					break;
				}
			}
		}
	}

	if (NOT bFound)
	{
		for (Tree* pStaticVar : FunctionInfo::m_vStaticVariables)
		{
			if (pStaticVar->getAdditionalInfoFor("text") == sVariable)
			{
				sFullyQualifiedName = pStaticVar->getAdditionalInfoFor("text");
				bFound = true;
				break;
			}
		}
	}

	// Check for 'Member Variables'
	if (NOT bFound)
	{
		Tree* pStructNode = pNode->m_pParentNode;
LABEL_STRUCT_INFO:
		if (pStructNode->m_eASTNodeType == ASTNodeType::ASTNode_STRUCTDEF)
		{
			// 1. Check if its a Member Variable of the 'Struct'
			for (Tree* pChildNode : pStructNode->m_vStatements) // Starts with index 0
			{
				if (pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_TYPE
					||
					pChildNode->m_eASTNodeType == ASTNodeType::ASTNode_TYPEARRAY
					) {
					if (pChildNode->m_sAdditionalInfo == sVariable)
					{
						sFullyQualifiedName = pChildNode->getAdditionalInfoFor("text");
						bFound = true;
						break;
					}
				}
			}

			// 2. Check if its a Member Variable of the Parent 'Struct'
			if (NOT bFound)
			{
				std::string sStructParentName = GET_INFO_FOR_KEY(pStructNode, "extends");
				if (NOT sStructParentName.empty())
				{
					pStructNode = getStructNodeByName(sStructParentName);
					goto LABEL_STRUCT_INFO;
				}
			}
		}

	}

	if (sFullyQualifiedName.empty())
	{
		if (pNode->m_eASTNodeType == ASTNodeType::ASTNode_FUNCTIONDEF)
		{
			sFullyQualifiedName = getFullyQualifiedNameForVariable(pNode->m_pRightNode, sVariable); // Search in the 'Arg list'.
		}
		else
		{
			sFullyQualifiedName = getFullyQualifiedNameForVariable(pNode->m_pParentNode, sVariable);
		}
	}

	return sFullyQualifiedName;
}


void TinyAssemberReader::addType(std::string sType)
{
	GrammerUtils::m_vTypes.push_back(sType);
}

bool TinyAssemberReader::isValidType(std::string sType)
{
	for(std::string sInBuiltType : GrammerUtils::m_vTypes)
	{
		if(sType == sInBuiltType)
		{
			GrammerUtils::m_pPrevToken = GrammerUtils::m_pToken;
			GrammerUtils::getNextToken();

			return true;
		}
	}
	
	return false;
}

void TinyAssemberReader::addStructType(std::string sType)
{
	GrammerUtils::m_vUserDefinedTypes.push_back(sType);
}

bool TinyAssemberReader::isValidStructType(std::string sType)
{
	for(std::string sInBuiltType : GrammerUtils::m_vUserDefinedTypes)
	{
		if(sType == sInBuiltType)
		{
			GrammerUtils::m_pPrevToken = GrammerUtils::m_pToken;
			GrammerUtils::getNextToken();

			return true;
		}
	}
	
	return false;
}

void TinyAssemberReader::addInterfaceType(std::string sType)
{
	GrammerUtils::m_vUserDefinedInterfaces.push_back(sType);
}

bool TinyAssemberReader::isValidInterfaceType(std::string sType)
{
	for(std::string sInBuiltInterface : GrammerUtils::m_vUserDefinedInterfaces)
	{
		if(sType == sInBuiltInterface)
		{
			GrammerUtils::m_pPrevToken = GrammerUtils::m_pToken;
			GrammerUtils::getNextToken();

			return true;
		}
	}
	
	return false;
}

Tree* TinyAssemberReader::hasNodeOfType(Tree* pNode, ASTNodeType eASTNodeType)
{
	Tree* pReturnNode = nullptr;
	if (pNode != nullptr)
	{
		for (Tree* pChild : pNode->m_vStatements)
		{
			if (pChild != nullptr)
			{
				if (pChild->m_eASTNodeType == eASTNodeType)
				{
					pReturnNode = pChild;
					break;
				}
			}
		}
	}

	return pReturnNode;
}

void TinyAssemberReader::startBlockMarker()
{
	m_vLocalHeapVarStack.push("{");
}

void TinyAssemberReader::pushLocalHeapVar(std::string sVariableName)
{
	if (NOT sVariableName.empty())
	{
		m_vLocalHeapVarStack.push(sVariableName);
	}
}

void TinyAssemberReader::endBlockMarker()
{
	while (NOT m_vLocalHeapVarStack.empty())
	{
		std::string sLocalHeapVar = m_vLocalHeapVarStack.top();
		m_vLocalHeapVarStack.pop();
		if (sLocalHeapVar == "{")
		{
			break;
		}
		else
		{
			Tree* pFreeASTNode = GrammerUtils::createNodeOfType(ASTNodeType::ASTNode_FREE, sLocalHeapVar.c_str());
			if (pFreeASTNode != nullptr)
			{
				m_pASTCurrentNode->addChild(pFreeASTNode);
			}
		}
	}
}

E_VARIABLESCOPE TinyAssemberReader::getCurrentScope()
{
	E_VARIABLESCOPE eCurrentScope = E_VARIABLESCOPE::INVALID;
	if (m_bStructInProcess)
	{
		if (m_bFunctionInProcess)
		{
			eCurrentScope = E_VARIABLESCOPE::LOCAL;
		}
		else
		{
			eCurrentScope = E_VARIABLESCOPE::MEMBER;
		}
	}
	else
	{
		if (m_bFunctionInProcess)
		{
			eCurrentScope = E_VARIABLESCOPE::LOCAL;
		}
		else
		{
			eCurrentScope = E_VARIABLESCOPE::STATIC;
		}
	}

	return eCurrentScope;
}

std::string TinyAssemberReader::getCurrentScopeString()
{
	return toString(getCurrentScope());
}

Tree* TinyAssemberReader::getStructNodeByName(std::string sStructName)
{
	for (Tree* pStruct : m_vStructs)
	{
		if (GET_INFO_FOR_KEY(pStruct, "text") == sStructName)
		{
			return pStruct;
		}
	}

	assert(false);
}

void TinyAssemberReader::handleFunctionCallInExpr(std::string sFunctionType)
{
	// The idea here is to create a temporary variable of the type returned by the function
	// & add it before the expression statement.
	// The temporary variable is then inserted in the expression.
	// Eg:
	// 		int32_t iRet = 10;
	//		iRet = 10 + retFunc(); // where retFunc return type is "int32_t".
	//		This will create a dummy code as follows:
	//			int32_t iRet = 10;
	//			int32_t iRet_retFunc = retFunc();	// This line of code will be inserted by the following piece of code.
	//			iRet = 10 + iRet_retFunc;
	
	Tree* pExpressionNode = m_pASTCurrentNode;
	Tree* pAssignNode = pExpressionNode->m_pParentNode;
	Tree* pFunctionCallNode = pExpressionNode->getLastStatement();
	{
		pFunctionCallNode->removeFromParent();
	}
	assert(pFunctionCallNode != nullptr);
	{
		char sUniqueFuncName[255] = {};
		sprintf_s(sUniqueFuncName, "%u", pFunctionCallNode);
	
		std::string sFuncName = GET_INFO_FOR_KEY(pFunctionCallNode, "text");
		std::string sFullyQualifiedTempVariableName;
		sFullyQualifiedTempVariableName.append( GET_INFO_FOR_KEY(pAssignNode, "text") );
		sFullyQualifiedTempVariableName.append("_");
		sFullyQualifiedTempVariableName.append(sFuncName);
		sFullyQualifiedTempVariableName.append("_");
		sFullyQualifiedTempVariableName.append(sUniqueFuncName);
		
		Tree* pPrimIntNode = makeLeaf(ASTNodeType::ASTNode_TYPE, sFullyQualifiedTempVariableName.c_str());
		{
			GrammerUtils::m_vUserDefinedVariables.push_back(sFullyQualifiedTempVariableName);
			GrammerUtils::m_vUserDefinedVariables.push_back(sFuncName);

			pPrimIntNode->m_sAdditionalInfo.append(sFullyQualifiedTempVariableName);
			SET_INFO_FOR_KEY(pPrimIntNode, "givenName", sFuncName);
			if (NOT sFunctionType.empty())
			{
				SET_INFO_FOR_KEY(pPrimIntNode, "modifyType", sFunctionType.c_str());
			}
			SET_INFO_FOR_KEY(pPrimIntNode, "type", "int32_t");
			SET_INFO_FOR_KEY(pPrimIntNode, "scope", getCurrentScopeString());
			m_pASTCurrentBlockNode->addChild(pPrimIntNode);
		}
		
		Tree* pExpressionLeftLeaf = makeLeaf(ASTNodeType::ASTNode_EXPRESSION, "");
		{
			pPrimIntNode->m_pLeftNode = pExpressionLeftLeaf;
			pExpressionLeftLeaf->m_pParentNode = pPrimIntNode;
			pExpressionLeftLeaf->addChild(pFunctionCallNode);
		}
		
		m_vPostFix.push_back(sFullyQualifiedTempVariableName);
	}
}

void TinyAssemberReader::clearTopOfExpressionStack()
{
	int32_t iPostFixSize = m_vPostFix.size();
	if (iPostFixSize > 0)
	{
		if (iPostFixSize == 1 && m_vPostFix[0] == "_START_")
			m_vPostFix.clear();
		else
		if (m_vPostFix[iPostFixSize - 1] == "_START_")
			m_vPostFix.erase(m_vPostFix.begin() + iPostFixSize - 1);
	}

	if (m_vOperatorStack.size() > 0 && m_vOperatorStack.top() == "_START_")
		m_vOperatorStack.pop();
}

void TinyAssemberReader::pushNewStmt(E_OPCODEASM eE_OPCODEASM)
{
	std::unique_ptr<OpcodeStmt> pAsmStmt = std::make_unique<OpcodeStmt>(eE_OPCODEASM);
	{
		m_vAsmStmts.push_back(std::move(pAsmStmt));
		m_pCurrentStmt = m_vAsmStmts.back().get();
	}
}


bool TinyAssemberReader::def() {
if(!GrammerUtils::match("section", MANDATORY_))
return false;

											
											
while(true) {
if(sectionBody()) {
}
else
break;
}

return true;

}

bool TinyAssemberReader::sectionBody() {
if(!GrammerUtils::match('.', MANDATORY_))
return false;
if(!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
return false;
while(true) {
if(instructions()) {
}
else
break;
}

return true;

}

bool TinyAssemberReader::instructions() {
if(!label()) {
}
else {
}

if(!instruction())
return false;
return true;

}

bool TinyAssemberReader::label() {
if(!GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, MANDATORY_))
return false;
if(!GrammerUtils::match(':', MANDATORY_))
return false;
return true;

}

bool TinyAssemberReader::instruction() {
if(instructionHlt()) {
return true;
}
else
if(instructionCall()) {
return true;
}
else
if(instructionRet()) {
return true;
}
else
if(instructionNot()) {
return true;
}
else
if(instructionNeg()) {
return true;
}
else
if(instructionNop()) {
return true;
}
else
if(instructionMov()) {
return true;
}
else
if(instructionPush()) {
return true;
}
else
if(instructionPop()) {
return true;
}
else
if(instructionLea()) {
return true;
}
else
if(instructionAdd()) {
return true;
}
else
if(instructionSub()) {
return true;
}
else
if(instructionInc()) {
return true;
}
else
if(instructionDec()) {
return true;
}
else
if(instructionIMul()) {
return true;
}
else
if(instructionIDiv()) {
return true;
}
else
if(instructionAnd()) {
return true;
}
else
if(instructionOr()) {
return true;
}
else
if(instructionXor()) {
return true;
}
else
if(instructionShl()) {
return true;
}
else
if(instructionShr()) {
return true;
}
else
if(instructionJmp()) {
return true;
}
else
if(instructionCmp()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::register_() {
if(GrammerUtils::match("EDI", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("ESI", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("ESP", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("EBP", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("EAX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("EBX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("ECX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("EDX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R0", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R1", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R2", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R3", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R4", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R5", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R6", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("R7", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("AX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("BX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("CX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("DX", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("AH", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("BH", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("CH", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("DH", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("AL", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("BL", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("CL", OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match("DL", OPTIONAL_)) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::immediate() {
if(GrammerUtils::match(TokenType_::Type::TK_INTEGER, OPTIONAL_)) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::memory() {
if(!GrammerUtils::match('[', MANDATORY_))
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
												
if(!baseIndexOperand())
return false;
if(!GrammerUtils::match(']', MANDATORY_))
return false;
return true;

}

bool TinyAssemberReader::baseIndexOperand() {
if(!register_())
return false;

												m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
												m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::BASE_REG, PREV_TOKEN_TEXT);
											
if(!addressComponents()) {
}
else {
}

return true;

}

bool TinyAssemberReader::addressComponents() {
if(scalingMultiplier()) {
return true;
}
else
if(negDisplacementComponent()) {
return true;
}
else
if(indexComponent()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::scalingMultiplier() {
if(!GrammerUtils::match('*', MANDATORY_))
return false;
if(!scale())
return false;

													m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::SCALE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::displacementComponent() {
if(!GrammerUtils::match(TokenType_::Type::TK_INTEGER, MANDATORY_))
return false;

													m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::DISP_CONST, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::negDisplacementComponent() {
if(!GrammerUtils::match('-', MANDATORY_))
return false;

													m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::DISP_ADD_SUB, "-");
												
if(!displacementComponent())
return false;
return true;

}

bool TinyAssemberReader::indexComponent() {
if(!GrammerUtils::match('+', MANDATORY_))
return false;
if(displacementComponent()) {

														m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::DISP_ADD_SUB, "+");
												
return true;
}
else
if(indexRegComponent()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::indexRegComponent() {
if(!register_())
return false;

													m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::INDEX_REG, PREV_TOKEN_TEXT);
												
if(!scalingMultiplier()) {
}
else {
}

if(!addOrSub()) {
}
else {

														m_pCurrentStmt->setMemBit(m_eE_OPERAND_NUM, E_MEMOP::DISP_ADD_SUB, PREV_TOKEN_TEXT);
												
if(!displacementComponent())
return false;
}

return true;

}

bool TinyAssemberReader::addOrSub() {
if(GrammerUtils::match('+', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match('-', OPTIONAL_)) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::scale() {
if(GrammerUtils::match('1', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match('2', OPTIONAL_)) {
return true;
}
else
if(GrammerUtils::match('4', OPTIONAL_)) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionHlt() {
if(!GrammerUtils::match("HLT", MANDATORY_))
return false;

												NEW_STMT(E_OPCODEASM::ASM_HLT);
											
return true;

}

bool TinyAssemberReader::instructionCall() {
if(!GrammerUtils::match("CALL", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_CALL);

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionRet() {
if(!GrammerUtils::match("RET", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_RET);
												
return true;

}

bool TinyAssemberReader::instructionNot() {
if(!GrammerUtils::match("NOT", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_NOT_R);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memoryOrRegister())
return false;

													SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, -1)

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionNeg() {
if(!GrammerUtils::match("NEG", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_NEG_R);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memoryOrRegister())
return false;

													SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, -1)

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::memoryOrRegisterOrConst32() {
if(register_()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													
return true;
}
else
if(memory()) {
return true;
}
else
if(const32()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::memoryOrRegister() {
if(memory()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
													
return true;
}
else
if(register_()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionNop() {
if(!GrammerUtils::match("NOP", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_NOP);
												
return true;

}

bool TinyAssemberReader::instructionMov() {
if(!movType())
return false;
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_MOVD_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::movType() {
if(GrammerUtils::match("MOVB", OPTIONAL_)) {

													NEW_STMT(E_OPCODEASM::ASM_MOVB_R_R);
												
return true;
}
else
if(GrammerUtils::match("MOVW", OPTIONAL_)) {

													NEW_STMT(E_OPCODEASM::ASM_MOVW_R_R);
												
return true;
}
else
if(GrammerUtils::match("MOVD", OPTIONAL_)) {

													NEW_STMT(E_OPCODEASM::ASM_MOVD_R_R);
												
return true;
}
else
if(GrammerUtils::match("MOV", OPTIONAL_)) {

													NEW_STMT(E_OPCODEASM::ASM_MOVD_R_R);
												
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::registerOrConst_To_memory() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memory())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, "");
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(register_()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;
}
else
if(const32()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::registerOrConst_To_register() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(register_()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;
}
else
if(const32()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::registerOrMemoryOrConst_To_register() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(!memoryOrRegisterOrConst32())
return false;

													if(m_eE_OPERAND_TYPE == E_OPERAND_TYPE::_CONST || m_eE_OPERAND_TYPE == E_OPERAND_TYPE::_LABEL)
														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionPush() {
if(!GrammerUtils::match("PUSH", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_PUSH_R);
													
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memoryOrRegisterOrConst32())
return false;

													if(m_eE_OPERAND_TYPE == E_OPERAND_TYPE::_CONST || m_eE_OPERAND_TYPE == E_OPERAND_TYPE::_LABEL)
														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
												
return true;

}

bool TinyAssemberReader::instructionPop() {
if(!GrammerUtils::match("POP", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_POP_R);
													
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memoryOrRegister())
return false;

													SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, -1)

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::register_To_register() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!register_())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(!register_())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::memory_To_register() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(!memory())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::register_To_memory() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!memory())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::register_To_immediate() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!immediate())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(!register_())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionLea() {
if(!GrammerUtils::match("LEA", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_LEA_R_M);
												
if(!memory_To_register())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionAdd() {
if(!GrammerUtils::match("ADD", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_ADD_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_ADD_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionSub() {
if(!GrammerUtils::match("SUB", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_SUB_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_SUB_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionInc() {
if(!GrammerUtils::match("INC", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_INC_R);
													
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(register_()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
														SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													
return true;
}
else
if(memory()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_INC_M);
												
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionDec() {
if(!GrammerUtils::match("DEC", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_DEC_R);
													
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(register_()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
														SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													
return true;
}
else
if(memory()) {

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_DEC_M);
												
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionIMul() {
if(!GrammerUtils::match("IMUL", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_IMUL_R_R);
												
if(!imm_To_regMem_To_register())
return false;
return true;

}

bool TinyAssemberReader::imm_To_regMem_To_register() {

													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
if(!GrammerUtils::match(',', MANDATORY_))
return false;

													SET_CUR_OP_NUM(E_OPERAND_NUM::_1);
												
if(optionalConst_To_register()) {
return true;
}
else
if(optionalConst_To_memory()) {
return true;
}
else
if(optionalConst_To_const()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::optionalConst_To_register() {
if(!register_())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_R);
												
if(!GrammerUtils::match(',', OPTIONAL_)) {

}

else {


													SET_CUR_OP_NUM(E_OPERAND_NUM::_2);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_R_K);
												
}

return true;

}

bool TinyAssemberReader::optionalConst_To_memory() {
if(!memory())
return false;

													m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_M);
												
if(!GrammerUtils::match(',', OPTIONAL_)) {

}

else {


													SET_CUR_OP_NUM(E_OPERAND_NUM::_2);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_M_K);
												
}

return true;

}

bool TinyAssemberReader::optionalConst_To_const() {
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_K);
												
if(!GrammerUtils::match(',', OPTIONAL_)) {

}

else {


													SET_CUR_OP_NUM(E_OPERAND_NUM::_2);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
													SET_OPCODE(E_OPCODEASM::ASM_IMUL_R_K_K);
												
}

return true;

}

bool TinyAssemberReader::instructionIDiv() {
if(!GrammerUtils::match("IDIV", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_IDIV_R);
													
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(register_()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_REG;
														SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
														SET_OPCODE(E_OPCODEASM::ASM_IDIV_R);
													
return true;
}
else
if(memory()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_MEM;
														SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
														SET_OPCODE(E_OPCODEASM::ASM_IDIV_M);
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionAnd() {
if(!GrammerUtils::match("AND", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_AND_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_AND_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionOr() {
if(!GrammerUtils::match("OR", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_OR_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_OR_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionXor() {
if(!GrammerUtils::match("XOR", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_XOR_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_XOR_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionShl() {
if(!GrammerUtils::match("SHL", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_SHL_R_R);
												
if(registerOrConst_To_register()) {

														SET_OPCODE(E_OPCODEASM::ASM_SHL_R_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_SHL_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionShr() {
if(!GrammerUtils::match("SHR", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_SHR_R_R);
												
if(registerOrConst_To_register()) {

														SET_OPCODE(E_OPCODEASM::ASM_SHR_R_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_SHR_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::const_To_register() {
if(!register_())
return false;
if(!GrammerUtils::match(',', MANDATORY_))
return false;
if(!const32())
return false;
return true;

}

bool TinyAssemberReader::const32() {
if(immediate()) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_CONST;
													
return true;
}
else
if(GrammerUtils::match(TokenType_::Type::TK_IDENTIFIER, OPTIONAL_)) {

														m_eE_OPERAND_TYPE = E_OPERAND_TYPE::_LABEL;
													
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::instructionJmp() {
if(jmp()) {
return true;
}
else
if(je()) {
return true;
}
else
if(jne()) {
return true;
}
else
if(jz()) {
return true;
}
else
if(jg()) {
return true;
}
else
if(jge()) {
return true;
}
else
if(jl()) {
return true;
}
else
if(jle()) {
return true;
}
else
return false;

return true;

}

bool TinyAssemberReader::jmp() {
if(!GrammerUtils::match("JMP", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JMP);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::je() {
if(!GrammerUtils::match("JE", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JE);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jne() {
if(!GrammerUtils::match("JNE", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JNE);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jz() {
if(!GrammerUtils::match("JZ", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JZ);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jg() {
if(!GrammerUtils::match("JG", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JG);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jge() {
if(!GrammerUtils::match("JGE", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JGE);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jl() {
if(!GrammerUtils::match("JL", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JL);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::jle() {
if(!GrammerUtils::match("JLE", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_JLE);
													SET_CUR_OP_NUM(E_OPERAND_NUM::_0);
												
if(!const32())
return false;

													SET_OP_TYPE(m_eE_OPERAND_NUM, m_eE_OPERAND_TYPE, PREV_TOKEN_TEXT);
												
return true;

}

bool TinyAssemberReader::instructionCmp() {
if(!GrammerUtils::match("CMP", MANDATORY_))
return false;

													NEW_STMT(E_OPCODEASM::ASM_CMP_R_R);
												
if(registerOrMemoryOrConst_To_register()) {

														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_MEM, E_OPERAND_TYPE::_CONST)
													
return true;
}
else
if(registerOrConst_To_memory()) {

														SET_OPCODE(E_OPCODEASM::ASM_CMP_M_R);
														SET_OPCODE_BY_TYPE(E_OPERAND_TYPE::_REG, E_OPERAND_TYPE::_CONST, -1)
													
return true;
}
else
return false;

return true;

}

