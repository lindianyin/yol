Include("script/mission/mission.lua");
Include("script/mission/level/xoyogame_def.lua");
-- 逍遥谷闯关活动脚本
-- Trap点加载
XoyoGame.BaseGame = Mission:New();
local BaseGame = XoyoGame.BaseGame;
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, BaseGame._OnLogout, BaseGame);

--[[
local tbTrap = {}
-- 玩家触发trap点
function tbTrap:OnPlayerTrap(szClassName)
	if not self.tbGame then
		return 0;
	end

	local tbRoom = self.tbGame:GetPlayerRoom(me.dwId);
	if tbRoom and tbRoom.OnPlayerTrap then
		tbRoom:OnPlayerTrap(szClassName);
	elseif self.tbGame.tbTrap[szClassName] then
		me.TeleportTo(me.nMapId, unpack(self.tbGame.tbTrap[szClassName]));
	end
end

-- 定义玩家进入地图事件
function tbTrap:OnEnter()
	if (not self.tbGame) or (not self.nMapId) then
		return 0;
	end
	self.tbGame:JoinNextGame(me);
end

-- 定义玩家离开地图事件
function tbTrap:OnLeave()

end
--]]

-- 初始化地图
function BaseGame:MapInit(nMapId)
	local tbMapTrap = Map:GetClass(nMapId);
	for szFnc in pairs(tbTrap) do			-- 复制函数
		tbMapTrap[szFnc] = tbTrap[szFnc];
	end
	tbMapTrap.tbGame = self;
	tbMapTrap.nMapId = nMapId;
end

-- 初始化关卡
function BaseGame:InitGame(tbMap, nTeamId)
	--[[for i = 1, #tbMap do      -- zzx
		self:MapInit(tbMap[i]);
	end]]
	self.tbMap = tbMap;
	self.tbRoom = {};			-- 房间对象表
	self.tbTrap = {};			-- Trap点传送
	self.tbTeam = {};			-- 队伍信息
	self.tbPlayer = {};			-- 玩家所在房间信息
	self.nTeamId = nTeamId;
	self.tbAddXoyoTimesPlayerId = {};   -- 已扣除逍遥次数的玩家id，防刷

	self:Open();
end

-- 报名下次闯关(不检查资格,资格检查在npc逻辑上做)  -- TODO
function BaseGame:JoinNextGame(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	if self.tbTeam[nTeamId] then
		return 0;
	end

	if nTeamId == 0 then
		Dbg:WriteLog("XoyoGame", string.format("有角色在逍遥谷内登陆了 %d", pPlayer.dwId));
		pPlayer.SysMsg(Lang.mission.str807[Lang.Idx]);
		Timer:Register(5 * Env.GAME_FPS, self.QuitMap, self, pPlayer.dwId);
		return 0;
	end

	pPlayer.GetTempTable("XoyoGame").tbGame = self;

	if self:GetPlayerGroupId(pPlayer) >= 0 then
		self:KickPlayer(pPlayer);
		return 0;
	end
	
	self:JoinPlayer(pPlayer, 1);
	
	if not self.tbPlayers then
		self.tbPlayers = {};
	end
	table.insert(self.tbPlayers, pPlayer.dwId);
end

function BaseGame:QuitMap(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		local tbPos = XoyoGame.tbPlayerLeavePos[nPlayerId];
		if tbPos then
			pPlayer.TeleportTo(tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ);
		else
			local tbLeavePos = XoyoGame.LEAVE_POS;
			pPlayer.TeleportTo(tbLeavePos[1], tbLeavePos[2], tbLeavePos[3], tbLeavePos[4]);
			--pPlayer.ReturnToLastCity();
		end
		XoyoGame.tbPlayerLeavePos[nPlayerId] = nil;
	end
	return 0;
end

function BaseGame:OnJoin(nGroupId)
	local nLastFrameTime = 0
	if self.nTimerId and self.nTimerId > 0 then
		nLastFrameTime = tonumber(Timer:GetRestTime(self.nTimerId));
	else
		nLastFrameTime = XoyoGame.PREPARE_TIME * Env.GAME_FPS;
	end
	if nLastFrameTime > 0 then
		local tbMembers, nCount = KTeam.GetTeamMemberList(self.nTeamId);
		for _, nPlayerId in ipairs(tbMembers) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId)
			if pPlayer then
				pPlayer.CallClientScript({"QClientScript.Mission:SetReadyStartTime", nLastFrameTime});
			end
		end
		
		--me.CallClientScript({"QClientScript.Mission:SetReadyStartTime", nLastFrameTime});
	end

	--XoyoGame.Achievement:JoinGames(me);
end

function BaseGame:LogOutRV()
	-- 拔萝卜用到了打雪仗技能
	for _, nSkillId in pairs(Esport.tbTemplateId2Skill) do
		if me.IsHaveSkill(nSkillId) == 1 then
			me.DelFightSkill(nSkillId);
		end
	end

	for _, nBuffId in pairs(Esport.tbTemplateId2Buff) do
		if me.GetSkillState(nBuffId) > 0 then
			me.RemoveSkillState(nBuffId);
		end
	end

	me.RemoveSkillState(1450); -- 头顶的萝卜状态
	XoyoGame.RoomCarrot.DeleteCarrotInBag(me);
end

function BaseGame:GetStartRoomLevel(nTeamId)
	local nDifficulty = XoyoGame:GetDifficuty(nTeamId);
	if nDifficulty == 1 then
		return 1;
	elseif nDifficulty == 2 then
		return 2;
	elseif nDifficulty == 3 then
		return 3;
	else
		assert(false);
	end
end

-- 开始一轮闯关
function BaseGame:StartNewGame()
	local tbTemp = {};
	local nTeamId = self.nTeamId;

	self.tbAddXoyoTimesPlayerId = {};
	self:CalcRoomOccupy();

	XoyoGame.tbStartTime[nTeamId] = KGameBase.GetTime();
	table.insert(tbTemp, nTeamId);
	self.tbTeam[nTeamId] = {};
	self.tbTeam[nTeamId].nCurRoomCount = 0; -- 玩过几个房间

	self:AddXoyoTimes(tbTemp);
	if #tbTemp > 0 then
		local nLevel = self:GetStartRoomLevel(nTeamId);
		self:RandomRoom(tbTemp, nLevel);
	end
	self.nNextTeamCount = 0;
	
	self.nTimerId = Timer:Register(XoyoGame.START_GAME_TIME * Env.GAME_FPS, self.CloseTimer, self);
end

function BaseGame:StartNewDTTGame(nLevel)
	local nTeamId = self.nTeamId;

	XoyoGame.tbStartTime[nTeamId] = KGameBase.GetTime();
	self.tbTeam[nTeamId] = {};

	if (nLevel == nil) then
		nLevel = 1;
	end

	nLevel = tonumber(nLevel);
	
	local tbRoomIds = XoyoGame.RoomSetting.RoomList[nLevel];
	if (tbRoomIds == nil) then
		return;
	end

	local tbRoomInfo = nil;
	local nRoomId = nil;
	for _, id in pairs(tbRoomIds) do
		local info = XoyoGame.RoomSetting.tbRoom[id];
		if (info.nType ~= nil and info.nType == 1) then
			tbRoomInfo = info;
			nRoomId = id;
		end
	end

	if (tbRoomInfo ~= nil and nRoomId ~= nil) then
		-- 必须在本服加载地图
		local nMapId = self.tbMap[XoyoGame.RoomSetting.tbRoom[nRoomId].nMapIndex];
		KScene.LoadScene(nMapId, nTeamId);

		self:AddRoom(nRoomId, 1, nTeamId);
		local nTimerId = Timer:Register(XoyoGame.ROOM_DTT_TIME * Env.GAME_FPS, self.EndRoomDTTTime, self, nRoomId);
		self.tbRoom[nRoomId].nTimerId = nTimerId;
		self.nCurDTTRoomId = nRoomId;
	end

	self.nTimerId = Timer:Register(XoyoGame.START_GAME_TIME * Env.GAME_FPS, self.CloseTimer, self);
end

function BaseGame:AddXoyoTimes(tbTeam)
	local fnExcute = function (pPlayer)
		self.tbAddXoyoTimesPlayerId[pPlayer.dwId] = 1;
		-- 须弥幻境不限制玩家进入的次数
		--[[local nTimes = XoyoGame:GetPlayerTimes(pPlayer)
		if nTimes > 0 then
			pPlayer.SetTask(XoyoGame.TASK_GROUP, XoyoGame.TIMES_ID, nTimes - 1);
		else
			Dbg:WriteLog("xoyogame", "Error 逍遥谷次数为0却仍然能进逍遥谷！");
		end
		pPlayer.AddOfferEntry(10, WeeklyTask.GETOFFER_TYPE_XOYOGAME);	-- 参加逍遥谷奖励10点贡献度

		-- 记录玩家参加逍遥谷的次数
		Stats.Activity:AddCount(pPlayer, Stats.TASK_COUNT_XOYOGAME, 1);

		-- KStatLog.ModifyAdd("xoyogame", string.format("本日参加了第%d次逍遥谷的人数", nTimes + 1), "总量", 1);

		-- 师徒成就：参加逍遥谷
		Achievement_ST:FinishAchievement(pPlayer.dwId, Achievement_ST.XOYOGAME);]]
	end
	XoyoGame.BaseRoom:TeamPlayerExcute(fnExcute, tbTeam);
end

function BaseGame:CloseTimer()
	self.nTimerId = nil;
	return 0;
end

-- 看看队里有没有没扣逍遥次数的玩家
function BaseGame:CheckTeamValidity(nTeamId)
	local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
	local nValidPlayerNum = 0;
	if not tbMember then
		print("Xoyogame","BaseGame找不到队伍");
		return 0;
	end
	for _, nPlayerId in ipairs(tbMember) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			if not self.tbAddXoyoTimesPlayerId[pPlayer.dwId] then
				--[[pPlayer.Msg("你是怎么进来逍遥的？别以为老夫年纪大了，眼睛就不好使了。"); -- zzx
				if  XoyoGame:GetPlayerGame(pPlayer.dwId) == self then
					self:KickPlayer(pPlayer);
				else
					Setting:SetGlobalObj(pPlayer);
					self:OnLeave();
					Setting:RestoreGlobalObj();
					self:QuitMap(pPlayer.dwId);
				end]]
			else
				nValidPlayerNum = nValidPlayerNum + 1;
			end
		end
	end
	if nValidPlayerNum == 0 then
		return 0;
	else
		return 1;
	end
end

function BaseGame:FilterTeam(tbTeam)
	local tbRes = {};
	for _, nTeamId in ipairs(tbTeam) do
		if self:CheckTeamValidity(nTeamId) == 1 then
			table.insert(tbRes, nTeamId);
		end
	end

	return tbRes;
end
--
function BaseGame:RandomRoom(tbTeam, nRoomLevel)
	tbTeam = self:FilterTeam(tbTeam);
	local tbNewRooms = {};

	local RoomIds = XoyoGame.RoomSetting.RoomList[nRoomLevel];
	assert(RoomIds ~= nil and #RoomIds > 1);
	local nRoomId = nil;

	local RoomRandIds = {};

	-- 下次随机不会随到相同的地图
	for _, nId in ipairs(RoomIds) do
		local nMapIdx = XoyoGame.RoomSetting.tbRoom[nId].nMapIndex;
		if self.nMapId ~= self.tbMap[nMapIdx] then
			table.insert(RoomRandIds, nId);
		end
	end

	-- 需要排除登天塔地图
	local tbTempIds = {};
	for _, nId in pairs(RoomRandIds) do
		local  tbRoomInfo = XoyoGame.RoomSetting.tbRoom[nId];
		if (tbRoomInfo.nType == nil or tbRoomInfo.nType == 0) then
			table.insert(tbTempIds, nId);
		end
	end
	RoomRandIds = tbTempIds;

	for _, nTeamId in pairs(tbTeam) do
		nRoomId = RoomRandIds[KUnify.MathRandom(1, #RoomRandIds)];
		local nDifficulty = XoyoGame:GetDifficuty(nTeamId);
		local nRet = self:AddRoom(nRoomId, nDifficulty, nTeamId);
		if nRet == 1 then
			table.insert(tbNewRooms, nRoomId);
		end
	end

	-- 结束循环
	self.tbNewRooms = tbNewRooms;
	for _, nRoomId in ipairs(tbNewRooms) do
		if self.tbRoom[nRoomId] then
			local nTimerId = Timer:Register(XoyoGame.ROOM_TIME[nRoomLevel] * Env.GAME_FPS, self.EndRoomTime, self, nRoomId);
			self.tbRoom[nRoomId].nTimerId = nTimerId;
		end
	end
end

function BaseGame:EndRoomTeamProcess(tbTeams, tbNextLevel, nRoomId, nIsWinner)
	for _, nTeamId in pairs(tbTeams) do
		if self.tbTeam[nTeamId] then
			self.tbTeam[nTeamId].nCurRoomCount = self.tbTeam[nTeamId].nCurRoomCount + 1;
			self.nCurRoomCount = self.tbTeam[nTeamId].nCurRoomCount;
			local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
			if self.tbTeam[nTeamId].nCurRoomCount >= XoyoGame.PLAY_ROOM_COUNT or nCount == 0 then
				self:KickTeam(nTeamId, 1);
			else
				table.insert(tbNextLevel, nTeamId);
			end

			if nIsWinner == 1 then
				Dbg:WriteLog("XoyoGame", nTeamId, nRoomId .."号房间成功");
			else
				Dbg:WriteLog("XoyoGame", nTeamId, nRoomId .."号房间失败");
			end

			--失败LOG统计
			--[[
			if XoyoGame.LOG_ATTEND_OPEN == 1  and nIsWinner ~= 1 then
				if not tbMember then
					return;
				end
				for i = 1, #tbMember do
					local pPlayer = KGameBase.GetPlayerById(tbMember[i]);
					if pPlayer then
						DataLog:WriteELog(pPlayer.szName, 1, 3, nRoomId, nTeamId, 0);
					end
				end
			end
			--]]
		end
	end
end

-- 登天塔房间时间到
function BaseGame:EndRoomDTTTime(nRoomId, bForceOut)
	if (nRoomId == nil or nRoomId == 0) then
		return 0;
	end

	if (self.tbRoom[nRoomId] == nil) then
		print("BaseGame:EndRoomDTTTime 取不到登天塔roomid");
		return;
	end

	if (self.tbRoom[nRoomId].tbTeam[1] == nil) then
		print("BaseGame:EndRoomDTTTime 取不到登天塔Team");
		return;
	end

	local bWin = self.tbRoom[nRoomId].tbTeam[1].bIsWiner;
	local nCloseLock = self.tbRoom[nRoomId].tbTeam[1].nCloseLock;
	local nLevel = self.tbRoom[nRoomId].tbSetting.nRoomLevel + 1;
	local nCurLevel = self.tbRoom[nRoomId].tbSetting.nRoomLevel;
	local nWaveNum = self.tbRoom[nRoomId].tbTeam[1].nWaveNum or 1;
	local nTeamId = self.nTeamId;
	local bOver = 0;
	local bForceOut = bForceOut or 0;

	if (bForceOut == 1) then
		bWin = 0;
	end

	-- 关闭房间
	if (self.tbRoom[nRoomId]) then
		if (self.tbRoom[nRoomId].nTimerId) then
			Timer:Close(self.tbRoom[nRoomId].nTimerId);
			self.tbRoom[nRoomId].nTimerId = nil;
		end
		self.tbRoom[nRoomId]:Close(nCloseLock);
		self.tbRoom[nRoomId] = nil;
		self.nCurDTTRoomId = nil;
	end

	if (bWin == 1) then
		local tbRoomIds = XoyoGame.RoomSetting.RoomList[nLevel];

		local tbRoomInfo = nil;
		local nRoomId = nil;
		if (tbRoomIds ~= nil) then
			for _, id in pairs(tbRoomIds) do
				local info = XoyoGame.RoomSetting.tbRoom[id];
				if (info.nType ~= nil and info.nType == 1) then
					tbRoomInfo = info;
					nRoomId = id;
				end
			end
		end

		if (tbRoomInfo ~= nil and nRoomId ~= nil) then
			local nMapId = self.tbMap[XoyoGame.RoomSetting.tbRoom[nRoomId].nMapIndex];
			KScene.LoadScene(nMapId, nTeamId);
			self:AddRoom(nRoomId, 1, nTeamId);
			local nTimerId = Timer:Register(XoyoGame.ROOM_DTT_TIME * Env.GAME_FPS, self.EndRoomDTTTime, self, nRoomId);
			self.tbRoom[nRoomId].nTimerId = nTimerId;
			self.nCurDTTRoomId = nRoomId;
		else
			bOver = 1;
		end
		self.nTimerId = Timer:Register(XoyoGame.START_GAME_TIME * Env.GAME_FPS, self.CloseTimer, self);
	else
		bOver = 1;
	end

	if (bOver == 1) then
		-- 关卡结束
		local nTimeUsed = KGameBase.GetTime() - XoyoGame.tbStartTime[nTeamId];
		local szUsed = os.date(Lang.mission.str808[Lang.Idx], nTimeUsed);
		local szDesc = "";
		if (bWin == 1) then
			szDesc = string.format(Lang.mission.str809[Lang.Idx], szUsed);
		else
			szDesc = string.format(Lang.mission.str810[Lang.Idx], szUsed);
		end

		self:CloseGame();

		local tbTeamer, nCount = KTeam.GetTeamMemberList(nTeamId);
		for i = 1, #tbTeamer do
			local pPlayer = KGameBase.GetPlayerById(tbTeamer[i]);
			if (pPlayer) then
				pPlayer.SysMsg(szDesc);
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:BroadcastMissionComplete", "UI_TASKTIPS", "Begin", szDesc});
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetDTTGameState", 0});
				Timer:Register(1 * Env.GAME_FPS, self.QuitMap, self, pPlayer.dwId);
				pPlayer.LeaveTeam();
			end
		end

		-- 出登天塔要结算奖励
		XoyoGame:FinishDTTAward(nTeamId, nCurLevel, nWaveNum);
	end

	return 0;
end

-- 房间时间到
function BaseGame:EndRoomTime(nRoomId)
	local tbRoomsId = { nRoomId };
	local nLevel = self.tbRoom[nRoomId].tbSetting.nRoomLevel;
	local tbUpgrateTeam = {};
	local tbStayTeam 	= {};
	local bWin = self.tbRoom[nRoomId].tbTeam[1].bIsWiner;
	local nCloseLock = self.tbRoom[nRoomId].tbTeam[1].nCloseLock;
	local nTeamId = self.tbRoom[nRoomId].tbTeam[1].nTeamId;
	for i = 1, #tbRoomsId do
		if self.tbRoom[tbRoomsId[i]] then
			local tbWinner, tbLoser = self.tbRoom[tbRoomsId[i]]:CheckWinner();
			self:EndRoomTeamProcess(tbWinner, tbUpgrateTeam, tbRoomsId[i], 1); -- 队伍晋级
			self:EndRoomTeamProcess(tbLoser, tbStayTeam, tbRoomsId[i], 0); -- 留下来
			self.tbRoom[tbRoomsId[i]]:Close(nCloseLock);
			self.tbRoom[tbRoomsId[i]] = nil;
		end
	end
	if #tbUpgrateTeam > 0 and nLevel < XoyoGame.ROOM_MAX_LEVEL then
		--KStatLog.ModifyAdd("xoyogame", string.format("本日到达%s级房间的队伍", nLevel + 1), "总量", #tbUpgrateTeam); --zzx
		self:RandomRoom(tbUpgrateTeam, nLevel + 1);		-- 队伍房间升级
	end
	if nLevel == XoyoGame.ROOM_MAX_LEVEL and bWin == 1 then
		XoyoGame:CalcTotalTime(
			XoyoGame:GetDifficuty(nTeamId),
			XoyoGame.tbStartTime[nTeamId],
			nTeamId
			);
	end
	if #tbStayTeam > 0 then
		self:RandomRoom(tbStayTeam, nLevel)
	end
	
	if self.nCurRoomCount >= XoyoGame.PLAY_ROOM_COUNT then
		if bWin == 1 then
			local nDifficulty = XoyoGame:GetDifficuty(nTeamId);
			XoyoGame:FinishAward(nDifficulty, nTeamId);	-- 通关奖励
		end
		self:CloseGame();
	end
	
	return 0;
end

-- 剔除队伍
function BaseGame:KickTeam(nTeamId, bAward, szMsg)
	szMsg = szMsg or "";
	if not self.tbTeam[nTeamId] then
		return 0;
	end
	if self.tbTeam[nTeamId].nRoomId then
		local nRoomId = self.tbTeam[nTeamId].nRoomId
		if self.tbRoom[nRoomId] then
			self.tbRoom[nRoomId]:DelTeamInfo(nTeamId);
		end
	end

	local tbTeamer, nCount = KTeam.GetTeamMemberList(nTeamId);
	for i=1, nCount do
		local pPlayer = KGameBase.GetPlayerById(tbTeamer[i]);
		if pPlayer then
			if bAward and bAward == 1 then
			-- 给单人奖励
			end
			self:QuitMap(pPlayer.dwId);
			self:KickPlayer(pPlayer, szMsg);
			pPlayer.LeaveTeam();
		end
	end
	self.tbTeam[nTeamId] = nil;		-- 删除队伍信息
end

function BaseGame:OnLeave(nGroupId, szReason)
	local nPlayerId = me.dwId;
	local nTeamId = me.GetTeamId();
	if self.tbPlayer[nPlayerId] then	-- 玩家可能仍在某个房间的逻辑内
		local nRoomId = self.tbPlayer[nPlayerId];
		self.tbPlayer[nPlayerId] = nil;
		if self.tbRoom[nRoomId] then
			self.tbRoom[nRoomId]:PlayerLeaveRoom(nPlayerId);
		end
	end
	--[[me.TeamDisable(0); --zzx
	me.ForbidExercise(0);
	me.ForbidEnmity(0);]]
	--me.LeaveTeam(); --应是需要的(zzx)
	me.GetTempTable("XoyoGame").tbGame = nil;
end

-- 关房间
function BaseGame:CloseRoom(nRoomId)
	if self.tbRoom and self.tbRoom[nRoomId] then
		if (self.tbRoom[nRoomId].nTimerId ~= nil) then
			Timer:Close(self.tbRoom[nRoomId].nTimerId);
			self.tbRoom[nRoomId].nTimerId = nil;
		end
		self.tbRoom[nRoomId]:Close();
		self.tbRoom[nRoomId] = nil;
	end
end

function BaseGame:CloseGame()
    XoyoGame:ClearGameInfo(self.nTeamId);
	
	for _, nId in ipairs(self.tbPlayers) do
		XoyoGame.tbPlayerGames[nId] = nil;
	end
	
	if self.tbRoom then
		for nId, _ in pairs(self.tbRoom) do
			self:CloseRoom(nId);
		end
	end

	if self.nEndRoomTimerId then
		Timer:Close(self.nEndRoomTimerId);
		self.nEndRoomTimerId = nil;
	end
	if self:IsOpen() == 1 then
		self:Close();
	end
	
end

function BaseGame:OnCharacterEnterScene(pPlayer)
	for nRoomId, pRoom in pairs(self.tbRoom) do
		if KScene.GetMapOwnerId(pPlayer.dwSceneId) == pPlayer.dwId then
			pRoom:OnCharacterEnterScene(pPlayer);
		end
	end
end

-- 增加一个房间
function BaseGame:AddRoom(nRoomId, nDifficuty, ...)
	self.nCurRoomId = nRoomId;
	if self.tbRoom[nRoomId] then		-- 房间已经被占用;
		print("The Room is occupy", nRoomId, ...)
		return 0;
	end
	if not XoyoGame.RoomSetting.tbRoom[nRoomId] then	-- 没有这个配置的房间
		return 0;
	end
	local tbRoomSetting = XoyoGame.RoomSetting.tbRoom[nRoomId];
	local tbRoomExp = XoyoGame.tbRoomExp[nRoomId];

	if not XoyoGame.RoomSetting.tbRoom[nRoomId].DerivedRoom then
		self.tbRoom[nRoomId] = Lib:NewClass(XoyoGame.BaseRoom);
	else
		self.tbRoom[nRoomId] = Lib:NewClass(XoyoGame.RoomSetting.tbRoom[nRoomId].DerivedRoom);
	end

	local nOwnerId = KTeam.GetTeamLeader(self.nTeamId);
	local nMapId = self.tbMap[tbRoomSetting.nMapIndex];
	self.nMapId = nMapId;

	self.tbRoom[nRoomId]:InitRoom(self, tbRoomSetting, nMapId, nRoomId, tbRoomExp);
	self.tbRoom[nRoomId].nDifficuty = nDifficuty;
	self.tbRoom[nRoomId]:JoinRoom(...);
	--self.tbRoom[nRoomId]:Start();  --改成动态场景加载完成后执行
	return 1;
end

-- 统计房间的占用
function BaseGame:CalcRoomOccupy()
	local szLog = ""
	for nRoomId, _ in pairs(self.tbRoom) do
		szLog = nRoomId..Lang.mission.str811[Lang.Idx];
	end
	Dbg:WriteLog("XoyoGame", szLog)
end

function BaseGame:SetPlayerInRoom(nPlayerId, nRoomId)
	self.tbPlayer[nPlayerId] = nRoomId;
end

function BaseGame:GetPlayerRoom(nPlayerId)
	if not self.tbPlayer or not self.tbRoom then
		return nil;
	end
	local  nRoomId = self.tbPlayer[nPlayerId];
	if nRoomId then
		return self.tbRoom[nRoomId];
	end
	return nil;
end

function BaseGame:__TeamJoinNextGame(nTeamId)
	local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
	for i = 1, nCount do
		local pPlayer = KGameBase.GetPlayerById(tbMember[i])
		if pPlayer then
			self:KickPlayer(pPlayer);
			self:JoinNextGame(pPlayer);
		end
	end
end

-- 测试房间用,实际功能不需要
function BaseGame:TestRoom(nMap, tbMap, nRoomId, nDifficuty)
	if me.nTeamId == 0 then
		return 0;
	end
	self:CloseRoom(nRoomId);
	self:InitGame(tbMap, nMap);
	self:__TeamJoinNextGame(me.nTeamId);
	self.tbTeam[me.nTeamId] = {};
	self.tbTeam[me.nTeamId].nCurRoomCount = XoyoGame.PLAY_ROOM_COUNT - 1;
	self:AddRoom(nRoomId, nDifficuty, me.nTeamId);
	local nRoomLevel = XoyoGame.RoomSetting.tbRoom[nRoomId].nRoomLevel;
	self.nEndRoomTimerId = Timer:Register(XoyoGame.ROOM_TIME[nRoomLevel] * Env.GAME_FPS, self.EndRoomTime, self, nRoomId);
	self.tbRoom[nRoomId].nTimerId = self.nEndRoomTimerId;
	XoyoGame.tbStartTime[me.nTeamId] = GetTime();
end

--
function BaseGame:TestGame(nMap, tbMap)
	if me.nTeamId == 0 then
		return 0;
	end
	self:InitGame(tbMap, nMap);
	self:JoinNextGame(me);
	self:StartNewGame();
end

function BaseGame:OnLogin(pPlayer)
	for nRoomId, pRoom in pairs(self.tbRoom) do
		if KScene.GetMapOwnerId(pPlayer.dwSceneId) == pPlayer.dwId then
			pRoom:OnLogin(pPlayer);
		end
	end

	local nDTTRoomId = self.nCurDTTRoomId;
	if (nDTTRoomId and self.tbRoom[nDTTRoomId] 
		and self.tbRoom[nDTTRoomId].tbSetting.nType
		and self.tbRoom[nDTTRoomId].tbSetting.nType == 1) then
		self:EndRoomDTTTime(nDTTRoomId);
	end
end

function BaseGame:GetPlayersNum()
	return #self.tbPlayers;
end

function BaseGame:OnLogout(pPlayer)
	local nDTTRoomId = self.nCurDTTRoomId;
	if (nDTTRoomId and self.tbRoom[nDTTRoomId] 
		and self.tbRoom[nDTTRoomId].tbSetting.nType
		and self.tbRoom[nDTTRoomId].tbSetting.nType == 1) then
		self:EndRoomDTTTime(nDTTRoomId);
	else
		pPlayer.LeaveTeam();
		self:KickPlayer(pPlayer);
		
		if self:GetPlayersNum() <= 0 then
			self:CloseGame();
		end
		
		XoyoGame.tbPlayerGames[pPlayer.dwId] = nil;
	end
end

--[[
function BaseGame:TestPKRoom(nMap, tbMap, nRoomId, nRoomLevel, nTeamId2)
	local nTeamId1 = me.nTeamId
	if nTeamId1 == 0 or not nTeamId2 then
		return 0;
	end

	self:CloseRoom(nRoomId);
	self:InitGame(tbMap, nMap);
	self:__TeamJoinNextGame(nTeamId1);
	self:__TeamJoinNextGame(nTeamId2);

	self.tbTeam[nTeamId1] = {};
	self.tbTeam[nTeamId1].nCurRoomCount = XoyoGame.PLAY_ROOM_COUNT - 1;
	self.tbTeam[nTeamId2] = {};
	self.tbTeam[nTeamId2].nCurRoomCount = XoyoGame.PLAY_ROOM_COUNT - 1;
	self:AddRoom(nRoomId, nTeamId1, nTeamId2);
	self.nEndRoomTimerId  = Timer:Register(XoyoGame.ROOM_TIME[nRoomLevel] * Env.GAME_FPS, self.EndRoomTime, self, {nRoomId}, nRoomLevel);
	self.tbRoom[nRoomId].nTimerId = self.nEndRoomTimerId;
end
--]]

function BaseGame:TestEnterGame(nRoomId)
	local tbTemp = {};
	local nTeamId = self.nTeamId;

	self.tbAddXoyoTimesPlayerId = {};
	self:CalcRoomOccupy();

	XoyoGame.tbStartTime[nTeamId] = KGameBase.GetTime();
	table.insert(tbTemp, nTeamId);
	self.tbTeam[nTeamId] = {};
	self.tbTeam[nTeamId].nCurRoomCount = 0; -- 玩过几个房间

	self:AddXoyoTimes(tbTemp);
	
	local nDifficulty = XoyoGame:GetDifficuty(nTeamId);
	self:AddRoom(nRoomId, nDifficulty, nTeamId);

	if self.tbRoom[nRoomId] then
		local nRoomLevel = self.tbRoom[nRoomId].tbSetting.nRoomLevel;
		local nTimerId = Timer:Register(XoyoGame.ROOM_TIME[nRoomLevel] * Env.GAME_FPS, self.EndRoomTime, self, nRoomId);
		self.tbRoom[nRoomId].nTimerId = nTimerId;
	end
	
	self.nNextTeamCount = 0;
	
	self.nTimerId = Timer:Register(XoyoGame.START_GAME_TIME * Env.GAME_FPS, self.CloseTimer, self);
end