
#pragma once
#include "Core/QLinkArray.h"
// -------------------------------------------------------------------------
// 分配从1到nMaxNum的ID
class QIdPool
{
public:
	QIdPool();
	virtual ~QIdPool();

public:
	// 初始化,nMaxNum为最大ID值
	VOID Init(INT nMaxNum);
	VOID Uninit();
	// 申请使用一个空闲id，失败返回0
	INT New();
	// 释放一个已用id，未申请或重复释放返回FALSE
	BOOL Delete(INT nId);
	// 获取最大ID
	INT GetMaxNum() { return m_nMaxNum; }
	// 获取已分配的ID数
	INT GetUsedCount() { return m_cUsedArray.GetCount(); }
	// 获取下一个已分配ID，若参数为0则获取第一个，已无下一个返回0
	INT GetNextUsedId(INT nLastId) { return m_cUsedArray.GetNext(nLastId); }
protected:
	INT* m_pnFreeId; // 空闲id数组
	INT m_nFreeCount;
	INT m_nMaxNum;
	QLinkArray m_cUsedArray; // 已分配的Id
};
