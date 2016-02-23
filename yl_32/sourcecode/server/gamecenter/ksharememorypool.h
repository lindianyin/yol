/* -------------------------------------------------------------------------
//	文件名		：	ksharememorypool.h
//	创建者		：	luobaohang
//	创建时间	：	2010/8/4 9:57:11
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KSHAREMEMORYPOOL_H__
#define __KSHAREMEMORYPOOL_H__

#include "ksharememorybuffer.h"
#include "Misc/objectpool.h"
//KSHARE_MEM_BLOCK
// -------------------------------------------------------------------------
class KShareMemPool
{
	struct KSHARE_MEM_BLOCK_LIST
	{
		KSHARE_MEM_LONG_PTR lptrFreeBlockList;	// 空闲块表
		INT nCursor;	// 当前游标
		INT nTail; // 末端（最大块数）
	};
	enum
	{
		emKPOOL_BASE_SIZE = 1024,
		emKPOOL_INC_SIZE = 1024,
	};
public:
	static KShareMemPool* Inst();
	// Called by Loader
	BOOL Init();
	// 申请一个内存块，需手动释放
	KShareMemBuffer* Alloc(UINT uSize);
	// 释放
	BOOL Free(KShareMemBuffer* pBuffer);
	// 申请内存块，返回SharePtr
	KSHARE_MEM_PTR Alloc(BYTE byPageType);
	// 释放一个SharePtr
	BOOL Free(KSHARE_MEM_PTR ptr);
	// 申请一个内存块，智能指针自动释放
	KShareMemBufferAuto AllocAuto(UINT uSize);
protected:
	KShareMemPool();
protected:
	// BlockList，Vector下标为BlockType
	// 使用双链表缓冲，两个链表都使用满则释放掉一个
	KSHARE_MEM_BLOCK_LIST m_aBufLstFirst[countof(s_auShareMemBlock)];
	KSHARE_MEM_BLOCK_LIST m_aBufLstSecond[countof(s_auShareMemBlock)];
	// KShareMemBuffer对象分配
	KIncObjPool<KShareMemBuffer> m_cSahreMemBufferAlloctor;
};

// -------------------------------------------------------------------------

#endif /* __KSHAREMEMORYPOOL_H__ */
