
#pragma once

#include "onlinegameworld/kplayerprotocolprocess.h"
#include "onlinegameworld/kmoduleinterface.h"
#include "onlinegameworld/kprotocol.h"
// -------------------------------------------------------------------------
class KModuleBase : public IKModuleInterface
{
public:
	// 模块ID，范围1~127
	KModuleBase(CHAR cModuleId) : m_cModuleId(cModuleId),
		m_cProtocolProcessor(cModuleId + KD_MAX_BASE_PROTOCOL_HEADER)
	{
		QCONFIRM(cModuleId > 0);
		KPTC_SIZE::SetSizeCheckC2S(cModuleId + KD_MAX_BASE_PROTOCOL_HEADER, m_anProtocolSizeC2S);
		KPTC_SIZE::SetSizeCheckS2C(cModuleId + KD_MAX_BASE_PROTOCOL_HEADER, m_anProtocolSizeS2C);
	}
	KPlayerProtocolProcessor* GetProtocolProcessor() { return &m_cProtocolProcessor;}
	BYTE GetModuleId() { return (BYTE)m_cModuleId; }

#ifdef GAME_SERVER
	// 处理玩家客户端协议
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen) { return TRUE; }
#endif

#ifdef GAME_CLIENT
	// 处理服务端玩家协议
	virtual BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen) { return TRUE; }
#endif

protected:
	virtual BOOL PreInit() { return TRUE; }
	virtual BOOL OnInit() { return TRUE; }
	virtual BOOL OnUnInit() { return TRUE; }
	virtual BOOL Breath() { return TRUE; }
	virtual BYTE GetGcModuleId() { return 0; }
private:
	// 模块中重载此函数，使用下面的宏初始化协议（大小）
	virtual BOOL InitProtocol() { ZeroStruct(m_anProtocolSizeS2C); ZeroStruct(m_anProtocolSizeC2S); return TRUE; }
	// 初始化定长协议结构
#define KD_PTC_FIXED_SIZE_S2C(structPtc) m_anProtocolSizeS2C[structPtc::protocol] = sizeof(structPtc)
#define KD_PTC_FIXED_SIZE_C2S(structPtc) m_anProtocolSizeC2S[structPtc::protocol] = sizeof(structPtc)
	// 初始化变长协议结构
#define KD_PTC_LENGTNED_SIZE_S2C(structPtc) m_anProtocolSizeS2C[structPtc::protocol] = -1
#define KD_PTC_LENGTNED_SIZE_C2S(structPtc) m_anProtocolSizeC2S[structPtc::protocol] = -1
protected:
	CHAR m_cModuleId;
	KPlayerProtocolProcessor m_cProtocolProcessor;
	INT m_anProtocolSizeS2C[UCHAR_MAX + 1];
	INT m_anProtocolSizeC2S[UCHAR_MAX + 1];
};

// -------------------------------------------------------------------------

