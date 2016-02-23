/* -------------------------------------------------------------------------
//	文件名		：	ksharememorybuffer.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/4/30 14:37:50
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ksharememorybuffer.h"
#include "ksharememorypool.h"
#include "ksharememoryprovider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

LPVOID KShareMemBuffer::GetData()
{
	return m_pData;
}

UINT KShareMemBuffer::GetSize()
{
	return m_uDataSize;
}
// -------------------------------------------------------------------------
KShareMemBufferAuto::KShareMemBufferAuto()
{
	m_pBuf = NULL;
}

KShareMemBufferAuto::KShareMemBufferAuto( const KShareMemBufferAuto& cObject )
{
	m_pBuf = cObject.m_pBuf;
	m_pBuf->m_nRef++;
}

KShareMemBufferAuto::KShareMemBufferAuto( KShareMemBuffer* pBuf ) : m_pBuf(pBuf)
{
	m_pBuf->m_nRef++;
}

KShareMemBufferAuto::~KShareMemBufferAuto()
{
	if (m_pBuf && --m_pBuf->m_nRef <= 0)
		KShareMemPool::Inst()->Free(m_pBuf);
}

LPVOID KShareMemBufferAuto::GetData()
{
	return m_pBuf ? m_pBuf->GetData() : NULL;
}

UINT KShareMemBufferAuto::GetSize()
{
	return m_pBuf ? m_pBuf->GetSize() : 0;
}
// -------------------------------------------------------------------------
