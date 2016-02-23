
#pragma once

#ifdef GAME_SERVER

#include "ksocketinterface.h" 
#include "Runtime/qstring.h"

class KPlayer;
// -------------------------------------------------------------------------
class KNetConnectPlayer : public IKSocketEventProcessor
{
	friend class KPlayerProtocolProcessor;
public:
	KNetConnectPlayer();
	~KNetConnectPlayer();

	virtual BOOL Init(INT nMaxConnectId);
	virtual BOOL ProcessEvent(INT nConnectId, INT nEventId, LPCVOID pData, INT nData);
	virtual BOOL ProcessPacket(INT nConnectId, LPCBYTE pData, UINT uDataLen);

	// Client连上GameServer
	INT OnClientLogin2( DWORD dwRoleId, const GUID& rConnGuid, INT nConnectId, BOOL bHidden /*= FALSE*/,BOOL bTinyTerminal);
	// 主动踢掉玩家
	BOOL KickOutPlayer(INT nPlayerIndex);
	// 玩家断线或被SocketServer断开
	BOOL OnPlayerDisconnect(INT nPlayerIndex);
	// 每帧处理
	virtual BOOL Activate();
	BOOL Uninit();

	LPCSTR GetProtocolStat(std::string& rstrBuffer);

	KPlayer* GetPlayerByAccount(LPCSTR pszAccount);
	BOOL KickByAccount(LPCSTR pszAccount);

	INT GetConnIndex(INT nPlayerIndex);
private:
	BOOL SendData(INT nPlayerIndex, LPCBYTE pData, UINT uDataLen);
	VOID RxStat(LPCBYTE pData, UINT uDataLen); // 接收流量统计
	VOID TxStat(LPCBYTE pData, UINT uDataLen); // 发送流量统计
private:
	INT* m_pnConnId2PlayerIndex;
	INT* m_pnPlayerIndex2ConnId;
	INT m_nMaxConnId; // 最大连接id

	std::map<QString, INT> m_mpAccount2Player;

	SIZE_T m_uBytesRxBaseProtocol[USHRT_MAX+1]; // 统计基础模块各协议接收流量
	SIZE_T m_uBytesRxProtocolFamily[USHRT_MAX+1]; // 统计各协议族（模块）接收流量
	SIZE_T m_uBytesTxBaseProtocol[USHRT_MAX+1]; // 统计基础模块各协议发送流量
	SIZE_T m_uBytesTxProtocolFamily[USHRT_MAX+1]; // 统计各协议族（模块）发送流量

	SIZE_T m_uRxBaseProtocol[USHRT_MAX+1]; // 统计基础模块各协议接收次数
	SIZE_T m_uRxProtocolFamily[USHRT_MAX+1]; // 统计各协议族（模块）接收次数
	SIZE_T m_uTxBaseProtocol[USHRT_MAX+1]; // 统计基础模块各协议发送次数
	SIZE_T m_uTxProtocolFamily[USHRT_MAX+1]; // 统计各协议族（模块）发送次数
};

extern KNetConnectPlayer g_cNetConnectPlayer;
// -------------------------------------------------------------------------
#endif

