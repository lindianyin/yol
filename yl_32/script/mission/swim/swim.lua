--泡澡活动

if not Swim.tbSetting then
	Swim.tbSetting =
	{
		Name = "泡澡",						-- 名称
		RequireLevel = 15,					--开放等级
		tbOpenTimes = {{123000,130000},{153000,160000}},	--开放时间
		BaseTime = 30,						--基础时间单位，30表示30秒发一次奖励
		tbAwardCfg = {},					--奖励配置表
		nMapId = 5,							--泡澡地图ID
		nRemindStartIdx = 8,				--开始泡澡倒计时提醒
		nRemindGoingIdx = 9,				--泡澡进行中提醒
		nRemindEndIdx	= 10,				--泡澡结束倒计时提醒
		REMIND_START_TIME = 300,			--300s开始倒计时
		REMIND_END_TIME	  = 300,			--300s结束倒计时
		tbJoinPos = {
						[4]={{3994,7431,1048576},	--通天镇
						 {3408,7319,1048576},
						 {3198,6934,1048576},
						 {2608,7429,1048576},
						 },
						[5]={{5854,6414,1048576},	--凌霄殿
						 {5881,6926,1048576},
						 {6214,7186,1048576},
						 {6638,7240,1048576},
						 {6864,7036,1048576},
						 {7191,6907,1048576},
						 {7254,6566,1048576},
						 {7027,6265,1048576}},
						[6]={{4193,4851,1048576},	--蓬莱岛
						 {3761,5349,1048576},
						 {3443,5801,1048576},
						 {3735,5818,1048576},
						 {4115,5864,1048576},
						 {4510,5802,1048576},
						 {4869,5608,1048576},
						 {4442,5424,1048576}},
					},
	};
end

function Swim:c2s_JoinSwim(pPlayer)
	if self.IsOpen == 0 then
		pPlayer.SendBlackMsg(Lang.mission.str984[Lang.Idx]);
	elseif pPlayer.GetLevel() < Swim.tbSetting.RequireLevel then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str985[Lang.Idx],Swim.tbSetting.RequireLevel));
	elseif pPlayer.IsSwim() == 1 then
		pPlayer.SendBlackMsg(string.format(Lang.mission.str986[Lang.Idx]));
	else

		local nCount = Player.tbDegree:GetDegree(pPlayer, "JoinPZ")
		if nCount == 0 then
			pPlayer.SendBlackMsg(Lang.mission.str987[Lang.Idx]);
			return
		end

		--Player.tbDegree:ReduceDegree(pPlayer, "JoinPZ", 1)

		local nMapId = Swim.tbSetting.nMapId;
		if Swim.tbSetting.tbJoinPos[pPlayer.dwSceneTemplateId] ~= nil then
			nMapId = pPlayer.dwSceneTemplateId;
		end
		local nRand = KUnify.MathRandom(1, #Swim.tbSetting.tbJoinPos[nMapId]);
		local tbPos = Swim.tbSetting.tbJoinPos[nMapId][nRand];
		if pPlayer.dwSceneTemplateId == nMapId then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:AutoGoTo", nMapId, tbPos[1],tbPos[2], tbPos[3]});
		else
			pPlayer.TeleportTo(nMapId, tbPos[1], tbPos[2], tbPos[3]);
		end
		
		--统计点  ldy
		local szStr = string.format("name=%s","JoinSwim");
		pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,68,szStr);
		
	end
end

function Swim:Init()
	self.nRequireLevel = Swim.tbSetting.RequireLevel;
	self.tbOpenTimes = Swim.tbSetting.tbOpenTimes;
	self.nBaseTime = Swim.tbSetting.BaseTime;
	self.nStartTime = 0;	--记录当前正在进行的开始时间秒数
	self.nEndTime = 0;		--记录当前正在进行的结束时间秒数
	self.IsOpen = 0;		--是否开启的标记
	self.tbPlayerExp = {};	--玩家累计经验
	self.tbPlayerEnergy = {};	--玩家累计游龙真气
	self.tbPlayerLastState = {}; --上次是否在泡澡状态
end

function Swim:SetSwimOpenTime(nStartTime, nEndTime)
	assert(nStartTime and nEndTime);
	assert(nStartTime < nEndTime);
	table.insert(self.tbOpenTimes, {nStartTime, nEndTime});
end

function Swim:ProcessSwimming()
	local curDayTime = Lib:GetLocalDayTime();

	if self.IsOpen == 0 then
		assert(Swim.tbSetting.tbOpenTimes);
		for _, tbOpenTime in pairs(Swim.tbSetting.tbOpenTimes) do
			local startTime = Timer:CycleTimeToSeconds(tbOpenTime[1]);
			local endTime = Timer:CycleTimeToSeconds(tbOpenTime[2]);
			if curDayTime == startTime + Swim.tbSetting.REMIND_START_TIME then
				--对所有玩家提醒
				local tbAllPlayer = KGameBase.GetAllPlayers();
				for _, pPlayer in pairs(tbAllPlayer) do
					Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindStartIdx, Swim.tbSetting.REMIND_START_TIME);
				end
			end

			if  curDayTime >= startTime and curDayTime <= endTime then
				self.IsOpen = 1;
				self.nStartTime = startTime;
				self.nEndTime = endTime;
				KChat.BroadcastNoticeMsg(Lang.mission.str988[Lang.Idx]);

				local tbAllPlayer = KGameBase.GetAllPlayers();
				for _, pPlayer in pairs(tbAllPlayer) do
					Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindGoingIdx, -1);
				end
				break;
			end
		end
	else	--泡澡进行中
		if curDayTime > self.nEndTime then
			Swim:ResetData();
			return;
		end
		local tbCurAwardCfg = nil;
		--是否发放奖励
		for nShowId, tbCfg in pairs(Swim.tbSetting.tbAwardCfg) do
			if curDayTime == self.nStartTime + nShowId * Swim.tbSetting.BaseTime then
				tbCurAwardCfg = tbCfg;
				break;
			end
		end

		--给所有在泡澡中的玩家发奖励，遍历gs玩家
		local nRestTimeSeconds = self.nEndTime - curDayTime;
		local tbAllPlayer = KGameBase.GetAllPlayers();
		for _, pPlayer in pairs(tbAllPlayer) do
			--结束倒计时提醒
			if nRestTimeSeconds ==  Swim.tbSetting.REMIND_END_TIME then
				local tbAllPlayer = KGameBase.GetAllPlayers();
				for _, pPlayer in pairs(tbAllPlayer) do
					Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindEndIdx, Swim.tbSetting.REMIND_END_TIME);
				end
			end

			if pPlayer.GetLevel() >= Swim.tbSetting.RequireLevel then
				if pPlayer.IsSwim() == 1 then
					if not self.tbPlayerExp[pPlayer.dwId] then
						self.tbPlayerExp[pPlayer.dwId] = 0;
					end
					if not self.tbPlayerEnergy[pPlayer.dwId] then
						self.tbPlayerEnergy[pPlayer.dwId] = 0;
					end

					if tbCurAwardCfg ~= nil then
						local totalExp = KGameBase.GetNextLevelExp(pPlayer.GetLevel() + 1) * tbCurAwardCfg.nExp / 100;
						local addExp = math.floor( totalExp * Swim.tbSetting.BaseTime / (self.nEndTime - self.nStartTime)); 	--lua里计算都是double，可以不用考虑溢出
						self.tbPlayerExp[pPlayer.dwId] = self.tbPlayerExp[pPlayer.dwId] + addExp;
						self.tbPlayerEnergy[pPlayer.dwId] = self.tbPlayerEnergy[pPlayer.dwId] + tbCurAwardCfg.nEnergy;
						pPlayer.AddExp(addExp);
						pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, tbCurAwardCfg.nEnergy);
						pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetSwimPanelContent", self.tbPlayerExp[pPlayer.dwId], self.tbPlayerEnergy[pPlayer.dwId], nRestTimeSeconds});
					end
					--当玩家进入泡澡的时候执行一次发送
					if not self.tbPlayerLastState[pPlayer.dwId] then
						pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetSwimPanelContent", self.tbPlayerExp[pPlayer.dwId], self.tbPlayerEnergy[pPlayer.dwId], nRestTimeSeconds});
						self.tbPlayerLastState[pPlayer.dwId] = 1;
					end
				else
					if self.tbPlayerExp[pPlayer.dwId] then
						self.tbPlayerExp[pPlayer.dwId] = 0;
					end
					if self.tbPlayerEnergy[pPlayer.dwId] then
						self.tbPlayerEnergy[pPlayer.dwId] = 0;
					end
					if self.tbPlayerLastState[pPlayer.dwId] then
						self.tbPlayerLastState[pPlayer.dwId] = nil;
					end
					--pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:HideSwimPanel"});
				end
			end
		end
	end
end

function Swim:ResetData()
	print("泡澡活动关闭");
	local tbAllPlayer = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbAllPlayer) do
		if pPlayer.GetLevel() >= Swim.tbSetting.RequireLevel and pPlayer.IsSwim() == 1 then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:HideSwimPanel"});
		end
	end
	self.IsOpen = 0;
	self.nStartTime = 0;
	self.nEndTime = 0;
	self.tbPlayerExp = {};
	self.tbPlayerEnergy = {};
	self.tbPlayerLastState = {};
end

function Swim:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);

	if not pPlayer then
		return;
	end

	local curDayTime = Lib:GetLocalDayTime();
	assert(Swim.tbSetting.tbOpenTimes);
	for _, tbOpenTime in pairs(Swim.tbSetting.tbOpenTimes) do
		local startTime = Timer:CycleTimeToSeconds(tbOpenTime[1]);
		local endTime = Timer:CycleTimeToSeconds(tbOpenTime[2]);
		if curDayTime > startTime - Swim.tbSetting.REMIND_START_TIME and curDayTime < startTime then
			Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindStartIdx, startTime - curDayTime);
		elseif self.IsOpen == 1 then
			local nRestTimeSeconds = self.nEndTime - curDayTime;
			if nRestTimeSeconds <= Swim.tbSetting.REMIND_END_TIME then
				Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindEndIdx, nRestTimeSeconds);
			else
				Remind:AddRemindToPlayer(pPlayer, Swim.tbSetting.nRemindGoingIdx, -1);
			end
		end
	end
end

--load配置
function Swim:LoadSwimAwardCfg()
	local cfgs = Swim.tbSetting.tbAwardCfg;
	local tbFileData = Lib:LoadTabFile("/setting/shower/shower_award.txt");
	for _, tbRow in pairs(tbFileData) do
		local nShowerId = tonumber(tbRow.ShowerId);
		assert(nShowerId);
		if not cfgs[nShowerId] then
			cfgs[nShowerId] = {
			nExp = tonumber(tbRow.Exp);
			nEnergy = tonumber(tbRow.ZhenQi);
			};
		end
	end
end

if #Swim.tbSetting.tbAwardCfg <= 0 then
	Swim:LoadSwimAwardCfg();
	Swim:Init();
end

Timer:Register(Env.GAME_FPS, Swim.ProcessSwimming, Swim)

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Swim.OnLogin, Swim);
