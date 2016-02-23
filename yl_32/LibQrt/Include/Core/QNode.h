
#pragma once

#include "Runtime/Debug.h"

class QNode
{
public:
	QNode* m_pNext;
	QNode* m_pPrev;

public:
	QNode();
	virtual ~QNode(){};
	QNode*	GetNext();
	QNode*	GetPrev();
	void	InsertBefore(QNode* pNode);
	void	InsertAfter(QNode* pNode);
	void	Remove();
	int		IsLinked();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 函数:	Knode
// 功能:	构造
//---------------------------------------------------------------------------
inline QNode::QNode()
{
	m_pNext = NULL;
	m_pPrev = NULL;
}
//---------------------------------------------------------------------------
// 函数:	GetNext
// 功能:	下一个结点
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QNode::GetNext()
{
	if (m_pNext && m_pNext->m_pNext)
		return m_pNext;
	return NULL;
}
//---------------------------------------------------------------------------
// 函数:	GetPrev
// 功能:	前一个结点
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QNode::GetPrev()
{
	if (m_pPrev && m_pPrev->m_pPrev)
		return m_pPrev;
	return NULL;
}
//----------------------------------------------------------------------------
// 函数:	InsertBefore
// 功能:	在前面插入一个结点
// 参数:	QNode*
//---------------------------------------------------------------------------
inline void QNode::InsertBefore(QNode *pNode)
{
	QASSERT(m_pPrev);
	if (!pNode || !m_pPrev)
		return;

	if(pNode->m_pNext || pNode->m_pPrev)
	{
		_ASSERT(0);
	}

	pNode->m_pPrev = m_pPrev;
	pNode->m_pNext = this;
	m_pPrev->m_pNext = pNode;
	m_pPrev = pNode;
}
//---------------------------------------------------------------------------
// 函数:	InsertAfter
// 功能:	在后面插入一个结点
// 参数:	QNode*
//---------------------------------------------------------------------------
inline void QNode::InsertAfter(QNode *pNode)
{
	QASSERT(m_pNext);
	if (!pNode || !m_pNext)
		return;
	pNode->m_pPrev = this;
	pNode->m_pNext = m_pNext;
	m_pNext->m_pPrev = pNode;
	m_pNext = pNode;
}
//---------------------------------------------------------------------------
// 函数:	Remove the node
// 功能:	删除这个结点
// 返回:	QNode*
//---------------------------------------------------------------------------
inline void QNode::Remove()
{
	QASSERT(m_pPrev);
	QASSERT(m_pNext);
	if (m_pPrev)
		m_pPrev->m_pNext = m_pNext;
	if (m_pNext)
		m_pNext->m_pPrev = m_pPrev;
	m_pPrev = NULL;
	m_pNext = NULL;
}
//---------------------------------------------------------------------------
// 函数:	IsLinked
// 功能:	这个节点是否链接
// 返回:	bool
//---------------------------------------------------------------------------
inline int QNode::IsLinked()
{
	return (m_pPrev && m_pNext);
}
//--------------------------------------------------------------------------------

