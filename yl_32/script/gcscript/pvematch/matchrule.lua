-- File       : matchrule.lua
-- Creator    : liyun
-- Date       : 2012-10-22 09:12:01
-- Description: 副本排队规则逻辑实现

-- data structure
-- 副本等待队列，组队是一个单独的队列，使用副本索引作key
-----------------------------------------------------------------
if not (MODULE_GAMECENTER) then
	return
end

-- local tbInstanceWaitQueueSample = {
--	[1] = {
--		"Tank" = {{p = player1, time = KGameBase:GetTime(), Accept = 0}, item, item}, 
--		"Help" = {p1, p2}, 
--		"Dps" = {p3, p5}, 
--		"Team" = {team, team}
--	},
--	[2] = {"Tank" = {player1, player2, player3}, "Help" = {p1, p2}, "Dps" = {p3, p5}, "Team" = {team, team}},
--	[5] = {"Tank" = {player1, player2, player3}, "Help" = {p1, p2}, "Dps" = {p3, p5}, "Team" = {team, team}},
--	[10] = {"Tank" = {player1, player2, player3}, "Help" = {p1, p2}, "Dps" = {p3, p5}, "Team" = {team, team}},
--}


-- 副本已经选择的玩家，最多只能选择一个组队
-- local tbSelecteSample = {
--	[1] = {
--		"Tank" = {player, player}, 
--		"help" = {player, plyaer}, 
--		"Dps" = {player, player}, 
--		"Team" = {p1, p2}
--	},
--	[3] = {"Tank" = {player, player}, "help" = {player, plyaer}, "Dps" = {player, player}, "Team" = {p1, p2}},
--	[7] = {"Tank" = {player, player}, "help" = {player, plyaer}, "Dps" = {player, player}, "Team" = {p1, p2}},
--	[10] = {"Tank" = {player, player}, "help" = {player, plyaer}, "Dps" = {player, player}, "Team" = {p1, p2}},
--	[35] = {"Tank" = {player, player}, "help" = {player, plyaer}, "Dps" = {player, player}, "Team" = {p1, p2}},
--}


PVEMatch.MAX_TEAM_NUM = 3;
PVEMatch.MATCH_INTERVAL = 100; -- 匹配时间间隔，单位为Timer的帧数
PVEMatch.ENTER_TIME_OUT = 30; -- 匹配成功后选择进入拒绝副本关卡的限制时间，以秒为单位

-- 副本等待队列
PVEMatch.tbInstanceWaitQueue = {};

-- 已经匹配上选择的玩家
PVEMatch.tbSelected = {};

-- 跟踪已经进入队列的玩家，防止重复进入使用玩家ID作索引
PVEMatch.tbEnteredPlayers = {};

-- 记录已经匹配完成的副本索引
PVEMatch.tbMatchFinishedInstanceIndex = {};

-- 记录定时器ID
PVEMatch.tbMatchFinishedTimerID = {};

PVEMatch.tbVeryfyTeamMember = {};

-------------------------------------------------------------------------------
-- 玩家加入副本等待列表，tbPlayer是自定义的数据结构，与客户端传入的pPlayer不同
--[[tbPlayer_Sample = {
	dwId = playid,  -- 玩家Id
	szType = "Dps",  -- 队伍职责类型，有Dps,Help,Tank
	nFaction = 1,  -- 玩家门派，枚举值是1，2，3，4
	nRoute = 1001, -- 这里是用faction*1000 + route，整合了门派和天赋路线二个值方便处理，天赋枚举值：0,1,2,3
	bAccept = 0,  -- 玩家是否同意进入副本，与客户端五图标玩家状态对话框界面对应
	time = KGameBase.GetTime(),  -- 玩家进入排队的时间，组队玩家没有这个值，只有队伍进入的时间
}
]]

function PVEMatch:RequestEnterStage_GC(tbPlayerIds, nDifficulty)
	GlobalExecute({"Stage:DoEnterGame", tbPlayerIds, nDifficulty});
end

function PVEMatch:RequestEnterMultiInstance_GC(tbPlayerIds, nMapId)
	GlobalExecute({"MultiInstance:DoEnterGame", tbPlayerIds, nMapId});
end

-- 单人加入
function PVEMatch:SingleRequestEnterInstance_GC(tbPlayer, tbInstanceIndex)
	if PVEMatch:IsPlayerEntered(tbPlayer) == 1 then
		print("PVEMatch:SingleRequestEnterInstance_GC: player re-entered is not allowed, pPlayer.dwId="..tbPlayer.dwId);
		return; -- 不重复进入队列
	end
	
	local tbQueue = self.tbInstanceWaitQueue;
	if type(tbInstanceIndex) ~= "table" or tbInstanceIndex[1] == nil then
		print("PVEMatch:SingleRequestEnterInstance_GC: parm tbInstanceIndex must be non-empty table");
		return;
	end
	
	local szType = tbPlayer.szType;
	local bNotified = 0;
	for _, nInstanceIndex in pairs(tbInstanceIndex) do
		if type(tbQueue[nInstanceIndex]) ~= "table" then
			tbQueue[nInstanceIndex] = {};
		end
		if type(tbQueue[nInstanceIndex][szType]) ~= "table" then
			tbQueue[nInstanceIndex][szType] = {};
		end
		
		print("PVEMatch:SingleRequestEnterInstance_GC: player "..tbPlayer.dwId.." entered queue. InstanceIndex is "..nInstanceIndex);
		table.insert(tbQueue[nInstanceIndex][szType], tbPlayer);
		if (bNotified == 0) then
			bNotified = 1;
			GlobalExecute({"PVEMatch:NotifyPlayerEnterQueue_GS", tbPlayer, nInstanceIndex, 1});
		else
			GlobalExecute({"PVEMatch:NotifyPlayerEnterQueue_GS", tbPlayer, nInstanceIndex, 0});
		end
	end 
	
	self:RecordEnteredPlayer(tbPlayer);
end

PVEMatch.ROOM_XOYO_EASY = "XoyoGameEasy";
PVEMatch.ROOM_XOYO_NORMAL = "XoyoGameNormal";
PVEMatch.ROOM_XOYO_HARD = "XoyoGameHard";
PVEMatch.ROOM_XOYO_VERY_HARD = "XoyoGameVeryHard";

-- 组队加入
-- tbTeam = {tbPlayer, tbPlayer, tbPlayer, ...}
function PVEMatch:TeamRequestEnterInstance_GC(tbTeam, tbInstanceIndex)
	--[[
	if #tbTeam == 1 then
		print("PVEMatch:TeamRequestEnterInstance_GC: team must have 2 or more players");
		return;
	end
	--]]
	
	if self:IsTeamEntered(tbTeam) == 1 then
		print("PVEMatch:TeamRequestEnterInstance_GC: team re-entered is not allowed");
		return;
	end
	
	if type(tbInstanceIndex) ~= "table" or tbInstanceIndex[1] == nil then
		print("PVEMatch:TeamRequestEnterInstance_GC: parm tbInstanceIndex must be non empty table");
		return; 
	end
	
	-- 组队直接进，不排队了
	if tbInstanceIndex[1] == self.ROOM_XOYO_EASY or
		tbInstanceIndex[1] == self.ROOM_XOYO_NORMAL or
		tbInstanceIndex[1] == self.ROOM_XOYO_HARD or
		tbInstanceIndex[1] == self.ROOM_XOYO_VERY_HARD then
		GlobalExecute({"PVEMatch:DelayEnterStageRequest_GS", tbTeam});
	else
	    GlobalExecute({"PVEMatch:DelayEnterMultiInstanceRequest_GS", tbTeam, tbInstanceIndex[1]});
	end
	
	--[[
	-- 把组队加入到副本的等待队列中
	local tbQueue = self.tbInstanceWaitQueue;
	local bNotified = 0;
	for _, nInstanceIndex in pairs(tbInstanceIndex) do
		if type(tbQueue[nInstanceIndex]) ~= "table" then
			tbQueue[nInstanceIndex] = {};
		end
		if type(tbQueue[nInstanceIndex]["Team"]) ~= "table" then
			tbQueue[nInstanceIndex]["Team"] = {};
		end
		
		print("PVEMatch:TeamRequestEnterInstance_GC: entered queue. InstanceIndex is "..nInstanceIndex);
		table.insert(tbQueue[nInstanceIndex]["Team"], tbTeam);
		if (bNotified == 0) then
			bNotified = 1;
			GlobalExecute({"PVEMatch:NotifyTeamEnterQueue_GS", tbTeam, nInstanceIndex, 1});
		else
			GlobalExecute({"PVEMatch:NotifyTeamEnterQueue_GS", tbTeam, nInstanceIndex, 0});
		end
	end
	
	self:RecordEnteredTeam(tbTeam);
	--]]
end

function PVEMatch:VeryfyTeamMember_GC(tbTeamMem, nCaptainId, nInstanceIndex)
	for _, nPlayerId in pairs(tbTeamMem) do
		local bOnline = IsPlayerOnLine(nPlayerId);
		if (bOnline == 0) then
			GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, Lang.gcscript.str22[Lang.Idx]});
			return;
		end
	end

	self.tbVeryfyTeamMember[nCaptainId] = {};

	local tbVeryfy = self.tbVeryfyTeamMember[nCaptainId];
	tbVeryfy["InstanceIndex"] = nInstanceIndex;
	tbVeryfy["Team"] = {};
	tbVeryfy["TeamLevel"] = {};
	tbVeryfy["NoDegree"] = {};
	tbVeryfy["InMapCopy"] = {};
	tbVeryfy["StageMoping"] = {};
	tbVeryfy["VeryfyInfo"] = {};

	for _, nPlayerId in pairs(tbTeamMem) do
		tbVeryfy["VeryfyInfo"][nPlayerId] = {
			["Veryfied"] = 0,
			["Name"] = "",
		};
	end

	GlobalExecute({"PVEMatch:VeryfyTeamMember_GS", tbTeamMem, nCaptainId, nInstanceIndex});
end

function PVEMatch:VeryfyTeamMemberResult_GC(nCaptainId, tbVeryfied, tbNoDegree, tbInMapCopy, tbStageMoping, tbTeam, tbTeamLevel)
	if (self.tbVeryfyTeamMember[nCaptainId] == nil) then
		print("PVEMatch:VeryfyTeamMemberResult_GC " .. "nCaptainId nil ");
		return;
	end

	local tbVeryfy = self.tbVeryfyTeamMember[nCaptainId];
	if (tbVeryfy == nil) then
		print("PVEMatch:VeryfyTeamMemberResult_GC nCaptainId error");
		return;
	end

	for nPlayerId, szName in pairs(tbVeryfied) do
		if tbVeryfy["VeryfyInfo"][nPlayerId] == nil then
			print("PVEMatch:VeryfyTeamMemberResult_GC member id error");
			return;
		end

		tbVeryfy["VeryfyInfo"][nPlayerId]["Veryfied"] = 1;
		tbVeryfy["VeryfyInfo"][nPlayerId]["Name"] = szName;
	end

	for _, nPlayerId in pairs(tbNoDegree) do
		table.insert(tbVeryfy["NoDegree"], nPlayerId);
	end

	for _, nPlayerId in pairs(tbInMapCopy) do
		table.insert(tbVeryfy["InMapCopy"], nPlayerId);
	end
	
	for _, nPlayerId in pairs(tbStageMoping) do
		table.insert(tbVeryfy["StageMoping"], nPlayerId);
	end

	for _, tbTeamInfo in pairs(tbTeam) do
		table.insert(tbVeryfy["Team"], tbTeamInfo);
	end

	for nPlayerId, nMemLevel in pairs(tbTeamLevel) do
		tbVeryfy["TeamLevel"][nPlayerId] = nMemLevel;
	end

	local bGetAllResult = 1;
	for _, tbVeryfyInfo in pairs(tbVeryfy["VeryfyInfo"]) do
		if (tbVeryfyInfo["Veryfied"] == 0) then
			bGetAllResult = 0;
		end
	end

	if (bGetAllResult == 1) then
		if (#tbVeryfy["NoDegree"] > 0) then
			local nPlayerId = tbVeryfy["NoDegree"][1];
			--local szName = tbVeryfy[nPlayerId]["Name"];
			local szName = tbVeryfy["VeryfyInfo"][nPlayerId]["Name"];
			if (nPlayerId == nCaptainId) then
				if (tbVeryfy["InstanceIndex"] >= 0) then
					GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, Lang.gcscript.str23[Lang.Idx]});
				else
					GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, Lang.gcscript.str24[Lang.Idx]});
				end
			else
				if (tbVeryfy["InstanceIndex"] >= 0) then 
					GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, string.format(Lang.gcscript.str25[Lang.Idx],szName)})
				else
					GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, string.format(Lang.gcscript.str26[Lang.Idx],szName)})
				end
			end
			return;
		end

		if (#tbVeryfy["InMapCopy"] > 0) then
			local nPlayerId = tbVeryfy["InMapCopy"][1];
			local szName = tbVeryfy["VeryfyInfo"][nPlayerId]["Name"];
			if (nCaptainId == nPlayerId) then
				GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, Lang.gcscript.str27[Lang.Idx]});
			else
				GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId,  string.format(Lang.gcscript.str28[Lang.Idx],szName)});
			end
			return;
		end
		
		if (#tbVeryfy["StageMoping"] > 0) then
			local nPlayerId = tbVeryfy["StageMoping"][1];
			local szName = tbVeryfy["VeryfyInfo"][nPlayerId]["Name"];
			if (nCaptainId == nPlayerId) then
				GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, Lang.gcscript.str29[Lang.Idx]});
			else
				GlobalExecute({"PVEMatch:NotifyTeamInfo_GC", nCaptainId, string.format(Lang.gcscript.str30[Lang.Idx],szName) });
			end
			return;
		end

		GlobalExecute({"PVEMatch:VeryfyTeamLevel_GS", nCaptainId, tbVeryfy["InstanceIndex"], tbVeryfy["TeamLevel"], tbVeryfy["Team"], tbVeryfy["VeryfyInfo"]});
	end
end

-- 同意进入副本
function PVEMatch:AcceptEnterInstance_GC(nPlayerId)
	assert(nPlayerId > 0);

	local nInstanceIndex = self:GetInstanceIndexByPlayerIdFromSelected(nPlayerId);
	if (nInstanceIndex == nil) then
		assert(0);
		return;
	end

	-- 设置同意状态
	local tbSelected = self.tbSelected[nInstanceIndex];
	local bFind = 0;
	local tbAcceptPlayer = nil;

	for _, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			if tbPlayer.dwId == nPlayerId then
				-- 重复接受
				if tbPlayer.nAccept == 1 then
					return;
				end
				
				tbPlayer.nAccept = 1;
				bFind = 1;
				tbAcceptPlayer = tbPlayer;
				break;
			end
		end
		if bFind == 1 then
			break;
		end
	end
	
	-- 没找到这个玩家，则不处理
	if bFind == 0 or tbAcceptPlayer == nil then
		assert(0);
		return;
	end
	
	-- 广播接受信息
	local tbSelPlayer = {};
	for _, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			table.insert(tbSelPlayer, tbPlayer);
		end
	end

	if #tbSelPlayer > 0 then
		GlobalExecute({"PVEMatch:NotifyAcceptEnterInstance_GS", tbSelPlayer, tbAcceptPlayer});
	end
	
	-- 所有玩家全部同意进入副本
	if self:IsAllAcceptedEnterInstance(nInstanceIndex) == 1 then
		local tbSelected = self.tbSelected[nInstanceIndex];
		self.tbSelected[nInstanceIndex] = nil;
		local tbPlayers = {};

		for _, tbP in pairs(tbSelected) do
			for _, tbPlayer in pairs(tbP) do
				self:ClearEnteredPlayer(tbPlayer);
				table.insert(tbPlayers, tbPlayer);
			end
		end

		-- 清除定时器
		local nTimerId = self.tbMatchFinishedTimerID[nInstanceIndex];
		self.tbMatchFinishedTimerID[nInstanceIndex] = 0;
		if (nTimerId and nTimerId > 0) then
			Timer:Close(nTimerId);
		end

		-- 五人直接进入副本
		self.tbMatchFinishedInstanceIndex[nInstanceIndex] = 0;
		GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbPlayers, nInstanceIndex, 0});
		self:EnterInstance(tbPlayers, nInstanceIndex);
	end
end

 -- 玩家被匹配成功后拒绝进入副本，单人玩家则移除选择，组队玩家则需要把整个队伍移除
function PVEMatch:RejectEnterInstance_GC(nPlayerId)
	local nInstanceIndex = self:GetInstanceIndexByPlayerIdFromSelected(nPlayerId);
	if nInstanceIndex == nil then
		assert(0);
		return;
	end
	
	-- 从选择中移除，如果在队伍中，则把整个队伍移除
	local tbNotifyPlayers = {};
	local tbNotifyLeave = {};
	local tbSelected = self.tbSelected[nInstanceIndex];
	local bFind = 0;
	local tbRejectPlayer = nil;

	for szType, tbPlayers in pairs(tbSelected) do
		for k, tbPlayer in pairs(tbPlayers) do
			if tbPlayer.dwId == nPlayerId then
				bFind = 1;
				tbRejectPlayer = tbPlayer;
				if szType == "Team" then
					for _, varP in pairs(tbPlayers) do
						table.insert(tbNotifyPlayers, varP);
						table.insert(tbNotifyLeave, varP);
					end
					self:ClearEnteredTeam(tbPlayers);
					tbSelected["Team"] = nil;
				else
					table.insert(tbNotifyPlayers, tbPlayer);
					table.insert(tbNotifyLeave, tbPlayer);
					self:ClearEnteredPlayer({dwId = nPlayerId});
					table.remove(tbPlayers, k);
				end
				self.tbMatchFinishedInstanceIndex[nInstanceIndex] = 0;
				break;
			end
		end
		if bFind == 1 then 
			break; 
		end;
	end
	
	if bFind == 0 or tbRejectPlayer == nil then
		assert(0);
		return;
	end

	-- 清除定时器
	local nTimerId = self.tbMatchFinishedTimerID[nInstanceIndex];
	self.tbMatchFinishedTimerID[nInstanceIndex] = 0;
	if (nTimerId and nTimerId > 0) then
		Timer:Close(nTimerId);
	end
	
	-- 通知其他被选择的玩家有人拒绝进入副本
	for _, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			tbPlayer.nAccept = 0; -- 清所有玩家同意状态
			table.insert(tbNotifyPlayers, tbPlayer);
		end
	end
	
	if #tbNotifyPlayers > 0 then
		GlobalExecute({"PVEMatch:NotifyRejectEnterInstance_GS", tbNotifyPlayers, tbRejectPlayer});
	end

	if #tbNotifyLeave > 0 then
		GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbNotifyLeave, nInstanceIndex})
	end
end

-- 从等待队列中取个玩家出来
function PVEMatch:GetWaitPlayer()
	local nPlayerId = 0;

	for nInstanceIndex, tbWaitQueue in pairs(self.tbInstanceWaitQueue) do
		for szType, tbPlayers in pairs(tbWaitQueue) do
			if (szType ~= "Team") then
				for _, tbPlayer in pairs(tbPlayers) do
					nPlayerId = tbPlayer.dwId;
					break;
				end
			end
			if (nPlayerId > 0) then
				break;
			end
		end
		if (nPlayerId > 0) then
			break;
		end
	end
	
	if (nPlayerId > 0) then
		self:RemovePlayerFromWaitQueue({dwId = nPlayerId}, 0, 1);
	end
	
	return nPlayerId;
end

-- 离开队列
function PVEMatch:PlayerLeaveWaitQueue(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

-- 玩家离开，可能下线，掉线，等各种情况离开处理
function PVEMatch:PlayerLeave(nPlayerId)
	-- 三种情况：在等待列表中，被匹配上，已经匹配完成
	local nInstanceIndex = self:GetInstanceIndexByPlayerIdFromSelected(nPlayerId);
	if nInstanceIndex == nil then -- 在等待队列中，还没有被匹配上
		self:RemovePlayerFromWaitQueue({dwId = nPlayerId}, 1, 1);
	elseif self:IsMatchFinished(nInstanceIndex) == 0 then -- 被匹配上
		self:RemovePlayerFromSelected({dwId = nPlayerId}, 1, 1);
		self:RemovePlayerFromWaitQueue({dwId = nPlayerId}, 1, 1);
	elseif self:IsAllAcceptedEnterInstance(nInstanceIndex) == 0 then -- 已经匹配完成
		self:RejectEnterInstance_GC(nPlayerId);
	else
		print("PVEMatch:PlayerLeave: unknow condition with player leave, please check");
		self:RemovePlayerFromSelected({dwId = nPlayerId}, 1, 1);
		self:RemovePlayerFromWaitQueue({dwId = nPlayerId}, 1, 1);
		self:RejectEnterInstance_GC(nInstanceIndex);
	end
end

-- 在整个副本等待队列中移除单人玩家，如果是组队，则移除整个队伍
function PVEMatch:RemovePlayerFromWaitQueue(tbPlayer, bNotify, bClearPlayer)
	bNotify = bNotify or 0;
	bClearPlayer = bClearPlayer or 0;

	local bNotified = 0;
	
	for nInstanceIndex, tbWaitQueue in pairs(self.tbInstanceWaitQueue) do
		bNotified = self:RemovePlayerFRomInstanceWaitQueue(nInstanceIndex, tbPlayer, bNotify, bClearPlayer, bNotified);
	end
end

function PVEMatch:RemovePlayerFRomInstanceWaitQueue(nInstanceIndex, tbPlayer, bNotify, bClearPlayer, bNotified)
	local bFind = 0;
	local tbWaitQueue = self.tbInstanceWaitQueue[nInstanceIndex];

	repeat
		bFind = 0;

		for szType, tbPlayers in pairs(tbWaitQueue) do
			if (szType == "Team") then
				for key, tbTeam in pairs(tbPlayers) do
					for _, tbPlayer2 in pairs(tbTeam) do
						if (tbPlayer2.dwId == tbPlayer.dwId) then
							bFind = 1;
							table.remove(tbPlayers, key);
							if (bNotify == 1) then
								if (bNotified == 0) then
									bNotified = 1;
									GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbTeam, nInstanceIndex, 1});
								else
									GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbTeam, nInstanceIndex, 0});
								end
							end
							if (bClearPlayer == 1) then
								self:ClearEnteredTeam(tbTeam);
							end
						end
					end
				end
			else
				for key, tbPlayer2 in pairs(tbPlayers) do
					if (tbPlayer2.dwId == tbPlayer.dwId) then
						bFind = 1;
						table.remove(tbPlayers, key);
						if (bNotify == 1) then
							if (bNotified == 0) then
								bNotified = 1;
								GlobalExecute({"PVEMatch:NotifyPlayerLeaveQueue_GS", tbPlayer2, nInstanceIndex, 1});
							else
								GlobalExecute({"PVEMatch:NotifyPlayerLeaveQueue_GS", tbPlayer2, nInstanceIndex, 0});
							end
						end
						if (bClearPlayer == 1) then
							self:ClearEnteredPlayer({dwId = tbPlayer2.dwId});
						end
					end
				end
			end
		end
	until bFind == 0;

	return bNotified;
end

-- 从匹配中移除玩家，单人则直接移除，组队则移除整个队伍
function PVEMatch:RemovePlayerFromSelected(tbPlayer, bNotify, bClearPlayer, nExcludeIndex)
	bClearPlayer = bClearPlayer or 0;
	bNotify = bNotify or 0;
	nExcludeIndex = nExcludeIndex or 0;
	local bNotified = 0;

	for nInstanceIndex, tbWaitQueue in pairs(self.tbSelected) do
		if nInstanceIndex ~= nExcludeIndex then
			bNotified = self:RemovePlayerFromInstanceSelected(nInstanceIndex, tbPlayer, bNotify, bClearPlayer, bNotified);
		end
	end
end

function PVEMatch:RemovePlayerFromInstanceSelected(nInstanceIndex, tbPlayer, bNotify, bClearPlayer, bNotified)
	local bFind = 0;
	local tbWaitQueue = self.tbSelected[nInstanceIndex];

	repeat
		bFind = 0;
		for szType, tbPlayers in pairs(tbWaitQueue) do
			for key, tbPlayer2 in pairs(tbPlayers) do
				if (tbPlayer2.dwId == tbPlayer.dwId) then
					bFind = 1;
					if (szType == "Team") then
						tbWaitQueue["Team"] = nil;
						if (bClearPlayer == 1) then
							self:ClearEnteredTeam(tbPlayers);
						end
						if (bNotify == 1) then
							if (bNotified == 0) then
								bNotified = 1;
								GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbPlayers, nInstanceIndex, 1});
							else
								GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbPlayers, nInstanceIndex, 0});
							end
						end
					else
						table.remove(tbPlayers, key);
						if (bClearPlayer == 1) then
							self:ClearEnteredPlayer({dwId = tbPlayer2.dwId});
						end
						if (bNotify == 1) then
							if (bNotified == 0) then
								bNotified = 1;
								GlobalExecute({"PVEMatch:NotifyPlayerLeaveQueue_GS", tbPlayer2, nInstanceIndex, 1});
							else
								GlobalExecute({"PVEMatch:NotifyPlayerLeaveQueue_GS", tbPlayer2, nInstanceIndex, 0});
							end
						end
					end
				end
			end
		end
	until bFind == 0

	return bNotified;
end

-- 在等待队列中移除组队
function PVEMatch:RemoveTeamFromWaitQueue(tbTeam)

	for nInstanceIndex, tbWaitQueue in pairs(self.tbInstanceWaitQueue) do

		local bFind = 0;
		repeat
			bFind = 0;
			for key, tbTeam in pairs(tbWaitQueue["Team"]) do
				if (tbTeam == tbTeam) then
					bFind = 1;
					table.remove(tbWaitQueue["Team"], key);
				end
			end
		until bFind == 0;

	end
end

-- 判断匹配的玩家是否同意进入副本，需要全部同意
function PVEMatch:IsAllAcceptedEnterInstance(nInstanceIndex)
	local tbs = self.tbSelected[nInstanceIndex];
	local nNum = 0;
	if type(tbs) == "table" then
		for _, tbPlayers in pairs(tbs) do
			for _, tbPlayer in pairs(tbPlayers) do
				if tbPlayer.nAccept == 1 then
					nNum = nNum + 1;
				end
			end
		end
	end
	
	if nNum >= self.MAX_TEAM_NUM then
		return 1;
	else
		return 0;
	end
end

-- 判断是否匹配完成，只要匹配了5人就可以
function PVEMatch:IsMatchFinished(nInstanceIndex)
	local tbs = self.tbSelected[nInstanceIndex];
	local nSelNum = 0;
	if type(tbs) == "table" then
		for _, tbPlayers in pairs(tbs) do
			for _, tbPlayer in pairs(tbPlayers) do
				nSelNum = nSelNum + 1;
			end
		end
	end
	
	if nSelNum >= self.MAX_TEAM_NUM then 
		return 1;
	else
		return 0;
	end
end

-- 记录玩家是否进入副本匹配队列
function PVEMatch:RecordEnteredPlayer(tbPlayer)
	self.tbEnteredPlayers[tbPlayer.dwId] = 1;
end

-- 清除玩家记录
function PVEMatch:ClearEnteredPlayer(tbPlayer)
	self.tbEnteredPlayers[tbPlayer.dwId] = nil;
end

function PVEMatch:IsPlayerEntered(tbPlayer)
	if self.tbEnteredPlayers[tbPlayer.dwId] == 1 then
		return 1;
	end
	
	return 0;
end

function PVEMatch:IsTeamEntered(tbTeam)
	for _, tbPlayer in pairs(tbTeam) do
		if self.tbEnteredPlayers[tbPlayer.dwId] ~= nil then
			return 1;
		end
	end
	
	return 0;
end

function PVEMatch:ClearEnteredTeam(tbTeam)
	for _, tbPlayer in pairs(tbTeam) do
		self.tbEnteredPlayers[tbPlayer.dwId] = nil;
	end
end

function PVEMatch:RecordEnteredTeam(tbTeam)
	for _, tbPlayer in pairs(tbTeam) do
		self.tbEnteredPlayers[tbPlayer.dwId] = 1;
	end
end

function PVEMatch:GetInstanceIndexByPlayerIdFromSelected(nPlayerId)
	for nInstanceIndex, tbQueue in pairs(self.tbSelected) do
		for _, tbPlayers in pairs(tbQueue) do
			for _, tbPlayer in pairs(tbPlayers) do
				if tbPlayer.dwId == nPlayerId then
					return nInstanceIndex;
				end
			end
		end
	end
end

function PVEMatch:EnterInstance(tbPlayers, nInstanceIndex)
	-- 组成战斗ID
	local tbPlayerIds = {};
	for _, tbPlayer in pairs(tbPlayers) do
		table.insert(tbPlayerIds, tbPlayer.dwId);
	end
	local nSysTeamId = CreateSystemTeam(unpack(tbPlayerIds));
	GlobalExecute({"PVEMatch:DoEnterInstance_GS", tbPlayers, nInstanceIndex});
end

function PVEMatch:TestEnterMission(nMember1, nMember2, nMember3, nInstanceIndex)
	local tbPlayerIds = {};
	
	if nMember1 ~= 0 then
		table.insert(tbPlayerIds, nMember1)
	end
	
	if nMember2 ~= 0 then
		table.insert(tbPlayerIds, nMember2)
	end
	
	if nMember3 ~= 0 then
		table.insert(tbPlayerIds, nMember3)
	end

	local nSysTeamId = CreateSystemTeam(unpack(tbPlayerIds));
	GlobalExecute({"PVEMatch:DoEnterMultiInstance_GS", tbPlayerIds, nInstanceIndex});
end

function PVEMatch:TestEnterMissionRemote(nMember1, nMember2, nMember3, nInstanceIndex)
	GlobalExecute({"PVEMatch:DoEnterMultiInstanceRemote_GS", nMember1, nInstanceIndex});
end

function PVEMatch:BroadcastOverallTeamMsg(nMapId, dwLeaderCombinedIdx, nRequireFightscore, szCreaterName)
	GlobalExecute({"PVEMatch:DoBroadcastOverallTeamMsg_GS", nMapId, dwLeaderCombinedIdx, nRequireFightscore, szCreaterName});
end

-- event
------------------------------------------------------------------
-- 登录事件
function PVEMatch:PlayerLogin_GC(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

-- 退出事件
function PVEMatch:PlayerLogout_GC(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

-- 玩家离开城市地图进入活动地图事件
function PVEMatch:PlayerLeaveCity_GC(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

-- 玩家离开组队，视为整个队伍都离开队列
function PVEMatch:PlayerLeaveTeam_GC(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

-- 玩家进行组队
function PVEMatch:PlayerEnterTeam_GC(nPlayerId)
	self:PlayerLeave(nPlayerId);
end

function PVEMatch:OnMatchFinish(nInstanceIndex)
	if nInstanceIndex ~= nil then
		local tbPlayers = {};
		for _, tbPlayers2 in pairs(self.tbSelected[nInstanceIndex]) do
			for _, tbPlayer in pairs(tbPlayers2) do
				table.insert(tbPlayers, tbPlayer);
			end
		end

		for _, tbPlayer in pairs(tbPlayers) do
			self:RemovePlayerFromSelected(tbPlayer, 0, 0, nInstanceIndex);
			self:RemovePlayerFromWaitQueue(tbPlayer);
		end

		-- 设置一个定时器，限定时间内没有同意进入副本关卡则视为拒绝进入副本关卡
		local ntimerId = Timer:Register(self.ENTER_TIME_OUT * Env.GAME_FPS, self.OnEntertimeOut, self, nInstanceIndex);
		self.tbMatchFinishedTimerID[nInstanceIndex] = ntimerId;
		
		GlobalExecute({"PVEMatch:NotifyMatchFinished_GS", tbPlayers, nInstanceIndex, self.ENTER_TIME_OUT});
	end
end

function PVEMatch:OnEntertimeOut(nInstanceIndex)
	-- 超时时间到了，没有选择同意进入副本关卡的要T掉，其他的人要收到通知
	if (self.tbMatchFinishedTimerID[nInstanceIndex] == nil 
		or self.tbMatchFinishedTimerID[nInstanceIndex] == 0)
	then
		return 0;
	end

	self.tbMatchFinishedTimerID[nInstanceIndex] = 0;
	local tbRemovedPlayers = {};
	local tbRemainPlayers = {};
	local tbTimeOutPlayers = {};
	local tbSelected = self.tbSelected[nInstanceIndex];

	local bFind = 0
	repeat
		bFind = 0;

		for szType, tbPlayers in pairs(tbSelected) do
			if (szType == "Team") then
				local bFindInTeam = 0;
				for key, tbPlayer in pairs(tbPlayers) do
					if (tbPlayer.nAccept ~= 1) then
						bFind = 1;
						bFindInTeam = 1;
						table.insert(tbTimeOutPlayers, tbPlayer);
					end
				end
				if (bFindInTeam == 1) then
					for _, tbPlayer in pairs(tbPlayers) do
						table.insert(tbRemovedPlayers, tbPlayer);
					end
					self:ClearEnteredTeam(tbPlayers);
					tbSelected["Team"] = nil;
				end
			else
				for key, tbPlayer in pairs(tbPlayers) do
					if (tbPlayer.nAccept ~= 1) then
						bFind = 1;
						table.insert(tbRemovedPlayers, tbPlayer);
						table.insert(tbTimeOutPlayers, tbPlayer);
						self:ClearEnteredPlayer(tbPlayer);
						table.remove(tbPlayers, key);
					end
				end
			end
		end
	until bFind == 0

	for szType, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			tbPlayer.nAccept = 0;
			table.insert(tbRemainPlayers, tbPlayer);
		end
	end

	self.tbMatchFinishedInstanceIndex[nInstanceIndex] = 0;

	-- 通知玩家超时
	if (#tbRemovedPlayers > 0) then
		GlobalExecute({"PVEMatch:NotifyPlayerEnterTimeOut_GS", tbRemovedPlayers, tbTimeOutPlayers, nInstanceIndex});
		GlobalExecute({"PVEMatch:NotifyTeamLeaveQueue_GS", tbRemovedPlayers, nInstanceIndex, 1});
	end

	if (#tbRemainPlayers > 0) then
		GlobalExecute({"PVEMatch:NotifyPlayerEnterTimeOut_GS", tbRemainPlayers, tbTimeOutPlayers, nInstanceIndex});
	end

	return 0; -- 返回零让定时器自动清除
end

function PVEMatch:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex)
	GlobalExecute({"PVEMatch:NotifyPlayerMatchSuccess_GS", tbPlayer, nInstanceIndex});
end

function PVEMatch:OnTeamMatchSuccess(tbTeam, nInstanceIndex)
	GlobalExecute({"PVEMatch:NotifyTeamMatchSuccess_GS", tbTeam, nInstanceIndex});
end

-- main logic
--------------------------------------------------------------------------------
function PVEMatch:StartMatch()
	local tbInstanceIndex = {};
	for nInstanceIndex, tbWaitQueue in pairs(self.tbInstanceWaitQueue) do
		if type(self.tbSelected[nInstanceIndex]) ~= "table" then
			self.tbSelected[nInstanceIndex] = {};
		end
		
		table.insert(tbInstanceIndex, nInstanceIndex);
	end
	
	-- 随机化副本匹配顺序
	if #tbInstanceIndex > 1 then
		for i = 1, #tbInstanceIndex - 1 do
			local nRandNum = KUnify.MathRandom(i, #tbInstanceIndex);
			local tmp = tbInstanceIndex[nRandNum];
			tbInstanceIndex[nRandNum] = tbInstanceIndex[i];
			tbInstanceIndex[i] = tmp;
		end
	end
	
	for _, nInstanceIndex in pairs(tbInstanceIndex) do	
		-- 是否匹配成功，条件是匹配到5人
		if self.tbMatchFinishedInstanceIndex[nInstanceIndex] == 0 or self.tbMatchFinishedInstanceIndex[nInstanceIndex] == nil then
			self:MatchPlayer(nInstanceIndex);
			if self:IsMatchFinished(nInstanceIndex) == 1 then
				if self.tbMatchFinishedInstanceIndex[nInstanceIndex] ~= 1 then
					self.tbMatchFinishedInstanceIndex[nInstanceIndex] = 1;
					self:OnMatchFinish(nInstanceIndex);
				end
			else
				self.tbMatchFinishedInstanceIndex[nInstanceIndex] = 0;
			end
		end
	end
end

-- 进行匹配，每轮至少选一个
-- 具体规则：第一轮进行完美匹配，优先选择队伍，其实选择单人，选择后再查找单人进行完美匹配。
-- 第二轮以后则进行非完美匹配，如果遇到完美的，则会选择，另外，等待列表不为空，则会匹配一个不完美玩家
-- 完美匹配就是1坦克1治疗3输出，然后其中四个是不同门派，另外一个是同门派但不同职业
function PVEMatch:MatchPlayer(nInstanceIndex)
	local s = self.tbSelected[nInstanceIndex];
	local q = self.tbInstanceWaitQueue[nInstanceIndex]; 
	
	s["Tank"] = s["Tank"] or {};
	s["Help"] = s["Help"] or {};
	s["Dps"] = s["Dps"] or {};
	s["Team"] = s["Team"] or {};
	q["Tank"] = q["Tank"] or {};
	q["Help"] = q["Help"] or {};
	q["Dps"] = q["Dps"] or {};
	q["Team"] = q["Team"] or {};
	
	-- 移除空组队
	for k, tbTeam in pairs(q["Team"]) do
		if type(tbTeam) == "table" and #tbTeam == 0 then
			table.remove(q["Team"], k);
		end
	end
	
	-- 判断是否是第一轮，优先选择组队
	if #s["Tank"] == 0 and #s["Help"] == 0 and #s["Dps"] == 0 and #s["Team"] == 0 then
		if #q["Team"] > 0 then -- 有组队在等待，则优先选择组队
			local tbTeam = q["Team"][1];
			s["Team"] = tbTeam;
			table.remove(q["Team"], 1);
			-- 将这个组队从所有等待列表中移除
			-- self:RemoveTeamFromWaitQueue(tbTeam);
			-- 通知组队成员匹配成功，问其是否同意进入副本
			self:OnTeamMatchSuccess(s["Team"], nInstanceIndex);
		end
	end

	if (#s["Team"] == nil) then
		s["Team"] = {};
	end
	
	if #s["Team"] < self.MAX_TEAM_NUM then
		-- 进行完美匹配，1T1N3D，不同门派不同职业，可能一次选择了5个，也可能一次只选少于5个，但选择的必须是完美的
		self:PerfectMatch(nInstanceIndex);
		
		-- 非第一轮则进行非完美匹配
		-- 非完美匹配必须至少选择一个非完美玩家，优先级顺序：队伍职责TND，门派，职业
		-- 先尝试选职责互斥，无则选门派互斥，再无则选职业互斥，再无则按进入时间选择一个
		self:LessMatch(nInstanceIndex);
	end
end

-- 只进行完美匹配
-- 无T且T等待列表不为空则完美选1T，无N且N等待列表不为空则完美选1N，无D或D人数少于3且D等待列表不为空，则完美选1D，按已有D的个数尝试选3D
function PVEMatch:PerfectMatch(nInstanceIndex)
	local q = self.tbInstanceWaitQueue[nInstanceIndex];
	local s = self.tbSelected[nInstanceIndex];
	local k = nil;

	-- 先完美匹配T，已选玩家中无T且副本T等待列表不为空，则从T等待列表中排除已经有的门派和职业，选择序号最前的一个	
	if #s["Tank"] + self:GetTeamTypeNum(s["Team"], "Tank") == 0 and #q["Tank"] > 0 then
		k = self:_PerfectMatchSingle(s, q["Tank"]);
		if k > 0 then
			local tbPlayer = q["Tank"][k];
			-- self:RemovePlayerFromWaitQueue(q["Tank"][k]);
			table.remove(q["Tank"], k);
			table.insert(s["Tank"], tbPlayer);
			self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
		end
	end

	-- 再完美匹配N，已选玩家无N且副本N等待列表不为空，则从N等待列表中排除已有的门派和职业，选择序号最前的一个
	if #s["Help"] + self:GetTeamTypeNum(s["Team"], "Help") == 0 and #q["Help"] > 0 then
		k = self:_PerfectMatchSingle(s, q["Help"]);
		if k > 0 then
			local tbPlayer = q["Help"][k];
			-- self:RemovePlayerFromWaitQueue(q["Help"][k]);
			table.remove(q["Help"], k);
			table.insert(s["Help"], tbPlayer);
			self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
		end
	end

	-- 最后再匹配D，已选玩家中D少于3且列表不为空，则从D等待列表中排除已有的门派和职业，选择序号最前的一个
	-- 由于要选3个D所以尝试选3次
	while #s["Dps"] + self:GetTeamTypeNum(s["Team"], "Dps") < 3 and #q["Dps"] > 0 do
		k = self:_PerfectMatchSingle(s, q["Dps"]);
		if k > 0 then
			local tbPlayer = q["Dps"][k];
			-- self:RemovePlayerFromWaitQueue(q["Dps"][k]);
			table.remove(q["Dps"], k);
			table.insert(s["Dps"], tbPlayer);
			self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
		else
			break; -- 无完美D，则退出
		end
	end

	return s;
end

-- 尝试从单人等待队列中选择完美匹配的玩家
-- 如有，则返回队列的数组索引，大于0的整数，没有则返回-1
function PVEMatch:_PerfectMatchSingle(tbSelected, tbWaitQueue)
	local nSelectedNum = 0;
	local nNewMatchKey = -1;
	-- 找出已经有的门派
	local tbHasFaction = {};
	for _, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			tbHasFaction[tbPlayer.nFaction] = 1;
			nSelectedNum = nSelectedNum + 1;
		end
	end

	if nSelectedNum + 1 <= self.MAX_TEAM_NUM then
		-- 排除相同的门派
		for i = 1, #tbWaitQueue do
			if tbHasFaction[tbWaitQueue[i].nFaction] == nil then
				nNewMatchKey = i;
				break;
			end
		end
	end
	
	return nNewMatchKey;
end

-- 非完美匹配，等待列表不为空时选择且只选择一个玩家，优先顺序为：队伍职责互斥TND，门派互斥，职业类型互斥
function PVEMatch:LessMatch(nInstanceIndex)
	-- print("LessMatch");
	-- 无T且T列表有人则选一个T，非完美匹配下总是能选一个
	local q = self.tbInstanceWaitQueue[nInstanceIndex];
	local s = self.tbSelected[nInstanceIndex];
	local k = -1;
	
	-- 无T且T列表有人则选一个T，非完美匹配下总能选一个
	if k <= 0 then
		if #s["Tank"] + self:GetTeamTypeNum(s["Team"], "Tank") == 0 and #q["Tank"] > 0 then
			k = self:_LessMatchSingle(s, q["Tank"]);
			if k > 0 then
				local tbPlayer = q["Tank"][k];
				-- self:RemovePlayerFromWaitQueue(q["Tank"][k]);
				table.remove(q["Tank"], k);
				table.insert(s["Tank"], tbPlayer);
				self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
			end
		end
	end
	
	-- 无奶且奶列表有人则选一个奶，非完美匹配下总是能选一个
	if k <= 0 then
		if #s["Help"] + self:GetTeamTypeNum(s["Team"], "Help") == 0 and #q["Help"] > 0 then
			k = self:_LessMatchSingle(s, q["Help"]);
			if k > 0 then
				local tbPlayer = q["Help"][k];
				-- self:RemovePlayerFromWaitQueue(q["Help"][k]);
				table.remove(q["Help"], k);
				table.insert(s["Help"], tbPlayer);
				self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
			end
		end
	end
	
	-- Dps人数少于3个，则非完美匹配下总是能选一个
	if k <= 0 then
		if #s["Dps"] + self:GetTeamTypeNum(s["Team"], "Dps") < 3 and #q["Dps"] > 0 then
			k = self:_LessMatchSingle(s, q["Dps"]);
			if k > 0 then
				local tbPlayer = q["Dps"][k];
				-- self:RemovePlayerFromWaitQueue(q["Dps"][k]);
				table.remove(q["Dps"], k);
				table.insert(s["Dps"], tbPlayer);
				self:OnPlayerMatchSuccess(tbPlayer, nInstanceIndex);
			end
		end
	end
	
	-- 最后，实在找不到人，则找等待列表中最先进入的一个
	if k <= 0 then
		local f = {};
		if #q["Tank"] > 0 then
			f.p = q["Tank"][1];
			f.t = 1;
		end
		
		if #q["Help"] > 0 then
			if f.p == nil then
				f.p = q["Help"][1];
				f.t = 2;
			elseif f.p.time > q["Help"][1].time then
				f.p = q["Help"][1];
				f.t = 2;
			end
		end
		
		if #q["Dps"] > 0 then
			if f.p == nil then
				f.p = q["Dps"][1];
				f.t = 3;
			elseif f.p.time > q["Dps"][1].time then
				f.p = q["Dps"][1];
				f.t = 3;
			end
		end
		
		if f.p ~= nil then
			if f.t == 1 then
				-- self:RemovePlayerFromWaitQueue(f.p);
				table.remove(q["Tank"], 1);
				table.insert(s["Tank"], f.p);
				self:OnPlayerMatchSuccess(f.p, nInstanceIndex);
			elseif f.t == 2 then
				-- self:RemovePlayerFromWaitQueue(f.p);
				table.remove(q["Help"], 1);
				table.insert(s["Help"], f.p);
				self:OnPlayerMatchSuccess(f.p, nInstanceIndex);
			elseif f.t == 3 then
				-- self:RemovePlayerFromWaitQueue(f.p);
				table.remove(q["Dps"], 1);
				table.insert(s["Dps"], f.p);
				self:OnPlayerMatchSuccess(f.p, nInstanceIndex);
			end
		end
	end
end

-- 先尝试在不同门派中选，选不出来则尝试从不同职业中选，再选不出来则选总体序号第一个
-- 如有则返回大于0的整数索引，如没有则返回-1，除非队列为空，否则总能选一个
function PVEMatch:_LessMatchSingle(tbSelected, tbWaitQueue)
	-- 尝试找不同门派的
	local nNewMatchKey = -1;
	local tbHasFaction = {};
	for _, tbPlayers in pairs(tbSelected) do
		for _, tbPlayer in pairs(tbPlayers) do
			tbHasFaction[tbPlayer.nFaction] = 1;
		end
	end

	for i = 1, #tbWaitQueue do
		if tbHasFaction[tbWaitQueue[i].nFaction] == nil then
			nNewMatchKey = i;
			break;
		end
	end
	
	-- 尝试找不同职业，可能会同门派
	if nNewMatchKey < 0 then
		-- 找出已有的天赋路线
		local tbHasFactionRoute = {};
		for _, tbPlayers in pairs(tbSelected) do
			for _, tbPlayer in pairs(tbPlayers) do
				tbHasFactionRoute[tbPlayer.nFaction] = tbHasFactionRoute[tbPlayer.nFaction] or {};
				tbHasFactionRoute[tbPlayer.nFaction][tbPlayer.nRoute] = 1;
			end
		end

		for i = 1, #tbWaitQueue do
			local nFaction = tbWaitQueue[i].nFaction;
			local nRoute = tbWaitQueue[i].nRoute;
			if tbHasFactionRoute[nFaction] == nil or tbHasFactionRoute[nFaction][nRoute] == nil then
				nNewMatchKey = i;
				break;
			end
		end
	end
	
	-- 实在找不到人了，则取序号第一个
	if nNewMatchKey < 0 and #tbWaitQueue > 0 then
		nNewMatchKey = 1;
	end
	
	return nNewMatchKey;
end

-- 启动定时器，每隔指定时间进行匹配一次
function PVEMatch:BeginTimer()
	if type(self.nRegisterTimerID) == "nil" then
		self.nRegisterTimerID = Timer:Register(self.MATCH_INTERVAL, self.StartMatch, self);
	end
end

function PVEMatch:EndTimer()
	if type(self.nRegisterTimerID) == "nil" then
		Timer:Close(self.nRegisterTimerID);
		self.nRegisterTimerID = nil;
	end
end

-- 获取"Dps" "Help" "Tank"人员个数
function PVEMatch:GetTeamTypeNum(tbTeam, szType)
	local num = 0;
	if type(tbTeam) == "table" and #tbTeam > 0 then
		for _, varPlayer in pairs(tbTeam) do
			if varPlayer.szType == szType then
				num = num + 1;
			end
		end
	end
	return num;
end

-- ?gc PVEMatch:SetMaxTeamNum(2);
function PVEMatch:SetMaxTeamNum(nNum)
	self.MAX_TEAM_NUM = tonumber(nNum) or 1;
	print("PVEMatch:SetMaxTeamNum set number : " .. self.MAX_TEAM_NUM);
end

PVEMatch:BeginTimer();

