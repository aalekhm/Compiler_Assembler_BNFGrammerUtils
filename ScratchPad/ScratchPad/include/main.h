#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "math.h"

#define NOT !

struct Tree
{
	Tree(std::string sValue)
		: m_sValue(sValue)
		, m_pLeftNode(nullptr)
		, m_pRightNode(nullptr)
		, m_iRegCount(-1)
	{ }

	Tree*			m_pLeftNode;
	Tree*			m_pRightNode;

	std::string		m_sValue;
	int32_t			m_iRegCount;
};

bool isOperator(std::string sValue)
{
	return (sValue == "*" || sValue == "/" || sValue == "+" || sValue == "-" || sValue == "^");
}

void convertPostFixExpressionToTree(std::vector<Tree*>& vExpression)
{
	std::vector<Tree*>::iterator itr;
	int32_t iCount = 0;
	for (itr = vExpression.begin(); itr != vExpression.end();)
	{
		Tree* pNode = *itr;
		if ( isOperator(pNode->m_sValue) )
		{
			Tree* pLeftNode = *(itr - 2);
			pNode->m_pLeftNode = pLeftNode;

			Tree* pRightNode = *(itr - 1);
			pNode->m_pRightNode = pRightNode;

			vExpression.erase(itr - 2, itr);

			iCount = (iCount - 2) + 1;
			itr = vExpression.begin() + iCount;

			if (vExpression.size() == 1)
				break;
			continue;
		}

		itr++;
		iCount++;
	}
}

/*
 * In-Order Traversal:
 *		In case of binary search trees (BST), 
 *		Inorder traversal gives nodes in non-decreasing order. 
 *		To get nodes of BST in non-increasing order, 
 *		a variation of Inorder traversal where Inorder traversal s reversed can be used.
*/
void inOrder(Tree* pRoot)
{
	if (pRoot == nullptr)
		return;

	inOrder(pRoot->m_pLeftNode);
	std::cout << pRoot->m_sValue << " ";
	inOrder(pRoot->m_pRightNode);
}

/*
* Pre-Order Traversal:
*		Preorder traversal is used to create a copy of the tree. 
*		Preorder traversal is also used to get prefix expression on of an expression tree. 
*		Please see http://en.wikipedia.org/wiki/Polish_notation to know why prefix expressions are useful.
*/
void preOrder(Tree* pRoot)
{
	if (pRoot == nullptr)
		return;

	std::cout << pRoot->m_sValue << " ";

	preOrder(pRoot->m_pLeftNode);
	preOrder(pRoot->m_pRightNode);
}

/*
* Post-Order Traversal:
*		Postorder traversal is used to delete the tree. 
*		Please see the question for deletion of tree for details. 
*		Postorder traversal is also useful to get the postfix expression of an expression tree. 
*		Please see http://en.wikipedia.org/wiki/Reverse_Polish_notation to for the usage of postfix expression.
*/
void postOrder(Tree* pRoot)
{
	if (pRoot == nullptr)
		return;
	
	postOrder(pRoot->m_pLeftNode);
	postOrder(pRoot->m_pRightNode);

	std::cout << pRoot->m_sValue << " ";
}

void Traverse(Tree* pRoot)
{
	inOrder(pRoot);		std::cout << std::endl;
	preOrder(pRoot);	std::cout << std::endl;
	postOrder(pRoot);	std::cout << std::endl;
}

void labelize_PostOrderTraversal(Tree* pRoot)
{
	if (pRoot == nullptr)
		return;

	labelize_PostOrderTraversal(pRoot->m_pLeftNode);
	labelize_PostOrderTraversal(pRoot->m_pRightNode);

	if (isOperator(pRoot->m_sValue))
	{
		if (pRoot->m_pLeftNode->m_iRegCount == pRoot->m_pRightNode->m_iRegCount)
			pRoot->m_iRegCount = pRoot->m_pLeftNode->m_iRegCount + 1;
		else
			pRoot->m_iRegCount = fmax(pRoot->m_pLeftNode->m_iRegCount, pRoot->m_pRightNode->m_iRegCount);
	}
	else
	{
		pRoot->m_iRegCount = 1;
	}

	std::cout << pRoot->m_sValue << " ";
}