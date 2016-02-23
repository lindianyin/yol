-- 
-- 逍遥谷关卡 GS逻辑
--
Include("script/mission/level/xoyogame_def.lua");
Include("script/lib/timer.lua");
XoyoGame.BaseManager = {};
XoyoGame.tbNpcLevel = {};
XoyoGame.tbXoyoRank = {};
local BaseManager = XoyoGame.BaseManager;

function BaseManager:Init(nMapId, tbData)
	self.nManagerId = nMapId;
	self.tbData = tbData;
end

function XoyoGame:Init()
	self.tbGameIdToManager = {};			-- 地图关卡对象映射表
	self.tbGame = {};
	self.tbManager = {};
	self.tbMap2Game = {};
	self.tbStartTime = {};
	for nManagerId, tbCitySet in pairs(self.MANAGER_GROUP) do
		for _, nDataId in ipairs(tbCitySet) do
			self.tbGameIdToManager[nDataId] = nManagerId;
		end
	end
end

function XoyoGame:OnSyncRankData(tbData)
	self.tbXoyoRank = tbData;
end

function XoyoGame:RecordTime(nDifficuty, nTimeUsed, nTeamId)
	XoyoGame.tbXoyoRank[nDifficuty] = XoyoGame.tbXoyoRank[nDifficuty] or {};
	local tbRank = XoyoGame.tbXoyoRank[nDifficuty];
	local nRank = 0;
	if (#tbRank < XoyoGame.RANK_RECORD) then
		nRank = #tbRank + 1;
	end
	for i = #tbRank, 1, -1 do
		if (tbRank[i].nTime > nTimeUsed) then
			nRank = i;
		end
	end
	if nRank > 0 then
		local tbMember = {};
		local tbList, nCount = KTeam.GetTeamMemberList(nTeamId);
		if nCount == 0 then
			return nRank;
		end
		for nIndex, nPlayerId in pairs(tbList) do
		    local pPlayer = KGameBase.GetPlayerById(nPlayerId);
			tbMember[nIndex] = pPlayer.szName;--KGCPlayer.GetPlayerName(nPlayerId);
		end
		--XoyoGame:RecordRankData(nTimeUsed, nDifficuty, tbMember); --zzx
	end
	return nRank;
end

function XoyoGame:AwardPlayer(pPlayer, nDifficulty, tbAward)
	local nTimes = self:GetTodayMissionTimes(pPlayer);
	local tbAwardPercent = 100;
	
	if nTimes > XoyoGame.TIMES_FULL_REWARD then
		tbAwardPercent = 80;	-- 超过次数只给80%奖励
	end
	
	local nExp = math.floor(tbAward.ExpAward * tbAwardPercent / 100);
	local nMoney = math.floor(tbAward.CoinAward * tbAwardPercent / 100);
	
	-- 给经验
	pPlayer.AddExp(nExp);
	-- 给钱
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, nMoney);
	-- 每天前6次有额外物品奖励
	if nTimes <= XoyoGame.TIMES_FULL_REWARD then
		KItem.AddPlayerItem(pPlayer, tbAward.ItemGerne, tbAward.ItemDetail, tbAward.ItemParticularType, tbAward.ItemLevel);
	end
end

-- 通关奖励
function XoyoGame:FinishAward(nDifficuty, nTeamId)
	local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
	if nCount == 0 then
		return;
	end
	for _, nPlayerId in pairs(tbMember) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		local tbMissionAward = XoyoGame.RoomSetting.tbMissionAward;
		local tbAward = tbMissionAward[nDifficuty][pPlayer.GetLevel()];
		if not tbAward then
			return;
		end
		self:AddTodayMissionTimes(pPlayer);		-- 增加今日通关次数
		self:AwardPlayer(pPlayer, nDifficuty, tbAward);
	end
end

function XoyoGame:FinishDTTAward(nTeamId, nLevel, nWaveNum)
	local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
	if nCount == 0 then
		return;
	end

	for _, nPlayerId in pairs(tbMember) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		local tbDTTAward = XoyoGame.RoomSetting.tbDTTGameAward;
		if (not tbDTTAward[nLevel]) then
			return;
		end
		if (not tbDTTAward[nLevel][nWaveNum]) then
			return;
		end

		local tbAward = tbDTTAward[nLevel][nWaveNum];
		if (not tbAward) then
			return;
		end

		local tbType = {};
		local tbValue = {};
		for i = 1, 5, 1 do
			local nType = tonumber(tbAward["type"..i]) or 0;
			local value = tbAward["value"..i] or "";
			table.insert(tbType, nType);
			table.insert(tbValue, value);

			if (nType == 1) then -- 装备
				if (value ~= nil) then
					local tbItemGDPLN = Lib:SplitStr(value, ",");

					if (tbItemGDPLN ~= nil) then
						local g = tonumber(tbItemGDPLN[1]);
						local d = tonumber(tbItemGDPLN[2]);
						local p = tonumber(tbItemGDPLN[3]);
						local l = tonumber(tbItemGDPLN[4]);
						local n = tonumber(tbItemGDPLN[5]);

						if (g ~= nil and g > 0 and 
							d ~= nil and d > 0 and 
							p ~= nil and p > 0 and 
							l ~= nil and l >= 0 and -- level 可以为0
							n ~= nil and n > 0)
						then
							for j = 1, n, 1 do
								KItem.AddPlayerItem(pPlayer, g, d, p, l);
							end
						end
					end
				end
			elseif (nType == 2) then -- 奖钱
				local nMoney = tonumber(value);
				if (nMoney > 0) then
					pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, nMoney);
				end
			elseif (nType == 3) then -- 奖经验
				local nExp = tonumber(value);
				if (nExp > 0) then
					pPlayer.AddExp(nExp);
				end
			elseif (nType == 4) then -- 奖励游龙真气
				local nYLEnergy = tonumber(value);
				if (nYLEnergy > 0) then
					pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, nYLEnergy);
				end
			end
		end
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetDTTAward", nLevel, nWaveNum, tbType, tbValue});
	end
		-- self:AwardPlayer(pPlayer, nDifficuty, tbAward);
end

function XoyoGame:CalcTotalTime(nDifficuty, nStartTime, nTeamId)
	local nTimeUsed = KGameBase.GetTime() - nStartTime;
	local szUsed = os.date(Lang.mission.str808[Lang.Idx], nTimeUsed);
	local szDesc;
	local szDifficuty = XoyoGame.LevelDesp[nDifficuty][2];
	local nRank = self:RecordTime(nDifficuty, nTimeUsed, nTeamId);
	szDesc = string.format(Lang.mission.str816[Lang.Idx], szUsed);
	--[[
	if nRank > 0 then
		szDesc = string.format("恭喜您打破了逍遥谷[%s难度]通关记录，用时:%s，排名第%d。", szDifficuty, szUsed, nRank);
	else
		szDesc = string.format("恭喜您通关逍遥谷关卡[%s难度]，用时：%s。但没有破记录。", szDifficuty, szUsed);
	end
	--]]
	local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
	if nCount == 0 then
		return;
	end
	for _, nPlayerId in pairs(tbMember) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			pPlayer.SysMsg(szDesc);
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:BroadcastMissionComplete", "UI_TASKTIPS", "Begin", szDesc}); --zzx
		end		
	end
end

-- 创建管理对象(回调)
function XoyoGame:CreateManager_GS2(nMapId, tbData)
	if self.tbManager and self.tbManager[nMapId] then
		self.tbManager[nMapId]:Init(nMapId, tbData);
	end
end

-- 同步关卡数据(队伍数)
function XoyoGame:SyncGameData_GS2(nGameId, nData)
	if self.tbGameIdToManager[nGameId] then
		local nManagerId = self.tbGameIdToManager[nGameId]
		if self.tbManager[nManagerId] and self.tbManager[nManagerId].tbData then
			self.tbManager[nManagerId].tbData[nGameId] = nData;
		end
	end
end

-- 队伍减少
function XoyoGame:ReduceTeam_GS2(nGameId)
	if self.tbGameIdToManager[nGameId] then
		local nManagerId = self.tbGameIdToManager[nGameId]
		if self.tbManager[nManagerId] and self.tbManager[nManagerId].tbData and self.tbManager[nManagerId].tbData[nGameId] then
			self.tbManager[nManagerId].tbData[nGameId] = self.tbManager[nManagerId].tbData[nGameId] - 1;
		end
	end
end

function XoyoGame:JoinNextGame(pPlayer)
    self.tbGame[1]:JoinNextGame(pPlayer);
end

-- 创建关卡对象
function XoyoGame:CreatGame()
	--GCExcute({"XoyoGame:ApplySyncData"}); --zzx
	local tbNpcLevel = Lib:LoadTabFile(XoyoGame.NPC_LEVEL_FILE);
	for _, tbInfo in pairs(tbNpcLevel) do
		XoyoGame.tbNpcLevel[tonumber(tbInfo.nBase)] = tbInfo;
	end
	
	--XoyoGame.nBroadcastTimerId = Timer:Register(Env.GAME_FPS * 30, XoyoGame.BroadcastRank, XoyoGame); --zzx
	local tbLoadedCity = {};
	for nManagerMap,_ in pairs(self.MANAGER_GROUP) do
		--if IsMapLoaded(nManagerMap) == 1 then --zzx
			self.tbManager[nManagerMap] = Lib:NewClass(self.BaseManager);
			--GCExcute{"XoyoGame:CreateManager_GC", nManagerMap}; --zzx
		--end
	end
	for nCityMapId, tbIndexMap in pairs(self.MAP_GROUP) do
		--if IsMapLoaded(nCityMapId) == 1 and IsMapLoaded(tbIndexMap[1]) == 1 then -- zzx
			self.tbGame[nCityMapId] = Lib:NewClass(self.BaseGame);
			self.tbGame[nCityMapId]:InitGame(tbIndexMap, nCityMapId);
			for _, nMapId in pairs(tbIndexMap) do
				self.tbMap2Game[nMapId] = self.tbGame[nCityMapId];
			end
			--GCExcute{"XoyoGame:SyncGameData_GC", nCityMapId, 0}; --zzx
		--end
	end
end

function XoyoGame:GetPlayerRoom(nPlayerId)
	local tbGame = self:GetPlayerGame(nPlayerId);
	if tbGame then
		return tbGame:GetPlayerRoom(nPlayerId);
	end
end

function XoyoGame:GetPlayerGame(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer or not self.tbMap2Game then
		return;
	end
	
	--local nMapId, _, _ = pPlayer.GetWorldPos();
	--local tbGame = self.tbMap2Game[nMapId];
	local tbGame = pPlayer.GetTempTable("XoyoGame").tbGame;
	return tbGame;
end

function XoyoGame:__close()
	for _, tbGame in pairs(self.tbGame) do
		tbGame:CloseGame();
	end
	XoyoGame.BaseGame:CloseGame(); -- TestRoom/TestPKRoom 用
end

function XoyoGame:LockManager()
	for _, tbManager in pairs(self.tbManager) do
		if tbManager.tbData then
			for i, nData in pairs(tbManager.tbData) do
				tbManager.tbData[i] = nil; 
			end
		end
	end
end

function XoyoGame:StartGame_GS2()
	for i, tbGame in pairs(self.tbGame) do
		tbGame:StartNewGame();
	end
	return 0; -- 返回0表示让定时器只调用一次
end

function XoyoGame:NpcUnLock(pNpc)
	local tbTmp = pNpc.GetTempTable("XoyoGame")
	if not tbTmp then
		return 0;
	end
	if (not tbTmp.tbRoom) or (not tbTmp.nLock) then
		return 0;
	end
	if not tbTmp.tbRoom.tbLock[tbTmp.nLock] then
		return 0;
	end
	tbTmp.tbRoom.tbLock[tbTmp.nLock]:UnLockMulti();
	
	-- KK:传递给这个Instance,记录杀敌数改变
	-- 判断锁的杀敌数要求是否大于0, 是则传送TargetInfo
	if (tbTmp.tbRoom.tbLock[tbTmp.nLock]:IsClose() ~= 1) then
		if (tbTmp.tbRoom.tbLock[tbTmp.nLock].nInitMultiNum > 0) then
			-- 更新targetInfo，显示杀人数量
			tbTmp.tbRoom:SetTargetInfo(-1, tbTmp.tbRoom.szTargetInfo, tbTmp.nLock);
		end
	end
end

function XoyoGame:NpcClearLock(pNpc)
	local tbTmp = pNpc.GetTempTable("XoyoGame")
	if not tbTmp then
		return 0;
	end
	tbTmp.tbRoom = nil;
	tbTmp.nLock = nil;
end

function XoyoGame:IsInLock(pNpc)
	local tbTmp = pNpc.GetTempTable("XoyoGame")
	if not tbTmp then
		return 0;
	end
	if (not tbTmp.tbRoom) or (not tbTmp.nLock) then
		return 0;
	end
	return 1;
end

-- 增加今日完成关卡的次数
function XoyoGame:AddTodayMissionTimes(pPlayer)
	local nTimes = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.FINISH_TASK_ID);
	local nDay = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.LAST_FINISH_DAY);
	local nToday = Lib:GetLocalDay(KGameBase.GetTime());
	if nDay ~= nToday then
		nTimes = 0;
	end
	nTimes = nTimes + 1;
	pPlayer.pTask.DirectSetTask(self.FINISH_TASK_GID, self.FINISH_TASK_ID, nTimes);
	pPlayer.pTask.DirectSetTask(self.FINISH_TASK_GID, self.LAST_FINISH_DAY, nToday);
end

-- 得到今日完成关卡的次数
function XoyoGame:GetTodayMissionTimes(pPlayer)
	local nTimes = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.FINISH_TASK_ID);
	local nDay = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.LAST_FINISH_DAY);
	local nToday = Lib:GetLocalDay(KGameBase.GetTime());
	if nDay ~= nToday then
		nTimes = 0;
	end
	return nTimes;
end

if MODULE_GAMESERVER then
	XoyoGame:Init();
	--ServerEvent:RegisterServerStartFunc(XoyoGame.CreatGame, XoyoGame);
end
