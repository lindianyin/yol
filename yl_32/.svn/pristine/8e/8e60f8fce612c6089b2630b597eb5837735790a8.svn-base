
#pragma once

#include "QNode.h"
//---------------------------------------------------------------------------

class QList
{
public:
	QNode	m_ListHead;		// 头节点（不是第一个节点）
	QNode	m_ListTail;		// 尾节点（不是最后的节点）
public:
	QList();
	QNode*	GetHead();		// 取得第一个节点
	QNode*	GetTail();		// 取得最后一个节点
	void AddHead(QNode *pNode); // 在最前面增加一个节点
	void AddTail(QNode *pNode); // 在最后面增加一个节点
	QNode*	RemoveHead();	// 删除第一个节点
	QNode*	RemoveTail();	// 删除最后一个节点
	int		IsEmpty();		// 是否是个空的链表
	long	GetNodeCount();
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// 函数:	KList
// 功能:	构造
inline QList::QList()
{
	m_ListHead.m_pNext = &m_ListTail;
	m_ListTail.m_pPrev = &m_ListHead;
}

//-------------------------------------------------------------------------
// 函数:	IsEmpty
// 功能:	是否为空
// 返回:	int
inline int QList::IsEmpty()
{
	return (m_ListHead.GetNext() == NULL);
}
//-------------------------------------------------------------------------
// 函数:	GetHead
// 功能:	取得链表的头
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QList::GetHead()
{
	return m_ListHead.GetNext();
}
//-------------------------------------------------------------------------
// 函数:	GetTail
// 功能:	取得链表的尾
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QList::GetTail()
{
	return m_ListTail.GetPrev();
}
//-------------------------------------------------------------------------
// 函数:	AddHead
// 功能:	在头部增加一个节点
// 参数:	QNode*
// 返回:	int
//---------------------------------------------------------------------------
inline void QList::AddHead(QNode *pNode)
{
	m_ListHead.InsertAfter(pNode);
}
//-------------------------------------------------------------------------
// 函数:	AddTail
// 功能:	在末尾增加一个节点
// 参数:	QNode*
//---------------------------------------------------------------------------
inline void QList::AddTail(QNode *pNode)
{
	m_ListTail.InsertBefore(pNode);
}
//-------------------------------------------------------------------------
// 函数:	RemoveHead
// 功能:	删除第一个节点
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QList::RemoveHead()
{
	QNode* pNode = m_ListHead.GetNext();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// 函数:	RemoveTail
// 功能:	删除最后一个节点
// 返回:	QNode*
//---------------------------------------------------------------------------
inline QNode* QList::RemoveTail()
{
	QNode* pNode = m_ListTail.GetPrev();
	if (pNode)
		pNode->Remove();
	return pNode;
}
//-------------------------------------------------------------------------
// 函数:	GetNodeCount
// 功能:	取得节点个数
// 返回:	long
//---------------------------------------------------------------------------
inline long QList::GetNodeCount()
{
	long nNode = 0;
	QNode* pNode = GetHead();
	while (pNode)
	{
		pNode = pNode->GetNext();
		nNode++;
	}
	return nNode;
}
//-------------------------------------------------------------------------

