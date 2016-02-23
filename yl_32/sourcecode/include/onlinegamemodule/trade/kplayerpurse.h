
#pragma once

// -------------------------------------------------------------------------
#include "Misc/objevent.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kcharacterctrlbase.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegamemodule/trade/kplayerpurse_i.h" 



class KPlayer;
class KPurseData;
class KLuaPlayerPurse;

class KPlayerPurse : public KCharacterCtrlBase, public IKPlayerPurse
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_PURSE	};

public:
	KPlayerPurse(KCharacter& rcPlayer);
	~KPlayerPurse();
	
	KLuaPlayerPurse* GetScriptInterface()
	{	return m_pLuna;	}

public:
	BOOL Init();
	BOOL UnInit();
	BOOL OnLogin();
	BOOL OnLogout();

	KPurseData* GetPurseData()
	{	return m_pPurseData;	}
	
	//珍币
	BOOL RegistCurrencyType(BYTE byCurrencyID);
	BOOL SetCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync = 1);
	INT  GetCurrency(BYTE byCurrencyID);
	
	// 通用钱
	BOOL RegistMoneyType(BYTE byMoneyType);
	BOOL SetMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync = 1);
	INT  GetMoney(BYTE byMoneyType);

	//金币
	INT  GetCoin();
	BOOL SetCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync = 1);

#ifdef GAME_SERVER
	BOOL AddCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync = 1);
	BOOL AddMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync = 1);
	BOOL LogMoney(BYTE byMoneyType, INT nMoney,INT nActionType, KE_PURSE_RESULT* pResult, BOOL bSync = 1,char *str = "");
	BOOL AddCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync = 1);

	BOOL SyncCurrency(BYTE byCurrencyID, INT nCurrency);	
	BOOL SyncRegistCurrencyType(BYTE byCurrencyID);
	BOOL SyncCoin();
	BOOL SyncMoney(BYTE byMoneyType, INT nMoney);
	BOOL SyncRegistMoneyType(BYTE byMoneyType);	

	BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);
	BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);
	BOOL OnClientReady();
	//同步聚宝盆数据
	BOOL SyncBowlData(INT bRefresh);
	BOOL SyncInfoToC(INT nActionType,DWORD dwRoleID,INT nMoney,const char* pszComment); 
		//判断是否是微端
	BOOL IsTinyTerminal() { return m_rcPlayer.GetTinyTerminal();};
protected:
	BOOL OnSavePurseData(LPBYTE& pBuffer, 
						 SIZE_T uBuffSize, 
						 SIZE_T& uUsedSize);

	BOOL OnLoadPurseData(LPCBYTE pData, SIZE_T uSize);

	//玩家退出保存聚宝盆免费获取累计在线时间
	BOOL OnSaveBowlFreeCumulativeTimeLogout();



	
#endif//#ifdef GAME_SERVER

private:
	KPurseData*		 m_pPurseData;
	KLuaPlayerPurse* m_pLuna;
	KPlayer&		 m_rcPlayer;
	INT nMoneyTimes;
	INT nEnergyTimes;
};

// -------------------------------------------------------------------------
