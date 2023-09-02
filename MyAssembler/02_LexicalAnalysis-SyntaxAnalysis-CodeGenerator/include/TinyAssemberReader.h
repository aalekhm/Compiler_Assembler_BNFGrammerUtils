#pragma once

#include <stack>
#include <vector>
#include "GrammerUtils.h"
#include <assert.h>
#include <memory>

class TinyAssemberReader {
	public:
		TinyAssemberReader();

		void addKeywords() {
			GrammerUtils::m_vKeywords.push_back("section");
			GrammerUtils::m_vKeywords.push_back("EDI");
			GrammerUtils::m_vKeywords.push_back("ESI");
			GrammerUtils::m_vKeywords.push_back("ESP");
			GrammerUtils::m_vKeywords.push_back("EBP");
			GrammerUtils::m_vKeywords.push_back("EAX");
			GrammerUtils::m_vKeywords.push_back("EBX");
			GrammerUtils::m_vKeywords.push_back("ECX");
			GrammerUtils::m_vKeywords.push_back("EDX");
			GrammerUtils::m_vKeywords.push_back("R0");
			GrammerUtils::m_vKeywords.push_back("R1");
			GrammerUtils::m_vKeywords.push_back("R2");
			GrammerUtils::m_vKeywords.push_back("R3");
			GrammerUtils::m_vKeywords.push_back("R4");
			GrammerUtils::m_vKeywords.push_back("R5");
			GrammerUtils::m_vKeywords.push_back("R6");
			GrammerUtils::m_vKeywords.push_back("R7");
			GrammerUtils::m_vKeywords.push_back("AX");
			GrammerUtils::m_vKeywords.push_back("BX");
			GrammerUtils::m_vKeywords.push_back("CX");
			GrammerUtils::m_vKeywords.push_back("DX");
			GrammerUtils::m_vKeywords.push_back("AH");
			GrammerUtils::m_vKeywords.push_back("BH");
			GrammerUtils::m_vKeywords.push_back("CH");
			GrammerUtils::m_vKeywords.push_back("DH");
			GrammerUtils::m_vKeywords.push_back("AL");
			GrammerUtils::m_vKeywords.push_back("BL");
			GrammerUtils::m_vKeywords.push_back("CL");
			GrammerUtils::m_vKeywords.push_back("DL");
			GrammerUtils::m_vKeywords.push_back("HLT");
			GrammerUtils::m_vKeywords.push_back("CALL");
			GrammerUtils::m_vKeywords.push_back("RET");
			GrammerUtils::m_vKeywords.push_back("NOT");
			GrammerUtils::m_vKeywords.push_back("NEG");
			GrammerUtils::m_vKeywords.push_back("NOP");
			GrammerUtils::m_vKeywords.push_back("MOVB");
			GrammerUtils::m_vKeywords.push_back("MOVW");
			GrammerUtils::m_vKeywords.push_back("MOVD");
			GrammerUtils::m_vKeywords.push_back("MOV");
			GrammerUtils::m_vKeywords.push_back("PUSH");
			GrammerUtils::m_vKeywords.push_back("POP");
			GrammerUtils::m_vKeywords.push_back("LEA");
			GrammerUtils::m_vKeywords.push_back("ADD");
			GrammerUtils::m_vKeywords.push_back("SUB");
			GrammerUtils::m_vKeywords.push_back("INC");
			GrammerUtils::m_vKeywords.push_back("DEC");
			GrammerUtils::m_vKeywords.push_back("IMUL");
			GrammerUtils::m_vKeywords.push_back("IDIV");
			GrammerUtils::m_vKeywords.push_back("AND");
			GrammerUtils::m_vKeywords.push_back("OR");
			GrammerUtils::m_vKeywords.push_back("XOR");
			GrammerUtils::m_vKeywords.push_back("SHL");
			GrammerUtils::m_vKeywords.push_back("SHR");
			GrammerUtils::m_vKeywords.push_back("JMP");
			GrammerUtils::m_vKeywords.push_back("JE");
			GrammerUtils::m_vKeywords.push_back("JNE");
			GrammerUtils::m_vKeywords.push_back("JZ");
			GrammerUtils::m_vKeywords.push_back("JG");
			GrammerUtils::m_vKeywords.push_back("JGE");
			GrammerUtils::m_vKeywords.push_back("JL");
			GrammerUtils::m_vKeywords.push_back("JLE");
			GrammerUtils::m_vKeywords.push_back("CMP");
		}

		bool def();
		bool sectionBody();
		bool instructions();
		bool label();
		bool instruction();
		bool register_();
		bool immediate();
		bool memory();
		bool baseIndexOperand();
		bool addressComponents();
		bool scalingMultiplier();
		bool displacementComponent();
		bool negDisplacementComponent();
		bool indexComponent();
		bool indexRegComponent();
		bool addOrSub();
		bool scale();
		bool instructionHlt();
		bool instructionCall();
		bool instructionRet();
		bool instructionNot();
		bool instructionNeg();
		bool memoryOrRegisterOrConst32();
		bool memoryOrRegister();
		bool instructionNop();
		bool instructionMov();
		bool movType();
		bool registerOrConst_To_memory();
		bool registerOrConst_To_register();
		bool registerOrMemoryOrConst_To_register();
		bool instructionPush();
		bool instructionPop();
		bool register_To_register();
		bool memory_To_register();
		bool register_To_memory();
		bool register_To_immediate();
		bool instructionLea();
		bool instructionAdd();
		bool instructionSub();
		bool instructionInc();
		bool instructionDec();
		bool instructionIMul();
		bool imm_To_regMem_To_register();
		bool optionalConst_To_register();
		bool optionalConst_To_memory();
		bool optionalConst_To_const();
		bool instructionIDiv();
		bool instructionAnd();
		bool instructionOr();
		bool instructionXor();
		bool instructionShl();
		bool instructionShr();
		bool const_To_register();
		bool const32();
		bool instructionJmp();
		bool jmp();
		bool je();
		bool jne();
		bool jz();
		bool jg();
		bool jge();
		bool jl();
		bool jle();
		bool instructionCmp();

		
		void 						read(const char* sFile);
		std::string					popOperator();
		ENUM_OP_PRECEDENCE 			opFromString(std::string sOperator);
		void						checkOpPrecedenceAndPush(std::string sCurrentOperator);
		
		static Tree*				makeLeaf(ASTNodeType eASTNodeType, const char* sText);
		Tree*						makeNode(ASTNodeType eASTNodeType, Tree* pLeft, Tree* pRight);
		Tree*						createPostFixExpr(Tree* pLeaf = nullptr);
		
		void						updateBlockString(std::string sIdent);
		void						removeLastFromBlockString();
		std::string					getBlockString();
		std::string					getFullyQualifiedNameForVariable(Tree* pNode, std::string sVariable);

		void 						addType(std::string sType);
		bool						isValidType(std::string sType);
		void 						addStructType(std::string sType);
		bool 						isValidStructType(std::string sType);
		Tree* 						hasNodeOfType(Tree* pNode, ASTNodeType eASTNodeType);
		
		void 						addInterfaceType(std::string sType);
		bool 						isValidInterfaceType(std::string sType);
		
		void						pushLocalHeapVar(std::string sVariableName);
		void						startBlockMarker();
		void						endBlockMarker();
		
		E_VARIABLESCOPE 			getCurrentScope();
		std::string 				getCurrentScopeString();
		
		Tree*						getStructNodeByName(std::string sStructName);
		void						handleFunctionCallInExpr(std::string sFunctionType);
		void 						clearTopOfExpressionStack();
		
		void 						pushNewStmt(E_OPCODEASM eE_OPCODEASM);
	protected:
		
	private:
		std::stack<std::string>		m_vOperatorStack;
		std::vector<std::string>	m_vPostFix;
		
		std::stack<std::string>		m_vLocalHeapVarStack;
		
		Tree*						m_pASTRootNode;
		Tree*						m_pASTCurrentNode;
		Tree*						m_pASTCurrentBlockNode;
		Tree*						m_pASTPrevBlockNode;
		
		std::vector<std::string>	m_sBlockStringList;
		bool						m_bInterfaceInProcess;
		bool						m_bStructInProcess;
		bool						m_bFunctionInProcess;
		
		std::vector<Tree*>			m_vStructs;
		std::vector<Tree*>			m_vInterfaces;
		
		std::vector<std::unique_ptr<OpcodeStmt>>	m_vAsmStmts;
		OpcodeStmt* 								m_pCurrentStmt;
		
		std::string									m_sLastLocalData;
		
		E_OPERAND_NUM								m_eE_OPERAND_NUM;
		E_OPERAND_TYPE								m_eE_OPERAND_TYPE;		

};
