
#pragma once

class KPlayerProtocolProcessor;
// -------------------------------------------------------------------------
class IKModuleInterface
{
public:
	virtual BOOL PreInit() = 0;
	virtual BOOL OnInit() = 0;
	virtual BOOL OnUnInit() = 0;
	virtual BOOL Breath() = 0;
	virtual BYTE GetModuleId() = 0;
	virtual BYTE GetGcModuleId() = 0;
	virtual BOOL InitProtocol() = 0;
	virtual KPlayerProtocolProcessor* GetProtocolProcessor() = 0;

#ifdef GAME_SERVER
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen) = 0;
#endif

#ifdef GAME_CLIENT
	virtual BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen) = 0;
#endif
};

// 各模块特化该函数
template<CHAR cModule>
IKModuleInterface* GetModuleInterFace();

