if (not MODULE_GAMESERVER) then
	return;
end

EightTrigram.TIME_ADDED_PER_UPDATE = 10 * 60; -- 每升级一次卦象增加的冷却时间
EightTrigram.TIME_ADDED_PER_TAICHI = 30 * 60; -- 每升级一次天地二仪增加的冷却时间
EightTrigram.MIN_PERIOD_CAN_UPDATE = 0;--60 * 60; -- 能够升级的最小间隿

EightTrigram.CURRENT_UPDATE_TASK_GID = 2090; -- 当前生成信息GroupID
EightTrigram.LAST_UPDATE_TIME_ID = 1; -- 上次升级时间
EightTrigram.REMAIN_TIME = 2; -- 上次升级后剩余的冷却时间
EightTrigram.SPEED_DAY = 3;
EightTrigram.SPEED_LEVEL = 4;

EightTrigram.MAX_LEVEL = 50;
EightTrigram.EXCHANGE_RATE = 10;
EightTrigram.MODULE_NAME = "diagrams_level";
EightTrigram.tbTimeAddedList = {};
EightTrigram.tbBaordMsg = {
							Lang.eighttrigram.str1[Lang.Idx],
							Lang.eighttrigram.str2[Lang.Idx],
							Lang.eighttrigram.str3[Lang.Idx],
							Lang.eighttrigram.str4[Lang.Idx],
							Lang.eighttrigram.str5[Lang.Idx],
							Lang.eighttrigram.str6[Lang.Idx],
							Lang.eighttrigram.str7[Lang.Idx],
							Lang.eighttrigram.str8[Lang.Idx],
							Lang.eighttrigram.str9[Lang.Idx],
							Lang.eighttrigram.str10[Lang.Idx],
							};

function EightTrigram:Load()
	local tbFileData = Lib:LoadTabFile("/setting/faction/eight_trigram_time.txt")
	for _, tbRow in pairs(tbFileData) do
		local nIdx 											= #EightTrigram.tbTimeAddedList + 1;
		EightTrigram.tbTimeAddedList[nIdx] 					= {};
		EightTrigram.tbTimeAddedList[nIdx].nLv 				= tonumber(tbRow.lv) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram1 	= tonumber(tbRow.eightTrigram1) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram2 	= tonumber(tbRow.eightTrigram2) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram3 	= tonumber(tbRow.eightTrigram3) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram4 	= tonumber(tbRow.eightTrigram4) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram5 	= tonumber(tbRow.eightTrigram5) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram6 	= tonumber(tbRow.eightTrigram6) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram7 	= tonumber(tbRow.eightTrigram7) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nEightTrigram8 	= tonumber(tbRow.eightTrigram8) or 0;
		EightTrigram.tbTimeAddedList[nIdx].nTaiChi 			= tonumber(tbRow.taiChi) or 0;
	end
end

-- 升级卦象
function EightTrigram:c2s_TryUpdateLevel(pPlayer, bCoin)
	self:TryUpdateLevel(pPlayer, bCoin);
	local nLevel1 = pPlayer.GetEightTrigramLevel1();
	local nLevel2 = pPlayer.GetEightTrigramLevel2();
	local nLevel3 = pPlayer.GetEightTrigramLevel3();
	local nLevel4 = pPlayer.GetEightTrigramLevel4();
	local nLevel5 = pPlayer.GetEightTrigramLevel5();
	local nLevel6 = pPlayer.GetEightTrigramLevel6();
	local nLevel7 = pPlayer.GetEightTrigramLevel7();
	local nLevel8 = pPlayer.GetEightTrigramLevel8();
	local nLevelTaiChi = pPlayer.GetEightTrigramLevelTaiChi();
	KRank.ModifyRankValue(pPlayer.dwId, Rank.LG_RANK,  nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	KRank.ModifyRankValue(pPlayer.dwId, Rank.RANK_LG_OS,  nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	
	--统计点  ldy
	local szStr = string.format("nGx=%d",nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,62,szStr);

	DailyTasks:commitTaskValue(me,"",DailyTasks.TYPE_EIGHTTRIGRAM,1);
	
end

function EightTrigram:c2s_TryAutoUpdateLevel(pPlayer, bCoin)
	local update = 1;
	repeat
		update = self:TryUpdateLevel(pPlayer, bCoin);
	local nLevel1 = pPlayer.GetEightTrigramLevel1();
	local nLevel2 = pPlayer.GetEightTrigramLevel2();
	local nLevel3 = pPlayer.GetEightTrigramLevel3();
	local nLevel4 = pPlayer.GetEightTrigramLevel4();
	local nLevel5 = pPlayer.GetEightTrigramLevel5();
	local nLevel6 = pPlayer.GetEightTrigramLevel6();
	local nLevel7 = pPlayer.GetEightTrigramLevel7();
	local nLevel8 = pPlayer.GetEightTrigramLevel8();
	local nLevelTaiChi = pPlayer.GetEightTrigramLevelTaiChi();
	KRank.ModifyRankValue(pPlayer.dwId, Rank.LG_RANK,  nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	KRank.ModifyRankValue(pPlayer.dwId, Rank.RANK_LG_OS,  nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	
	--统计点  ldy
	local szStr = string.format("nGx=%d",nLevelTaiChi * 10 + (nLevel1 + nLevel2 + nLevel3+ nLevel4 +nLevel5 +nLevel6 +nLevel7 +nLevel8)%8);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,62,szStr);

	
	until update == 0
end

function EightTrigram:c2s_TrySpeedUp(pPlayer)
	local nCurTime = KGameBase.GetTime();
	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nCoolDownPeriod = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME);
	local nElapsedTime = nCurTime - nLastUpdateTime;
	
	local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	local nNeedCoin = 0;
	local nRate = 1;
	if(Lang.Language == "vi") then 
		nRate = 5
	end
	
	if nCoolDownPeriod > nElapsedTime then
		nNeedCoin = (nCoolDownPeriod - nElapsedTime + 59) / 60 * nRate;
	end
	
	if nCurCoin < nNeedCoin then 
		pPlayer.SendBlackMsg(Lang.eighttrigram.str14[Lang.Idx]);
		do return end;
	end
	local szMsg = string.format(Lang.mission.str1219[Lang.Idx],nNeedCoin);
	Dialog:_AskForSure(pPlayer.dwId, szMsg, ({"EightTrigram:DoSubMoneyAndClearCoolDownTime", pPlayer,nNeedCoin}), "DoSubMoneyAndClearCoolDownTime", 1);
end

function EightTrigram:DoSubMoneyAndClearCoolDownTime(pPlayer,nNeedCoin) 
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -nNeedCoin);
	self:ClearCoolDownTime(pPlayer);
end

function EightTrigram:TryUpdateLevel(pPlayer, bCoin)
	if (pPlayer == nil) then
		return 0;
	end

	-- 检查冷却时闿
	local nCurTime = KGameBase.GetTime();
	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nCoolDownPeriod = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME);
	local nElapsedTime = nCurTime - nLastUpdateTime;
	-- if (nCoolDownPeriod - nElapsedTime > self.MIN_PERIOD_CAN_UPDATE) then
		-- return;
	-- end

	local nLevel1 = pPlayer.GetEightTrigramLevel1();
	local nLevel2 = pPlayer.GetEightTrigramLevel2();
	local nLevel3 = pPlayer.GetEightTrigramLevel3();
	local nLevel4 = pPlayer.GetEightTrigramLevel4();
	local nLevel5 = pPlayer.GetEightTrigramLevel5();
	local nLevel6 = pPlayer.GetEightTrigramLevel6();
	local nLevel7 = pPlayer.GetEightTrigramLevel7();
	local nLevel8 = pPlayer.GetEightTrigramLevel8();
	local nLevelTaiChi = pPlayer.GetEightTrigramLevelTaiChi();
	
	--print(nLevelTaiChi .. "   12345678901212::::" .. nLevel1 .. nLevel2 .. nLevel3 .. nLevel4 .. nLevel5 .. nLevel6 .. nLevel7 .. nLevel8);
	if nLevelTaiChi == EightTrigram.MAX_LEVEL then
		pPlayer.SendBlackMsg(Lang.eighttrigram.str12[Lang.Idx]);
		return 0;
	end
	
	if (nCoolDownPeriod - nElapsedTime > self.MIN_PERIOD_CAN_UPDATE and nLevel8 - nLevelTaiChi == 1) then
		pPlayer.SendBlackMsg(Lang.eighttrigram.str13[Lang.Idx]);
		return 0;
	end

	local tbLevel = {
--		{nLevel1, nLevelTaiChi, pPlayer.SetEightTrigramLevel1, 1}, -- ???m
		{nLevel2, nLevel1, pPlayer.SetEightTrigramLevel2, 2},
		{nLevel3, nLevel2, pPlayer.SetEightTrigramLevel3, 3},
		{nLevel4, nLevel3, pPlayer.SetEightTrigramLevel4, 4},
		{nLevel5, nLevel4, pPlayer.SetEightTrigramLevel5, 5},
		{nLevel6, nLevel5, pPlayer.SetEightTrigramLevel6, 6},
		{nLevel7, nLevel6, pPlayer.SetEightTrigramLevel7, 7},
		{nLevel8, nLevel7, pPlayer.SetEightTrigramLevel8, 8},
--		{nLevelTaiChi, nLevel8, pPlayer.SetEightTrigramLevelTaiChi, 9} -- ???m
	};

	if (nLevel1 == nLevelTaiChi) then
		local nSpirit = self:CalcTrigramSpirit(nLevel1 + 1, 1);
		local nCurSpirit = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SPIRIT);
		
		if (nSpirit ~= nil and nCurSpirit ~= nil and nCurSpirit >= nSpirit) then
			pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, -nSpirit);
			pPlayer.SetEightTrigramLevel1(nLevel1 + 1);
			self:OnUpdateLevel(pPlayer, 0, nLevel1 + 1, 1);		
		elseif bCoin == 1 then
			if EightTrigram:OnUpdateLevelByCoin(pPlayer, 0, nLevel1 + 1, 1) == 1 then
				pPlayer.SetEightTrigramLevel1(nLevel1 + 1);
				self:OnUpdateLevel(pPlayer, 0, nLevel1 + 1, 1);
			else
				return 0;
			end
		else
			pPlayer.SendBlackMsg(Lang.eighttrigram.str11[Lang.Idx]);
			return 0;
		end

		return 1;
	end

	-- 升天地二仿
	if (nLevel8 - nLevelTaiChi == 1) then
		local nSpirit = self:CalcTrigramSpirit(nLevelTaiChi + 1, 9);
		local nCurSpirit = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SPIRIT);
		
		if (nSpirit ~= nil and nCurSpirit ~= nil and nCurSpirit >= nSpirit) then
			pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, -nSpirit);
			pPlayer.SetEightTrigramLevelTaiChi(nLevelTaiChi + 1);
			self:OnUpdateLevel(pPlayer, 1, nLevelTaiChi + 1, 9);
			
			if (math.mod(nLevel8, 10) == 0) then
				KChat.BroadcastNoticeMsg(string.gsub(EightTrigram.tbBaordMsg[nLevel8 / 10], "name", pPlayer.szName));
			end
		elseif bCoin == 1 then
			if EightTrigram:OnUpdateLevelByCoin(pPlayer, 1, nLevelTaiChi + 1, 9) ==  1 then 
				pPlayer.SetEightTrigramLevelTaiChi(nLevelTaiChi + 1);
				self:OnUpdateLevel(pPlayer, 1, nLevelTaiChi + 1, 9);
				
				if (math.mod(nLevel8, 10) == 0) then
					KChat.BroadcastNoticeMsg(string.gsub(EightTrigram.tbBaordMsg[nLevel8 / 10], "name", pPlayer.szName));
				end
			else
				return 0;
			end
		else
			pPlayer.SendBlackMsg(Lang.eighttrigram.str11[Lang.Idx]);
			return 0;
		end
		KRank.ModifyRankValue(pPlayer.dwId, Rank.LG_RANK, nLevelTaiChi + 1);
		KRank.ModifyRankValue(pPlayer.dwId, Rank.RANK_LG_OS, nLevelTaiChi + 1);
		return 1;
	end

	for _, tbOneLevel in pairs(tbLevel) do
		local nCurLevel = tbOneLevel[1];
		local nPreConditionLevel = tbOneLevel[2];
		local pUpdateFunc = tbOneLevel[3];
		local nIndex = tbOneLevel[4];
		
		if (nPreConditionLevel - nCurLevel == 1) then
			local nSpirit = self:CalcTrigramSpirit(nCurLevel + 1, nIndex);
			local nCurSpirit = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SPIRIT);
			
			if (nSpirit ~= nil and nCurSpirit ~= nil and nCurSpirit >= nSpirit) then
				pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, -nSpirit);
				pUpdateFunc(nCurLevel + 1);
				self:OnUpdateLevel(pPlayer, 0, nCurLevel + 1, nIndex);
			elseif bCoin == 1 then
				if EightTrigram:OnUpdateLevelByCoin(pPlayer, 0, nCurLevel + 1, nIndex) == 1 then
					pUpdateFunc(nCurLevel + 1);
					self:OnUpdateLevel(pPlayer, 0, nCurLevel + 1, nIndex);
				else
					return 0;
				end
			else
				pPlayer.SendBlackMsg(Lang.eighttrigram.str11[Lang.Idx]);
				return 0;
			end

			return 1;
		end
	end
	
	return 0;
end

-- 升级卦象所需银两
-- 相关公式参考策划文桿
function EightTrigram:CalcTrigramSilver(nLevel)
	if (nLevel >= 0 and nLevel <= 50) then
		return 10 * nLevel * nLevel + 110 * nLevel - 70;
	end
end

-- 升级卦象所需游龙真气
function EightTrigram:CalcTrigramYLEnergy(nLevel)
	if (nLevel >= 0 and nLevel <= 50) then
		return 5 * nLevel * nLevel + 55 * nLevel - 10;
	end
end

-- 升级天地二仪所需银两
function EightTrigram:CalcTaiChiSilver(nLevel)
	if (nLevel >= 0 and nLevel <= 50) then
		return 50 * nLevel * nLevel + 550 * nLevel - 350;
	end
end

-- 升级天地二仪所需游龙真气
function EightTrigram:CalcTaiChiYLEnergy(nLevel)
	if (nLevel >= 0 and nLevel <= 50) then
		return 25 * nLevel * nLevel + 275 * nLevel - 50;
	end
end

function EightTrigram:CalcTrigramSpirit(nLevel, nTrigram)
	local nLv = (nLevel == 0 and nLevel) or (nLevel - 1) * 9 + nTrigram;
	local tbNumericalAward = KAward.GetNumericalAwardData(EightTrigram.MODULE_NAME, 1, nLv);
	if tbNumericalAward ~= nil then
		return tbNumericalAward["nCost"];
	end
	return 0;
end

function EightTrigram:OnUpdateLevelByCoin(pPlayer, bTaiChi, nLevel, nIndex)
	local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	local nRate = KPurse.GetExchangeRate(Purse.EM_MONEY_SPIRIT);
	local nSpirit = self:CalcTrigramSpirit(nLevel, nIndex);
	local nCurSpirit = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SPIRIT);
	
	local nNeedCoin = math.floor((nCurSpirit > nSpirit and 0) or (nSpirit - nCurSpirit + nRate - 1) / nRate);
	if nNeedCoin > 0 and nCurCoin >= nNeedCoin then 
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, -nCurSpirit);
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -nNeedCoin);
		--pPlayer.SetEightTrigramLevel1(nLevel);
		--self:OnUpdateLevel(pPlayer, bTaiChi, nLevel, nIndex);
		return 1;
	else
		pPlayer.SendBlackMsg(Lang.eighttrigram.str14[Lang.Idx]);
	end
	
	return 0;
end

function EightTrigram:OnUpdateLevel(pPlayer, bTaiChi, nLevel, nIndex)
	if (pPlayer == nil) then
		return;
	end

	bTaiChi = bTaiChi or 0;

	local nCurTime = KGameBase.GetTime();
	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nCoolDownPeriod = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME);
	local nElapsedTime = nCurTime - nLastUpdateTime;
	
	local tbTimeAdd = {
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram1, 
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram2,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram3,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram4,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram5,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram6,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram7,
		EightTrigram.tbTimeAddedList[nLevel].nEightTrigram8,
		EightTrigram.tbTimeAddedList[nLevel].nTaiChi,
	};

	if (nElapsedTime >= nCoolDownPeriod) then
		nCoolDownPeriod = 0;
	else
		nCoolDownPeriod = nCoolDownPeriod - nElapsedTime;
	end
	
	nCoolDownPeriod = nCoolDownPeriod + tbTimeAdd[nIndex];
	
	-- if (bTaiChi == 1) then
		-- nCoolDownPeriod = nCoolDownPeriod + self.TIME_ADDED_PER_TAICHI;
	-- else
		-- nCoolDownPeriod = nCoolDownPeriod + self.TIME_ADDED_PER_UPDATE;
	-- end

	pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID, nCurTime);
	pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME, nCoolDownPeriod);

	-- 同步客户端时闿
	self:SyncCoolDownTime(pPlayer);
end

function EightTrigram:SyncCoolDownTime(pPlayer)
	if (pPlayer == nil) then
		return;
	end

	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nCoolDownPeriod = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME);

	pPlayer.CallClientScript({"YoulongApp.UI.QUIEightTrigramManager:SyncEightTrigramTime", nLastUpdateTime, nCoolDownPeriod});
end

-- for tester
function EightTrigram:ClearCoolDownTime(pPlayer)
	if (pPlayer == nil) then
		return;
	end
	
	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nCoolDownPeriod = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME);
	
	if Vip:IsOpen(pPlayer, Vip.ID_LIANGUA, 0) == 1 then
		nLastUpdateTime = 0;
		nCoolDownPeriod = 0;
	elseif Vip:IsOpen(pPlayer, Vip.ID_LIANGUA_SPEED) == 1 then
		local nSpeedDay = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.SPEED_DAY);
		local nSpeedLevel = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.SPEED_LEVEL);
		local nToday = Lib:GetLocalDay(KGameBase.GetTime());
		local nLevelTaiChi = pPlayer.GetEightTrigramLevelTaiChi();
		print("..nSpeedDay ~= nToday",nSpeedDay ~= nToday);
		print("..nSpeedLevel ~= nLevelTaiChi",nSpeedLevel ~= nLevelTaiChi);
		--if nSpeedDay ~= nToday or nSpeedLevel ~= nLevelTaiChi then
		--if nSpeedLevel ~= nLevelTaiChi then
			nLastUpdateTime = KGameBase.GetTime();
			nCoolDownPeriod = math.ceil(nCoolDownPeriod / 2);
			
			pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.SPEED_DAY, nToday);
			pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.SPEED_LEVEL, nLevelTaiChi);
		--end
	else
		return;
	end

	pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID, nLastUpdateTime);
	pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME, nCoolDownPeriod);
	self:SyncCoolDownTime(pPlayer);
end

-- for tester
function EightTrigram:ClearLevel(pPlayer)
	if (pPlayer == nil) then
		return;
	end

	pPlayer.SetEightTrigramLevel1(0);
	pPlayer.SetEightTrigramLevel2(0);
	pPlayer.SetEightTrigramLevel3(0);
	pPlayer.SetEightTrigramLevel4(0);
	pPlayer.SetEightTrigramLevel5(0);
	pPlayer.SetEightTrigramLevel6(0);
	pPlayer.SetEightTrigramLevel7(0);
	pPlayer.SetEightTrigramLevel8(0);
	pPlayer.SetEightTrigramLevelTaiChi(0);
end

function EightTrigram:SetCoolDownTime(pPlayer, nTime)
	if (pPlayer == nil) then
		return;
	end

	local nLastUpdateTime = pPlayer.pTask.GetTask(self.CURRENT_UPDATE_TASK_GID, self.LAST_UPDATE_TIME_ID);
	local nRemainTime = KGameBase.GetTime() - nLastUpdateTime + nTime;

	pPlayer.pTask.SetTask(self.CURRENT_UPDATE_TASK_GID, self.REMAIN_TIME, nRemainTime);
	self:SyncCoolDownTime(pPlayer);
end

function EightTrigram:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer) then
		self:SyncCoolDownTime(pPlayer);
	end
end

function EightTrigram:Init()
	EightTrigram.Load();
end

EightTrigram:Init();
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, EightTrigram.OnLogin, EightTrigram);