Include("script/mission/instance/instance_base.lua")

Instance.tbPlayerInstances = {};

function Instance:TryEnterInstance(tbPlayerIds, nMapId,bTask)
	local nOwnerId;
    if nMapId == nil or self.tbSetting[nMapId] == nil then
        return;
    end

	assert(type(tbPlayerIds) == "table" and #tbPlayerIds > 0);
	print(string.format("..Instance:TryEnterInstance #tbPlayerIds=%d nMapId=%d",#tbPlayerIds,nMapId));
	-- 先传送到同一张地图
	for _, nPlayerId in pairs(tbPlayerIds) do
		local pMember = KGameBase.GetPlayerById(nPlayerId);
		
		if pMember then
			if Instance:IsTaskInstance(nMapId) ~= 1 and self.tbSetting[nMapId].nTaskInstance == 1 and Player.tbDegree:GetDegree(pMember, "Instance") <= 0 then
				assert(false);
			end
			
			if pMember.GetOverallTeamIdx() > 0 then
				Dialog:_AskForSure(pMember.dwId, Lang.mission.str559[Lang.Idx], ({"Instance:DoLeaveOverallTeamAndEnter", pMember, nMapId}));
				return;
			end
			
			local tbPos = self.tbSetting[nMapId].tbBeginPos;
			-- 进副本前下马
			Pet:GetDownRide(pMember);
			pMember.TeleportTo(nMapId, tonumber(tbPos[1]), tonumber(tbPos[2]), tonumber(tbPos[3]));
			if(bTask == 0) then 
				print("..ReduceDegree ")
				Player.tbDegree:ReduceDegree(pMember, "Instance", 1);
			end
		end
	end
end

function Instance:DoLeaveOverallTeamAndEnter(pPlayer, nMapId)
	KGameBase.LeaveOverallTeam(pPlayer);
	
	local tbPos = self.tbSetting[nMapId].tbBeginPos;
	-- 进副本前下马
	Pet:GetDownRide(pPlayer);
	pPlayer.TeleportTo(nMapId, tonumber(tbPos[1]), tonumber(tbPos[2]), tonumber(tbPos[3]));
end

function Instance:OnDeath(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if not pPlayer or pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "fb" then
		return;
	end
	
	local pGame = self.tbPlayerInstances[dwCharacterId];
	if pGame then
		pGame.nDeathTimes = pGame.nDeathTimes + 1;
	end
end

function Instance:GetMyInstance(nPlayerId)
	local pInstance = Instance.tbPlayerInstances[nPlayerId];
	
	return pInstance;
end

function Instance:OnCharacterEnterScene(dwCharacterId)
	local pInstance = nil;
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return
	end

	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneType(nMapId) ~= 1 or self.tbSetting[nMapId] == nil then
		return;
	end
	
	pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if pInstance then
		return;
	end
	
	-- 消耗次数
	--if Instance:IsTaskInstance(nMapId) ~= 1 then
		--Player.tbDegree:ReduceDegree(pPlayer, "Instance", 1);
	--end
	
	Player:FullHealth(pPlayer);	-- 回满血
	Player:ClearTempRevive(pPlayer);	-- 清空复活信息
	
	pInstance = Lib:NewClass(self.BaseGame);
	pInstance:InitGame(nMapId, pPlayer.dwSceneId, pPlayer.dwId, self.tbSetting[nMapId]);
	
	-- 注册副本定时关闭
	print("OnCharacterEnterScene", self.tbSetting[nMapId].nNoTimeLimit)
	if self.tbSetting[nMapId].nNoTimeLimit == nil then
	    local nTimerId = Timer:Register(Instance.END_TIME * Env.GAME_FPS, pInstance.CloseGame, pInstance);
	    pInstance.nTimerId = nTimerId;
	end
	
	pInstance.nStartTime = KGameBase.GetTime();
	
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId > 0 then
		local tbMembers = KTeam.GetTeamMemberList(nTeamId);
		for _, nPlayerId in ipairs(tbMembers) do
			local pMember = KGameBase.GetPlayerById(nPlayerId);
			if pMember then
				self.tbPlayerInstances[nPlayerId] = pInstance;
				pInstance:_JoinPlayer(pMember);
				CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_EXPSCENE_BEGIN, nPlayerId);
			end
		end
	else
		self.tbPlayerInstances[pPlayer.dwId] = pInstance;
		pInstance:_JoinPlayer(pPlayer);
		CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_EXPSCENE_BEGIN, pPlayer.dwId);
	end
	
	--Timer:Register(5 * Env.GAME_FPS, pInstance.StartGame, pInstance); --由客户端触发调用
end

function Instance:c2s_StartGame(pPlayer)
	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if pInstance then
		pInstance:StartGame();
	end
end

function Instance:c2s_UnlockGod(pPlayer, nIndex)
	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	pInstance.tbLock[nIndex]:UnLockMulti();
end

function Instance:NpcUnLock(pNpc)
	local tbTmp = pNpc.GetTempTable("Instance");
	if not tbTmp then
		return 0;
	end
	if (not tbTmp.tbInstance) or (not tbTmp.nLock) then
		return 0;
	end
	if not tbTmp.tbInstance.tbLock[tbTmp.nLock] then
		return 0;
	end
	
	tbTmp.tbInstance.tbLock[tbTmp.nLock]:UnLockMulti();
	
	-- KK:传递给这个Instance,记录杀敌数改变
	-- 判断锁的杀敌数要求是否大于0, 是则传送TargetInfo
	if (tbTmp.tbInstance.tbLock[tbTmp.nLock]:IsClose() ~= 1) then
		if (tbTmp.tbInstance.tbLock[tbTmp.nLock].nInitMultiNum > 0) then
			-- 更新targetInfo，显示杀人数量
			tbTmp.tbInstance:SetTargetInfo(-1, tbTmp.tbInstance.szTargetInfo, tbTmp.nLock);
		end
	end
end

function Instance:c2s_TryEnterInstance(pPlayer, nMapId)
	print("..Instance:c2s_TryEnterInstance ..nMapId = " .. nMapId );
	local nCurrentSweepId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID) or 0;
	if (nCurrentSweepId ~= 0 and self.tbSetting[nMapId].nTaskInstance == 1) then
		-- 当前有正在扫荡的副本，不能进入副本
		pPlayer.SysMsg(Lang.mission.str560[Lang.Idx]);
		return;
	end

	local nTotalCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT);
	local nRemainSweepCount = Instance:GetRemainInstanceCount(pPlayer);
	local bFinish = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_INSTANCE_FINISH) or 0;
	--print("___________________________" .. nRemainSweepCount - nTotalCount .. self.tbSetting[nMapId].nTaskInstance .. bFinish .. "___________________________")
	if (nRemainSweepCount - nTotalCount <= 0 and self.tbSetting[nMapId].nTaskInstance == 1 and bFinish ~= 0) then
		pPlayer.SysMsg(Lang.mission.str561[Lang.Idx]);
		return;
	end

	local InstanceNum = Player.tbDegree:GetDegree(pPlayer, "Instance");
	if(InstanceNum <= 0) then
		pPlayer.SendBlackMsg(Lang.mission.str561[Lang.Idx])
		return
	end
	if (nMapId and self.tbSetting[nMapId] and self.tbSetting[nMapId].nTaskInstance == 1) then
    	-- 根据策划需求，进单人任务副本时需要自动退出队伍，见单人剧情副本扫荡
    	if (pPlayer.GetTeamId() > 0) then
    		pPlayer.LeaveTeam();
    	end
    end

	--[[
	local nTeamId = pPlayer.GetTeamId();
	if (nTeamId > 0) then
		local tbTeamMem, nTeamNum = KTeam.GetTeamMemberList(nTeamId);
		if (nTeamNum >= 2) then
			pPlayer.SysMsg("只有一个人才能扫荡副本");
			return;
		end
	end
	--]]

	local nFinishTaskGroupId = 1022;
	local bFinish = pPlayer.pTask.GetTask(nFinishTaskGroupId, nMapId) or 0;
	print(string.format("..bFinish=%d",bFinish));
	if (nMapId == 0) then
		-- 随机一个副本ID
		local tbInstanceIds = PVEMatch:GetPlayerSuitableInstanceIndexes(pPlayer.GetLevel(), 0);
		if (#tbInstanceIds > 0) then
			local nRand = KUnify.MathRandom(1, #tbInstanceIds);
			-- print(tbInstanceIds[nRand]);
			Instance:TryEnterInstance({pPlayer.dwId}, tbInstanceIds[nRand]);
			bFinish = pPlayer.pTask.GetTask(nFinishTaskGroupId, tbInstanceIds[nRand]) or 0;
		end
	else
		Instance:TryEnterInstance({pPlayer.dwId}, nMapId,bFinish);
	end
	
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_INSTANCE_FINISH, bFinish);
end

-- 领取玩家奖励
function Instance:c2s_GetAward(pPlayer)
	print("..Instance:c2s_GetAward pPlayer.dwId=",pPlayer.dwId);
	local nMapId = pPlayer.dwSceneTemplateId;

	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if not pInstance or not pInstance.bFinishTask then
		return;
	end
	
	if pInstance.bFinishTask and pInstance.bFinishTask == 1 then
		Task:OnFinishMission(pPlayer, pInstance.nMapId);
		-- 客户端完成副本事件
		pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:FinishMissionEvent", pInstance.nMapId});
	end

	if (pInstance.tbAwardedPlayers == nil) then
		pInstance.tbAwardedPlayers = {};
	end

	if (pInstance.tbAwardedPlayers[pPlayer.dwId] == 1) then
		return;
	end
	pInstance.tbAwardedPlayers[pPlayer.dwId] = 1;
	
	--if KAward.HasAssetAward("Instance", pInstance.nMapId) == 1 then
	--	KAward.AddPlayerAward(pPlayer, "Instance", pInstance.nMapId, 1, 100, 0);
	--end

	-- 离开副本
	Instance:LeaveMyInstance(pPlayer);
	--Timer:Register(Env.GAME_FPS * 10, Instance.LeaveMyInstance, Instance, pPlayer);
	
	--[[
	local tbAwardInfo = self.tbTaskInstanceAward[pInstance.nMapId];
	if (tbAwardInfo == nil) then
		return;
	end
	
	pInstance.nShowReward = 0;

	for _, tbSlotAward in pairs(tbAwardInfo) do
		if (tbSlotAward.Type == 1) then -- 奖励装备
			local tbItemGDPL = Lib:SplitStr(tbSlotAward.Value, ",");
			if (tbItemGDPL ~= nil) then
				local g = tonumber(tbItemGDPL[1]);
				local d = tonumber(tbItemGDPL[2]);
				local p = tonumber(tbItemGDPL[3]);
				local l = tonumber(tbItemGDPL[4]);
				local num = tonumber(tbItemGDPL[5]);

				if (g ~= nil and g > 0 and 
					d ~= nil and d > 0 and 
					p ~= nil and p > 0 and 
					l ~= nil and l >= 0) then
					KItem.AddPlayerItem(pPlayer, g, d, p, l, num);
				end
			end
		elseif (tbSlotAward.Type == 2) then -- 奖励金钱
			local nMoney = tonumber(tbSlotAward.Value);
			if (nMoney > 0) then
				pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, nMoney);
			end
		elseif (tbSlotAward.Type == 3) then -- 奖励经验
			local nExpPercent = tonumber(tbSlotAward.Value);
			
			if (nExp > 0) then
			   nLevelExp = KGameBase.GetNextLevelExp(pPlayer.GetLevel() + 1)
			   local nExp = nLevelExp * nExpPercent;
				 pPlayer.AddExp(nExp);
			end
		elseif (tbSlotAward.Type == 4) then -- 奖励游龙真气
			local nYLEnergy = tonumber(tbSlotAward.Value);
			if (nYLEnergy > 0) then
				pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, nYLEnergy);
			end
		end
	end
	--]]

	-- 如果是任务副本则需要减少进入的次数
	local nToday = Lib:GetLocalDay(KGameBase.GetTime());
	local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME) or 0;
	local nStartDay = Lib:GetLocalDay(nStartTime);
	local bFinish = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_INSTANCE_FINISH) or 0;
	if (pInstance.tbInstanceSetting.nTaskInstance ~= nil and pInstance.tbInstanceSetting.nTaskInstance == 1 and bFinish ~= 0 and (nToday == nStartDay or nStartTime == 0)) then
	   	local nOldCount = Instance:GetRemainInstanceCount(pPlayer);
	   	if (nOldCount > 0) then
	   		Instance:SetRemainInstanceCount(pPlayer, nOldCount - 1);
	   	end
	end

	-- 困难副本增加扫荡功能
	if(nMapId >= 916 and nMapId < 1000) then
		pPlayer.pTask.DirectSetTask(1022, nMapId, 1);
	end
	
end

function Instance:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szGroup)
	local pInstance = Instance:GetMyInstance(dwExValue);
	if not pInstance or pInstance.nSceneId ~= dwSceneId then
		return;
	end
	
	-- 将生成的npc重新加入到group里面
	for _, pNpc in ipairs(tbRetNpc) do
		pInstance:AddNpcInGroup(pNpc, szGroup);
	end
	
	if pInstance.tbDelRegenerateNpc[szGroup] then
		pInstance:DelNpc(szGroup);
	end
end

-- 增加今日完成副本的次数
function Instance:AddTodayInstanceTimes(pPlayer)
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

-- 得到今日完成副本的次数
function Instance:GetTodayInstanceTimes(pPlayer)
	local nTimes = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.FINISH_TASK_ID);
	local nDay = pPlayer.pTask.GetTask(self.FINISH_TASK_GID, self.LAST_FINISH_DAY);
	local nToday = Lib:GetLocalDay(KGameBase.GetTime());
	if nDay ~= nToday then
		nTimes = 0;
	end
	return nTimes;
end

-- 设置玩家剩余进入副本次数
function Instance:SetRemainInstanceCount(pPlayer, nNewCount)
	if (nNewCount and nNewCount >= 0) then
		-- if (nNewCount > self.MAX_SWEEP_COUNT) then
			-- nNewCount = self.MAX_SWEEP_COUNT; -- 直接设置的次数不能超过这个值
		-- end
		-- pPlayer.pTask.DirectSetTask(self.REMAIN_INSTANCE_COUNT_GID, self.REMAIN_INSTANCE_COUNT_ID, nNewCount);
		
		Player.tbDegree:SetDegree(pPlayer, "Instance", nNewCount, 1);
	end
end

-- 获取玩家剩余副本次数
function Instance:GetRemainInstanceCount(pPlayer)
	-- local nCount = pPlayer.pTask.GetTask(self.REMAIN_INSTANCE_COUNT_GID, self.REMAIN_INSTANCE_COUNT_ID) or 0;
	-- local nDay = pPlayer.pTask.GetTask(self.REMAIN_INSTANCE_COUNT_GID, self.REMAIN_INSTANCE_COUNT_DAY_ID) or 0;
	-- local nToday = Lib:GetLocalDay(KGameBase.GetTime());

	-- if (nCount == 0 and nDay == 0) then -- 新号首次登录
		-- nCount = self.MAX_AUTO_ADD_COUNT;
	-- end

	-- nCount = math.ceil(nCount);
	
	-- if (nDay ~= nToday) then
	    -- pPlayer.pTask.DirectSetTask(self.REMAIN_INSTANCE_COUNT_GID, self.REMAIN_INSTANCE_COUNT_DAY_ID, nToday);

	    -- if (nCount < self.MAX_AUTO_ADD_COUNT) then
	    	-- nCount = nCount + self.DAY_AUTO_ADD_COUNT;

	    	-- if (nCount > self.MAX_AUTO_ADD_COUNT) then
	    		-- nCount = self.MAX_AUTO_ADD_COUNT; -- 自然增长的不能超过最大值
	    	-- end
	    -- end

		-- pPlayer.pTask.DirectSetTask(self.REMAIN_INSTANCE_COUNT_GID, self.REMAIN_INSTANCE_COUNT_ID, nCount);
	-- end
	local nCount = Player.tbDegree:GetDegree(pPlayer, "Instance") or 0;
	return nCount;
end

function Instance:c2s_StartSweepInstance(pPlayer, nInstaceId, nCount)
	Instance:StartSweepInstance(pPlayer.dwId, nInstaceId, nCount)
end

-- 开始扫荡关卡
function Instance:StartSweepInstance(nPlayerId, nInstaceId, nCount)
	print("..Instance:StartSweepInstance(nPlayerId, nInstaceId, nCount)",nPlayerId, nInstaceId, nCount);
	if (nPlayerId == nil or nPlayerId <= 0) then
		return
	end

	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer == nil) then
		print("..pPlayer == nil");
		return;
	end

	if (Instance.tbSetting[nInstaceId] == nil) then
		print("..Instance.tbSetting[nInstaceId] == nil");
		return;
	end

	if (pPlayer.GetLevel() < 20 or pPlayer.GetLevel() < Instance.tbSetting[nInstaceId].nMinLevel) then
		print("..pPlayer.GetLevel() < 20..");
		return;
	end

	-- 战斗地图中不能扫荡
	local nMapId = pPlayer.dwSceneTemplateId;
	local nState = KScene.GetSceneFightState(nMapId);
	if (nState > 0) then
		print("..if (nState > 0) then");
		return;
	end

	-- 必须是任务副本才能扫荡
	-- if (Instance.tbSetting[nInstaceId].nTaskInstance == nil or Instance.tbSetting[nInstaceId].nTaskInstance ~= 1) then
		-- return;
	-- end

	-- 必须已经打通一次才能扫荡
	local nFinishTaskGroupId = 1022;
	local bFinish = pPlayer.pTask.GetTask(nFinishTaskGroupId, nInstaceId) or 0;
	if (bFinish == 0) then
		print("..if (bFinish == 0) then");
		return;
	end

	nCount = math.ceil(nCount);
	if (nCount == nil or nCount <= 0) then
		print("..nCount == nil or nCount <= 0");
		return;
	end

	if (nCount + Instance:GetRemainInstanceCount(pPlayer) > self.MAX_SWEEP_COUNT) then
		print("..if (nCount + Instance:GetRemainInstanceCount(pPlayer) > self.MAX_SWEEP_COUNT) then");
		return;
	end

	local nCurrentSweepId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID);
	if (nCurrentSweepId and nCurrentSweepId > 0) then
		-- 当前已经有正在扫荡的副本，不能重复扫荡
		print("..if (nCurrentSweepId and nCurrentSweepId > 0) then");
		return;
	end

	-- 开始扫荡
	local nStartTime = KGameBase.GetTime();
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID, nInstaceId);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME, nStartTime);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT, nCount);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT, 0);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH, 0);

	local nTime = self.SWEEP_ONCE_TIME * nCount * Env.GAME_FPS;-- * Vip:GetRate(pPlayer, Vip.ID_INSTANCE_SPEED) / 100;
	self.nSweepTimerId = Timer:Register(nTime, self.CheckSweepInstance, self, nPlayerId);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, self.nSweepTimerId);
	
	pPlayer.SetMopState(PVEMatch.INSTANCE_MOP);

	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:StartSweepInstance", nInstaceId, nCount, nStartTime});
end

function Instance:c2s_SpeedUpInstance(pPlayer)
	if pPlayer == nil then
		return;
	end
	
	local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID) or 0;
	if (nInstanceId == 0) then
		-- 当前无正在扫荡副本
		return;
	end
	
	local nTotalCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT) or 0;
	if (nTotalCount == 0) then
		pPlayer.SendBlackMsg(Lang.mission.str562[Lang.Idx])
		return;
	end
	
	local vipRate = Vip:GetRate(pPlayer, Vip.ID_INSTANCE_SPEED);
	local nIsSpeed = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_IS_SPEED);
	if nIsSpeed == 1 and vipRate > 0 then 
		return;
	end

	local nCurTime = KGameBase.GetTime();
	local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME) or 0;
	local nFinishCount = (nCurTime - nStartTime) / self.SWEEP_ONCE_TIME;
		
	-- 取整
	local nCeil = math.ceil(nFinishCount);
	if (nCeil ~= nFinishCount) then
		nFinishCount = nCeil - 1;
	end
		
	if (nFinishCount > nTotalCount) then
		nFinishCount = nTotalCount;
		return;
	end
	
	-- local nNeedGold = Instance.BUY_NEED_COIN * (nTotalCount - nFinishCount);
	-- if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold and pPlayer.GetVipLevel() > 0 then
		-- pPlayer.SendBlackMsg(string.format("扫荡加速需要%s元宝，您的元宝不足", nNeedGold))
		-- return;
	-- end
	
	local nLeftTime = (self.SWEEP_ONCE_TIME * nFinishCount + (self.SWEEP_ONCE_TIME * (nTotalCount - nFinishCount)) * vipRate / 100 - (nCurTime - nStartTime) - 1) * Env.GAME_FPS ;
	
	-- if pPlayer.GetVipLevel() > 0 then
		-- pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -1 * nNeedGold);
	-- end
	
	local nTimerId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID) or 0;
	if (nTimerId and nTimerId > 0) then
		Timer:Close(nTimerId);
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, 0);
	end
	
	if nLeftTime > 0 and vipRate > 0 then
		
		self.nSweepTimerId = Timer:Register(nLeftTime, self.CheckSweepInstance, self, pPlayer.dwId);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SpeedUpInstance", nFinishCount});
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_SPEED_COUNT, nFinishCount);
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_IS_SPEED, 1);
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, self.nSweepTimerId);
		--pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH, 1);
		--pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT, nFinishCount);
	else
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_IS_SPEED, 1);
		Instance:StopSweepInstance(pPlayer.dwId, nTotalCount);
	end
end

function Instance:c2s_StopSweepInstance(pPlayer)
	Instance:StopSweepInstance(pPlayer.dwId);
end

function Instance:c2s_BuyCount(pPlayer, nNum)
	if pPlayer == nil then
		return;
	end
	
	if Vip:IsOpen(pPlayer, Vip.ID_INSTANCE) ~= 1 then
		return 0;
	end
	
	nNum = nNum or 1;
	
	if Vip:CanBuyCount(pPlayer, Vip.ID_INSTANCE, nNum) ~= 1 then
		pPlayer.SendBlackMsg(Lang.mission.str563[Lang.Idx]);
		return 0;
	end
	
	local nNeedGold = Instance.BUY_NEED_COIN * nNum;
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str564[Lang.Idx], nNeedGold))
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,117);
	
	Vip:DoBuyCount(pPlayer, Vip.ID_INSTANCE, nNum);
	
end

-- 停止扫荡关卡
function Instance:StopSweepInstance(nPlayerId, nFinishCount)
	print("..Instance:StopSweepInstance(nPlayerId, nFinishCount)",nPlayerId, nFinishCount);
	if (nPlayerId == nil or nPlayerId <= 0) then
		return;
	end

	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer == nil) then
		return;
	end
	
	local nTimerId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID) or 0;
	if (nTimerId and nTimerId > 0) then
		Timer:Close(nTimerId);
		--self.nSweepTimerId = 0;
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, 0);
	end

	local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID) or 0;
	if (nInstanceId == 0) then
		-- 当前无正在扫荡副本
		return;
	end

	local nTotalCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT) or 0;
	if (nTotalCount == 0) then
		return;
	end

	if (nFinishCount == nil) then
		local nCurTime = KGameBase.GetTime();
		local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME) or 0;
		nFinishCount = (nCurTime - nStartTime) / self.SWEEP_ONCE_TIME;
			
		-- 取整
		local nCeil = math.ceil(nFinishCount);
		if (nCeil ~= nFinishCount) then
		    nFinishCount = nCeil - 1;
		end
			
			
		if (nFinishCount > nTotalCount) then
			nFinishCount = nTotalCount;
		end
	end

	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH, 1);
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT, nFinishCount);
	
	local nTotalFinishCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.TOTAL_SWEEP_FINISH_COUNT) or 0;
	pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.TOTAL_SWEEP_FINISH_COUNT, nTotalFinishCount + nFinishCount); -- 记录历史完成次数


	--提前给奖励
	local bFinish = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH);
	if (bFinish ~= 1) then
		return;
	end
	local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID);
	local nFinishCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT);
	if (nInstanceId > 0 and nFinishCount >= 0 and nFinishCount < self.MAX_SWEEP_COUNT) then
		if KAward.HasAssetAward("SweepInstance", nInstanceId) == 1 and nFinishCount > 0 then
			KAward.AddPlayerAward(pPlayer, "SweepInstance", nInstanceId, nFinishCount);
		end
	end
	
	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:StopSweepInstance", nInstanceId, nTotalCount, nFinishCount});
end

function Instance:c2s_FinishSweepInstance(pPlayer)
	Instance:FinishSweepInstance(pPlayer);
end

function Instance:FinishSweepInstance(pPlayer)
	print("..Instance:FinishSweepInstance pPlayer.dwId",pPlayer.dwId)
	if (pPlayer == nil) then
		return; 
	end

	local bFinish = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH);
	if (bFinish ~= 1) then
		-- 正在扫荡中，必须先停止扫荡才能完成扫荡
		return;
	end
	
	pPlayer.SetMopState(0);

	local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID);
	local nFinishCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT);
	
	local nNewCount = 0;
	
	if (nInstanceId > 0 and nFinishCount >= 0 and nFinishCount < self.MAX_SWEEP_COUNT) then
--		if KAward.HasAssetAward("Instance", nInstanceId) == 1 and nFinishCount > 0 then
--			KAward.AddPlayerAward(pPlayer, "Instance", nInstanceId, nFinishCount);
--		end
		--[[
		local tbAwardInfo = self.tbTaskInstanceAward[nInstanceId];
		if (tbAwardInfo and nFinishCount > 0) then
			for _, tbSlotAward in pairs(tbAwardInfo) do
				if (tbSlotAward.Type == 1) then -- 奖励装备
					local tbItemGDPL = Lib:SplitStr(tbSlotAward.Value, ",");
					if (tbItemGDPL ~= nil) then
						local g = tonumber(tbItemGDPL[1]);
						local d = tonumber(tbItemGDPL[2]);
						local p = tonumber(tbItemGDPL[3]);
						local l = tonumber(tbItemGDPL[4]);

						if (g ~= nil and g > 0 and 
							d ~= nil and d > 0 and 
							p ~= nil and p > 0 and 
							l ~= nil and l >= 0) then
							KItem.AddPlayerItem(pPlayer, g, d, p, l);
						end
					end
				elseif (tbSlotAward.Type == 2) then -- 奖励金钱
					local nMoney = tonumber(tbSlotAward.Value);
					if (nMoney > 0) then
						pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, nMoney * nFinishCount);
					end
				elseif (tbSlotAward.Type == 3) then -- 奖励经验
					local nExp = tonumber(tbSlotAward.Value);
					if (nExp > 0) then
						pPlayer.AddExp(nExp * nFinishCount);
					end
				elseif (tbSlotAward.Type == 4) then -- 奖励游龙真气
					local nYLEnergy = tonumber(tbSlotAward.Value);
					if (nYLEnergy > 0) then
						pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, nYLEnergy * nFinishCount);
					end
				end
			end
		end
		--]]
		
		local nToday = Lib:GetLocalDay(KGameBase.GetTime());
		local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME) or 0;
		local nStartDay = Lib:GetLocalDay(nStartTime);
		local nRemainCount = Instance:GetRemainInstanceCount(pPlayer);
		nNewCount = nRemainCount;
		if (nRemainCount ~= nil and nToday == nStartDay) then
			nNewCount = nRemainCount - nFinishCount;
			if (nNewCount < 0) then
				nNewCount = 0;
			end
		end
			
		Instance:SetRemainInstanceCount(pPlayer, nNewCount);
		
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID, 0);
	    pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME, 0);
	    pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT, 0);
	    pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH_COUNT, 0);
	    pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_FINISH, 0);
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_SPEED_COUNT, 0);
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_IS_SPEED, 0);

	    pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:FinishSweepInstance", nNewCount});
	end
end

function Instance:CheckSweepInstance(nPlayerId, bSyncInfo)
	bSyncInfo = bSyncInfo or 0;
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	
	local nRetCode = 0; -- 必须返回0停止定时器
	if (nPlayerId == nil or nPlayerId <= 0 or pPlayer == nil) then
		Instance:StopSweepInstance(nPlayerId, 0);
		return nRetCode;
	end

	local nTimerId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID) or 0;
	if (nTimerId and nTimerId > 0) then
		Timer:Close(nTimerId);
		--self.nSweepTimerId = 0;
		pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, 0);
	end

	if (pPlayer) then
		local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID);
		if (nInstanceId == nil or Instance.tbSetting[nInstanceId] == nil) then
			Instance:StopSweepInstance(nPlayerId, 0);
			return nRetCode;
		end

		local nCurTime = KGameBase.GetTime();
		local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME);
		local nElpasedTime = nCurTime - nStartTime;
		if (nElpasedTime < 0) then
		    Instance:StopSweepInstance(nPlayerId, 0);
		    return nRetCode;
		end
		
		local nTotalCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT);
		local nFinishCount = nElpasedTime / self.SWEEP_ONCE_TIME;
		local nRemainSceonds = nTotalCount * self.SWEEP_ONCE_TIME - nElpasedTime;
		local nSpeedCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_SPEED_COUNT);
		local nIsSpeed = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_IS_SPEED);
		if nIsSpeed == 1 then
			if Vip:GetRate(pPlayer, Vip.ID_INSTANCE_SPEED) ~= 0 then
				nFinishCount = (nElpasedTime - nSpeedCount * self.SWEEP_ONCE_TIME) / self.SWEEP_ONCE_TIME / Vip:GetRate(pPlayer, Vip.ID_INSTANCE_SPEED) * 100;
				nRemainSceonds = nSpeedCount * self.SWEEP_ONCE_TIME + (nTotalCount - nSpeedCount) * self.SWEEP_ONCE_TIME * Vip:GetRate(pPlayer, Vip.ID_INSTANCE_SPEED) / 100 - nElpasedTime;
			else
				nFinishCount = nTotalCount;
			end
		end

		local nCeil = math.ceil(nFinishCount);
		if (nCeil ~= nFinishCount) then
			nFinishCount = nCeil - 1;
		end
		local bOver = 0;
		if (nFinishCount >= nTotalCount) then
			nFinishCount = nTotalCount;
			bOver = 1;
		end

		if (bOver == 1) then
			Instance:StopSweepInstance(nPlayerId, nFinishCount);
		elseif (bOver == 0) then
			local nSeconds = math.ceil(nRemainSceonds);
			if (nSeconds ~= nRemainSceonds) then
				nRemainSceonds = nSeconds - 1;
			end
		    if (nRemainSceonds > 0) then
		        self.nSweepTimerId = Timer:Register(nRemainSceonds * Env.GAME_FPS, self.CheckSweepInstance, self, nPlayerId);
				pPlayer.pTask.DirectSetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_TIMER_ID, self.nSweepTimerId);
			else
				Instance:StopSweepInstance(nPlayerId, nTotalCount);
		    end

			if (bSyncInfo == 1) then
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SyncSweepInfo", nInstanceId, nTotalCount, nStartTime, nIsSpeed, nSpeedCount});
			end
		end
	end

	return nRetCode;
end

function Instance:GetCurrentSweepInstance(pPlayer)
	if (pPlayer) then
		local nInstanceId = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_INSTANCE_ID);
		local nStartTime = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_START_TIME);
		local nTotalCount = pPlayer.pTask.GetTask(self.CURRENT_SWEEP_TASK_GID, self.CURRENT_SWEEP_COUNT);
		return nInstanceId, nStartTime, nTotalCount;
	end
end

function Instance:IsTaskInstance(nMapId)
	local tbSetting = Instance.tbSetting[nMapId];
	if tbSetting then
		if tbSetting.nTaskInstance and tbSetting.nTaskInstance == 1 then
			return 1;
		end
	end
	return 0;
end

function Instance:OnLogin(nPlayerId)
	Instance:CheckSweepInstance(nPlayerId, 1);
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if pInstance then
		pInstance:OnLogin(pPlayer);
	end
end

function Instance:OnLogout(nPlayerId)
	--self:CheckSweepInstance(nPlayerId, 1);
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if pInstance then
		pInstance:OnLogout(pPlayer); 
	end
end

-- 退出副本
function Instance:LeaveMyInstance(pPlayer)
	local pInstance = Instance:GetMyInstance(pPlayer.dwId);
	if pInstance then
		pInstance:CloseGame()
	end
end
-- 注册登录事件
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Instance.OnLogin, Instance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Instance.OnLogout, Instance);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_DEATH, Instance.OnDeath, Instance);

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Instance.OnCharacterEnterScene, Instance);