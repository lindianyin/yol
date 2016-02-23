--=======================================================================
-- 文件名　：purse.lua
-- 创建者　：Yulei1(yulei1@kingsoft.com)
-- 创建时间：2010-07-26 10:06:05
-- 功能描述: 钱包（玩家的金钱以及材料货币相关的操作处理）
--=======================================================================
if not MODULE_GAMESERVER then
	return
end

if not _G.Player then
	print("_G.Player未定义，无法加载金钱-材料货币系统")
	return nil;
end

local tbPurse = {};

--预加载金钱-材料货币系统的配置文件
local PreLoadCurrencyConfig = function()
	if _G.Player.tbPurse then
		print("金钱-材料货币系统_G.Player.tbPurse 已经加载，无法重复加载");
		return false;
	end
	
	_G.Player.tbPurse = tbPurse;
	
	local szCurrencyFile = "/setting/currency/currency.txt";
	local tbAmountField = {["Id"] = 1};
	
	local tbCurrencyDatas = Lib:LoadTabFile(szCurrencyFile, tbAmountField);

	tbPurse.tbCurrencyDatas = {};
	for _,tbCurrencyData in pairs(tbCurrencyDatas) do
		tbPurse.tbCurrencyDatas[tbCurrencyData.Id] = tbCurrencyData;
	end
	return true;
end;

if _G.Player.tbPurse then
	print("_G.Player.tbPurse已经加载")
	return nil;
end

PreLoadCurrencyConfig();		--预加载货币系统配置


-- 获得Id为n的货币种类信息
function tbPurse:GetCurrencyInfo(nId)
	return tbPurse.tbCurrency[nId];
end

function tbPurse:GetAlChemyTimes(pPlayer)
	local nTimes = Player.tbDegree:GetDegree(pPlayer, "AlChemyTimes")
	return nTimes;
end

-- 元宝换银两
function tbPurse:AddMoneyCoin(pPlayer, nCoinNum)
	local tbAward = KAward.GetAssetAwardData("ExchangeMoney", pPlayer.GetLevel());
	local nSilver = tbAward.nSilver;
	local nEnergy = tbAward.nEnergy;
	if not nTimes then
		nSilver = nSilver * nTimes;
	end
	pPlayer.pPurse.AddMoney();
	
	--[[
	if pPlayer then
		KAward.AddPlayerAward(pPlayer, "ExchangeMoney", pPlayer.GetLevel());
		pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nCoinNum);
	else
		return nil;
	end
	--]]
end

-- 元宝换真气
function tbPurse:AddZhenQi(pPlayer, nCoinNum)
	
	if pPlayer then
		KAward.AddPlayerAward(pPlayer, "ExchangeZhengQi", pPlayer.GetLevel());
		pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nCoinNum);
	else
		return nil;
	end
end 

--TODO 增加玩家身上的银两
function tbPurse:AddMoney(pPlayer, nAmount)
	if not pPlayer then
		return nil;
	end
end

--TODO 消费玩家身上的银两
function tbPurse:CostMoney(pPlayer, nAmount)
	if not pPlayer then
		return nil;
	end
end

--TODO 获取玩家身上的银两数量
function tbPurse:GetMoney(pPlayer)
	if not pPlayer then
		return nil;
	end
	
end

-- 增加玩家身上Id为n的材料货币
function tbPurse:AddCurrency(pPlayer, nId, nAmount)
	if not pPlayer then
		return nil;
	end
	--TODO 一些操作限制
	pPlayer.pPlayerPurse.AddCurrency(nId, nAmount);
end

-- 消耗玩家身上Id为n的材料货币
function tbPurse:CostCurrency(pPlayer, nId, nAmount)
	if not pPlayer then
		return nil;
	end
	--TODO 一些操作限制
	pPlayer.pPlayerPurse.CostCurrency(nId, nAmount);
end

-- 获取玩家当前所有的货币数量信息
function tbPurse:GetAllCurrencyAmount(pPlayer)
	if not pPlayer then
		return nil;
	end
	return pPlayer.pPlayerPurse.GetCurrencyList();
end

-- 获取玩家身上Id为n的材料货币的数量
function tbPurse:GetCurrencyAmountById(pPlayer, nId)
	if not pPlayer then
		return nil;
	end
	local tbAllCurrency = pPlayer.pPlayerPurse.GetCurrencyList();
	-- 脚本这样频繁调用会不会有效率问题？提供新接口？
	for _,tbCurrency in pairs(tbAllCurrency) do
		if (tbCurrency.nCurrencyId == nId) then
			return tbCurrency.nCurrencyAmount;
		end
	end
	return nil;
end
-- 测试用,因为暂时没有地方能看到货币，就在命令台显示一下吧
function tbPurse:Show(pPlayer)
	local tbAllCurrency = pPlayer.pPlayerPurse.GetCurrencyList();
	if not tbAllCurrency then
		return;
	end
	for _,tbCurrency in pairs(tbAllCurrency) do
		if tbPurse.tbCurrencyDatas[tbCurrency.nCurrencyId] then
			print(tbPurse.tbCurrencyDatas[tbCurrency.nCurrencyId].Name.." 数量："..tbCurrency.nCurrencyAmount);
		end
	end
end

function tbPurse:c2s_AddMoney(pPlayer, nCoinNum, nCount)
	Player.tbDegree:ReduceDegree(pPlayer, "AlFreeTimes", nCount);
	self:AddMoneyCoin(pPlayer, nCoinNum)
end

function tbPurse:c2s_AddYLEnergy(pPlayer, nCoinNum, nCount)
	Player.tbDegree:ReduceDegree(pPlayer, "AlCoinTimes", nCount);
	self:AddZhenQi(pPlayer, nCoinNum)
end

--免费获取
function tbPurse:c2s_AddMenoyYLEnergy_Free(pPlayer, nCoinNum, nCount)
	local bRet = Player.tbDegree:ReduceDegree(pPlayer, "AlFreeTimes", nCount);
	if(not bRet) then 
		print("..tbPurse:c2s_AddMenoyYLEnergy_Free error");
		return;
	end
	local tbAwardMoney 	= KAward.GetAssetAwardData("ExchangeMoney", pPlayer.GetLevel());
	local nSilver 		= tbAwardMoney.nSilver * nCount;
	local tbAwardEnergy = KAward.GetAssetAwardData("ExchangeZhengQi", pPlayer.GetLevel());
	local nEnergy 		= tbAwardEnergy.nEnergy * nCount;
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, nSilver);
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, nEnergy);
end

--元宝获取
function tbPurse:c2s_AddMenoyYLEnergy_Coin(pPlayer, nCoinNum, nCount)
	local bRet = Player.tbDegree:ReduceDegree(pPlayer, "AlCoinTimes", nCount);
	if(not bRet) then 
		print("..tbPurse:c2s_AddMenoyYLEnergy_Coin error");
		return;
	end
	local tbAwardMoney 	= KAward.GetAssetAwardData("ExchangeMoney", pPlayer.GetLevel());
	local nSilver 		= tbAwardMoney.nSilver * nCount;
	local tbAwardEnergy = KAward.GetAssetAwardData("ExchangeZhengQi", pPlayer.GetLevel());
	local nEnergy 		= tbAwardEnergy.nEnergy * nCount;
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, nSilver);
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, nEnergy);
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -1 * nCoinNum);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -1 * nCoinNum,104);
end