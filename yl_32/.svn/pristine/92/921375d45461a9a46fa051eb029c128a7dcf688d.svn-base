
#pragma once

// -------------------------------------------------------------------------
class KChatProtocolProcessor
{
public:
#ifdef GAME_SERVER
	
public:
	// 处理GC的应答
	static BOOL ProtocolGCProcess(LPCBYTE pProtocol, UINT nDataLen);
private:
	static BOOL ProcessRespondMsg(LPCBYTE pProtocol, UINT nDataLen);
	
public:
	// 处理Client的请求
	static BOOL ProtocolProcess(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen);
private:
	static BOOL ProcessRequstMsg(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen);
	static BOOL ProcessSendMsgToPlayer(INT nPlayerIdx, LPCBYTE pProtocol, UINT nDataLen);

#else
	// 处理GS应答
	static BOOL ProtocolProcess(LPCBYTE pProtocol, UINT nDataLen);
private:
	static BOOL ProcessRespondMsg(LPCBYTE pProtocol, UINT nDataLen);
#endif
};


