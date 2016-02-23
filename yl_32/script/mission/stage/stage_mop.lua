-------------------------- local function --------------------------

local function GetStartLayer(pPlayer)
	local nDifficulty = KStage.GetMopStage(pPlayer);
	local nStartLayer = (nDifficulty - 1) * Stage.MAX_STAGE_LEVEL + 1;
	return nStartLayer;
end

local function GetTotalMopTime(pPlayer)
	local nDifficulty = KStage.GetMopStage(pPlayer);
	
	local nStartLayer = (nDifficulty - 1) * Stage.MAX_STAGE_LEVEL + 1;
	local nEndLayer = nDifficulty * Stage.MAX_STAGE_LEVEL;
	
	local nTotalTime = 0;
	for i = nStartLayer, nEndLayer do
		local tbCfg = Stage.LayerCfgs[i];
		nTotalTime = nTotalTime + tbCfg.nMopTime;
	end
	
	return nTotalTime;
end

local function GetLayerTime()
	return Stage.LayerCfgs[1].nMopTime;
end

local function GetCurMopLayer(pPlayer)
	
	local nLeftTime = KStage.GetLeftMopTime(pPlayer);
	local nTotalTime = GetTotalMopTime(pPlayer);
	local nLayerTime = GetLayerTime();
	
	local nCurLayer = math.floor((nTotalTime - nLeftTime) / nLayerTime) + GetStartLayer(pPlayer) - 1;
	
	return nCurLayer;
end

-------------------------- tower function --------------------------

function Stage:StartMop(pPlayer, nDifficulty)
	assert(nDifficulty >= 1 and nDifficulty <= 2);
	
	if nDifficulty == 2 and Vip:IsOpen(pPlayer, Vip.ID_STAGE_MOP_HARD) ~= 1 then
		return;
	end

	--assert(pPlayer.GetMissionState() == PVEMatch.STATE_NONE);
	
	if KStage.GetStageStar(pPlayer, nDifficulty) < 5 then
		pPlayer.SendBlackMsg(Lang.mission.str1070[Lang.Idx]);
		return 0;
	end
	
	if Player.tbDegree:GetDegree(pPlayer, "RushStage") <= 0 then
		return 0;
	end
	
	KStage.SetMopStage(pPlayer, nDifficulty);
	
	local nTotalTime = GetTotalMopTime(pPlayer);
	
	KStage.StartMopStage(pPlayer, nTotalTime);
	
	pPlayer.SetMopState(PVEMatch.STAGE_MOP);
	
	return 1;
end

function Stage:c2s_StopMop(pPlayer)
	local nCurLayer = GetCurMopLayer(pPlayer);
	local bFinish = 0;
	if nCurLayer >= GetStartLayer(pPlayer) then
		bFinish = 1;
	end
	
	KStage.StopMopStage(pPlayer, bFinish);
end

function Stage:c2s_FinishMopNow(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_STAGE_FINISH_NOW) ~= 1 then
		return;
	end
	
	local nNeedGold = Stage.FINISH_NOW_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1071[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.mission.str1072[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Stage:DoBuyFinishMop", pPlayer}), "BuyStageFinishNow", 0);
end

function Stage:DoBuyFinishMop(pPlayer)

	if KStage.GetMopState(pPlayer) ~= Player.emMOP_STATE_DOING then
		pPlayer.SendBlackMsg(Lang.mission.str1073[Lang.Idx]);
		return;
	end
	
	local nNeedGold = Stage.FINISH_NOW_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1071[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,119);
	
	KStage.FinishMop(pPlayer);
end

function Stage:TakeAward(pPlayer)
	local nDifficulty = KStage.GetMopStage(pPlayer);
	local nStartLayer = (nDifficulty - 1) * Stage.MAX_STAGE_LEVEL + 1;
	local nCurLayer = GetCurMopLayer(pPlayer);
	
	local nBook = 0;
	local nPrestige = 0;
	
	for i = nStartLayer, nCurLayer do
		local tbCfg = Stage.LayerCfgs[i];
		nBook = nBook + tbCfg.nBook;
		nPrestige = nPrestige + tbCfg.nPrestige;
	end
	
	-- 经验书
	KItem.AddPlayerItem(pPlayer, Pet.PET_BOOK_GDPL[1], Pet.PET_BOOK_GDPL[2], Pet.PET_BOOK_GDPL[3], Pet.PET_BOOK_GDPL[4], nBook);
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_PRESTIGE, nPrestige);
	
	pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowStageMopSuccussPanel"});
	
	pPlayer.SetMopState(0);
	
	-- 扣次数
	Player.tbDegree:ReduceDegree(pPlayer, "RushStage");
	
	return 1;
end

function Stage:c2s_BuyRushTimes(pPlayer)
	if Vip:IsOpen(pPlayer, Vip.ID_STAGE_RUSH) ~= 1 then
		return 0;
	end
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_STAGE_RUSH, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str563[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Stage.BUY_RUSH_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1074[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.mission.str1075[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(pPlayer.dwId, text, ({"Stage:DoBuyRushTimes", pPlayer}), "BuyStageRushTimes", 0);
end

function Stage:DoBuyRushTimes(pPlayer)
	if Vip:CanBuyCount(pPlayer, Vip.ID_STAGE_RUSH, 1) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str563[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Stage.BUY_RUSH_COST;
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str1074[Lang.Idx], szPriceTips))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,117);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_STAGE_RUSH, 1);
end

function Stage:SpeedUp(pPlayer)
	assert(KStage.GetSpeedUp(pPlayer) == 0);
	
	if Vip:IsOpen(pPlayer, Vip.ID_STAGE_SPEEDUP) ~= 1 then
		return;
	end
	
	KStage.SpeedUpMop(pPlayer);
	
	pPlayer.SendBlackMsg(Lang.mission.str1076[Lang.Idx]);
end