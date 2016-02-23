
#pragma once

#include <new>
#include <set>

#include "CommonDefine.h"
#include "Misc/MSUnknown.h"

#include "Core/QLog.h"
#include "Core/QLock.h"
#include "Core/Interlocked.h"

extern const GUID IID_IMemBlock;   // {10453B40-EF1A-4bad-A08A-5FAB8970F7FE}
extern const GUID IID_IMemBlock_Resizable; // {A69EAEA5-C3C0-4e11-BA5B-E85EA1FB130F}


struct IMemBlock : IUnknown
{
public:
    virtual void    *GetData() = 0;
    virtual unsigned GetSize() = 0;
    virtual unsigned GetReserveSize() = 0;
};

struct IMemBlock_Resizable : IUnknown
{
	virtual int ResetSize() = 0;
	virtual int SetSmallerSize(unsigned uNewSize) = 0;
};

int QMemoryInit(void *pvContext);
int QMemoryUnInit(void *pvContext);
IMemBlock* QCreateMemBlock(unsigned uSize);


//#pragma pack(1)
//typedef struct _QMemoryBlock
//{
//    QSLIST_ENTRY  m_Next;
//    unsigned        m_uSize;
//    unsigned char   m_Data[1];
//} QMemoryBlock;
//#pragma pack()
//
//typedef struct _QMEMORY_LIST_STRUCT
//{
//    unsigned uBlockMaxSize;
//    QSLIST_HEADER List;
//} QMEMORY_LIST_STRUCT;
//
//
//class QMemory_Debug
//{
//public:
//    int AddBlock(QMemoryBlock *pBlock);
//    int RemoveBlock(QMemoryBlock *pBlock);
//    int CheckLeak();
//
//private:
//    typedef std::set<QMemoryBlock *> QMemoryBlockDebugSet;
//
//    QMemoryBlockDebugSet m_MemoryBlocks;
//
//    QLock                 m_Lock;
//};
//
////////////////////////////////////////////////////////////////////////////
//// _Memory_Internal function set
////////////////////////////////////////////////////////////////////////////
//int _QMemory_Impl_Init(
//    unsigned uMemoryListArrayCount,   QMEMORY_LIST_STRUCT MemoryListArray[], 
//    unsigned uSizeArrayCount,         unsigned uSizeArray[], 
//    void    *pvContext
//);
//
//int _QMemory_Impl_UnInit(
//    unsigned uMemoryListArrayCount,   QMEMORY_LIST_STRUCT MemoryListArray[],
//    void *pvContext
//);
//
//QMemoryBlock *_QMemory_Impl_Alloc(
//    unsigned uMemoryListArrayCount,   QMEMORY_LIST_STRUCT MemoryListArray[],
//    unsigned uSize
//);
//
//int _QMemory_Impl_Free(
//    unsigned uMemoryListArrayCount,   QMEMORY_LIST_STRUCT MemoryListArray[],
//    QMemoryBlock *pBlock
//);
//
//
////////////////////////////////////////////////////////////////////////////
//template <unsigned _uSizeArrayCount, unsigned _uSizeArray[]>
//class QMemory
//{
//public:
//    void *Alloc(unsigned uSize);
//    int   Free(void *pvBlock);
//
//    QMemory();
//    ~QMemory();
//
//private:
//    volatile long   m_lAllocCount;
//    volatile long   m_lFreeCount;
//
//    QMEMORY_LIST_STRUCT   m_MemoryListArray[_uSizeArrayCount];
//#ifdef _DEBUG
//    QMemory_Debug         m_Memory_Debug;
//#endif
//};
//
//template <unsigned _uSizeArrayCount, unsigned _uSizeArray[]>
//QMemory<_uSizeArrayCount, _uSizeArray>::QMemory(void)
//{
//    int nRetCode = false;
//
//    ASSERT(0    != _uSizeArrayCount);
//    ASSERT(NULL != _uSizeArray);
//
//    m_lAllocCount = 0;
//    m_lFreeCount  = 0;
//
//    nRetCode = _QMemory_Impl_Init(
//        _uSizeArrayCount,   m_MemoryListArray, 
//        _uSizeArrayCount,   _uSizeArray, 
//        NULL
//    );
//    ASSERT(nRetCode);
//}
//
//template <unsigned _uSizeArrayCount, unsigned _uSizeArray[]>
//QMemory<_uSizeArrayCount, _uSizeArray>::~QMemory(void)
//{
//    int nRetCode = false;
//
//    nRetCode = _QMemory_Impl_UnInit(_uSizeArrayCount, m_MemoryListArray, NULL);
//    ASSERT(nRetCode);
//
//    #ifdef _DEBUG
//    nRetCode = m_Memory_Debug.CheckLeak();
//    ASSERT(nRetCode);
//    #endif
//    
//    if (m_lAllocCount != m_lFreeCount)
//    {
//        ASSERT(!"Memory system : memory leak detected!");
//        QLogPrintf(
//            LOG_ERR, 
//            "Bad use of memory system : memory leak detected, leak block [%d]", 
//            m_lAllocCount - m_lFreeCount
//        );
//    }
//}
//
//template <unsigned _uSizeArrayCount, unsigned _uSizeArray[]>
//void *QMemory<_uSizeArrayCount, _uSizeArray>::Alloc(unsigned uSize)
//{
//    void *pvResult  = NULL;
//    int nRetCode    = false;
//    QMemoryBlock *pBlock = NULL;
//
//    PROCESS_ERROR(uSize > 0);
//
//    pBlock = _QMemory_Impl_Alloc(_uSizeArrayCount, m_MemoryListArray, uSize);
//    PROCESS_ERROR(pBlock);
//
//    InterlockedIncrement((long *)&m_lAllocCount);
//
//    #ifdef _DEBUG
//    nRetCode = m_Memory_Debug.AddBlock(pBlock);
//    ASSERT(nRetCode);
//    #endif
//
//    pvResult = (void *)pBlock->m_Data;
//	
//EXIT0:
//    return pvResult;
//}
//
//template <unsigned _uSizeArrayCount, unsigned _uSizeArray[]>
//int QMemory<_uSizeArrayCount, _uSizeArray>::Free(void *pvBufer)
//{
//    int nResult  = false;
//    int nRetCode = false;
//    QMemoryBlock *pBlock = NULL;
//
//    PROCESS_ERROR(pvBufer);
//
//    pBlock = CONTAINING_RECORD(pvBufer, QMemoryBlock, m_Data);
//    ASSERT(pBlock);
//
//    nRetCode = _QMemory_Impl_Free(
//        _uSizeArrayCount, m_MemoryListArray,
//        pBlock
//    );
//    PROCESS_ERROR(nRetCode);
//
//    InterlockedIncrement((long *)&m_lFreeCount);
//
//    #ifdef _DEBUG
//    nRetCode = m_Memory_Debug.RemoveBlock(pBlock);
//    ASSERT(nRetCode);
//    #endif
//
//    nResult = true;
//EXIT0:
//    return nResult;
//}
//
