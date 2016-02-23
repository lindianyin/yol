-------------------------- local function --------------------------

local function GetTotalMopTime(pPlayer)
	local nMaxLayer = pPlayer.GetTowerLayer();
	if nMaxLayer <= 0 then
		return 0;
	end
	
	local nTotalTime = 0;
	for i = 1, nMaxLayer do
		local tbCfg = Tower.LayerCfgs[i];
		nTotalTime = nTotalTime + tbCfg.nMopTime;
	end
	
	return nTotalTime;
end

local function GetLayerTime()
	return Tower.LayerCfgs[1].nMopTime;
end

local function GetCurMopLayer(pPlayer)
	local nLeftTime = KTower.GetLeftMopTime(pPlayer);
	--local nTotalTime = GetTotalMopTime(pPlayer);
	--local nLayerTime = GetLayerTime();
	--local nCurLayer = math.floor((nTotalTime - nLeftTime) / nLayerTime);
	--return nCurLayer;
	local nMaxLayer = pPlayer.GetTowerLayer()
	local nTotal = 0
	for i=nMaxLayer,1,-1 do 
		nTotal = nTotal +  Tower.LayerCfgs[i].nMopTime
		if(nTotal > nLeftTime) then 
			do return i end
		end
	end 
end

-------------------------- tower function --------------------------

function Tower:StartMop(pPlayer)
	local nMaxLayer = pPlayer.GetTowerLayer();
	if nMaxLayer <= 0 then
		return 0;
	end
	
	if Player.tbDegree:GetDegree(pPlayer, "MopTower") <= 0 then
		return 0;
	end
	
	local nTotalTime = GetTotalMopTime(pPlayer);
	
	KTower.StartMopTower(pPlayer, nTotalTime);
	
	pPlayer.SetMopState(PVEMatch.TOWER_MOP);
	
	return 1;
end

function Tower:StopMop(pPlayer)
	local nCurLayer = GetCurMopLayer(pPlayer);
	
	KTower.StopMopTower(pPlayer, nCurLayer);
end

function Tower:SpeedUp(pPlayer)
	assert(KTower.GetSpeedUp(pPlayer) == 0);
	
	if Vip:IsOpen(pPlayer, Vip.ID_TOWER_SPEEDUP) ~= 1 then
		return;
	end
	
	KTower.SpeedUpMop(pPlayer);
	pPlayer.SendBlackMsg(Lang.mission.str1060[Lang.Idx]);
end

function Tower:DoBuySpeedUp(pPlayer)
	--local nNeedGold = Tower.SPEED_UP_COST;
	local nLeftTime = KTower.GetLeftMopTime(pPlayer);
	local nNeedGold = math.ceil(nLeftTime/60) * 2
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1061[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,106);
	
	KTower.SpeedUpMop(pPlayer);
end

function Tower:FinishMopNow(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_TOWER_FINISH_NOW) ~= 1 then
		return;
	end
	
	--local nNeedGold = Tower.FINISH_NOW_COST;
	local nLeftTime = KTower.GetLeftMopTime(pPlayer);
	local nNeedGold = math.ceil(nLeftTime/60) * 3
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1062[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.mission.str1063[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Tower:DoBuyFinishMop", pPlayer}), "BuyTowerFinishNow", 0);
end

function Tower:DoBuyFinishMop(pPlayer)
	--local nNeedGold = Tower.FINISH_NOW_COST;
	local nLeftTime = KTower.GetLeftMopTime(pPlayer);
	local nNeedGold = math.ceil(nLeftTime/60) * 3
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1062[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -1 * nNeedGold,107);
	
	KTower.FinishMop(pPlayer);
end

function Tower:TakeAward(pPlayer)
	local nCurLayer = GetCurMopLayer(pPlayer);
	local nTotalExp = 0;
	local nTotalMoney = 0;
	for i = 1, nCurLayer do
		local tbCfg = Tower.LayerCfgs[i];
		
		--local nExp = Tower:GetLayerTotalExp(i, pPlayer.GetLevel());
		nTotalExp = nTotalExp + tbCfg.nExpPercent;
		
		nTotalMoney = nTotalMoney + tbCfg.nMoney;
	end
	
	pPlayer.AddExp(nTotalExp);
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, nTotalMoney);
	
	local tStartTime = KTower.GetMopStartTime(pPlayer);
	local tNow = KGameBase.GetTime();
	if Lib:GetLocalDay(tNow) == Lib:GetLocalDay(tStartTime) then
		Player.tbDegree:ReduceDegree(pPlayer, "MopTower");
	end
	
	pPlayer.SetMopState(0);
	
	pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowMopSuccussPanel"});
end

function Tower:c2s_BuyMopTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_TOWER_MOP) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_TOWER_MOP, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str563[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Tower.BUY_MOP_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1065[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.mission.str1066[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Tower:DoBuyMopTimes", pPlayer}), "BuyTowerMopTimes", 0);
end

function Tower:DoBuyMopTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_TOWER_MOP, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str563[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Tower.BUY_MOP_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1065[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,105);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_TOWER_MOP, 1);
end