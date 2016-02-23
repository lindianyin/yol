
#pragma once

// -------------------------------------------------------------------------
enum KE_PURSE_RESULT
{
	emKPURSE_S_SUCCESS = 0,				//永远排在第一位
	emKPURSE_E_FAILED,					//未知原因失败,通常程序出错
	emKPURSE_E_CURRENCYTYPE_NOTEXIST,   //珍币类型不存在
	emKPURSE_E_CURRENCYADD_OVERLIMIT,   //珍币数量超出极限
	emKPURSE_E_CURRENCYCOST_NOENOUGH,   //珍币数量不足
	emKPURSE_E_MONEYTYPE_NOTEXIST,		//钱类型不存在
	emKPURSE_E_MONEYADD_OVERLIMIT,		//钱数量超出极限
	emKPURSE_E_MONEYCOST_NOENOUGH,		//钱数量不足
	emKPURSE_E_COINADD_OVERLIMIT,		//金币数量超出极限，好像不太可能啊
	emKPURSE_E_COINCOST_NOENOUGH,		//金币数量不足

	emKPURSE_E_MAX = 255,				//内核错误定义最大值，脚本从256开始定义
};

enum KMONEYTYPE				// 交易时所采用的商品的货币类型
{
	EM_MONEY_COIN     = 1,  // 元宝
	EM_MONEY_BEGIN    = 1,
	EM_MONEY_SILVER	  = 2,  //银两
	EM_MONEY_YLENERGY = 3,	// 游龙真气
	EM_MONEY_SPIRIT   = 4,	// 战魂
	EM_MONEY_PRESTIGE = 5,	// 声望
	EM_MONEY_CONTRIBUTION = 6, // 家族贡献度
	EM_MONEY_MPJJ_SCORE = 7,//门派竞技积分
	EM_MONEY_JINGPO = 8,	// 精魄
	EM_MONEY_BIND_COIN = 9,  // 绑定元宝
	EM_MONEY_SHENMO_SCORE = 10, // 神魔积分
	EM_MONEY_MAX      = 11
};

class KPlayer;

interface IKPlayerPurse
{
	//珍币
	virtual BOOL RegistCurrencyType(BYTE byCurrencyID)	PURE;
	virtual BOOL SetCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync = 1)  PURE;
	virtual INT  GetCurrency(BYTE byCurrencyID) PURE;
	
	//钱
	virtual BOOL RegistMoneyType(BYTE byMoneyType) PURE;
	virtual INT  GetMoney(BYTE byMoneyType)        PURE;
	virtual BOOL SetMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync = 1) PURE;

	//金币	
	virtual BOOL SetCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync = 1) PURE;
	virtual INT  GetCoin() PURE;

#ifdef GAME_SERVER
	virtual BOOL AddCurrency(BYTE byCurrencyID, INT nCurrency, KE_PURSE_RESULT* pResult, BOOL bSync = 1)  PURE;
	virtual BOOL AddMoney(BYTE byMoneyType, INT nMoney, KE_PURSE_RESULT* pResult, BOOL bSync = 1) PURE;
	virtual BOOL LogMoney(BYTE byMoneyType, INT nMoney,INT nActionType, KE_PURSE_RESULT* pResult, BOOL bSync = 1,char *str = "") PURE;
	
	virtual BOOL AddCoin(INT nCoin, KE_PURSE_RESULT* pResult, BOOL bSync = 1) PURE;
#endif
};
// -------------------------------------------------------------------------

IKPlayerPurse* GetPlayerPurse(KPlayer* pPlayer);

