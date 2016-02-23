/* -------------------------------------------------------------------------
//	文件名		：	kconnectlocal.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/9 16:03:18
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kloader.h"
#include "kconnectlocal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// LocalConnect的协议缓冲区大小，以4M为增量
#define KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE (4 * 1024 * 1024)

// -------------------------------------------------------------------------

KConnectLocal::KConnectLocal() : KConnectBase(emKGC_CONNECT_LOCAL)
{
}

BOOL KConnectLocal::Send(BYTE byAcceptorModuleId, BYTE byConnectId, LPVOID pvData, UINT uDataSize)
{
	LPBYTE pbWritepos = KConnnectLocalMgr::Inst()->NewPackage(uDataSize + 2);
	// 写入InterfaceType和ConnectorModule作包头
	*pbWritepos++ = byConnectId;
	*pbWritepos++ = byAcceptorModuleId;
	memcpy(pbWritepos, pvData, uDataSize);
	return TRUE;
}

// -------------------------------------------------------------------------
KConnnectLocalMgr::KConnnectLocalMgr()
{
	NewPackageBufBlock();
	m_pCurBlockWritepos = m_vecPackageBuf.back();
	m_uCurBlockRestSize = KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE;
	m_nCurblock = 0;
}

KConnnectLocalMgr::~KConnnectLocalMgr()
{
	for (UINT i = 0; i < m_vecPackageBuf.size(); i++)
	{
		delete[] m_vecPackageBuf[i];
	}
}

KConnectLocal* KConnnectLocalMgr::NewConnect()
{
	m_lstConnect.push_back(KConnectLocal());
	return &m_lstConnect.back();
}

BOOL KConnnectLocalMgr::NewPackageBufBlock()
{
	LPBYTE pBlock = new BYTE [KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE];
	QCONFIRM_RET_FALSE(pBlock);
	m_vecPackageBuf.push_back(pBlock);
	return TRUE;
}

LPBYTE KConnnectLocalMgr::NewPackage(UINT uDataLen)
{
	QCONFIRM_RET_NULL(m_pCurBlockWritepos && uDataLen <= 65535);
	LPBYTE pBuf = NULL;
	// 如现有缓冲区大小不足
	if (m_uCurBlockRestSize < uDataLen + sizeof(WORD))
	{
		// 如果还有一个WORD大小，填0
		if (m_uCurBlockRestSize >= sizeof(WORD))
			*(LPWORD)m_pCurBlockWritepos = 0;
		if (++m_nCurblock >= (INT)m_vecPackageBuf.size())
			NewPackageBufBlock(); // 开辟下一个Block
		m_pCurBlockWritepos = m_vecPackageBuf.back();
		m_uCurBlockRestSize = KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE;

	}
	*(LPWORD)m_pCurBlockWritepos = uDataLen;
	m_pCurBlockWritepos += sizeof(WORD) + uDataLen;
	m_uCurBlockRestSize -= sizeof(WORD) + uDataLen;
	return m_pCurBlockWritepos - uDataLen;
}

BOOL KConnnectLocalMgr::ProcessPackage()
{
	for (INT i = 0; i <= m_nCurblock; i++)
	{
		LPBYTE pBuf = m_vecPackageBuf[i];
		LPBYTE pEndBuf = pBuf + KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE;
		if (i == m_nCurblock)	// 如果是最后一个Block，以m_pCurBlockWritepos为中止
			pEndBuf = m_pCurBlockWritepos;
		while(pEndBuf - pBuf >= sizeof(WORD))	// sizeof(WORD)是填充下一个包大小的位置
		{
			WORD wSize = *(LPWORD)pBuf;
			if (wSize <= 2)
				break;	// 这个块的后续无效了跳出到下一个Block
			pBuf += sizeof(WORD);
			BYTE byConnectId = *pBuf++;
			BYTE byInterfaceTypeId = *pBuf++;
			wSize -= 2;	// 两个字节是填ConnectorModuleId和InterfaceTypeId
			KLoader::Inst()->ProcessInterfaceData(byConnectId, byInterfaceTypeId, pBuf, wSize);
			pBuf += wSize;
		}
	}
	m_pCurBlockWritepos = m_vecPackageBuf.front();
	m_uCurBlockRestSize = KD_LOCAL_CONN_BUF_MEM_BLOCK_SIZE;
	m_nCurblock = 0;
	return TRUE;
}

KConnnectLocalMgr* KConnnectLocalMgr::Inst()
{
	static KConnnectLocalMgr sMgr;
	return &sMgr;
}
// -------------------------------------------------------------------------
