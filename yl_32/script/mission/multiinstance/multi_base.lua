
Include("script/mission/mission.lua");
Include("script/mission/baselock.lua")
Include("script/mission/multiinstance/multi_def.lua")
-- 副本

MultiInstance.BaseGame = Mission:New();
local BaseGame = MultiInstance.BaseGame;
local tbEventLock = Lib:NewClass(Lock.tbBaseLock);

BaseGame.EVENT_PROC  =
{
    [MultiInstance.DO_SCRIPT]	= "ExcuteScript",
    [MultiInstance.ADD_NPC]		= "AddNpc",
    [MultiInstance.DEL_NPC]		= "DelNpc",
	[MultiInstance.BLACK_MSG]	= "BlackMsg",
	[MultiInstance.NPC_BUBBLE] 	= "NpcBubble",
	[MultiInstance.TARGET_INFO]	= "SyncTargetInfo",
	[MultiInstance.TIME_INFO] 	= "SyncTimeInfo",
	[MultiInstance.REVIVE_INFO] = "ReviveInfo",
	[MultiInstance.CLEAR_BUFF] = "ClearPlayerBuff",
	[MultiInstance.CLEAR_NPC_BUFF] = "ClearNpcBuff",
	[MultiInstance.SET_TRAP_INVALID] = "SetTrapInvalid",		-- 设置trap点失效
	[MultiInstance.SHOW_NOTICE_MSG] = "ShowNoticeMsg",	-- 公告
	[MultiInstance.SHOW_NOTICE_STATE] = "ShowNoticeState",	-- 公告显示某种状态
	[MultiInstance.REMOVE_NOTICE_STATE] = "RemoveNoticeState",	-- 删除公告状态
	[MultiInstance.DEL_ALL_NPC]	= "DelAllNpc",
};

function tbEventLock:InitEventLock(tbInstance, nTime, nMultiNum, tbStartEvent, tbUnLockEvent)
	self:InitLock(nTime, nMultiNum);
	self.tbInstance = tbInstance;
	self.tbUnLockEvent 	= tbUnLockEvent;
	self.tbStartEvent 	= tbStartEvent;
end

function tbEventLock:OnUnLock()
	if self.tbInstance and self.tbUnLockEvent then
		for i = 1, #self.tbUnLockEvent do
			self.tbInstance:OnEvent(unpack(self.tbUnLockEvent[i]));
		end
	end
	local tbInstance = self.tbInstance;
	assert(tbInstance);
	
	-- 成功完成
	if tbInstance.bFinish == 1 then
		for _, nPlayerId in ipairs(tbInstance.tbGamePlayers) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId);
			if pPlayer then
				KInstance.SetMultiMapPassed(pPlayer, tbInstance.nMapId);-- 设置已通关状态
				MultiInstance:GivePlayerAward(pPlayer);
				--KInstance.SetMultiState(pPlayer, 0);
			end
		end
		
		--MultiInstance:CloseGame(tbInstance.nTeamId);
	end
	
	-- 失败
	if tbInstance.bFailFinish == 1 then
		for _, nPlayerId in ipairs(tbInstance.tbGamePlayers) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId);
			if pPlayer then
				-- 弹1星面板
				local nStar = 1;
				MultiInstance:GivePlayerAward(pPlayer, nStar);
				pPlayer.SysMsg(Lang.mission.str978[Lang.Idx]);
			end
		end
		Timer:Register(5 * Env.GAME_FPS, MultiInstance.DelayCloseGame, MultiInstance, tbInstance.nTeamId)
	end
end

function tbEventLock:OnStartLock()
	if self.tbInstance and self.tbStartEvent then
		for i = 1, #self.tbStartEvent do
			self.tbInstance:OnEvent(unpack(self.tbStartEvent[i]));
		end
	end
end

function BaseGame:UnInitGame()
	for _, tbLockInfo in pairs(self.tbLock) do
		if tbLockInfo.nTimerId > 0 then
			Timer:Close(tbLockInfo.nTimerId);
		end
	end
end

function BaseGame:InitGame(nMapId, nSceneId, nTeamId, tbInstanceSetting)
	self.nMapId = nMapId;
    self.nSceneId = nSceneId;
	self.nTeamId = nTeamId;
	self.nMultiLevel = tbInstanceSetting.nMultiLevel;
	self.nDifficulty = tbInstanceSetting.nDifficulty;
    self.tbLock = {};
    self.tbNpcGroup = {};
    self.tbAddNpcTimer = {};
	self.tbDelRegenerateNpc = {};		-- 删掉的重生npc
	self.dwStartTime = KGameBase.GetTime();
	self.tbDeathTimes = {};
	self:Open();
	
	self.tbInstanceSetting = tbInstanceSetting;
	for i, tbLockSetting in pairs(tbInstanceSetting.LOCK) do
		self.tbLock[i] = Lib:NewClass(tbEventLock);
		self.tbLock[i].nLockId = i;
		self.tbLock[i]:InitEventLock(self, tbLockSetting.nTime * Env.GAME_FPS, tbLockSetting.nNum, tbLockSetting.tbStartEvent, tbLockSetting.tbUnLockEvent);
	end
	for i, tbLockSetting in ipairs(tbInstanceSetting.LOCK) do -- 保证解锁顺序
		for _, verPreLock in pairs(tbLockSetting.tbPrelock) do
			if type(verPreLock) == "number" then
				self.tbLock[i]:AddPreLock(self.tbLock[verPreLock]);
			elseif type(verPreLock) == "table" then
				local tbPreLock = {}
				for j = 1, #verPreLock do
					if self.tbLock[verPreLock[j]] then
						table.insert(tbPreLock, self.tbLock[verPreLock[j]]);
					end
				end
				self.tbLock[i]:AddPreLock(tbPreLock);
			else
				return 0;
			end
		end
	end
	self.nMapId = nMapId;
	self.nEndTime = KGameBase.GetTime() + tbInstanceSetting.nTimeLimit;
	
	local nEndTimerId = Timer:Register(tbInstanceSetting.nTimeLimit * Env.GAME_FPS, self.OnTimeOut, self);
	self.nEndTimerId = nEndTimerId;
end

function BaseGame:OnTimeOut()
	for _, nPlayerId in ipairs(self.tbGamePlayers) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		--MultiInstance:SetFailedFlag(pPlayer);
	end
	self:CloseGame(1);
	return 0;
end

function BaseGame:_JoinPlayer(pPlayer)
	if not self.tbGamePlayers then
		self.tbGamePlayers = {};
	end
	table.insert(self.tbGamePlayers, pPlayer.dwId);
	self:JoinPlayer(pPlayer, 1);
end

function BaseGame:StartGame()
	self.tbLock[1]:StartLock();
	return 0;
end

function BaseGame:DoLeaveGame(pPlayer, bReturn)
	-- 未完成副本
	if self.bFinish ~= 1 and self.bFailFinish ~= 1 then
		KInstance.SetMultiState(pPlayer, -1);
	end
	
	for _, nPlayerId in ipairs(self.tbGamePlayers) do
		if nPlayerId == pPlayer.dwId then
			table.remove(self.tbGamePlayers, _);
			break;
		end
	end
	
	if bReturn and bReturn == 1 then
		local nStar = 0;
		if self.bFinish == 1 or self.bFailFinish == 1 then
			nStar = MultiInstance:CalcRushStar(pPlayer);
		end
		
		--pPlayer.ReturnToLastServer(nStar);
		pPlayer.ReturnToLastCity();
	end
	
	if #self.tbGamePlayers <= 0 then
		self:CloseGame();
	end
end

function BaseGame:OnLogout(pPlayer)
	self:DoLeaveGame(pPlayer);
end

function BaseGame:OnPlayerDeath(pPlayer)
	-- 最后一个人死亡后也不退出
	--[[
	local bAlive = 0;
	for _, nPlayerId in ipairs(self.tbGamePlayers) do
		local pMember = KGameBase.GetPlayerById(nPlayerId);
		if pMember.GetMoveState() ~= Character.MOVE_STATE_ON_DEATH then
			bAlive = 1;
		end
	end 
	
	-- 全部阵亡就退出当前副本
	if bAlive == 0 then
		MultiInstance:CloseGame(self.nTeamId);
	end
	--]]
	if not self.tbDeathTimes[pPlayer.dwId] then
		self.tbDeathTimes[pPlayer.dwId] = 0;
	end
	self.tbDeathTimes[pPlayer.dwId] = self.tbDeathTimes[pPlayer.dwId] + 1;
end

function BaseGame:GetDeathTimes(pPlayer)
	local nDeathTimes = 0;
	if self.tbDeathTimes[pPlayer.dwId] then
		nDeathTimes = self.tbDeathTimes[pPlayer.dwId];
	end
	return nDeathTimes;
end

function BaseGame:CloseGame(bReturn)
	self:SyncTargetInfo("");
	
	self:UnInitGame();
	
	if self.nEndTimerId then
		Timer:Close(self.nEndTimerId);
	end
	
	for _, nPlayerId in ipairs(self.tbGamePlayers) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		
		if bReturn and bReturn == 1 then
			pPlayer.ReturnToLastCity();
			--pPlayer.ReturnToLastServer();
		end
	end
	
	Timer:Register(3 * Env.GAME_FPS, self.ResetMap, self, self.nMapId, self.nSceneId);
	
	MultiInstance.tbGames[self.nTeamId] = nil;
	
	-- 跨服要加上
	--local nTeamLeaderId = KTeam.GetTeamLeader(self.nTeamId);
	--KTeam.DisbandTeam(nTeamLeaderId, self.nTeamId);
	
	return 0;
end

function BaseGame:ResetMap(nMapId, nSceneId)
	KScene.ResetMap(nMapId, nSceneId);
	return 0;
end

function BaseGame:OnEvent(nEventType, ...)
	if self.EVENT_PROC[nEventType] and self[self.EVENT_PROC[nEventType]] then
		self[self.EVENT_PROC[nEventType]](self, ...);
	else
		print("Undefind EventType ", nEventType, ...);
	end
end

function BaseGame:GroupPlayerExcute(fnExcute)
	for _, nPlayerId in ipairs(self.tbGamePlayers) do
        local pPlayer = KGameBase.GetPlayerById(nPlayerId)
        if pPlayer then
            fnExcute(pPlayer);
        end
    end
end

function BaseGame:ExcuteScript(szCmd)
	MultiInstance.tbExcuteTable = self;
	local fnExc = loadstring("local self = MultiInstance.tbExcuteTable;"..szCmd);
	if fnExc then
		xpcall(fnExc, Lib.ShowStack);
	end
	MultiInstance.tbExcuteTable = nil;
end

-- 同步剩余时间信息
function BaseGame:SyncTimeInfo(szTimeInfo, ...)
	local varLock;
	local nTimeFrame; -- 时间
	
	-- 变量读取
	if #arg == 0 then varLock = -1;  
	else varLock = unpack(arg); end
	
	if type(varLock) ~= "number" then
		print("[Lua:BaseGame:BroadcastTimeInfo]varLock非Number");
		assert(false);
	end
	
	-- 如果有填锁，锁时间，没填锁，用活动时间
	if (varLock ~= -1 and self.tbLock[varLock]) then
		nTimeFrame = self.tbInstanceSetting.LOCK[varLock].nTime * Env.GAME_FPS;
	else
		nTimeFrame = (self.nEndTime - KGameBase.GetTime()) * Env.GAME_FPS;
	end
	
	self.szTimeInfo = szTimeInfo;  -- set self
	local fnExcute = function(pPlayer)
		local nSec = nTimeFrame / Env.GAME_FPS;
		--pPlayer.SysMsg(string.format(szTimeInfo, Lib:TimeDesc(nSec)));
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", szTimeInfo, nTimeFrame});
	end
	self:GroupPlayerExcute(fnExcute);
end

-- 同步即时战报信息给所有玩家
function BaseGame:SyncTargetInfo(szInfo, ...)
	local varLock; -- 读取varLock， 哪个锁
	if #arg == 0 then varLock = -1;   -- 没有参数
	else varLock = unpack(arg);	end -- 获取第一个
	
	self.nCurLock = varLock;
	
	self.szTargetInfo = szInfo;  -- 设置targetInfo
	if (varLock ~= -1 and self.tbLock[varLock]) then  -- 存在该锁
		local nCurrentNum = self.tbLock[varLock].nMultiNum; -- 当前杀敌数
		local nInitNum = self.tbLock[varLock].nInitMultiNum; -- 总杀敌数
		szInfo = string.format("%s %d/%d", self.szTargetInfo, nInitNum - nCurrentNum, nInitNum);
	end
	
	local fnExcute = function (pPlayer)
		--pPlayer.SysMsg(szInfo);
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTargetInfo", szInfo});
	end
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:ShowNoticeMsg(szMsg)
	local fnExcute = function(pPlayer)
		pPlayer.SendNoticeMsg(szMsg);
	end
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:ShowNoticeState(szMsg)
	local fnExcute = function(pPlayer)
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeState", szMsg});
	end
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:RemoveNoticeState(szMsg)
	local fnExcute = function(pPlayer)
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:RemoveNoticeState", szMsg});
	end
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:ReviveInfo(bWait, bUseItem, bReturn, nScene, ...)
	local fnExcute = function (pPlayer)
		Player:SetTempRevive(pPlayer, bWait, bUseItem, bReturn, nScene, unpack(arg));
	end
	self:GroupPlayerExcute(fnExcute);	
end

function BaseGame:ClearPlayerBuff(nBuffId)
	local fnExcute = function (pPlayer)
		pPlayer.pBufferEffect.ClearBuff(nBuffId);
	end
	
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:SetTrapInvalid(nTrapTemplateId, nInvalid)
	local fnExcute = function (pPlayer)
		if type(nTrapTemplateId) == "table" then
			nTrapTemplateId = nTrapTemplateId[KUnify.MathRandom(1, #nTrapTemplateId)];
		end
		
		if not nInvalid or nInvalid == 1 then
			pPlayer.SetTrapInvalid(nTrapTemplateId);
		else
			pPlayer.RemoveTrapInvalid(nTrapTemplateId);
		end
	end
	
	self:GroupPlayerExcute(fnExcute);
end

function BaseGame:NpcBubble(szGroup, szMsg, delay)
	local fnExcute = function (pPlayer)
	    local pOldMe = me;
	    me = pPlayer;
		for _, nId in pairs(self.tbNpcGroup[szGroup]) do
			Dialog:NpcBubble(nId, szMsg, delay * Env.GAME_FPS);
		end
	    me = pOldMe;
	end
	
	self:GroupPlayerExcute(fnExcute);
end

-- 把NPC加到锁里
function BaseGame:AddNpcInLock(pNpc, nLock)
	if not nLock or nLock <= 0 then
		return 0;
	end
	local tbTmp = pNpc.GetTempTable("MultiInstance");
	tbTmp.nLock = nLock;
end

-- 把NPC加到组里
function BaseGame:AddNpcInGroup(pNpc, szGroup)
	if not self.tbNpcGroup[szGroup] then
		self.tbNpcGroup[szGroup] = {};
	end
	if pNpc then
		table.insert(self.tbNpcGroup[szGroup], pNpc.dwId);
	end
end

-- 把npc刷到不同位置上
-- nNum: 数量
-- tbIndex: npc索引表
function BaseGame:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)
	local pNpc;
	local tbNpcId = {};
	local nIndex;
	if #tbIndex == 1 then
		nIndex = tbIndex[1];
	end
	
	local tbNpcInfo;
	if nIndex then
	    tbNpcInfo = self.tbInstanceSetting.NPC[nIndex];
	else
	    tbNpcInfo = self.tbInstanceSetting.NPC[tbIndex[KUnify.MathRandom(#tbIndex)]];
	end
	
	local tbNpcT = Scene.tbNpcCfg[self.nMapId][tbNpcInfo.nTemplate];
	local tbNpc = {};
	tbNpc.tbPosList = tbNpcT.tbPosList;
	tbNpc.nTemplateId = tbNpcT.nTemplateId;
	tbNpc.nRegenerationTime = tbNpcT.nRegenerationTime;
	tbNpc.nNormalAI = tbNpcT.nNormalAI;
	tbNpc.nCount = nNum;
	
	local tbRetNpcs = Scene:_CallOneNpc(tbNpc, self.nSceneId, self.nMapId, nil, szPointName, self.nTeamId, szGroup);
	for _, pNpc in ipairs(tbRetNpcs) do
	    KCharacter.ResetNpcIntensity(pNpc, nPropId, nLevel);
	    self:AddNpcInLock(pNpc, nLock);
		self:AddNpcInGroup(pNpc, szGroup);
		table.insert(tbNpcId, pNpc.dwId);
		local tbTmp = pNpc.GetTempTable("MultiInstance")
		tbTmp.tbInstance = self;
	end
	
	return tbNpcId;
end

function BaseGame:GetAverageLevel()	
  local nAverageLevel = 0
  for _, nPlayerId in ipairs(self.tbGamePlayers) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			nAverageLevel = nAverageLevel + pPlayer.GetLevel()
		end
	end
	
	nAverageLevel = math.ceil(nAverageLevel / #self.tbGamePlayers)
	return nAverageLevel
end

function BaseGame:__AddNpc(tbIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	if not self.tbInstanceSetting or not self.tbInstanceSetting.NPC then
		return 0;
	end
	
	local tbNpcInfo = self.tbInstanceSetting.NPC[tbIndex[1]];
	local nPropId = KCharacter.GetNpcPropId(tbNpcInfo.nTemplate);
	local nLevel = tbNpcInfo.nLevel;
	if not tbNpcInfo.nLevel or tbNpcInfo.nLevel <= 0 then
		nLevel = self:GetAverageLevel();
		nLevel = KCharacter.GetNpcMatchLevel(nPropId, nLevel);
	else
		nLevel = tbNpcInfo.nLevel;
	end
	
	if not nRemove then
		nRemove = 1;
	end
	
	local tbNpcId = self:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)

    if nFrequency then
		for _, id in ipairs(self.tbAddNpcTimer[szTimerName].tbNpcId) do -- 删除上一轮的npc
			local pNpc = KGameBase.GetNpcById(id);
			if pNpc and nRemove == 1 then
			    KGameBase.RemoveNpc(pNpc.dwId);
			end
		end
		if nTimes == 1 then
			self.tbAddNpcTimer[szTimerName].nTimerId = nil;
			self.tbAddNpcTimer[szTimerName].tbNpcId = tbNpcId;
		else
			self.tbAddNpcTimer[szTimerName].nTimerId = Timer:Register(nFrequency * Env.GAME_FPS, self.__AddNpc, self, tbIndex, nNum, nLock, szGroup, szPointName, nTimes - 1, nFrequency, szTimerName, nRemove);
			self.tbAddNpcTimer[szTimerName].tbNpcId = tbNpcId;
		end
	end
	
	return 0;
end

-- 添加NPC
-- varIndex: nTemplateId or {id1,id2,id3...}, 可以是一个或者一组模板id
-- nNum: 刷几个
-- nLock
-- szGroup: 新刷出来的npc所属的组的名字
-- szPointName: 使用哪一组点刷怪
-- nTimes, nFrequency, szTimerName: 循环刷怪用，参数为：次数，隔多久刷一次，timer名字	
function BaseGame:AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	if (nTimes or nFrequency or szTimerName) and not (nTimes > 1 and nFrequency > 0 and szTimerName) then
		assert(false);
	end
	
	if type(varIndex) == "number" then
		varIndex = {varIndex};
	end
	for _, id in ipairs(varIndex) do
		local tbNpcInfo = self.tbInstanceSetting.NPC[id];
		assert(tbNpcInfo);
	end
	
	if nFrequency and not self.tbAddNpcTimer[szTimerName] then
		self.tbAddNpcTimer[szTimerName] = {};
		self.tbAddNpcTimer[szTimerName].tbNpcId = {};
	end

	self:__AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove);
end

-- 删除特定组的NPC
function BaseGame:DelNpc(szGroup, szTimerName)
	if not self.tbNpcGroup[szGroup] then
		return 0;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc and pNpc.IsDead() ~= 1 then
			KGameBase.RemoveNpc(pNpc.dwId);
		end
	end
	self.tbNpcGroup[szGroup] = nil;
	
	if szTimerName and self.tbAddNpcTimer[szTimerName] then
		Timer:Close(self.tbAddNpcTimer[szTimerName].nTimerId);
	end
	
	self.tbDelRegenerateNpc[szGroup] = 1;
end

-- 删除所有NPC
function BaseGame:DelAllNpc()
	for szGroup, tbGroup in pairs(self.tbNpcGroup) do
		for _, nId in pairs(tbGroup) do
			local pNpc = KGameBase.GetNpcById(nId);
			if pNpc and pNpc.IsDead() ~= 1 then
				KGameBase.RemoveNpc(pNpc.dwId);
			end
		end
		
		self.tbDelRegenerateNpc[szGroup] = 1;
	end
	
	self.tbNpcGroup = {};
	
	for szTimerName, tbTimer in pairs(self.tbAddNpcTimer) do
		Timer:Close(tbTimer.nTimerId);
	end
end

function BaseGame:ClearNpcBuff(szGroup, nBuffId)
	if not self.tbNpcGroup[szGroup] then
		return;
	end
	
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
			pNpc.pBufferEffect.ClearBuff(nBuffId);
		end
	end
end

function BaseGame:GetNpcByGroup(szGroup)
	local nNpcId = self.tbNpcGroup[szGroup][1];
	assert(nNpcId);
	local pNpc = KGameBase.GetNpcById(nNpcId);
	return pNpc;
end


-- 黑条消息
function BaseGame:BlackMsg(szMsg, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.BlackMsg, self, szMsg);
		return;
	end
	
	local fnExcute = function(pPlayer)
		Dialog:SendBlackBoardMsg(pPlayer, szMsg)
	end
	
	self:GroupPlayerExcute(fnExcute);
	
	return 0;
end

-- 以下函数都可以派生

function BaseGame:CalcRushStar()
	local nStar = 0;
	local nDeathTimes = MultiInstance.tbDeathTimes[pPlayer.dwId];
	local nLastTime = KGameBase.GetTime() - KInstance.GetMultiStartTime(pPlayer);
	
	if nLastTime <= (10 * 60) and nDeathTimes < 3 then
		nStar = 5;
	elseif nLastTime <= (15 * 60) and nDeathTimes < 6 then
		nStar = 4;
	elseif nLastTime < (20 * 60) then
		nStar = 3;
	elseif nLastTime < (25 * 60) then
		nStar = 2;
	else
		nStar = 1;
	end
	
	return nStar;
end

function BaseGame:OnKillNpc(pKiller, pNpc)
	
end
