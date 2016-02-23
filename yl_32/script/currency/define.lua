if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

--定义材料货币类型
Purse.EM_CURRENCY_BEGIN    =  1;
Purse.EM_CURRENCY_MAX    =  1; -- 不能超过255

--定义钱的类型
Purse.EM_MONEY_COIN      =  1;  -- 金币， 不能更改，排在第一位
Purse.EM_MONEY_BEGIN     =  1;
Purse.EM_MONEY_SILVER	 =  2;  -- 银两
Purse.EM_MONEY_YLENERGY  =  3;  -- 真气 kk
Purse.EM_MONEY_SPIRIT  	 =  4;	-- 战魂
Purse.EM_MONEY_PRESTIGE  =  5;  -- 声望
Purse.EM_MONEY_CONTRIBUTION = 6; -- 家族贡献度
Purse.EM_MONEY_MPJJ_SCORE = 7;  --战神无双(门派竞技)积分
Purse.EM_MONEY_JINGPO 		= 8;  --精魄
Purse.EM_MONEY_BIND_COIN  = 9;  --绑定元宝
Purse.EM_MONEY_SHENMO_SCORE = 10; -- 神魔积分榜
Purse.EM_MONEY_MAX       =  11;

Purse.EM_PLAYER_MAXCARRYCURRENCY = 99999999;	--玩家珍币最大携带量

--钱袋模块返回值

Purse.EM_S_SUCCESS = 0;					--永远排在第一位
Purse.EM_E_FAILED = 1;					--未知原因失败,通常程序出错
Purse.EM_E_CURRENCYTYPE_NOTEXIST = 2;   --珍币类型不存在
Purse.EM_E_CURRENCYADD_OVERLIMIT = 3;   --珍币数量超出极限
Purse.EM_E_CURRENCYCOST_NOENOUGH = 4;   --珍币数量不足
Purse.EM_E_MONEYTYPE_NOTEXIST    = 5;   --钱类型不存在
Purse.EM_E_MONEYADD_OVERLIMIT    = 6;	--钱数量超出极限
Purse.EM_E_MONEYCOST_NOENOUGH    = 7; 	--钱数量不足
Purse.EM_E_COINADD_OVERLIMIT     = 8;	--金币数量超出极限，好像不太可能啊
Purse.EM_E_COINCOST_NOENOUGH     = 9;	--金币数量不足

--脚本要定义返回值，从256开始, 避免和内核定义重叠
