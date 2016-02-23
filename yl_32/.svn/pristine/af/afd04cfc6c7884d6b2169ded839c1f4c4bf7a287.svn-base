/* -------------------------------------------------------------------------
//	文件名		：	ksocketserver.cpp
//	创建者		：	luobaohang
//	创建时间	：	2010/6/4 9:39:23
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "ksocketserver.h"
#include "kgcinterface.h"
#include "onlinegameworld/kprotocol.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static std::map<INT, KCenServerConnect* >::iterator _s_itCurr; // 临时解决循环中更改m_mpConnect
static INT _s_nCurrConn = 0;
// -------------------------------------------------------------------------
KSocketServer* KSocketServer::Inst()
{
	static KSocketServer sInst;
	return &sInst;
}

BOOL KSocketServer::Init( LPCSTR szListenIp, INT nPort )
{
	if (!KG_CenServer::Init(szListenIp, nPort))
		return FALSE;
	return TRUE;
}

VOID KSocketServer::UnInit()
{
	KG_CenServer::ShutDown();
}

VOID KSocketServer::Breath()
{
	if ((++m_nLoopCount & 15) == 0)
		CheckNewConnect();
	std::map<INT, KCenServerConnect* >::iterator it = m_mpConnect.begin();
	while (it != m_mpConnect.end())
	{
		_s_itCurr = it++;
		_s_nCurrConn = _s_itCurr->first;
		KCenServerConnect* pConnect = _s_itCurr->second;
		pConnect->ProcessPacket(this, &KSocketServer::ProcessPacket, _s_nCurrConn);
		//检查连接：
		if (!pConnect->IsValid())
		{
			struct in_addr RemoteAddr = { 0 };
			USHORT usRemotePort = 0;
			pConnect->GetSocketStream()->GetRemoteAddress(&RemoteAddr, &usRemotePort);
			Q_Printl("Connection Lost:" << inet_ntoa(RemoteAddr) << " ID = " << _s_itCurr->first);
			//NotifyServerDisconnect(curr->first);
			INT nModuleCount = countof(m_apiProcessor);
			for (INT i = 0; i < nModuleCount; ++i)
			{
				if (m_apiProcessor[i])
					m_apiProcessor[i]->OnConnectClose(_s_itCurr->first);
			}
			pConnect->Destroy();
			m_mpConnect.erase(_s_itCurr);
			continue;
		}
	};
}

INT KSocketServer::ProcessPacket( LPBYTE pData, UINT uSize, INT nConnectId ) 
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE *)pData;
	QCONFIRM_RET_FALSE(pData && uSize >= sizeof(*pHeader));
	QCONFIRM_RET_FALSE(m_apiProcessor[pHeader->byProtocolFamily]); // 找不到对应模块
	return m_apiProcessor[pHeader->byProtocolFamily]->ProcessData(nConnectId, pData, uSize);
}

BOOL KSocketServer::SendPacket( INT nConnectId, IMemBlock* piBuffer )
{
	if (nConnectId == -1)
	{
		std::map<INT, KCenServerConnect* >::iterator it;
		for (it = m_mpConnect.begin(); it != m_mpConnect.end(); ++it)
		{
			it->second->SendPacket(piBuffer);
		}
		return TRUE;
	}
	KCenServerConnect* pConnect = GetConnect(nConnectId);
	if (!pConnect)
	{
		Q_Error("Connection Not Exist: " << nConnectId);
		return FALSE;
	}
	return pConnect->SendPacket(piBuffer);
}

BOOL KSocketServer::Send2RandomPacket(IMemBlock* piBuffer)
{
	std::map<INT, KCenServerConnect* >::iterator it;
	for (it = m_mpConnect.begin(); it != m_mpConnect.end(); ++it)
	{
		it->second->SendPacket(piBuffer);
		break;
	}
	return TRUE;
}

KSocketServer::KSocketServer()
{
	m_nLoopCount = 0;
	ZeroStruct(m_apiProcessor);
}

BOOL KSocketServer::RegisterGsProcessor( BYTE byModuleId, IKProcessServer* piProcessor )
{
	// 重复注册？
	QCONFIRM_RET_FALSE(m_apiProcessor[byModuleId] == NULL);
	m_apiProcessor[byModuleId] = piProcessor;
	return TRUE;
}

VOID KSocketServer::OnConnectCreated(INT nID, KCenServerConnect *pConnect)
{
	INT nModuleCount = countof(m_apiProcessor);
	for (INT i = 0; i < nModuleCount; ++i)
	{
		if (m_apiProcessor[i])
			m_apiProcessor[i]->OnConnected(nID);
	}
}

BOOL KSocketServer::ReAssignServerId( INT nConnectId, INT nServerId )
{
	QCONFIRM_RET_FALSE(m_mpConnect.find(nConnectId) != m_mpConnect.end());
	QCONFIRM_RET_FALSE(m_mpConnect.find(nServerId) == m_mpConnect.end() && "ServerId Conflict");
	m_mpConnect[nServerId] = m_mpConnect[nConnectId];
	m_mpConnect.erase(nConnectId);
	_s_itCurr = m_mpConnect.find(nServerId);
	_s_nCurrConn = nServerId;
	QLogPrintf(LOG_DEBUG, "ReAssignServerId %d* -> %d", nConnectId, nServerId);
	return TRUE;
}

// -------------------------------------------------------------------------
