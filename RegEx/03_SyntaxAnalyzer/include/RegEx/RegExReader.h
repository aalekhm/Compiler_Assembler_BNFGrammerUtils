#pragma once

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>

class ASTNode 
{
    public:
        virtual             ~ASTNode() = default;
        virtual bool        match(const std::string& sInStr, size_t& iPos) const = 0;
        virtual std::string toString() const = 0;
};

class CharacterASTNode : public ASTNode 
{
    public:
        CharacterASTNode(char cChar) : m_cChar(cChar) {}
    
        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            if (iPos < sInStr.length() && sInStr[iPos] == m_cChar) 
            {
                iPos++;
                return true;
            }

            return false;
        }
    
        std::string toString() const override 
        {
            return std::string(1, m_cChar);
        }

    private:
        char m_cChar;
};

class DotASTNode : public ASTNode 
{
    public:
        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            if (iPos < sInStr.length() && sInStr[iPos] != '\n') 
            {
                iPos++;
                return true;
            }

            return false;
        }
    
        std::string toString() const override 
        {
            return ".";
        }
};

class RepeatASTNode : public ASTNode 
{
    public:
        RepeatASTNode(std::shared_ptr<ASTNode> pASTNode, int iMin, int iMax)
        : m_pChildAST(pASTNode), m_iMinReps(iMin), m_iMaxReps(iMax), m_bCountExactMin(false)
        {}

        void setMin(int iMin)
        {
            m_iMinReps = iMin;
        }
    
        void setMax(int iMax)
        {
            m_iMaxReps = iMax;
        }

        void setExactMin(bool bEnable)
        {
            m_bCountExactMin = bEnable;
        }


        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            size_t iCount = 0;
            size_t iOriginalPos = iPos;
        
            while ( (m_iMaxReps == -1 || iCount < m_iMaxReps) 
                    && 
                    m_pChildAST->match(sInStr, iPos)
            ) {
                iCount++;
            }
        
            if(m_bCountExactMin)
            {
                if (iCount == m_iMinReps) 
                {
                    return true;
                }                
            }
            else
            {
                if (iCount >= m_iMinReps) 
                {
                    return true;
                }
            }
        
            iPos = iOriginalPos;
            return false;
        }
    
        std::string toString() const override 
        {
            std::string sRep;
                    if (m_iMinReps == 0 && m_iMaxReps == -1)    sRep = "*";
            else    if (m_iMinReps == 1 && m_iMaxReps == -1)    sRep = "+";
            else    if (m_iMinReps == 0 && m_iMaxReps == 1)     sRep = "?";
            else    sRep = "{" + std::to_string(m_iMinReps) + "," + (m_iMaxReps == -1 ? "" : std::to_string(m_iMaxReps)) + "}";
        
            return m_pChildAST->toString() + sRep;
        }
    
    private:
        std::shared_ptr<ASTNode>    m_pChildAST;
        int                         m_iMinReps;  // Minimum repetitions
        int                         m_iMaxReps;  // Maximum repetitions (-1 for unlimited)
        bool                        m_bCountExactMin;
};

class SequenceASTNode : public ASTNode 
{
    public:
        void addChild(std::shared_ptr<ASTNode> pASTNode) 
        {
            m_vASTChildren.push_back(pASTNode);
        }
    
        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            size_t iOriginalPos = iPos;
        
            for (const auto& pASTChild : m_vASTChildren) 
            {
                if (!pASTChild->match(sInStr, iPos)) 
                {
                    iPos = iOriginalPos;
                    return false;
                }
            }
        
            return true;
        }
    
        std::string toString() const override 
        {
            std::string sResult;
            for (const auto& pASTChild : m_vASTChildren) 
            {
                sResult += pASTChild->toString();
            }

            return sResult;
        }
    
    private:
        std::vector<std::shared_ptr<ASTNode>> m_vASTChildren;
};

class AlternationASTNode : public ASTNode 
{
    public:
        AlternationASTNode(std::shared_ptr<ASTNode> pLeftAST, std::shared_ptr<ASTNode> pRightAST)
        : m_pLeftAST(pLeftAST), m_pRightAST(pRightAST) 
        {}
    
        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            size_t iOriginalPos = iPos;
        
            if (m_pLeftAST->match(sInStr, iPos)) 
            {
                return true;
            }
        
            iPos = iOriginalPos;
            return m_pRightAST->match(sInStr, iPos);
        }
    
        std::string toString() const override 
        {
            return m_pLeftAST->toString() + "|" + m_pRightAST->toString();
        }
    
    private:
        std::shared_ptr<ASTNode> m_pLeftAST;
        std::shared_ptr<ASTNode> m_pRightAST;
};

class CharacterClassASTNode : public ASTNode 
{
    public:
        CharacterClassASTNode (bool bNegate = false) : m_bNegated(bNegate) 
        {}

        void setNegated(bool bNegated)
        {
            m_bNegated = bNegated;
        }
    
        void addChar(char c) 
        {
            m_Chars.insert(c);
        }
    
        void addRange(char cStart, char cEnd) 
        {
            for (char c = cStart; c <= cEnd; c++) 
            {
                m_Chars.insert(c);
            }
        }
    
        bool match(const std::string& sInStr, size_t& iPos) const override 
        {
            if (iPos >= sInStr.length()) return false;
        
            bool bFound = m_Chars.find( sInStr[iPos] ) != m_Chars.end();
            if (    (bFound && !m_bNegated) 
                    || 
                    (!bFound && m_bNegated)
            ) {
                iPos++;
                return true;
            }
        
            return false;
        }
    
        std::string toString() const override 
        {
            std::string sResult = "[";
            if (m_bNegated) sResult += "^";
        
            for (char c : m_Chars) 
            {
                sResult += c;
            }
        
            sResult += "]";
            return sResult;
        }
    
    private:
        std::set<char>  m_Chars;
        bool            m_bNegated;
};

class Regex 
{
    public:
                                    Regex(const std::string& sPattern);
        bool                        match(const std::string& sText);
        std::vector<std::string>    findAll(const std::string& sText);
    
    private:
        std::shared_ptr<ASTNode>    m_pRootAST;
};

class RegExReader {
	public:
		RegExReader();

		void addKeywords() {
		}

		bool def(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_EXPRESSION(std::shared_ptr<ASTNode>& pReturnNode);
		bool ALTERNATE_EXPRESSION(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_TERM(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_FACTOR(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_PRIMARY(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_CHARACTER_SET(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_CHARACTER_RANGE(std::shared_ptr<CharacterClassASTNode>& pReturnNode);
		bool PARSE_CHARACTER_END_RANGE(std::shared_ptr<CharacterClassASTNode>& pReturnNode);
		bool PARSE_CHARACTER_OR_INTEGER();
		bool PARSE_GROUP_CAPTURES(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_QUANTIFIERS(std::shared_ptr<ASTNode>& pReturnNode);
		bool PARSE_QUANTIFIERS_X_TIMES(std::shared_ptr<ASTNode>& pReturnNode);

		
		void 													read(const char* sFile);
		std::shared_ptr<ASTNode>                                parse(const std::string& sPattern);

        std::shared_ptr<ASTNode>                                m_pRootASTNode;
	protected:							
									
	private:							

};
