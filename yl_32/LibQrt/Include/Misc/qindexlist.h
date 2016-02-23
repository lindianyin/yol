
#pragma once

#include <map>
#include "Core/QLinkArray.h"

class QIndexList
{
public:
	QIndexList()						{ Release(); }
	~QIndexList()						{ Release(); }

	BOOL Init(INT nMax);
	VOID Release(VOID);
	BOOL Use(INT nIndex);
	BOOL Free(INT nIndex);
	INT  FindFree(VOID) CONST			{ return m_cFreeIdx.GetNext(0); }
	INT  FindNext(INT nIndex) CONST		{ return m_cUseIdx.GetNext(nIndex); }
	INT  GetCount(VOID)					{ return m_cUseIdx.GetCount(); }
	INT  GetMax(VOID) CONST				{ return m_nMax; }

protected:
	INT				m_nMax;				// 最大索引个数
	QLinkArray		m_cFreeIdx;			// 可用表
	QLinkArray		m_cUseIdx;			// 已用表
};
