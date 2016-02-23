
#ifdef GAME_SERVER

#include "stdafx.h"
#include "serverbase/protocol/GatewayRelayProtocol.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/kmoduleinterface.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "knetconnectplayer.h"
#include "ksocketinterface.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "kplayerbaseprocess.h"
#include "kmodulemanager.h"
#include "config/kplayerbasedbdef.h"
#include "onlinegameworld/koverloadextension.h"
#include "kplayerdbmanager.h"
#include "KRelayClient.h"
#include "onlinegamebase/kguidhelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KD_PLAYER_CONNECT_WAIT_TIME 20000 // 等待玩家连接最大时间 (毫秒)

// -------------------------------------------------------------------------
template<class T>
class KReleaseHelper
{
	T& m_pObject;
public:
	KReleaseHelper(T& pObject) : m_pObject(pObject) {}
	~KReleaseHelper()
	{
		SAFE_RELEASE(m_pObject);
	}
};

// -------------------------------------------------------------------------
KNetConnectPlayer g_cNetConnectPlayer;

KNetConnectPlayer::KNetConnectPlayer()
{
	m_pnConnId2PlayerIndex = m_pnPlayerIndex2ConnId = NULL;
	m_nMaxConnId = 0;

	ZeroStruct(m_uBytesRxBaseProtocol);
	ZeroStruct(m_uBytesRxProtocolFamily);
	ZeroStruct(m_uBytesTxBaseProtocol);
	ZeroStruct(m_uBytesTxProtocolFamily);
	ZeroStruct(m_uRxBaseProtocol);
	ZeroStruct(m_uRxProtocolFamily);
	ZeroStruct(m_uTxBaseProtocol);
	ZeroStruct(m_uTxProtocolFamily);
}

KNetConnectPlayer::~KNetConnectPlayer()
{
	Uninit();
}

BOOL KNetConnectPlayer::Init( INT nMaxConnectId )
{
	// id范围是1~nMaxConnectId，为用数组下标直接访问多分配1位
	QLogPrintf(LOG_DEBUG, "..nMaxConnectId=%d", nMaxConnectId);
	m_pnConnId2PlayerIndex = new INT[nMaxConnectId + 1];
	m_pnPlayerIndex2ConnId = new INT[nMaxConnectId + 1];
	for (INT i = 0; i < nMaxConnectId + 1; i++)
	{
		m_pnConnId2PlayerIndex[i] = 0;
		m_pnPlayerIndex2ConnId[i] = -1;
	}
	m_nMaxConnId = nMaxConnectId;
	return TRUE;
}

BOOL KNetConnectPlayer::ProcessEvent( INT nConnectId, INT nEventId, LPCVOID pData, INT nData )
{
	QCONFIRM_RET_FALSE(nConnectId > 0 && nConnectId <= m_nMaxConnId);
	switch (nEventId)
	{
	case emKSOCKET_EVENT_CREATE:
		{
			Q_Printl("..Player Connection [" << nConnectId << "] Created. Client [" << (LPCSTR)pData << ":" << nData << "]");
			m_pnConnId2PlayerIndex[nConnectId] = 0;
		}
		break;
	case emKSOCKET_EVENT_LOST:
		{
			QLogPrintf(LOG_DEBUG, "..Player Connection[%d] Lost", nConnectId);
		}
		break;
	case emKSOCKET_EVENT_CLOSE:
		{
			QLogPrintf(LOG_DEBUG, "..Close Player Connection [%d]", nConnectId);

			INT nPlayerIndex = m_pnConnId2PlayerIndex[nConnectId];

			if (nPlayerIndex > 0)
			{
				g_cNetConnectPlayer.OnPlayerDisconnect(nPlayerIndex);
				m_pnPlayerIndex2ConnId[nPlayerIndex] = -1;
			}
		}
		m_pnConnId2PlayerIndex[nConnectId] = 0;
		break;
	case emKSOCKET_EVENT_MASS:
		{
			QLogPrintf(LOG_DEBUG, "..Player Connection [%d] Sent Too Much Data!", nConnectId);

			INT nPlayerIndex = m_pnConnId2PlayerIndex[nConnectId];

			if (nPlayerIndex > 0)
			{
				g_cNetConnectPlayer.OnPlayerDisconnect(nPlayerIndex);
				m_pnPlayerIndex2ConnId[nPlayerIndex] = -1;
			}
		}
		m_pnConnId2PlayerIndex[nConnectId] = 0;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}

BOOL KNetConnectPlayer::ProcessPacket( INT nConnectId, LPCBYTE pData, UINT uDataLen )
{
	QCONFIRM_RET_FALSE(nConnectId > 0 && nConnectId <= m_nMaxConnId);
	QCONFIRM_RET_FALSE(pData);

	RxStat(pData, uDataLen);

	INT nPlayerIndex = m_pnConnId2PlayerIndex[nConnectId];
	if (nPlayerIndex == 0)
	{
		// Process Login
		KC2S_LOGIN *pLL = (KC2S_LOGIN* )pData;
		QCONFIRM_RET_FALSE(pLL->byProtocol == emKPLBASE_PTC_C2S_LOGIN && uDataLen == sizeof(KC2S_LOGIN));
		//Q_Printl("pLL->byProtocol="<<pLL->byProtocol <<" uDataLen=" <<uDataLen);
		
		DWORD dwClientIp = 0;
		USHORT uClientPort = 0;
		g_piSocketServer->GetRemoteAddress(nConnectId, dwClientIp, uClientPort);
		Q_Printl("[ClientLogin] from [" << inet_ntoa(*(struct in_addr*)&dwClientIp) << ":" << uClientPort <<
			"] ConnectionToken" << KGuidHelper::GuidToString(pLL->guid) << " pLL->dwTinyTerminal=" << pLL->dwTinyTerminal);

		// 玩家已连接上，未发Login协议前应为0
		QCONFIRM(m_pnConnId2PlayerIndex[nConnectId] == 0);

		QCONFIRM_RET_FALSE(pLL->byGameServerVersion <= KD_PROTOCOL_VERSION &&
			pLL->byGameServerVersion >= KD_PROTOCOL_VERSION_LOWER_LIMIT);

		return OnClientLogin2(pLL->dwRoleId, pLL->guid, nConnectId, pLL->nHidden,pLL->dwTinyTerminal);
	}
	QCONFIRM_RET_FALSE(uDataLen >= sizeof(BYTE) * 2);

	// Check Protocol Size
	INT nPtcSize = KPTC_SIZE::GetSizeC2S(*pData, *(pData + 1));
	if (nPtcSize > 0)
	{
		QCONFIRM_RET_FALSE((UINT)nPtcSize == uDataLen);
	}
	else // 变长
	{
		QCONFIRM_RET_FALSE(uDataLen >= sizeof(BYTE) * 3 && *(LPWORD)(pData + 2) == uDataLen);
	}

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	// Doing 协议过滤
	if ( KNSBASE_EXT::DoingProtocolFilter[pPlayer->GetMoveState()] &&
		! (*KNSBASE_EXT::DoingProtocolFilter[pPlayer->GetMoveState()])(*pPlayer, pData, uDataLen) )
	{
		QLogPrintf(LOG_ERR, "[%s] Doing为[%d]时禁止发送[PF %u, P %u] 协议被屏蔽",
			pPlayer->GetName(), pPlayer->GetMoveState(), LPCBYTE(pData)[0], LPCBYTE(pData)[1]);
		return TRUE;
	}

	// base协议处理
	if (*pData == 0)
		return g_cPlayerBaseProcess.ProcessProtocol(nPlayerIndex, pData, uDataLen);

	PLAYER_GAME_STATE eGameState = pPlayer->GetGameState();
	if (eGameState != gsPlaying)
	{
		KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;

		QLogPrintf(LOG_ERR, "Protocol[%u, %u] blocked, Player[%d][%s] GameState[%d] is not 'gsPlaying'",
			pHeader->byProtocolFamily, pHeader->byProtocol,
			pPlayer->m_dwId, pPlayer->GetName(),
			eGameState);
		
		// 其他状态不接受扩展协议
		return TRUE;
	}

	// 子模块协议处理
	IKModuleInterface* piModule = g_cModuleManager.GetModule(BYTE(*pData) - KD_MAX_BASE_PROTOCOL_HEADER);
	if (!piModule)
	{
		Q_Error("Unrecognized Protocol " << BYTE(*pData) << " " << BYTE(*(pData+ 1)) << " Size " << uDataLen);
		return FALSE;
	}
	piModule->ProcessProtocol(nPlayerIndex, pData, uDataLen);
	return TRUE;
}

BOOL KNetConnectPlayer::SendData( INT nPlayerIndex, LPCBYTE pData, UINT uDataLen )
{
	BOOL bResult 		= FALSE;
	INT  nConnectIndex 	= 0;
	BOOL bRet 			= FALSE;
	TxStat(pData, uDataLen);
	LOG_PROCESS_ERROR(nPlayerIndex > 0 && nPlayerIndex <= m_nMaxConnId);
	nConnectIndex = m_pnPlayerIndex2ConnId[nPlayerIndex];
	PROCESS_ERROR(nConnectIndex > 0);
	bRet = g_piSocketServer->Send(nConnectIndex, 0, pData, uDataLen);
	LOG_PROCESS_ERROR(bRet);
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KNetConnectPlayer::Activate()
{
	return TRUE;
}

INT KNetConnectPlayer::OnClientLogin2( DWORD dwRoleId, const GUID& rConnGuid, INT nConnectId, BOOL bHidden /*= FALSE*/,BOOL bTinyTerminal)
{
	INT nRetCode = FALSE;
	KPlayer* pPlayer = NULL;
	INT nPlayerIndex = 0;
	pPlayer = g_cPlayerMgr.GetById(dwRoleId);
	LOG_PROCESS_ERROR(pPlayer);
	LOG_PROCESS_ERROR(pPlayer->GetGameState() == gsWaitForConnect);
	LOG_PROCESS_ERROR(pPlayer->m_cPlayerServer.m_Guid == rConnGuid);
	pPlayer->SetTinyTerminal(bTinyTerminal);
	nPlayerIndex = pPlayer->GetPlayerIndex();
	
	m_pnConnId2PlayerIndex[nConnectId] = nPlayerIndex;
	m_pnPlayerIndex2ConnId[nPlayerIndex] = nConnectId;

	m_mpAccount2Player[pPlayer->m_cPlayerServer.m_strAccount] = nPlayerIndex;

	g_RelayClient.DoConfirmPlayerLoginRequest(dwRoleId);

	pPlayer->SetGameState(gsWaitForPermit);
	pPlayer->m_nTimer       = 0;
	
	nRetCode = TRUE;
EXIT0:
	if (!nRetCode)
	{
		QLogPrintf(LOG_INFO, "Invalid Client: %d\n", nConnectId);

		if (pPlayer)
			QLogPrintf(LOG_INFO, "Player %d state %d\n", dwRoleId, pPlayer->GetGameState());
	}
	return nRetCode;
}

BOOL KNetConnectPlayer::OnPlayerDisconnect( INT nPlayerIndex )
{
	INT nConnectId = m_pnPlayerIndex2ConnId[nPlayerIndex];

	if (nConnectId > 0)
		m_pnConnId2PlayerIndex[nConnectId] = 0;
	m_pnPlayerIndex2ConnId[nPlayerIndex] = -1;

	KPlayer* pPlayer = g_cPlayerMgr.GetByIndex(nPlayerIndex);
	QCONFIRM_RET_FALSE(pPlayer);

	//Q_Printl("OnPlayerDisconnect [" << pPlayer->GetId() << "][" << pPlayer->GetName() <<
	//	"] PlayerIdx[" << pPlayer->GetPlayerIndex() << "] ConnIdx[" << nConnectId << "] ...");

	// 通知coordinator玩家离开
	g_RelayClient.DoPlayerLeaveGS(pPlayer->GetId());

	m_mpAccount2Player.erase(pPlayer->m_cPlayerServer.GetAccount());

	pPlayer->m_cPlayerServer.LogoutServer();
	pPlayer->m_nTimer = 0;

	g_cPlayerMgr.RemoveByIndex(nPlayerIndex);
	return TRUE;
}

BOOL KNetConnectPlayer::KickOutPlayer( INT nPlayerIndex )
{
	INT nConnIdx = m_pnPlayerIndex2ConnId[nPlayerIndex];

	if (nConnIdx > 0)
		return g_piSocketServer->Disconnect(nConnIdx);

	// not attached
	return OnPlayerDisconnect(nPlayerIndex);
}

BOOL KNetConnectPlayer::Uninit()
{
	SAFE_DELETE_ARRAY(m_pnConnId2PlayerIndex);
	SAFE_DELETE_ARRAY(m_pnPlayerIndex2ConnId);

	m_mpAccount2Player.clear();
	return TRUE;
}

KPlayer* KNetConnectPlayer::GetPlayerByAccount( LPCSTR pszAccount )
{
	KPlayer* pPlayer = NULL;
	std::map<QString, INT>::iterator it = m_mpAccount2Player.find(pszAccount);
	if (it != m_mpAccount2Player.end())
	{
		pPlayer = g_cPlayerMgr.GetByIndex(it->second);
	}
	return pPlayer;
}

BOOL KNetConnectPlayer::KickByAccount( LPCSTR pszAccount )
{
	BOOL bResult = FALSE;
	KPlayer* pPlayer = NULL;

	LOG_PROCESS_ERROR(pszAccount);

	pPlayer = g_cNetConnectPlayer.GetPlayerByAccount(pszAccount);
	if (pPlayer)
		bResult = pPlayer->m_cPlayerServer.KickOut(emKICK_PLAYER_REASON_NETWORK);

EXIT0:
	return bResult;
}

VOID KNetConnectPlayer::RxStat( LPCBYTE pData, UINT uDataLen )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;

	m_uRxProtocolFamily[pHeader->byProtocolFamily]++;
	m_uBytesRxProtocolFamily[pHeader->byProtocolFamily] += uDataLen;
	if (pHeader->byProtocolFamily == 0)
	{
		m_uRxBaseProtocol[pHeader->byProtocol]++;
		m_uBytesRxBaseProtocol[pHeader->byProtocol] += uDataLen;
	}
}

VOID KNetConnectPlayer::TxStat( LPCBYTE pData, UINT uDataLen )
{
	KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;

	m_uTxProtocolFamily[pHeader->byProtocolFamily]++;
	m_uBytesTxProtocolFamily[pHeader->byProtocolFamily] += uDataLen;
	if (pHeader->byProtocolFamily == 0)
	{
		m_uTxBaseProtocol[pHeader->byProtocol]++;
		m_uBytesTxBaseProtocol[pHeader->byProtocol] += uDataLen;
	}
}

LPCSTR KNetConnectPlayer::GetProtocolStat(std::string& rstrBuffer)
{
	// dump protocol statics
	std::stringstream strmBuffer;
	strmBuffer << "Base Protocol RxTimes/RxBytes: \n";
	for (UINT i = 0; i < countof(m_uRxBaseProtocol); ++i)
	{
		if (m_uBytesRxBaseProtocol[i] || m_uRxBaseProtocol[i])
		{
			strmBuffer << "[" << i << "] " << m_uRxBaseProtocol[i] << "/" << m_uBytesRxBaseProtocol[i] << " ";
		}
	}
	strmBuffer << "\nBase Protocol TxTimes/TxBytes: \n";
	for (UINT i = 0; i < countof(m_uTxBaseProtocol); ++i)
	{
		if (m_uBytesTxBaseProtocol[i] || m_uTxBaseProtocol[i])
		{
			strmBuffer << "[" << i << "] " << m_uTxBaseProtocol[i] << "/" << m_uBytesTxBaseProtocol[i] << " ";
		}
	}
	strmBuffer << "\nProtocol Family RxTimes/RxBytes: \n";
	for (UINT i = 0; i < countof(m_uRxProtocolFamily); ++i)
	{
		if (m_uBytesRxProtocolFamily[i] || m_uRxProtocolFamily[i])
		{
			strmBuffer << "[" << i << "] " << m_uRxProtocolFamily[i] << "/" << m_uBytesRxProtocolFamily[i] << " ";
		}
	}
	strmBuffer << "\nProtocol Family TxTimes/TxBytes: \n";
	for (UINT i = 0; i < countof(m_uTxProtocolFamily); ++i)
	{
		if (m_uBytesTxProtocolFamily[i] || m_uTxProtocolFamily[i])
		{
			strmBuffer << "[" << i << "] " << m_uTxProtocolFamily[i] << "/" << m_uBytesTxProtocolFamily[i] << " ";
		}
	}
	rstrBuffer = strmBuffer.str();
	return rstrBuffer.c_str();
}

INT KNetConnectPlayer::GetConnIndex( INT nPlayerIndex )
{
	QCONFIRM_RET_FALSE(nPlayerIndex > 0 && nPlayerIndex <= m_nMaxConnId);
	return m_pnPlayerIndex2ConnId[nPlayerIndex];
}


#endif
