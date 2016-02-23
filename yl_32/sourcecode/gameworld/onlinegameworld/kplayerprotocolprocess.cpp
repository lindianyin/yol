
#include "stdafx.h"
#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegameworld/kprotocolsize.h"
#include "ksocketinterface.h"
#include "onlinegameworld/koverloadextension.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// -------------------------------------------------------------------------
KPlayerProtocolProcessor _gcPlayerBaseProcessor(0);
KPlayerProtocolProcessor* g_pBaseSender = &_gcPlayerBaseProcessor;

#ifdef GAME_SERVER
#include "knetconnectplayer.h"

BOOL KPlayerProtocolProcessor::SendData( INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData )
{
	BOOL 	bResult    	= FALSE;
	INT 	nPtcSize 	= 0;
	INT 	nRet     	= 0;
	nPtcSize  = KPTC_SIZE::GetSizeS2C(m_byPtcFamily, rPtcData.byProtocol);
	LOG_PROCESS_ERROR(nPtcSize > 0);
	rPtcData.byProtocolFamily = m_byPtcFamily;
	nRet = g_cNetConnectPlayer.SendData(nPlayerIndex,(LPCBYTE)&rPtcData, nPtcSize);
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerProtocolProcessor::SendLenData( INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize )
{
	BOOL 	bResult    	= FALSE;
	INT 	nPtcSize 	= 0;
	INT     nRet        = 0;
	nPtcSize 	= KPTC_SIZE::GetSizeS2C(m_byPtcFamily, rPtcData.byProtocol);
	LOG_PROCESS_ERROR(nPtcSize == -1);
	//QLogPrintf(LOG_INFO,"..nPtcSize=%d,rPtcData.byProtocol=%d",nPtcSize,rPtcData.byProtocol);
	rPtcData.byProtocolFamily = m_byPtcFamily;
	rPtcData.wDataLen = uDataSize;
	nRet = g_cNetConnectPlayer.SendData(nPlayerIndex, (LPCBYTE)&rPtcData, uDataSize);
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPlayerProtocolProcessor::SendUnknownData( INT nPlayerIndex, LPSTR pData, UINT uDataSize )
{
	INT nPtcSize = KPTC_SIZE::GetSizeS2C(m_byPtcFamily, *(pData + 1));
	if (nPtcSize > 0)
	{
		QCONFIRM_RET_FALSE(nPtcSize == (INT)uDataSize);
		KPTC_HEADER_BASE& rPtcData = *(KPTC_HEADER_BASE* )pData;
		rPtcData.byProtocolFamily = m_byPtcFamily;
		return g_cNetConnectPlayer.SendData(nPlayerIndex,(LPCBYTE)&rPtcData, nPtcSize);
	}
	// else
	KLENPTC_HEADER_BASE& rPtcData = *(KLENPTC_HEADER_BASE* )pData;
	rPtcData.byProtocolFamily = m_byPtcFamily;
	rPtcData.wDataLen = uDataSize;
	return g_cNetConnectPlayer.SendData(nPlayerIndex, (LPCBYTE)&rPtcData, uDataSize);
}
#else
extern IKSocketClientInterface* g_piSocketClient;

BOOL KPlayerProtocolProcessor::SendData(KPTC_HEADER_BASE& rPtcData)
{
	INT nPtcSize = KPTC_SIZE::GetSizeC2S(m_byPtcFamily, rPtcData.byProtocol);
	QCONFIRM_RET_FALSE(nPtcSize > 0);
	rPtcData.byProtocolFamily = m_byPtcFamily;

	KPlayer* pClientPlayer = g_cOrpgWorld.GetClientPlayer();
	QCONFIRM_RET_FALSE(pClientPlayer);
	// 协议族过滤
	if( KNSBASE_EXT::FiltModule(*pClientPlayer, (LPBYTE)&rPtcData, nPtcSize) )
	{
		return TRUE;
	}
	// State 协议过滤
	if (KNSBASE_EXT::StateProtocolFilter[pClientPlayer->GetPlayState()])
	{
		if (!(*KNSBASE_EXT::StateProtocolFilter[pClientPlayer->GetPlayState()])(*pClientPlayer, (LPBYTE)&rPtcData, nPtcSize))
			return FALSE;
	}
	// Doing 协议过滤
	if (KNSBASE_EXT::DoingProtocolFilter[pClientPlayer->GetDoing()])
	{
		if (!(*KNSBASE_EXT::DoingProtocolFilter[pClientPlayer->GetDoing()])(*pClientPlayer, (LPBYTE)&rPtcData, nPtcSize))
			return FALSE;
	}
	return g_piSocketClient->Send((LPBYTE)&rPtcData, nPtcSize);
}

BOOL KPlayerProtocolProcessor::SendLenData(KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize)
{
	INT nPtcSize = KPTC_SIZE::GetSizeC2S(m_byPtcFamily, rPtcData.byProtocol);
	QCONFIRM_RET_FALSE(nPtcSize == -1);
	rPtcData.byProtocolFamily = m_byPtcFamily;
	rPtcData.wDataLen = uDataSize;

	KPlayer* pClientPlayer = g_cOrpgWorld.GetClientPlayer();
	QCONFIRM_RET_FALSE(pClientPlayer);
	// 协议族过滤
	if( KNSBASE_EXT::FiltModule(*pClientPlayer, (LPBYTE)&rPtcData, uDataSize) )
	{
		return TRUE;
	}
	// State 协议过滤
	if (KNSBASE_EXT::StateProtocolFilter[pClientPlayer->GetPlayState()])
	{
		if (!(*KNSBASE_EXT::StateProtocolFilter[pClientPlayer->GetPlayState()])(*pClientPlayer, (LPBYTE)&rPtcData, uDataSize))
			return FALSE;
	}
	// Doing 协议过滤
	if (KNSBASE_EXT::DoingProtocolFilter[pClientPlayer->GetDoing()])
	{
		if (!(*KNSBASE_EXT::DoingProtocolFilter[pClientPlayer->GetDoing()])(*pClientPlayer, (LPBYTE)&rPtcData, uDataSize))
			return FALSE;
	}
	return g_piSocketClient->Send((LPCBYTE)&rPtcData, uDataSize);
}
#endif // #ifdef GAME_SERVER
