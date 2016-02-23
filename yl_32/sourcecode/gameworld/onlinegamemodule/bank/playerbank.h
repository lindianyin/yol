#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
//#include "luaplayergem.h"
#include "bankdef.h"

#define BNAK_HONOR_ID  (1)
#define BNAK_NORMAL_ID (2)
#define BNAK_HONOR_DAYS  (30)
#define BNAK_NORMAL  (7)

struct BankSaveData
{
	INT nId;
	INT nCoins;
	INT nGetCoins;
	DWORD dwTime;
};

class PlayerBank : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_PLAYER_BANK	};

	explicit PlayerBank(KCharacter& rcPlayer);
	~PlayerBank();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	//LuaPlayerGem* GetScriptInterface() CONST;

	BOOL DoGetIncomeRequest(INT nId);

	BOOL DoInvestRequest(INT nConis, INT nId);

	BOOL SyncAllInvestInfo();

	BOOL SyncMyInvestInfo();

	//获取每日投宝收益(测试用)
	//str: 2014-08-02 12:12:12
	BOOL DoGetIncomeTest(INT nId, CHAR* str);

private:

	KPlayer&						m_rcPlayer;
	//LuaPlayerGem* m_pLunaObj;
	
	std::vector<BankSaveData> m_vecSaveData;


	BankSaveData* GetLastSaveData(INT nId);

	BankSaveData* GetInvestData(INT nId);

	//测试用
	BankSaveData* GetLastSaveDataTest(INT nId, CHAR* str);
};