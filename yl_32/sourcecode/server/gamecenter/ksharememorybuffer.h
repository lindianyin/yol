/* -------------------------------------------------------------------------
//	文件名		：	ksharememorybuffer.h
//	创建者		：	luobaohang
//	创建时间	：	2010/4/30 14:37:42
//	功能描述	：	实现共享内存缓冲区，用于作为进程间通信的协议包缓冲，并且使用IMemBlock与Socket协议包统一
//
// -----------------------------------------------------------------------*/
#ifndef __KSHAREMEMORYBUFFER_H__
#define __KSHAREMEMORYBUFFER_H__
// -------------------------------------------------------------------------
#include "ksharememoryprovider.h"

class KShareMemBuffer
{
public:
	LPVOID GetData();
	UINT GetSize();
	VOID Release();
private:
	LPVOID m_pData;
	UINT m_uDataSize;
	INT m_nRef;
	KSHARE_MEM_PTR m_cBlock;
	friend class KShareMemPool;
	friend class KShareMemBufferAuto;
};

class KShareMemBufferAuto
{
public:
	LPVOID GetData();
	UINT GetSize();
	~KShareMemBufferAuto();
	KShareMemBufferAuto();
	KShareMemBufferAuto(const KShareMemBufferAuto& cObject);
protected:
	KShareMemBufferAuto(KShareMemBuffer* pBuf);
	friend class KShareMemPool;
	KShareMemBuffer* m_pBuf;
};

// -------------------------------------------------------------------------

#endif /* __KSHAREMEMORYBUFFER_H__ */
