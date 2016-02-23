-- 文件名　：purseinit.lua
-- 创建者　：wdb
-- 创建时间：2010-10-19 10:52:04
-- 功能描述：玩家钱袋 初始化

if not MODULE_GAMESERVER then
	return;
end

function Purse:Init(pPlayer)
	assert(pPlayer);
	
	for i = Purse.EM_CURRENCY_BEGIN, Purse.EM_CURRENCY_MAX - 1 do
		pPlayer.pPlayerPurse.RegistCurrencyType(i);
	end;
	
	for j = Purse.EM_MONEY_BEGIN, Purse.EM_MONEY_MAX - 1 do
		pPlayer.pPlayerPurse.RegistMoneyType(j);
	end;
	
	return 1;
end


