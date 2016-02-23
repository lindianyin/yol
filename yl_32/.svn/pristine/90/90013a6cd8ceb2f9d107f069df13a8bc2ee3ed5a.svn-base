
#pragma once

// -------------------------------------------------------------------------
class KPlayerPurse;
class KPurseProtocolHandler
{
public:
	BOOL OnInit();
	//异步处理GS 发过来的协议
	BOOL ProtocolProcess(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);

#ifdef GAME_CLIENT
	BOOL (KPurseProtocolHandler::*m_aProtocolHandler[emPURSE_PTC_S2C_COUNT])(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);

protected:
	BOOL HandleAddCurrency(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);
	BOOL HandleRegistCurrencyType(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);
	BOOL HandleAddCoin(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);
	BOOL HandleAddMoney(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);
	BOOL HandleRegistMoneyType(KPlayerPurse* pPlayerPurse, LPCBYTE pData, INT nDataLen);

#endif//#ifdef GAME_CLIENT
};

inline KPurseProtocolHandler& GetProtocolHandler()
{
	static KPurseProtocolHandler s_hdl;
	return s_hdl;
};


