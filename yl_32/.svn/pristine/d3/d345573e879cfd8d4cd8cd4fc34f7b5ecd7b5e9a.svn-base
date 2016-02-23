
#pragma once

#include "onlinegameworld/kprotocol.h"

// 玩家协议处理器，封装协议族
class KPlayerProtocolProcessor
{
public:
	// btPtcFamily范围在0~128
	KPlayerProtocolProcessor(BYTE btPtcFamily) : m_byPtcFamily(btPtcFamily){}
#ifdef GAME_SERVER
	// 发送固定长度协议
	BOOL SendData(INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);
	// 发送变长协议
	BOOL SendLenData(INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);
	// 发送未知协议（不确定具体的协议结构，不知是否定长，此项不能滥用）
	BOOL SendUnknownData(INT nPlayerIndex, LPSTR pData, UINT uDataSize);
#else
	BOOL SendData(KPTC_HEADER_BASE& rPtcData);
	BOOL SendLenData(KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);
#endif
protected:
	BYTE m_byPtcFamily;
};
// -------------------------------------------------------------------------

