

#include "CorePrivate.h"
#include "Core/QMemory.h"
#include <algorithm>
#include <new>
#include "Core/QLog.h"
#include "Core/QLock.h"
#include "Core/Interlocked.h"

const GUID IID_IMemBlock = 
{ 0x10453b40, 0xef1a, 0x4bad, { 0xa0, 0x8a, 0x5f, 0xab, 0x89, 0x70, 0xf7, 0xfe } };

const GUID IID_IMemBlock_Resizable = 
{ 0xa69eaea5, 0xc3c0, 0x4e11, { 0xba, 0x5b, 0xe8, 0x5e, 0xa1, 0xfb, 0x13, 0xf } };


#define _MEMORY_BUFFER_RESERVE_SIZE   8
#define _MEMORY_USER_DATA_OFFSET (sizeof(QMemBlock) + _MEMORY_BUFFER_RESERVE_SIZE)


class QMemBlock : public IMemBlock, IMemBlock_Resizable
{
public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject
        );
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);
    virtual void       *GetData()        { return m_pvData; } 
    virtual unsigned    GetSize()        { return m_uSize;  }
    virtual unsigned    GetReserveSize() { return _MEMORY_BUFFER_RESERVE_SIZE; }
    virtual int         ResetSize()      { m_uSize = m_uOrignSize; return true;  }
    virtual int         SetSmallerSize(unsigned uNewSize);


    QMemBlock(unsigned uSize, void *pvData):
    m_lRefCount(1), m_uOrignSize(uSize), m_uSize(uSize), m_pvData(pvData)
    {}

private:  
    ~QMemBlock() { }    //make sure class _Buffer only use for new

    volatile long   m_lRefCount;
    unsigned        m_uOrignSize;

    unsigned        m_uSize;
    void           *m_pvData;
};


HRESULT STDMETHODCALLTYPE QMemBlock::QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void **ppvObject
) 
{ 
    HRESULT hrResult = E_FAIL;

    assert(ppvObject);
    
    *ppvObject = NULL;

    if (riid == IID_IMemBlock_Resizable)
    {
        *ppvObject = (void *)((IMemBlock_Resizable *)this);
    }
    else if (riid == IID_IMemBlock)
    {
        *ppvObject = (void *)((IMemBlock *)this);
    }
    else if (riid == IID_IUnknown)
    {
        *ppvObject = (void *)((IMemBlock *)this);
    }
    else
    {
        COM_PROC_ERR_RET_ERR(E_NOINTERFACE);
    }

    ((IUnknown *)(*ppvObject))->AddRef();
            
    hrResult = S_OK;
EXIT0:
    return hrResult;
}

int QMemBlock::SetSmallerSize(unsigned uNewSize)
{
    if (uNewSize > m_uOrignSize)
        return false;

    m_uSize = uNewSize;
    return true;
}

ULONG STDMETHODCALLTYPE QMemBlock::AddRef(void)
{   
    return InterlockedIncrement((long *)&m_lRefCount);
};

ULONG STDMETHODCALLTYPE QMemBlock::Release(void)
{
    int nRetCode = false;
    long lRefNow = 0;

    lRefNow = InterlockedDecrement((long *)&m_lRefCount);
    if (lRefNow > 0)
        return lRefNow;

    if (lRefNow < 0)
    {
        ASSERT(!"Unexpected memory release!");
        QLogPrintf(LOG_ERR, "%s", "Bad use of memory block : Unexpected memory release!", this);
    }
    void  *pBuffer     = (void*)this;
    //this->~QMemBlock();
    free(pBuffer);

    return lRefNow;
}


IMemBlock *QCreateMemBlock(unsigned uSize)
{
    int         nRetCode    = false;
    unsigned    uBuffSize   = 0;
    void       *pvBuffer    = NULL;
    void       *pvData      = NULL;
    QMemBlock  *pBuffer     = NULL;

    ASSERT(uSize);
    uBuffSize = uSize + _MEMORY_USER_DATA_OFFSET;
    ASSERT(uBuffSize > _MEMORY_USER_DATA_OFFSET && "It seems that uSize is negative!");

    pvBuffer = malloc(uBuffSize);
    assert(pvBuffer != NULL);

    pvData = (void *)(((unsigned char *)pvBuffer) + _MEMORY_USER_DATA_OFFSET);

    pBuffer = new(pvBuffer)QMemBlock(uSize, pvData);  // placement operator new
    return pBuffer;
}

int QMemoryInit(void *pvContext)
{
    int nResult = false;

    nResult = true;
//EXIT0:
    return nResult;
}

int QMemoryUnInit(void *pvContext)
{
    int nResult = false;

    nResult = true;
//EXIT0:
    return nResult;
}

