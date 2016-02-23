/* -------------------------------------------------------------------------
//	文件名		：	ksharememorypool.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/8/4 9:57:22
//	功能描述	：	进程内共享内存池，与跨进程共享内存池结合作为共享内存管理策略
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ksharememorypool.h"
#include "ksharememoryprovider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// -------------------------------------------------------------------------
KShareMemPool::KShareMemPool()
{
	ZeroStruct(m_aBufLstFirst);
	ZeroStruct(m_aBufLstSecond);
}

KShareMemPool* KShareMemPool::Inst()
{
	static KShareMemPool _sInst;
	return &_sInst;	
}

BOOL KShareMemPool::Init()
{
	m_cSahreMemBufferAlloctor.Init(emKPOOL_BASE_SIZE, emKPOOL_INC_SIZE);
	return TRUE;
}

KShareMemBuffer* KShareMemPool::Alloc(UINT uSize)
{
	BYTE byPageType = KShareMemProvider::Inst()->GetPageTypeBySize(uSize);

	KSHARE_MEM_PTR ptr = Alloc(byPageType);

	KShareMemBuffer* pBuffer = m_cSahreMemBufferAlloctor.New();
	pBuffer->m_cBlock = ptr;
	pBuffer->m_nRef = 0; // 非Auto都设为0
	pBuffer->m_pData = KShareMemProvider::Inst()->SharePtr2Pointer(ptr);
	pBuffer->m_uDataSize = uSize;
	
	return pBuffer;
}

KSHARE_MEM_PTR KShareMemPool::Alloc( BYTE byPageType )
{
	KSHARE_MEM_PTR ptr = { 0 };
	QCONFIRM_RET(byPageType > 0, ptr);
	KSHARE_MEM_BLOCK_LIST* pMemBlock = &m_aBufLstFirst[byPageType];
	if (pMemBlock->nCursor == 0) // 第一个空往第二个取
		pMemBlock = &m_aBufLstSecond[byPageType];
	if (pMemBlock->nCursor == 0) // 第二个也为空
	{
		QCONFIRM_RET(KShareMemProvider::Inst()->BuyBlockList(byPageType, pMemBlock->lptrFreeBlockList), ptr);
		pMemBlock->nCursor = pMemBlock->nTail = KD_SHARE_MEM_PAGE_SIZE / s_auShareMemBlock[byPageType];
	}
	KSHARE_MEM_PTR* pptr = (KSHARE_MEM_PTR* )KShareMemProvider::Inst()->
		SharePtr2Pointer(pMemBlock->lptrFreeBlockList);
	// 获取一个空闲块
	return pptr[--pMemBlock->nCursor];
}
BOOL KShareMemPool::Free( KShareMemBuffer* pBuffer )
{
	QCONFIRM_RET_FALSE(pBuffer && pBuffer->m_nRef == 0);
	Free(pBuffer->m_cBlock);
	m_cSahreMemBufferAlloctor.Free(pBuffer);
	return TRUE;
}

BOOL KShareMemPool::Free( KSHARE_MEM_PTR ptr )
{
	QCONFIRM_RET_FALSE(ptr.wPageId > 0);
	BYTE byPageType = KD_PAGE_TYPE_FROM_ID(ptr.wPageId);

	KSHARE_MEM_BLOCK_LIST* pMemBlockList = &m_aBufLstFirst[byPageType];
	if (pMemBlockList->nCursor >= pMemBlockList->nTail)
		pMemBlockList = &m_aBufLstSecond[byPageType];

	// 如果第二个也满了，释放一个FreeBlockList
	if (pMemBlockList->nCursor >= pMemBlockList->nTail)
	{
		KShareMemProvider::Inst()->FreeBlockList(byPageType, pMemBlockList->lptrFreeBlockList);
		pMemBlockList->nCursor = 0;
	}

	// 如果为空，申请空间
	if (pMemBlockList->lptrFreeBlockList.dwOffsetBegin == 0)
		KShareMemProvider::Inst()->FreeBlockList(byPageType, pMemBlockList->lptrFreeBlockList);

	KSHARE_MEM_PTR* pptr = (KSHARE_MEM_PTR* )KShareMemProvider::Inst()->
		SharePtr2Pointer(pMemBlockList->lptrFreeBlockList);
	
	pptr[pMemBlockList->nCursor++] = ptr;
	return TRUE;
}

KShareMemBufferAuto KShareMemPool::AllocAuto(UINT uSize)
{
	return KShareMemBufferAuto(Alloc(uSize));
}

// -------------------------------------------------------------------------
