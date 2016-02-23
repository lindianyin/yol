Include("script/mission/mission.lua");
Include("script/mission/baselock.lua")
Include("script/mission/activity/activitydef.lua")

Activity.ActivityStep = {}
Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.BASE] = Activity.ActivityStep -- 注册到活动工厂

local ActivityStep = Activity.ActivityStep
local tbEventLock = Lib:NewClass(Lock.tbBaseLock);

ActivityStep.EVENT_PROC  =
{
    [Activity.EVENT_TYPE.MOVIE_DIALOG] = "MovieDialog",
    [Activity.EVENT_TYPE.DO_SCRIPT]	= "ExcuteScript",
    [Activity.EVENT_TYPE.ADD_NPC]		= "AddNpc",
    [Activity.EVENT_TYPE.DEL_NPC]		= "DelNpc",
	[Activity.EVENT_TYPE.BLACK_MSG]	= "BlackMsg",
	[Activity.EVENT_TYPE.NPC_BUBBLE] 	= "NpcBubble",
	[Activity.EVENT_TYPE.ME_BUBBLE]	= "MeBubble",
	[Activity.EVENT_TYPE.CHANGE_TRAP] = "ChangeTrap",
	[Activity.EVENT_TYPE.HOOK_TRAP] = "TrapHook",
	[Activity.EVENT_TYPE.TARGET_INFO]	= "SetTargetInfo",
	[Activity.EVENT_TYPE.TIME_INFO] = "BroadcastTimeInfo",
	[Activity.EVENT_TYPE.SHOW_REWARD_PANEL] = "ShowRewardPanel",
	[Activity.EVENT_TYPE.TIMING_FUNCTION] = "TimingFunction",
	[Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT] = "AddExcAllEvent",
	[Activity.EVENT_TYPE.REMOVE_EXC_ALL_EVENT] = "RemoveExcAllEvent",
	[Activity.EVENT_TYPE.SET_RELATION] = "SetRelation",
	[Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK] = "AddPlayersInLock",
	[Activity.EVENT_TYPE.JOIN_STEP] = "JoinStep",
	[Activity.EVENT_TYPE.SHOW_NOTICE_MSG] = "ShowNoticeMsg",
	[Activity.EVENT_TYPE.REVIVE_INFO] = "ReviveInfo",
	[Activity.EVENT_TYPE.REDUCE_ALL_DEGREE] = "ReduceAllDegree",
	[Activity.EVENT_TYPE.DEL_ALL_NPC] = "DeleteAllNpc",
	[Activity.EVENT_TYPE.SET_TRAP_EFFECT] = "SetTrapEffect",
	[Activity.EVENT_TYPE.ADD_NPC_ON_TIME] = "AddNpcOnTime",
};

function tbEventLock:InitEventLock(tbActiveStep, nTime, nMultiNum, tbStartEvent, tbUnLockEvent)
	self:InitLock(nTime, nMultiNum);
	self.tbActiveStep 	= tbActiveStep;
	self.tbUnLockEvent 	= tbUnLockEvent;
	self.tbStartEvent 	= tbStartEvent;
end

function tbEventLock:OnUnLock()
	if self.tbActiveStep and self.tbUnLockEvent then
		for i = 1, #self.tbUnLockEvent do
			self.tbActiveStep:OnEvent(unpack(self.tbUnLockEvent[i]));
		end
	end
	local tbActiveStep = self.tbActiveStep;
	
	if tbActiveStep and tbActiveStep.bOver == 1 then
	    tbActiveStep:EndStep()
	end
end

function tbEventLock:OnStartLock()
	if self.tbActiveStep and self.tbStartEvent then
		for i = 1, #self.tbStartEvent do
			self.tbActiveStep:OnEvent(unpack(self.tbStartEvent[i]));
		end
	end
end

function ActivityStep:CopyTable(tbValue)
    local tbCopyValue = nil;
    if tbValue ~= nil then
        tbCopyValue = Lib:CopyTBN(tbValue);
    end
    return tbCopyValue;
end

function ActivityStep:Init(tbStep, nActivityIndex, nStepIndex)
    self.nTime = tbStep.Time;
    self.AssignType = tbStep.AssignType
    self.tbPos = self:CopyTable(tbStep.Pos);
    if tbStep.MapCopyIndex ~= nil then
        self.nMapCopyIndex = tbStep.MapCopyIndex;
    else
        self.nMapCopyIndex = 0;
    end
    self.nCurrPosIndex = 2
    self.NPC = self:CopyTable(tbStep.NPC)
    self.LOCK = self:CopyTable(tbStep.LOCK)
    self.IsOpen = 0
    self.nIsOpen = 0
    self.nActivityIndex = nActivityIndex
    self.nStepIndex = nStepIndex
    self.GroupType = tbStep.GroupType
    self.nGroupNum = tbStep.GroupNum
    self.nMaxPlayer = tbStep.MaxPlayer
    self.tbEnemy = self:CopyTable(tbStep.Enemy)
    self.IsNeedTransport = tbStep.NeedTransport
    self.nClearanceStep = tbStep.ClearanceStep
    self.tbAssignIndexs = self:CopyTable(tbStep.AssignIndexs)
    self.EVENT_PROC = ActivityStep.EVENT_PROC
    
    self.tbLock = {}
    self.tbNextSteps = {}
    self.tbNpcGroup = {}
    self.tbPlayers = {}
    self.tbPlayerGroups = {}
    self.tbEventsExcALL = {}
    
    self:OnInit();
    
    assert(self.nGroupNum <= Activity.MAX_GROUP_NUM)
end

function ActivityStep:Clean()
  local nSceneType = KScene.GetSceneType(self.tbPos[1]);
  if nSceneType == 2 and self.nSceneId ~= nil then --战场类型的地图是动态加载的，这里手动回收下
     KScene.UnloadBattleScene(self.nSceneId);
  end
  
  self.nIsOpen = 0;
  self.nSceneId = nil;
  self.tbPlayers = {}
  self.tbLock = {}
  self.tbPlayerGroups = {}
  self.tbEventsExcALL = {}
end

function ActivityStep:StartStep()
	self:GroupPlayers();
	self:OnStartStep();
	
	if self.nIsOpen == 1 then -- 已经开过
		return
    end
	
	print(string.format("..begin activity[%s] %d step..", Activity.ActivitySetting[self.nActivityIndex].ActivityName, self.nStepIndex))
	
	self.IsOpen = 1
	self.nIsOpen = 1
	
	self.dwStartTime = KGameBase.GetTime();
	
	-- 创建锁数据
	self.tbLock = {}
    self.tbNpcGroup = {}
    self.tbEventsExcALL = {}
	for i, tbLockSetting in pairs(self.LOCK) do
		self.tbLock[i] = Lib:NewClass(tbEventLock);
		self.tbLock[i].nLockId = i;
		self.tbLock[i]:InitEventLock(self, tbLockSetting.nTime * Env.GAME_FPS, tbLockSetting.nNum, tbLockSetting.tbStartEvent, tbLockSetting.tbUnLockEvent);
	end
	for i, tbLockSetting in ipairs(self.LOCK) do -- 保证解锁顺序
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
	
	local nSceneType = KScene.GetSceneType(self.tbPos[1]);
	if nSceneType == 0 then
	   -- 只在预加载好的地图上开始活动
		local isLoad = KScene.IsDynamicSceneLoad(self.tbPos[1])
		if isLoad == 1 then
			local nSceneId = KScene.GetDynamicSceneIdByTemplateId(self.tbPos[1])
			print("..scene["..self.tbPos[1].."] have loaded", self.nActivityIndex, self.nStepIndex, nSceneId)
			self:OnSceneLoaded(nSceneId, self.tbPos[1])
		end
	elseif nSceneType == 2 then -- 战场类型，检查有无加载过要包含mapcopyIndex
	   local isLoad = KScene.IsDynamicSceneLoad(self.tbPos[1], self.nMapCopyIndex);
	   if isLoad == 1 then
			 local nSceneId = KScene.GetDynamicSceneIdByTemplateId(self.tbPos[1], self.nMapCopyIndex)
			 print("..scene["..self.tbPos[1]..","..self.nMapCopyIndex.."]have loaded", self.nActivityIndex, self.nStepIndex, nSceneId)
			 self:OnSceneLoaded(nSceneId, self.tbPos[1]);
	   else
	     local tbStartStep = Activity.tbStartSteps[self.tbPos[1]];
	     
	     self.LoadedSceneFlag = 1;  -- 置加载过场景标志
	     -- 除了活动开始那张地图由CR自动分配到一个合适GS上外，其它固定本服，不然会导致逻辑进行不下去
	     if tbStartStep ~= nil and tbStartStep.nStepIndex == self.nStepIndex then
	         KScene.LoadScene(self.tbPos[1], 0, self.nMapCopyIndex);
	     else
	         KScene.LoadScene(self.tbPos[1], 0, self.nMapCopyIndex, 1);
	     end
	   end
	end
end


function ActivityStep:DeleteAllNpc()
	assert(self.nSceneId and self.nMapId);
	KScene.DeleteAllNpc(self.nMapId, self.nSceneId);
end

function ActivityStep:SetTrapEffect(szTrapClass, nEffect)
	Scene:SetTrapEffect(szTrapClass, nEffect)
end

function ActivityStep:OnSceneLoaded(nSceneId, nTemplateId)
    local nCopyIndex = KScene.GetMapCopyIndex(nSceneId)
	if (self.nMapCopyIndex > 0) then
	    print("ActivityStep:OnSceneLoaded___CopyIndex", nCopyIndex, self.nMapCopyIndex)
	end
	
	if self.tbPos ~= nil and nTemplateId == self.tbPos[1] and self.IsOpen == 1 and self.nMapCopyIndex ~= nil and self.nMapCopyIndex == nCopyIndex then
		self.nMapId = nTemplateId
		self.nSceneId = nSceneId
		
		self.nEndTime = KGameBase.GetTime() + self.nTime;
		Timer:Register(self.nTime * Env.GAME_FPS, self.TimeOut, self);
	
        if self.tbLock[1] ~= nil then
	        self.tbLock[1]:StartLock()
	    end
    end
end

-- 根据设置对玩家进行分组
function ActivityStep:GroupPlayers()
	if self.GroupType == Activity.GROUP_TYPE.RANDOM_GROUP then -- 完全随机分组
		for _, tbPlayer in pairs(self.tbPlayers) do
			local nRandomGroup = KUnify.MathRandom(1, self.nGroupNum)
			self:AddPlayerToGroup(tbPlayer.nPlayerId, nRandomGroup);
	    end
	elseif self.GroupType == Activity.GROUP_TYPE.FIGHT_SCORE_GROUP then	--战力排行分组
		self:GroupPlayersByFightScore();
    end
    
    if self.tbEnemy ~= nil then
    	for _, tbPlayer in pairs(self.tbPlayers) do
			local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId)
			if pPlayer ~= nil then
				pPlayer.SetEnableEnemyCondition(1)
				pPlayer.SetEnemyCondition1(self.tbEnemy[1])
		        pPlayer.SetEnemyCondition2(self.tbEnemy[2])
				pPlayer.SetEnemyCondition3(self.tbEnemy[3])
				pPlayer.SetEnemyCondition4(self.tbEnemy[4])
				pPlayer.SetEnemyCondition5(self.tbEnemy[5])
			end
	    end
    else
	    local nStartCamp = Activity.START_CAMP -- 活动阵营从4开始
	    for nGroup, tbPlayerGroup in pairs(self.tbPlayerGroups) do
			if nGroup > 0 then
				for _, nPlayerId in pairs(tbPlayerGroup) do
					local pPlayer = KGameBase.GetPlayerById(nPlayerId)
					if pPlayer ~= nil then
						pPlayer.SetCurrentCamp(nStartCamp);
						if self.GroupType == Activity.GROUP_TYPE.FIGHT_SCORE_GROUP then	--按阵营传送位置
							local nPosIndex = nGroup + 1;
							if #self.tbPos > 1 and self.tbPos[nPosIndex] ~= nil then
								pPlayer.TeleportTo(self.tbPos[1], self.tbPos[nPosIndex][1], self.tbPos[nPosIndex][2], self.tbPos[nPosIndex][3]);
							else
								pPlayer.TeleportTo(self.tbPos[1], self.tbPos[2][1], self.tbPos[2][2], self.tbPos[2][3]);
							end
						end
					end
				end
				nStartCamp = nStartCamp + 1;
			end
	    end
    end
end

--只支持分两组
function ActivityStep:GroupPlayersByFightScore()
	local tbPlayerFightScore = {};
	local tbFightScoreMap = {};
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pPlayer then
			table.insert(tbPlayerFightScore, {tbPlayer.nPlayerId, pPlayer.GetFightScore()});
			tbFightScoreMap[tbPlayer.nPlayerId] = pPlayer.GetFightScore();
		end
	end
	local sortFunc = function(a,b) return b[2] < a[2] end;
	table.sort(tbPlayerFightScore, sortFunc);
	local nIndex = 1;				--下标从1开始
	local tbPlayerIndex = {};
	for key, tbFightScore in ipairs(tbPlayerFightScore) do
		tbPlayerIndex[nIndex]= tbFightScore[1];
		nIndex = nIndex + 1;
	end
		
	--设置阵营
	nIndex = 1;
	local nGroup1FightScore = 0; 	--Group1总战力
	local nGroup2FightScore = 0;	--Group2总战力
	--只有一个人或者两个人的时候
	if #tbPlayerIndex == 1 or #tbPlayerIndex == 2 then		
		self:AddPlayerToGroup(tbPlayerIndex[1], 1);
		nGroup1FightScore = nGroup1FightScore + tbFightScoreMap[tbPlayerIndex[1]];
		
		if tbPlayerIndex[2] ~= nil then
			self:AddPlayerToGroup(tbPlayerIndex[2], 2);
			nGroup2FightScore = nGroup2FightScore + tbFightScoreMap[tbPlayerIndex[2]];
		end
	else
		for _, nPlayerId in pairs(tbPlayerIndex) do
		  -- 以两个人为单位分配时，如果整除2的值为偶数，两组人数可以分得刚刚好，如1,2,3,4
		  -- 1,4分到第一组，2,3分到第二组，刚好两组人数相等，反之（如1,2,3,4,5,6）会造成其中一组多出2个人
		  -- 这种情况，需跳出循环特殊处理
		  if (#tbPlayerIndex / 2) % 2 ~= 0 then  
				if nIndex * 2 >= #tbPlayerIndex then
					break;
				end
			else
				if nIndex * 2 > #tbPlayerIndex then
					break;
				end
			end
			
			local nAnotherPlayerId = tbPlayerIndex[#tbPlayerIndex - nIndex + 1];
			if nIndex % 2 ~= 0 then
				self:AddPlayerToGroup(nPlayerId, 1);
				nGroup1FightScore = nGroup1FightScore + tbFightScoreMap[nPlayerId];
				self:AddPlayerToGroup(nAnotherPlayerId, 1);
				nGroup1FightScore = nGroup1FightScore + tbFightScoreMap[nAnotherPlayerId];
			else
				self:AddPlayerToGroup(nPlayerId, 2);
				nGroup2FightScore = nGroup2FightScore + tbFightScoreMap[nPlayerId];
				self:AddPlayerToGroup(nAnotherPlayerId, 2);
				nGroup2FightScore = nGroup2FightScore + tbFightScoreMap[nAnotherPlayerId];
			end
			nIndex = nIndex + 1;
		end
		
		local exceedNum = nIndex * 2 - #tbPlayerIndex;
		-- 多出一个人则分给战力低的阵营,当只有3个人的时候比较特殊
		if exceedNum == 1 then
			local nPlayerId = tbPlayerIndex[nIndex];
			if nGroup1FightScore > nGroup2FightScore then
				self:AddPlayerToGroup(nPlayerId, 2);
				nGroup2FightScore = nGroup2FightScore + tbFightScoreMap[nPlayerId];
			else
				self:AddPlayerToGroup(nPlayerId, 1);
				nGroup1FightScore = nGroup1FightScore + tbFightScoreMap[nPlayerId];
			end
		elseif exceedNum == 0 then -- 两个人时一边一个人
	  	local nPlayerId = tbPlayerIndex[nIndex];
	  	self:AddPlayerToGroup(nPlayerId, 1);
	  	nGroup1FightScore = nGroup1FightScore + tbFightScoreMap[nPlayerId];
	  
	  	local nAnotherPlayerId = tbPlayerIndex[#tbPlayerIndex - nIndex + 1];
	  	self:AddPlayerToGroup(nAnotherPlayerId, 2);
	  	nGroup2FightScore = nGroup2FightScore + tbFightScoreMap[nAnotherPlayerId];
		end
	end
	
	
end

function ActivityStep:ReduceAllDegree()
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pPlayer and self.nActivityIndex == Activity.ACTIVITY_INDEX_ZBT  then
			Player.tbDegree:ReduceDegree(pPlayer, "ZhanBaoTang", 1);
		end		
	end
end

function ActivityStep:FetchWinnerInRandom()
	local nIndex = KUnify.MathRandom(1, #self.tbWinners)
	local tbPlayer = self.tbWinners[nIndex]
	table.remove(self.tbWinners, nIndex)
	return tbPlayer.nPlayerId
end

function ActivityStep:FetchWinner(nIndex)
	local tbPlayer = self.tbWinners[nIndex]
	table.remove(self.tbWinners, nIndex)
	return tbPlayer.nPlayerId
end

function ActivityStep:FetchGroupWinner()
	local nGroup = 0
	for i, tbWinnerGroup in pairs(self.tbWinnerGroups) do
		nGroup = i
		break;
    end
    local tbRetGroup = self.tbWinnerGroups[nGroup]
    table.remove(self.tbWinnerGroups, nGroup)
    return tbRetGroup, nGroup
end

function ActivityStep:EndStep()
	self:OnEndStep()
	
	self.IsOpen = 0;
    -- 先置步骤状态,以能加进人
    for _, tbStep in pairs(self.tbNextSteps) do
       tbStep.IsOpen = 1;
    end
   
	-- 如果tbWinner为空，表示剩余玩家即是胜利者, 加到下个step
	if self.tbWinners == nil then
		self.tbWinners = self.tbPlayers
		self.tbWinnerGroups = self.tbPlayerGroups
	elseif self.nClearanceStep ~= nil then -- 如有填清场步骤，先把除胜利者之外的人传出
		for _, tbPlayer in pairs(self.tbPlayers) do
			if self:IsWinner(tbPlayer.nPlayerId) ~= 1 then
				print(string.format("..activity[%s]step[%d] take player [%d] to setp [%d]", Activity.ActivitySetting[self.nActivityIndex].ActivityName, self.nStepIndex, tbPlayer.nPlayerId, self.nClearanceStep))
				self:KickPlayerAnyWhere(tbPlayer.nPlayerId)
				Activity:JoinActivity(tbPlayer.nPlayerId, self.nActivityIndex, self.nClearanceStep)
		    end
	    end
    end
   
    if self.AssignType == Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED and #self.tbNextSteps > 0 then  -- 随机分配
   	   local nPlayerCount = #self.tbWinners
   	   local nStepCount = #self.tbNextSteps
   	   local nStepAssignCount = nPlayerCount / nStepCount
   	   
   	   if self.tbAssignCount ~= nil then
   	   	   for stepIndex, assignCount in pairs(self.tbAssignCount) do
   	   	       local tbStep = self.tbNextSteps[stepIndex]
   	   	       for i = 1, assignCount do
   	   	       	   if #self.tbWinners > 0 then
	       	           local nPlayerId = self:FetchWinnerInRandom()
	       	           self:KickPlayerAnyWhere(nPlayerId)
	                   tbStep:JoinPlayer(nPlayerId)
	               end
	           end
   	       end
       else
	       for i = 1, nStepCount do
	       	   for j = 1,  nStepAssignCount do
	       	       local nPlayerId = self:FetchWinnerInRandom()
	       	       self:KickPlayerAnyWhere(nPlayerId)
	               self.tbNextSteps[i]:JoinPlayer(nPlayerId)
	           end
	       end
	       
	       --如还有剩余，分配到最后一个step
	       for _, tbPlayer in pairs(self.tbWinners) do
	       	   self.tbNextSteps[#self.tbNextSteps]:JoinPlayer(tbPlayer.nPlayerId) 
	       end
	   end
    elseif self.AssignType == Activity.ASSIGN_TYPE.GROUP_ORDER_ASSIGNED and #self.tbNextSteps > 0 then  --按组顺序分配
	   local tbPlayerGroup, nGroup = self:FetchGroupWinner()
	   if (tbPlayerGroup ~= nil) then
           for _, nPlayerId in pairs(tbPlayerGroup) do
               for _, tbStep in pairs(self.tbNextSteps) do
                   self:KickPlayerAnyWhere(nPlayerId)
                   tbStep:JoinPlayer(nPlayerId, nGroup)
                   tbStep:AddPlayerToGroup(nPlayerId, nGroup)
               end
           end
	   end
    elseif self.AssignType == Activity.ASSIGN_TYPE.SPECIFY_ASSIGNED and #self.tbNextSteps > 0 then  --指定分配，此种手工填写每个接继步骤分配的角色索引
        for stepIndex, tbStepAssignIndexs in pairs(self.tbAssignIndexs) do
        	local tbStep = self.tbNextSteps[stepIndex]
        	for _, nIndex in pairs(tbStepAssignIndexs) do
        		local tbPlayer = self.tbWinners[nIndex]
        		if tbPlayer ~= nil then
        			self:KickPlayerAnyWhere(tbPlayer.nPlayerId)
        		    tbStep:JoinPlayer(tbPlayer.nPlayerId)
        		end
            end
        end
    end
	
    print(string.format("..finish [%s] %d step", Activity.ActivitySetting[self.nActivityIndex].ActivityName, self.nStepIndex))
	
	--战场中开第二场时没有清除，则无法加入战斗场
    self.tbWinners = nil;	
	
    -- 开始下一步骤
	if #self.tbNextSteps == 0 then
		local tbTempPlayers = {};
		for _, tbPlayer in pairs(self.tbPlayers) do
			tbTempPlayers[tbPlayer.nPlayerId] = 1;
		end
		
		for nPlayerId, value in pairs(tbTempPlayers) do
			self:LeaveGame(nPlayerId);
		end
	else
		for _, tbStep in pairs(self.tbNextSteps) do
			tbStep:StartStep();
		end
	end
end

function ActivityStep:AddWinner(nPlayerId, nGroup)
	if self.tbWinners == nil then
		self.tbWinners = {}
		self.tbWinnerGroups = {}
    end
	
    local tbWinnerGroup = self.tbWinnerGroups[nGroup]
	if self.tbWinnerGroup == nil then
	    self.tbWinnerGroups[nPlayerId] = {}
	    tbWinnerGroup = self.tbWinnerGroups[nPlayerId]
    end
    
    tbWinnerGroup[nPlayerId] = nPlayerId
    self.tbWinners[#self.tbWinners + 1] = {nPlayerId = nPlayerId, nGroup = nGroup}
    print("..Add winner", nPlayerId, nGroup)
end

function ActivityStep:RemoveWinner(nPlayerId, nGroup)
	if self.tbWinners ~= nil then
		for index, nWinnerId in pairs(self.tbWinners) do
			if nWinnerId == nPlayerId then
				table.remove(self.tbWinners, index)
				self.tbWinnerGroup[nPlayerId] = nil
				break;
		    end
	    end
	end
end

function ActivityStep:IsWinner(nPlayerId)
	if self.tbWinners == nil then
		return 0
    end
    
    for _, tbWinner in pairs(self.tbWinners) do
    	if nPlayerId == tbWinner.nPlayerId then
    		return 1
        end
    end
    return 0
end

function ActivityStep:TimeOut()
	if self.IsOpen == 1 then
        self:EndStep()
    end
    return 0
end

-- self.tbPlayerGroups用哈希表形式储存，无法通过#获取大小
function ActivityStep:GetPlayerGroupCount(nGroup)
	local nCount = 0;
	local tbPlayerGroup = self.tbPlayerGroups[nGroup];
	
	if tbPlayerGroup ~= nil then
		for nPlayerIdKey, nPlayerId in pairs(tbPlayerGroup) do
			nCount = nCount + 1
		end
	end
	
	return nCount;
end

function ActivityStep:AddPlayerToGroup(nPlayerId, nGroup)
	local tbPlayerGroup = self.tbPlayerGroups[nGroup]
	if tbPlayerGroup == nil then
	    self.tbPlayerGroups[nGroup] = {}
	    tbPlayerGroup = self.tbPlayerGroups[nGroup]
    end
    
    tbPlayerGroup[nPlayerId] = nPlayerId
	
	--- TODO: 同时设置self.tbPlayers里的nGroup
	-- 设置group
	for index, tbPlayer in pairs(self.tbPlayers) do
		if (tbPlayer.nPlayerId == nPlayerId) then
			tbPlayer.nGroup = nGroup;
			self.tbPlayers[index] = tbPlayer;
		end
	end
	
end

function ActivityStep:GetPlayerGroup(nPlayerId)
	for _, tbPlayer in pairs(self.tbPlayers) do
		if tbPlayer.nPlayerId == nPlayerId then
			return tbPlayer.nGroup
	    end
    end
end

function ActivityStep:IsPlayerIn(nPlayerId)
	for _, tbPlayer in pairs(self.tbPlayers) do
		if tbPlayer.nPlayerId == nPlayerId then
			return 1
	    end
    end
    return 0
end

function ActivityStep:JoinPlayer(nPlayerId, nGroup, NeedTransport)
	if #self.tbPlayers > self.nMaxPlayer or self.IsOpen == 0 then
		print("[JoinPlayer]Error: Step Is not open")
		return 0
	end
	
	if self:IsPlayerIn(nPlayerId) == 1 then
		print("[JoinPlayer]Player already exist", nPlayerId)
		return 0
    end
	
	if (nGroup == nil) then nGroup = 0; end
	
    local index = #self.tbPlayers + 1;
    
    local tbPlayer = {}
    tbPlayer.nPlayerId = nPlayerId
    tbPlayer.nGroup = nGroup
    table.insert(self.tbPlayers, tbPlayer)
    --self.tbPlayers[index] = {nPlayerId = nPlayerId, nGroup = nGroup}

	self:OnJoinPlayer(nPlayerId, nGroup)  -- TODO:, 当前Group 0
	
    local pPlayer = KGameBase.GetPlayerById(nPlayerId)
    if pPlayer ~= nil and self.tbPos ~= nil and #self.tbPos > 1 and NeedTransport ~= 0 and self.IsNeedTransport ~= 0 then -- and pPlayer.dwSceneTemplateId ~= self.tbPos[1] then 
    	pPlayer.TeleportTo(self.tbPos[1], self.tbPos[self.nCurrPosIndex][1], self.tbPos[self.nCurrPosIndex][2], self.tbPos[self.nCurrPosIndex][3])
    	print(string.format("[%d]传入位置[%d,%d,%d,%d]]", self.tbPlayers[index].nPlayerId, self.tbPos[1], self.tbPos[self.nCurrPosIndex][1], self.tbPos[self.nCurrPosIndex][2], self.tbPos[self.nCurrPosIndex][3]))
    	self.nCurrPosIndex = self.nCurrPosIndex + 1
    	if self.nCurrPosIndex > #self.tbPos - 1 then --循环使用
    		self.nCurrPosIndex = 2
        end
    end
    
    if Activity.tbPlayerProgress[nPlayerId] == nil then
        Activity.tbPlayerProgress[nPlayerId] = {}
    end
    Activity.tbPlayerProgress[nPlayerId].nActivityIndex = self.nActivityIndex
    Activity.tbPlayerProgress[nPlayerId].nStepIndex = self.nStepIndex
    
    -- 执行需要对所有玩家都执行一次的事件
    self:ExcCacheEvent(nPlayerId)
    if #self.tbPos > 1 then 
		print(string.format("..JoinPlayer[activity: %s step: %d player ID: %d]", Activity.ActivitySetting[self.nActivityIndex].ActivityName, self.nStepIndex, self.tbPlayers[index].nPlayerId))
	end
	
    return 1
end

function ActivityStep:KickPlayerAnyWhere(nPlayerId)
	--  玩家可能在别的步骤里
	local tbCurActivity = Activity.tbPlayerProgress[nPlayerId]
	if tbCurActivity ~= nil and tbCurActivity.nStepIndex ~= self.nStepIndex then
		local tbActivity = Activity.tbActivities[tbCurActivity.nActivityIndex]
		if tbActivity ~= nil and tbActivity.tbSteps[tbCurActivity.nStepIndex] ~= nil then
			print(string.format("..at step [%d] take out at step [%d] player", self.nStepIndex, tbCurActivity.nStepIndex))
			tbActivity.tbSteps[tbCurActivity.nStepIndex]:KickPlayer(nPlayerId)
			return
		end
	else
		self:KickPlayer(nPlayerId)
	end
end

function ActivityStep:KickPlayer(nPlayerId, IsRemoveWinner)			

	for index, nId in pairs(self.tbPlayers) do
		local tbPlayer = self.tbPlayers[index]
		if tbPlayer.nPlayerId == nPlayerId then
			if self.tbPlayerGroups[tbPlayer.nGroup] ~= nil then
				local tbPlayerGroup = self.tbPlayerGroups[tbPlayer.nGroup]
				tbPlayerGroup[nPlayerId] = nil
		    end 
		    table.remove(self.tbPlayers, index)
			break
		end
    end
    
    -- TODO：先把战斗关系直接置空，如果以后除了活动还有其他地方用到战斗关系再修改进行恢复
    local pPlayer = KGameBase.GetPlayerById(nPlayerId)
    if pPlayer ~= nil then
    	pPlayer.SetEnableEnemyCondition(0)
    	pPlayer.SetEnemyCondition1(0)
		pPlayer.SetEnemyCondition2(0)
		pPlayer.SetEnemyCondition3(0)
		pPlayer.SetEnemyCondition4(0)
		pPlayer.SetEnemyCondition5(0)
    end
    
    if IsRemoveWinner == 1 then
    	self:RemoveWinner(nPlayerId)
    end
    
	self:OnKickPlayer(nPlayerId);
    --Activity.tbPlayerProgress[nPlayerId] = nil
    print("KickPlayer", self.nActivityIndex, self.nStepIndex, nPlayerId)
end

function ActivityStep:OnEvent(nEventType, ...)
	if self.EVENT_PROC[nEventType] and self[self.EVENT_PROC[nEventType]] then
		self[self.EVENT_PROC[nEventType]](self, ...);
	else
		print("Undefind EventType ", nEventType, ...);
	end
end

-- 遍历在当前步骤里的所有玩家
function ActivityStep:GroupPlayerExcute(fnExcute, nPlayerGroup)
	if nPlayerGroup == 0 or nPlayerGroup == -1 then  -- 对所有玩家执行
		for _, tbPlayer in ipairs(self.tbPlayers) do
	        local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId)
	        if pPlayer then
	            fnExcute(pPlayer);
	        end
	    end
	else
	  local tbPlayerGroup = self.tbPlayerGroups[nPlayerGroup]
	  if tbPlayerGroup ~= nil then
	      for _, nPlayerId in pairs(tbPlayerGroup) do
	        local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	        if pPlayer then
	            fnExcute(pPlayer);
	        end
	      end
	  end
	end
end

-- 执行脚本 -- TODO
function ActivityStep:ExcuteScript(szCmd)
	Activity.tbExcuteTable = self;
	local fnExc = loadstring("local self = Activity.tbExcuteTable;"..szCmd);
	if fnExc then
		xpcall(fnExc, Lib.ShowStack);
	end
	Instance.tbExcuteTable = nil;
end

-- 电影模式对话
function ActivityStep:MovieDialog(nPlayerGroup, szMovie, delay)
	if delay and delay > 0 then
		Timer:Register(delay * Env.GAME_FPS, self.MovieDialog, self, nPlayerGroup, szMovie);
		return;
	end
	
	local fnExcute = function (pPlayer)
		TaskAct:NormalTalk(pPlayer, szMovie);
	    --Dialog:Talk({szMovie});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	return 0;
end

function ActivityStep:SetPlayerActivityInfo(nPlayerId, szTimeInfo, szTargetInfo) 
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if szTimeInfo ~= nil then
	    pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", szTimeInfo, -1});
	end
	if szTargetInfo ~= nil then
	    pPlayer.CallClientScript({"QClientScript.Mission:SetMissionExInfo", szTargetInfo});
	end
end

-- 同步副本剩余时间信息
function ActivityStep:BroadcastTimeInfo(nPlayerGroup, szTimeInfo, ...)
	local varLock; -- 读取varLock， 哪个锁
	local nTimeFrame; -- 传输时间
	
	if #arg == 0 then varLock = -1; 
	else varLock = unpack(arg);	end
	
	if type(varLock) ~= "number" then
		print("[Lua:ActivityStep:BroadcastTimeInfo]varLock非Number");
		assert(false);
	end
	
	-- 如果有填锁，锁时间，没填锁，用活动时间
	if (varLock ~= -1 and self.tbLock[varLock]) then
	    local nLockTimerId = self.tbLock[varLock].nTimerId;
	    if (nLockTimerId ~= nil and nLockTimerId > 0) then
		    nTimeFrame = tonumber(Timer:GetRestTime(nLockTimerId));  -- 获取锁的剩余时间
		else  -- 锁未开始，用锁的时间
			print("错误！无法获取锁TimerId!");
			assert(false);
		end
	else
		nTimeFrame = (self.nEndTime - KGameBase.GetTime()) * Env.GAME_FPS;
	end
	
	self.szTimeInfo = szTimeInfo;
	local fnExcute = function(pPlayer)
		local nSec = nTimeFrame / Env.GAME_FPS;
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", szTimeInfo, nTimeFrame});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end


function ActivityStep:SetTargetInfo(nPlayerGroup, szInfo, ...)
	local varLock; -- 读取varLock， 哪个锁
	if #arg == 0 then varLock = -1;   -- 没有参数
	else varLock = unpack(arg);	end -- 获取第一个
	
	self.nCurLock = varLock;
	
	self.szTargetInfo = szInfo;  -- 设置targetInfo
	
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTargetInfo", szInfo});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:MeBubble(nPlayerGroup, szMsg, delay)
	local fnExcute = function (pPlayer)
	    local pOldMe = me;
	    me = pPlayer;
		Dialog:PlayerBubble(szMsg, delay * Env.GAME_FPS);
	    me = pOldMe;
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:NpcBubble(nPlayerGroup, szGroup, szMsg, delay)
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
function ActivityStep:ChangeTrap(szClassName, ...)
	assert(Scene.tbTrapInfo[szClassName]);
	local tbTrapInfo = Scene.tbTrapInfo[szClassName];
	tbTrapInfo.Param2 = arg[1];
	tbTrapInfo.Param3 = arg[2];
	tbTrapInfo.Param4 = arg[3];
end

-- 黑条消息
function ActivityStep:BlackMsg(nPlayerGroup, szMsg, delay)
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

-- 定时函数
function ActivityStep:TimingFunction(szFuncName, nTiming, nCurrentExecuteTime, nMaxExecuteTime, tbArgs)
	if self[szFuncName] ~= nil then
		self[szFuncName](unpack(tbArgs))
    end
	if nCurrentExecuteTime <= nMaxExecuteTime then 
	    return nTiming
	else
	    return 0
	end
end

-- 挂靠trap点，等玩家走到这个trap点的时候执行函数
function ActivityStep:TrapHook(szTrapClass, szScript)
	if self.TrapCall == nil then
		self.TrapCall = {}
	end
	self.TrapCall[szTrapClass] = szScript;
end

function ActivityStep:ExcuteTrapScript(szTrapClass)
	if not self.TrapCall or not self.TrapCall[szTrapClass] then
		return
	end
	self:ExcuteScript(self.TrapCall[szTrapClass]);
	-- 执行完后删除
	self.TrapCall[szTrapClass] = nil;
end

function ActivityStep:BroadcastActivityNotice(szMsg, nPlayerGroup)
  local fnExcute = function (pPlayer)
	   pPlayer.SendNoticeMsg(szMsg);
  end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:ShowNoticeMsg(szMsg, OnlyActivityPlayer)
	if self.CurrentKillerName ~= nil then
		szMsg = string.gsub(szMsg, "{Killer}", self.CurrentKillerName)
    end
    if self.CurrentDeadName ~= nil then
		szMsg = string.gsub(szMsg, "{Dead}", self.CurrentDeadName)
    end
	
	if not self.nKinId then
	  if OnlyActivityPlayer == 1 then
	     local fnExcute = function (pPlayer)
		       pPlayer.SendNoticeMsg(szMsg);
	     end
	     self:GroupPlayerExcute(fnExcute, 0);
	  else
			 -- 全体活动
			 KChat.BroadcastNoticeMsg(szMsg);
		end
	else
		-- 家族活动
		KChat.BroadcastKinNoticeMsg(self.nKinId, szMsg);
	end
end

-- tbPosList是类似 { {540,1256,1048576}, {580, 1824, 1048576} } 这种，复活时随机选取一个点复活
function ActivityStep:ReviveInfo(nPlayerGroup, bWait, bUseItem, bReturn, nScene, ...)
	local fnExcute = function (pPlayer)
		Player:SetTempRevive(pPlayer, bWait, bUseItem, bReturn, nScene, unpack(arg));
		print("..ActivityStep:ReviveInfo",pPlayer.szName);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:ShowRewardPanel(nPlayerGroup, delay)
	if delay and delay > 0 then
		Timer:Register(delay, self.ShowRewardPanel, self, nPlayerGroup);
		return
	end

	local fnExcute = function (pPlayer)
		local nElapseTime = KGameBase.GetTime() - (self.nEndTime - self.nTime); -- 单位是秒
	--	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowTaskAwardPanel", self.nMapId, nElapseTime, 1});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:AddExcAllEvent(nEventType, tbArg)
	self.tbEventsExcALL[nEventType] = tbArg
end

function ActivityStep:RemoveExcAllEvent(nEventType)
	self.tbEventsExcALL[nEventType] = nil
end

function ActivityStep:SetRelation(tbRelation)
	local fnExcute = function (pPlayer)
		print("Change battle relation", pPlayer.dwId, tbRelation[1], tbRelation[2], tbRelation[3], tbRelation[4], tbRelation[5])
		if tbRelation[1] == 0 and tbRelation[2] == 0 and tbRelation[3] == 0 and tbRelation[4] == 0 and tbRelation[5] == 0 then
			pPlayer.SetEnableEnemyCondition(0)
		else
		    pPlayer.SetEnableEnemyCondition(1)
		end
		pPlayer.SetEnemyCondition1(tbRelation[1])
		pPlayer.SetEnemyCondition2(tbRelation[2])
		pPlayer.SetEnemyCondition3(tbRelation[3])
		pPlayer.SetEnemyCondition4(tbRelation[4])
		pPlayer.SetEnemyCondition5(tbRelation[5])
	end
	self:GroupPlayerExcute(fnExcute, 0);
end

function ActivityStep:ExcCacheEvent(nPlayerId)
	for nEvent, tbArg in pairs(self.tbEventsExcALL) do
		local tbPlayerTemp = self.tbPlayers 
		self.tbPlayers = {}
		self.tbPlayers[1] = {nPlayerId = nPlayerId, nGroup = 0}
		if self.EVENT_PROC[nEvent] and self[self.EVENT_PROC[nEvent]] then
			self[self.EVENT_PROC[nEvent]](self, unpack(tbArg));
		else
			print("Undefind EventType ", nEvent, unpack(tbArg));
		end
		self.tbPlayers = tbPlayerTemp
	end
end

-- 计算玩家等级平均值
function ActivityStep:GetAverageLevel()
	local nLevel;
	-- 计算活动内玩家平均等级
	local nTotalLevel = 0;
	local nTotalPlayer = 0;
	
	local tbActivity = Activity.tbActivities[self.nActivityIndex]
  for _, tbStep in pairs(tbActivity.tbSteps) do
      for _, tbPlayer in pairs(tbStep.tbPlayers) do
      	local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId)
				if pPlayer then
					nTotalLevel = nTotalLevel + pPlayer.GetLevel();
					nTotalPlayer = nTotalPlayer + 1
				end
      end
  end
    
	--[[for _, tbPlayer in pairs(self.tbPlayers) do
    local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId)
		if pPlayer then
			nTotalLevel = nTotalLevel + pPlayer.GetLevel();
			nTotalPlayer = nTotalPlayer + 1
		end
	end--]]
	
	if (nTotalPlayer ~= 0) then
		nLevel = math.ceil(nTotalLevel / nTotalPlayer);
	else
		nLevel = 50;
	end
	return nLevel;
end

function ActivityStep:AddPlayerInLock(pPlayer, nLock)
	if not nLock or nLock <= 0 then
		return 0;
	end
	local tbTmp = pPlayer.GetTempTable("Activity")
	tbTmp.nLock = nLock
	tbTmp.tbStep = self
	print("add player in lock", pPlayer.dwId, nLock)
end

function ActivityStep:AddPlayersInLock(nLock, nPlayerGroup)
	local fnExcute = function (pPlayer)
		self:AddPlayerInLock(pPlayer, nLock)
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function ActivityStep:JoinStep(nActivityIndex, nStepIndex, IsRemoveWinner, nPlayerGroup, NeedTransport)
	local tbTmpPlayers = {};
	for _, tbPlayer in ipairs(self.tbPlayers) do
		table.insert(tbTmpPlayers, tbPlayer.nPlayerId);
	end
	
	for _, nPlayerId in ipairs(tbTmpPlayers) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
				KCharacter.Revive(pPlayer, 1000, 1000);
			end
			Activity:JoinActivity(pPlayer.dwId, nActivityIndex, nStepIndex, NeedTransport)
		end
	end
	
	--[[
	local fnExcute = function (pPlayer)
		--pPlayer.SysMsg(string.format("nActivityIndex:%d  nStepIndex:%d",  self.nActivityIndex, self.nStepIndex));
		print("Event joinstep execute", pPlayer.dwId, nActivityIndex, nStepIndex)
		if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
		  KCharacter.Revive(pPlayer, 1000, 1000);
		end
        Activity:JoinActivity(pPlayer.dwId, nActivityIndex, nStepIndex)
	end
	
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	--]]
	
	-- 把加到其他步骤的玩家删除
	local tbKickPlayerIndexs = {}
	for _,tbPlayer in pairs(self.tbPlayers) do
		if tbPlayer.nGroup == nPlayerGroup then
			self:KickPlayer(tbPlayer.nPlayerId, IsRemoveWinner);
			table.insert(tbKickPlayerIndexs, tbPlayer.nPlayerId)
	    end
    end
	
	for _, nPlayerId in pairs(tbKickPlayerIndexs) do
		self:KickPlayer(nPlayerId, IsRemoveWinner)
    end
end

--显示特效
function ActivityStep:ShowEffect(pPlayer, nType)
	if pPlayer and Activity.tbPlayerProgress[pPlayer.dwId] and Activity.tbPlayerProgress[pPlayer.dwId].nActivityIndex == self.nActivityIndex then
		pPlayer.CallClientScript({"QClientScript.Activity:ShowActivityEffect", nType});	--客户端特效显示
	end
end

-- 把NPC加到锁里
function ActivityStep:AddNpcInLock(pNpc, nLock)
	if not nLock or nLock <= 0 then
		return 0;
	end
	local tbTmp = pNpc.GetTempTable("Activity")
	tbTmp.nLock = nLock
	tbTmp.tbStep = self
end

-- 把npc刷到不同位置上
-- nNum: 数量
-- tbIndex: npc索引表
function ActivityStep:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)
	local pNpc;
	local tbNpcId = {};
	local nIndex;
	if #tbIndex == 1 then
		nIndex = tbIndex[1];
	end
	
	local tbNpcInfo;
	if nIndex then
	    tbNpcInfo = self.NPC[nIndex];
	else
	    tbNpcInfo = self.NPC[tbIndex[MathRandom(#tbIndex)]];
	end
	
	-- 用九天的刷怪方式
	local tbNpcT = Scene.tbNpcCfg[self.nMapId][tbNpcInfo.nTemplate];
	local tbNpc = {};
	tbNpc.tbPosList = tbNpcT.tbPosList;
	tbNpc.nTemplateId = tbNpcT.nTemplateId;
	tbNpc.nRegenerationTime = tbNpcT.nRegenerationTime;
	tbNpc.nNormalAI = tbNpcT.nNormalAI;
	tbNpc.nCount = nNum;
	
	--print("ActivityStep:__AllocNpc", tbNpc.nTemplateId, self.nSceneId, self.nMapId, tbNpc.nRegenerationTime, tbNpc.nNormalAI)
	
	local tbRetNpcs = Scene:_CallOneNpc(tbNpc, self.nSceneId, self.nMapId, nil, szPointName, 0, "");
	for _, pNpc in ipairs(tbRetNpcs) do
	    KCharacter.ResetNpcIntensity(pNpc, nPropId, nLevel);
	    self:AddNpcInLock(pNpc, nLock);
		self:AddNpcInGroup(pNpc, szGroup);
		table.insert(tbNpcId, pNpc.dwId);
		--local tbTmp = pNpc.GetTempTable("Activity")
		--tbTmp.tbInstance = self;
	end
	return tbNpcId;
end

function ActivityStep:__AddNpc(tbIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	if not self.NPC or self.nSceneId == nil then
		return 0;
	end
	
	local nLevel = -1;	-- 默认值
	local tbNpcInfo = self.NPC[tbIndex[1]];
	local nPropId = KCharacter.GetNpcPropId(tbNpcInfo.nTemplate);
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
			self.tbAddNpcTimer[szTimerName].nTimerId = Timer:Register(nFrequency * Env.GAME_FPS, self.__AddNpc, self, tbIndex, nNum, nLock, szGroup, szPointName, nTimes - 1, nFrequency, szTimerName);
			self.tbAddNpcTimer[szTimerName].tbNpcId = tbNpcId;
		end
	end
	--子类重载
	self:OnAddNpc(tbNpcId, tbNpcInfo.nTemplate);
	return 0;
end

-- 添加NPC
-- varIndex: nTemplateId or {id1,id2,id3...}, 可以是一个或者一组模板id
-- nNum: 刷几个
-- nLock
-- szGroup: 新刷出来的npc所属的组的名字
-- szPointName: 使用哪一组点刷怪
-- nTimes, nFrequency, szTimerName: 循环刷怪用，参数为：次数，隔多久刷一次，timer名字	
function ActivityStep:AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	if (nTimes or nFrequency or szTimerName) and not (nTimes > 1 and nFrequency > 0 and szTimerName) then
		assert(false);
	end
	
	if type(varIndex) == "number" then
		varIndex = {varIndex};
	end
	for _, id in ipairs(varIndex) do
		local tbNpcInfo = self.NPC[id];
		assert(tbNpcInfo);
	end
	
	if self.tbAddNpcTimer == nil then 
		self.tbAddNpcTimer = {};
	end
	
	if nFrequency and not self.tbAddNpcTimer[szTimerName] then
		self.tbAddNpcTimer[szTimerName] = {};
		self.tbAddNpcTimer[szTimerName].tbNpcId = {};
	end

	self:__AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove);
	return 0;
end

-- 定时添加NPC
-- nTimer:时间点 123456-> 12:34:56
-- varIndex: nTemplateId or {id1,id2,id3...}, 可以是一个或者一组模板id
-- nNum: 刷几个
-- nLock
-- szGroup: 新刷出来的npc所属的组的名字
-- szPointName: 使用哪一组点刷怪
-- nTimes, nFrequency, szTimerName: 循环刷怪用，参数为：次数，隔多久刷一次，timer名字	
function ActivityStep:AddNpcOnTime(nTimer, varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove)
	local curDayTime = Lib:GetLocalDayTime();
	local nAddTime = Timer:CycleTimeToSeconds(nTimer);
	if curDayTime < nAddTime then
		Timer:Register((nAddTime - curDayTime) * Env.GAME_FPS, self.AddNpc, self, varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName, nRemove);
	end
end

-- 把NPC加到组里
function ActivityStep:AddNpcInGroup(pNpc, szGroup)
	if not self.tbNpcGroup[szGroup] then
		self.tbNpcGroup[szGroup] = {};
	end
	if pNpc then
		table.insert(self.tbNpcGroup[szGroup], pNpc.dwId);
	end
end

-- 删除特定组的NPC
function ActivityStep:DelNpc(szGroup)
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
end

--发奖励
function ActivityStep:SendAward()
	self:OnSendAward();
end
-----------------------------子类重载函数---------------------------------------------
-- 得到分组，由子类重载

-- 玩家添加，由子类重载
function ActivityStep:OnJoinPlayer(nPlayerId, nGroup)
	self:SetPlayerActivityInfo(nPlayerId, nil, "");	
	Dbg:Output("ActivityStep", "OnJoinPlayer", "(Empty)", self);
	return 1
end

-- 活动步骤开始，由子类重载
function ActivityStep:OnStartStep(tbPlayers)
	Dbg:Output("ActivityStep", "OnStartStep", "(Empty)", self);
end

-- 活动步骤开始，由子类重载
function ActivityStep:OnEndStep(tbPlayers)
	Dbg:Output("ActivityStep", "OnEndStep", "(Empty)", self);
end

-- 活动步骤开始，由子类重载
function ActivityStep:OnKickPlayer(nPlayerId)
	Dbg:Output("ActivityStep", "OnKickPlayer", "(Empty)", self);
end

function ActivityStep:OnPlayerDeath(nDeathCharId, nKillerCharId)
	Dbg:Output("ActivityStep", "OnPlayerDeath", "(Empty)", self, nDeathCharId, nKillerCharId);
end

function ActivityStep:OnPlayerKill(nKillerCharId, nDeathCharId)
	Dbg:Output("ActivityStep", "OnPlayerKill", "(Empty)", self, nKillerCharId, nDeathCharId);
end

function ActivityStep:OnPlayerDamage(nCasterId, nInjuredId, nDamage)
	Dbg:Output("ActivityStep", "OnPlayerDamage", "(Empty)", self, nCasterId, nInjuredId, nDamage);
end

function ActivityStep:OnPlayerRevive(nPlayerId)
	Dbg:Output("ActivityStep", "OnPlayerRevive", "(Empty)", self, nCasterId, nInjuredId, nDamage);
end

function ActivityStep:OnPlayerInjured(nInjuredId, nCasterId, nDamage)
	print("ActivityStep", "OnPlayerInjured", "(Empty)", self, nInjuredId, nCasterId, nDamage);
end

function ActivityStep:OnNpcDeath(pKiller, pNpc)
	print("ActivityStep", "OnNpcDeath", "(Empty)", self, pKiller.dwId, pNpc.dwId);
end

function ActivityStep:GetProgressBarDescription()
	print("ActivityStep", "GetProgressBarDescription", "(Empty)", self);
	return "ActivityStep:GetProgressBarDescription (Empty)";
end

function ActivityStep:GetProgressBarTime()
	print("ActivityStep", "GetProgressBarTime", "(Empty)", self);
	return 5 * GAME_FPS; -- 默认5
end

function ActivityStep:OnAddNpc(tbNpcId, nTemplateId)
	print("ActivityStep", "OnAddNpc", "(Empty)", self, tbNpcId, nTemplateId);
end 

function ActivityStep:OnSendAward()
	print("ActivityStep", "OnSendAward", "(Empty)", self);
end

function ActivityStep:SetPlayerFightMode(pPlayer, nMode)
	print("ActivityStep", "SetPlayerFightMode", "(Empty)", self, pPlayer, nMode);
end

function ActivityStep:OnLeaveGame(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if pPlayer then
		pPlayer.CallClientScript({"QClientScript.Activity:HideBFPanel"});	--关战场战报面板
	end
	print("ActivityStep", "OnLeaveGame", "(Empty)", self);
end

function ActivityStep:OnInit()
end

-- 相应玩家掉线后再登录，默认处理是直接传回原场景,可重载
function ActivityStep:OnPlayerLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
end

function ActivityStep:OnEnterActivityScene(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if not pPlayer or pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	
	local szSceneType = KScene.GetSceneTypeName(nMapId);
	if szSceneType ~= "activity" and szSceneType ~= "activity_zb" then
		return;
	end
	
	--if szSceneType == "activity_zb" then
	--	self:OnJoinPlayer(pPlayer.dwId, nGroup)  -- TODO:, 当前Group 0
	--end
	
	-- 覆盖复活场景：从准备场传送至赛场时，还在准备场时已经执行过一次，复活时场景不对
	self:ExcCacheEvent(nCharacterId);
end

--角色离开场景, 默认角色离开场景时剔除玩家，子类可重载
function ActivityStep:LeaveGame(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if pPlayer and pPlayer.dwSceneTemplateId == self.tbPos[1] then
	    self:KickPlayer(nCharacterId, 1);
		pPlayer.ReturnToLastCity();
		Activity.tbPlayerProgress[nCharacterId] = nil;
	end
	self:OnLeaveGame(nCharacterId);
	print("LeaveGame", pPlayer.dwSceneTemplateId, self.tbPos[1]);
end