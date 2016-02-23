
Include("script/mission/mission.lua");
Include("script/mission/baselock.lua")
Include("script/mission/instance/instance_def.lua")
-- 副本

Instance.BaseGame = Mission:New();
local BaseGame = Instance.BaseGame;
local tbEventLock = Lib:NewClass(Lock.tbBaseLock);

BaseGame.EVENT_PROC  =
{
    [Instance.MOVIE_DIALOG] = "MovieDialog",
    [Instance.DO_SCRIPT]	= "ExcuteScript",
    [Instance.ADD_NPC]		= "AddNpc",
    [Instance.DEL_NPC]		= "DelNpc",
	[Instance.BLACK_MSG]	= "BlackMsg",
	[Instance.NPC_BUBBLE] 	= "NpcBubble",
	[Instance.ME_BUBBLE]	= "MeBubble",
	[Instance.CHANGE_TRAP] = "ChangeTrap",
	[Instance.HOOK_TRAP] = "TrapHook",
	[Instance.TARGET_INFO]	= "SetTargetInfo",
	[Instance.TIME_INFO] = "BroadcastTimeInfo",
	[Instance.SHOW_REWARD_PANEL] = "ShowRewardPanel",
	[Instance.SET_TASK] = "SetTaskValue",
	[Instance.REVIVE_INFO] = "ReviveInfo",
	[Instance.CLEAR_BUFF] = "ClearPlayerBuff",
	[Instance.NOVICE_GUIDE] = "ShowNoviceGuide",
	[Instance.SHOW_NOTICE_MSG] = "ShowNoticeMsg",
	[Instance.SHOW_NOTICE_STATE] = "ShowNoticeState",
	[Instance.REMOVE_NOTICE_STATE] = "RemoveNoticeState",
	[Instance.UNLOCK_GOD] = "UnlockGod",
};

function tbEventLock:InitEventLock(tbInstance, nTime, nMultiNum, tbStartEvent, tbUnLockEvent)
	self:InitLock(nTime, nMultiNum);
	self.tbInstance 	= tbInstance;
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

	if tbInstance and tbInstance.bOver == 1 then
		tbInstance:CloseGame();
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
	if self.tbInstanceSetting.OnUnInitCallback ~= nil then  -- 存在副本回调
		Lib:CallBack({self.tbInstanceSetting.OnUnInitCallback, self.nOwnerId})  -- 参数是玩家id
	end
end

function BaseGame:InitGame(nMapId, nSceneId, nOwnerId, tbInstanceSetting)
	self.nMapId = nMapId;
    self.nSceneId = nSceneId;
	self.nEnemyNum = tbInstanceSetting.nEnemyNum;
	self.nOwnerId = nOwnerId;
    self.bOver = 0; -- 结束标志
    self.tbLock = {};
    self.tbNpcGroup = {};
    self.tbAddNpcTimer = {};
	self.tbPlayerGroup = {};
	self.tbDelRegenerateNpc = {};		-- 删掉的重生npc
	self.nDeathTimes = 0;	-- 死亡次数
    self.tbMisCfg =
	{
		tbLeavePos	= {[1] = {}},	-- 离开坐标
		tbDeathRevPos = {},		-- 死亡重生点
		nDeathPunish = 1,
		nInLeagueState = 1,
	}
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
	self.nEndTime = KGameBase.GetTime() + Instance.END_TIME;

	-- by kk,  起始回调，副本开启后调用回调，可以自定义副本特殊功能, 如petisland
	if self.tbInstanceSetting.OnInitCallback ~= nil then  -- 存在副本回调
		Lib:CallBack({self.tbInstanceSetting.OnInitCallback, nOwnerId})  -- 参数是玩家id
	end
end

function BaseGame:JoinTeam(nTeamId)
	local tbMemberList, nCount = KTeam.GetTeamMemberList(self.tbTeam[i].nTeamId);
	for j = 1, nCount do
		local pPlayer = KGameBase.GetPlayerById(tbMemberList[j]);
		self.tbMisCfg.tbLeavePos[1][pPlayer.dwId] = pPlayer.GetPosition();
		self:JoinPlayer(pPlayer, 1);
	end
end

function BaseGame:_JoinPlayer(pPlayer)
	table.insert(self.tbPlayerGroup, pPlayer.dwId);
	self:JoinPlayer(pPlayer, 1);
end

function BaseGame:StartGame()
	self.tbLock[1]:StartLock();
	return 0;
end

function BaseGame:HasPlayer(nPlayerId)
	for i = 1, #self.tbPlayerGroup do
		if self.tbPlayerGroup[i] == nPlayerId then
			return 1;
		end
	end
	return 0;
end

function BaseGame:AwardPlayer(pPlayer, tbAward)
	local nTimes = Instance:GetTodayInstanceTimes(pPlayer);
	local tbAwardPercent = 100;

	if nTimes > Instance.TIMES_FULL_REWARD then
		tbAwardPercent = 80;	-- 超过次数只给80%奖励
	end

	local nExp = math.floor(tbAward.ExpAward * tbAwardPercent / 100);
	local nMoney = math.floor(tbAward.CoinAward * tbAwardPercent / 100);

	-- 给经验
	pPlayer.AddExp(nExp);
	-- 给钱
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, nMoney);
	-- 每天前6次有额外物品奖励
	if nTimes <= Instance.TIMES_FULL_REWARD then
		KItem.AddPlayerItem(pPlayer, tbAward.ItemGerne, tbAward.ItemDetail, tbAward.ItemParticularType, tbAward.ItemLevel);
	end
end

function BaseGame:FinishAward(pPlayer)
	local tbInstanceAward = Instance.tbInstanceAward;

	if not tbInstanceAward[self.nMapId] then
		return;
	end

	local tbAward = tbInstanceAward[self.nMapId][pPlayer.GetLevel()];
	if not tbAward then
		return;
	end
	Instance:AddTodayInstanceTimes(pPlayer); 	-- 增加副本完成次数
	self:AwardPlayer(pPlayer, tbAward);
end

function BaseGame:LeaveInstance(pPlayer)
	Instance.tbPlayerInstances[pPlayer.dwId] = nil;

	local nMapId = pPlayer.dwSceneTemplateId;
	if Instance.tbSetting[nMapId] then
		local tbPos = Instance.tbSetting[nMapId].tbLeavePos;
		if tbPos == nil then
			pPlayer.ReturnToLastCity();
		else
			pPlayer.TeleportTo(tbPos[1], tbPos[2], tbPos[3], tbPos[4]);
	    end
	end
end

function BaseGame:CloseGame()
	self:SetTargetInfo(-1, "");

	for _, nId in ipairs(self.tbPlayerGroup) do
        local pPlayer = KGameBase.GetPlayerById(nId)
        if pPlayer then
            self:KickPlayer(pPlayer);
			if self.bOver == 1 then
				self:FinishAward(pPlayer); -- 给通副本奖励
			end
			self:CloseTimeInfo(pPlayer);
			self:LeaveInstance(pPlayer);
			--pPlayer.LeaveTeam();		-- 这里千万不要加leaveteam, 单人副本没必要离队了
        end
    end

	self:UnInitGame();
	if self.nTimerId then
		Timer:Close(self.nTimerId);
	end

	Timer:Register(3 * Env.GAME_FPS, self.ResetMap, self, self.nMapId, self.nSceneId);
end

function BaseGame:ResetMap(nMapId, nSceneId)
	KScene.ResetMap(nMapId, nSceneId);
	return 0;
end

function BaseGame:OnTrap(pPlayer)
	if #self.tbPlayerGroup > 1 then
		self:DeleteFromGroup(pPlayer);		-- 从tbPlayerGroup中删除
		self:CloseTimeInfo(pPlayer);
        self:KickPlayer(pPlayer);
		self:LeaveInstance(pPlayer);
        -- 在副本中走trap点会离开副本，需要解散队伍
        pPlayer.LeaveTeam();
	else
		self:CloseGame();
	end
end

function BaseGame:OnEvent(nEventType, ...)
	if self.EVENT_PROC[nEventType] and self[self.EVENT_PROC[nEventType]] then
		self[self.EVENT_PROC[nEventType]](self, ...);
	else
		print("Undefind EventType ", nEventType, ...);
	end
end

-- 遍历在服务器副本里的所有玩家
function BaseGame:GroupPlayerExcute(fnExcute, nPlayerGroup)
	for _, nPlayerId in pairs(self.tbPlayerGroup) do
        local pPlayer = KGameBase.GetPlayerById(nPlayerId)
        if pPlayer then
            fnExcute(pPlayer);
        end
    end
end

function BaseGame:DeleteFromGroup(pPlayer)
	for nIdx, nPlayerId in ipairs(self.tbPlayerGroup) do
		if nPlayerId == pPlayer.dwId then
			table.remove(self.tbPlayerGroup, nIdx);
		end
    end
end

-- 执行脚本 -- TODO
function BaseGame:ExcuteScript(szCmd)
	Instance.tbExcuteTable = self;
	local fnExc = loadstring("local self = Instance.tbExcuteTable;"..szCmd);
	if fnExc then
		xpcall(fnExc, Lib.ShowStack);
	end
	Instance.tbExcuteTable = nil;
end

-- 电影模式对话
function BaseGame:MovieDialog(nPlayerGroup, szMovie, delay)
	if delay and delay > 0 then
		Timer:Register(delay * Env.GAME_FPS, self.MovieDialog, self, nPlayerGroup, szMovie);
		return;
	end

	local fnExcute = function (pPlayer)
		TaskAct:NormalTalk(pPlayer, szMovie);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	return 0;
end

-- 同步副本剩余时间信息
function BaseGame:BroadcastTimeInfo(nPlayerGroup, szTimeInfo, ...)
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
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 向玩家同步副本信息
function BaseGame:SendInstanceInfo(pPlayer)
	if self.nShowReward == 1 then
		self:ShowRewardPanel(-1, 5, self.nElapseTime);
	end

	if self.szTimeInfo then
		self:BroadcastTimeInfo(-1, self.szTimeInfo)
	end

	if self.szTargetInfo then
		self:SetTargetInfo(-1, self.szTargetInfo, self.nCurLock);
	end
end

function BaseGame:OnLogin(pPlayer)
	local nLastTime = KGameBase.GetTime() - self.nStartTime;
	if nLastTime <= Instance.END_TIME then
		self:SendInstanceInfo(pPlayer);
	else
		self:LeaveInstance(pPlayer);
	end

	if self.tbInstanceSetting.OnLoginCallback ~= nil then  -- 存在副本回调
		Lib:CallBack({self.tbInstanceSetting.OnLoginCallback, self.nOwnerId})  -- 参数是玩家id
	end
end

function BaseGame:OnLogout(pPlayer)
	if self.tbInstanceSetting.OnLogoutCallback ~= nil then  -- 存在副本回调
		Lib:CallBack({self.tbInstanceSetting.OnLogoutCallback, self.nOwnerId})  -- 参数是玩家id
	end
end

function BaseGame:CloseTimeInfo(pPlayer)
	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:BroadcastMissionComplete", "UI_TASKTIPS", "Begin", Lang.mission.str1138[Lang.Idx]});
end

function BaseGame:MeBubble(nPlayerGroup, szMsg, delay)
	local fnExcute = function (pPlayer)
	    local pOldMe = me;
	    me = pPlayer;
		Dialog:PlayerBubble(szMsg, delay * Env.GAME_FPS);
	    me = pOldMe;
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:NpcBubble(nPlayerGroup, szGroup, szMsg, delay)
	local fnExcute = function (pPlayer)
	    local pOldMe = me;
	    me = pPlayer;
		for _, nId in pairs(self.tbNpcGroup[szGroup]) do
			Dialog:NpcBubble(nId, szMsg, delay * Env.GAME_FPS);
		end
	    me = pOldMe;
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 改变Trap点传送位置
function BaseGame:ChangeTrap(szClassName, ...)
	assert(Scene.tbTrapInfo[szClassName]);
	local tbTrapInfo = Scene.tbTrapInfo[szClassName];
	tbTrapInfo.Param2 = arg[1];
	tbTrapInfo.Param3 = arg[2];
	tbTrapInfo.Param4 = arg[3];
end

-- 把NPC加到锁里
function BaseGame:AddNpcInLock(pNpc, nLock)
	if not nLock or nLock <= 0 then
		return 0;
	end
	local tbTmp = pNpc.GetTempTable("Instance")
	tbTmp.nLock = nLock
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
	-- 用九天的刷怪方式
	local tbNpcT = Scene.tbNpcCfg[self.nMapId][tbNpcInfo.nTemplate];
	local tbNpc = {};
	tbNpc.tbPosList = tbNpcT.tbPosList;
	tbNpc.nTemplateId = tbNpcT.nTemplateId;
	tbNpc.nRegenerationTime = tbNpcT.nRegenerationTime;
	tbNpc.nNormalAI = tbNpcT.nNormalAI;
	tbNpc.nCount = nNum;

	local tbRetNpcs = Scene:_CallOneNpc(tbNpc, self.nSceneId, self.nMapId, nil, szPointName, self.nOwnerId, szGroup);
	for _, pNpc in ipairs(tbRetNpcs) do
	    KCharacter.ResetNpcIntensity(pNpc, nPropId, nLevel);
	    self:AddNpcInLock(pNpc, nLock);
		self:AddNpcInGroup(pNpc, szGroup);
		table.insert(tbNpcId, pNpc.dwId);
		local tbTmp = pNpc.GetTempTable("Instance")
		tbTmp.tbInstance = self;
	end
	return tbNpcId;
end

function BaseGame:GetPlayerNum()
	return #self.tbPlayerGroup;
end

-- 计算玩家等级平均值
function BaseGame:GetAverageLevel()
	local nLevel;
	-- 计算房间内玩家平均等级
	local nTotalLevel = 0;
	local nTotalPlayer = 0;

	for _, tbPlayerInfo in pairs(self.tbPlayers) do
        local pPlayer = KGameBase.GetPlayerById(tbPlayerInfo.nPlayerId)
		if pPlayer then
			nTotalLevel = nTotalLevel + pPlayer.GetLevel();
			nTotalPlayer = nTotalPlayer + 1
		end
	end

	if (nTotalPlayer ~= 0) then
		nLevel = math.ceil(nTotalLevel / nTotalPlayer);
	else
		nLevel = 50;
	end
	return nLevel;
end

function BaseGame:__AddNpc(tbIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	if not self.tbInstanceSetting or not self.tbInstanceSetting.NPC then
		return 0;
	end

	local nLevel = -1;	-- 默认值
	local tbNpcInfo = self.tbInstanceSetting.NPC[tbIndex[1]];
	local nPropId = KCharacter.GetNpcPropId(tbNpcInfo.nTemplate);
	if not tbNpcInfo.nLevel or tbNpcInfo.nLevel <= 0 then
		nLevel = self:GetAverageLevel();
		nLevel = KCharacter.GetNpcMatchLevel(nPropId, nLevel);
	else
		nLevel = tbNpcInfo.nLevel;
	end
	local tbNpcId = self:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)
	
	if not nRemove then
		nRemove = 1;
	end

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

-- 同步即时战报信息给所有玩家
function BaseGame:SetTargetInfo(nPlayerGroup, szInfo, ...)
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
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:ShowNoticeState(nPlayerGroup, szMsg)
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeState", szMsg});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:RemoveNoticeState(nPlayerGroup, szMsg)
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:RemoveNoticeState", szMsg});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:UnlockGod(nPlayerGroup, nIndex, nStep)
	local fnExcute = function (pPlayer)
		local tmpStep = pPlayer.pTask.GetTask(Instance.UNLOCK_GOD_GID, Instance.UNLOCK_GOD_STEP) or 0;
		if tmpStep < nStep then
			pPlayer.pTask.DirectSetTask(Instance.UNLOCK_GOD_GID, Instance.UNLOCK_GOD_STEP, nStep);
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:UnlockGod", nIndex, nStep});
			
			if nStep == Instance.UNLOCK_GOD_TOTAL_STEP then
				pPlayer.SetAnger(100);
			end
		else
			local pInstance = Instance:GetMyInstance(pPlayer.dwId);
			pInstance.tbLock[nIndex]:UnLockMulti();
		end
	end
	
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:ShowNoticeMsg(nPlayerGroup, szMsg)
	local fnExcute = function (pPlayer)
		pPlayer.SendNoticeMsg(szMsg);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
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

-- 黑条消息
function BaseGame:BlackMsg(nPlayerGroup, szMsg, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.BlackMsg, self, nPlayerGroup, szMsg);
		return
	end

	local fnExcute = function (pPlayer)
		Dialog:SendBlackBoardMsg(pPlayer, szMsg)
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	return 0;
end

-- 改变玩家变量值
function BaseGame:SetTaskValue(nPlayerGroup, nGroupId, nTaskId, nTaskValue)
	local fnExcute = function (pPlayer)
		pPlayer.pTask.SetTask(nGroupId, nTaskId, nTaskValue);
	end

	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:ReviveInfo(bWait, bUseItem, bReturn, nScene, ...)
	local fnExcute = function (pPlayer)
		Player:SetTempRevive(pPlayer, bWait, bUseItem, bReturn, nScene, unpack(arg));
	end
	self:GroupPlayerExcute(fnExcute, -1);
end

-- 挂靠trap点，等玩家走到这个trap点的时候执行函数
function BaseGame:TrapHook(szTrapClass, szScript)
	if self.TrapCall == nil then
		self.TrapCall = {}
	end
	self.TrapCall[szTrapClass] = szScript;
end

function BaseGame:ExcuteTrapScript(szTrapClass)
	if not self.TrapCall or not self.TrapCall[szTrapClass] then
		return
	end
	self:ExcuteScript(self.TrapCall[szTrapClass]);
	-- 执行完后删除
	self.TrapCall[szTrapClass] = nil;
end

function BaseGame:ClearPlayerBuff(nPlayerGroup, nBuffId)
	local fnExcute = function (pPlayer)
		pPlayer.pBufferEffect.ClearBuff(nBuffId);
	end

	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:ShowNoviceGuide(nPlayerGroup, nNoviceId)
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowGuideTip", nNoviceId});
	end

	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseGame:ShowRewardPanel(nPlayerGroup, delay, nETime)
	print("..BaseGame:ShowRewardPanel",nPlayerGroup, delay, nETime);
	if delay and delay > 0 then
		Timer:Register(delay, self.ShowRewardPanel, self, nPlayerGroup);
		return
	end

	local nElapseTime = 0;

	local fnExcute = function (pPlayer)
		if not nETime then
			nElapseTime = KGameBase.GetTime() - (self.nEndTime - Instance.END_TIME); -- 单位是秒
		else
			nElapseTime = nETime;
		end
		local nStar = self:CalcRushStar(pPlayer, nElapseTime);
		local pInstance = Instance:GetMyInstance(pPlayer.dwId);
		if KAward.HasAssetAward("Instance", pInstance.nMapId) == 1 then
			KAward.AddPlayerAward(pPlayer, "Instance", pInstance.nMapId, 1, 100, 0);
		end
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowTaskAwardPanel", "Instance", self.nMapId, nElapseTime, nStar, Reward.emTASK_AWARD_INSTANCE});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);

	self.nElapseTime = nElapseTime;
	self.nShowReward = 1;

	return 0;
end

-- 评判本次星级
function BaseGame:CalcRushStar(pPlayer, nLastTime)
	local nStar = 1;
	local nMaxLife = pPlayer.GetMaxLife();
	if pPlayer.GetAngerLeftTime() > 0 then
		nMaxLife = math.floor(nMaxLife / 2);
	end
	
	local nBloodPercent = math.floor(pPlayer.GetCurrentLife() * 100 / nMaxLife);
	
	local nBloodStar = 1;
	local nTimeStar = 1;
	local nDeathStar = 5;
	
	-- 计算剩余血量
	if nBloodPercent >= 70 then
		nBloodStar = 5;
	elseif nBloodPercent >= 50 then
		nBloodStar = 4;
	elseif nBloodPercent >= 30 then
		nBloodStar = 3;
	elseif nBloodPercent >= 10 then
		nBloodStar = 2;
	else
		nBloodStar = 1;
	end
	
	if nLastTime <= 5 * 60 then
		nTimeStar = 5;
	elseif nLastTime <= 10 * 60 then
		nTimeStar = 4;
	elseif nLastTime <= 15 * 60 then
		nTimeStar = 3;
	elseif nLastTime <= 20 * 60 then
		nTimeStar = 2;
	else
		nTimeStar = 1;
	end
	
	if self.nDeathTimes <= 0 then
		nDeathStar = 5;
	elseif self.nDeathTimes == 1 then
		nDeathStar = 4;
	elseif self.nDeathTimes == 2 then
		nDeathStar = 3;
	else
		nDeathStar = 2;
	end
	
	-- 取小的
	if nBloodStar < nTimeStar then
		nStar = nBloodStar;
	else
		nStar = nTimeStar;
	end
	
	if nStar > nDeathStar then
		nStar = nDeathStar;
	end
	
	return nStar;
end