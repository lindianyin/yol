#pragma once
#include "onlinegameworld/kcharacterctrlbase.h"
#include "config/kcharacterdef.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/korpgworld.h"
#include "luaplayervip.h"
#include "vipdef.h"


struct VipBuyCount
{
	INT nId;
	INT nCount;
};

#define VIP_FREE_EXP_LEVEL (2)	// vip体验等级2级
#define VIP_FREE_EXP_TIME (20 * 60)		// 免费体验vip时间，20分钟

// Vip体验功能结构
struct VipFreeExp
{
	DWORD dwFreeTime;	// 免费体验时间
	BYTE byRecVipLevel;		// 免费体验前的vip等级
};

struct VipSaveData
{
	INT nExp;
	VipFreeExp sFree;
	//INT arrViplevel[MAX_VIP_LEVEL];
	CHAR  szMsg[256];
	std::map<INT, VipBuyCount> mapVipCount;
};

class PlayerVip : public KCharacterCtrlBase
{
public:
	enum 
	{	emKCtrlId = emKCHARACTER_CTRL_VIP	};

	explicit PlayerVip(KCharacter& rcPlayer);
	~PlayerVip();

	virtual BOOL Init();

	virtual BOOL UnInit();

	virtual BOOL OnLogin();

	virtual BOOL OnLogout();

	virtual BOOL Active();

	// 读盘
	virtual BOOL OnDbLoad(LPCBYTE pData, SIZE_T uSize);

	// 存盘
	virtual BOOL OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize);

	LuaPlayerVip* GetScriptInterface() CONST;

	BOOL IsOpen(INT nId, BOOL bSendMsg);

	INT GetTotalBuyCount(INT nId);

	INT GetNowBuyCount(INT nId);

	INT GetRate(INT nId);


	BOOL SyncBuyCount();

	//BOOL SyncRecevie();
	BOOL AddExp(INT nCoins);

	INT GetExp();

	BOOL SyncExp();

	BOOL DoSync();

	BOOL DoRecevie(INT nVipLevel);

	BOOL DoBuyCount(INT nId, INT nNum);

	BOOL CanBuyCount(INT nId, INT nNum);

	BOOL DoAddDegree();

	BOOL DoAddDegree(INT nOldVipLevel, INT nNewVipLevel);

	BOOL RefreshSaveData();

	INT GetOverviewCoins(INT nTargetVipLevel);

	BOOL SyncFreeExpTime();

	BOOL GetOrSetMsg(INT nType,CHAR *szMsg);
public:
	BOOL IsFreeExpState(INT nNewVipLevel, INT& nOldVipLevel);

	VOID SetFreeVipState(DWORD dwFreeTime = 0);

	VOID ResetFreeState();

	VOID SetRecFreeVipLevel(INT nVipLevel);

	VOID DoGiveFirstRechargeReward();

private:
	KPlayer&						m_rcPlayer;

	VipSaveData						m_SaveData;

	LuaPlayerVip* m_pLunaObj;
};