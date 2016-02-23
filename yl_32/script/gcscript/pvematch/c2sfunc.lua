-- File       : c2sfunc.lua
-- Creator    : liyun
-- Date       : 2012-11-1 10:15:54
-- Description: 副本排队相关c2s函数

if not (MODULE_GAMESERVER) then
	return
end

Include("/script/character/characterdef.lua");

-- 玩家的天赋路线决定了在队伍中的职业
PVEMatch.tbRoute2Type = {
	[Character.FACTION_WUSHANG] = {
		[Character.ROUTE_WUSHANG_JIANSHEN] 	= "Dps",	-- 剑神无上
		[Character.ROUTE_WUSHANG_YUHE] 		= "Help",	-- 愈合无上
	},
	[Character.FACTION_TIANYAO] = {
		[Character.ROUTE_TIANYAO_FANGHU] 	= "Tank",	-- 防护天曜
		[Character.ROUTE_TIANYAO_SHENSHENG] = "Help",	-- 神圣天曜
	},
	[Character.FACTION_YOUMING] = {
		[Character.ROUTE_YOUMING_WENGU] 	= "Tank",	-- 稳固幽冥
		[Character.ROUTE_YOUMING_CHONGJI] 	= "Dps",	-- 冲击幽冥
	},
	[Character.FACTION_SENLUO] = {
		[Character.ROUTE_SENLUO_SHOULIE] 	= "Dps",	-- 狩猎森罗
		[Character.ROUTE_SENLUO_CISHA] 		= "Dps",	-- 刺杀森罗
	},
};

-- 调试输出开关
PVEMatch.bChatMsg = 0;
PVEMatch.bBlackMsg = 1;
function PVEMatch:OutSysMsg(pPlayer, szMsg)
	if pPlayer then
		if (self.bChatMsg == 1) then
			pPlayer.SysMsg(szMsg);
		end

		if (self.bBlackMsg == 1) then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowBlackMessage", szMsg});
		end
	end
end

PVEMatch.ROOM_XOYO_EASY = "XoyoGameEasy";
PVEMatch.ROOM_XOYO_NORMAL = "XoyoGameNormal";
PVEMatch.ROOM_XOYO_HARD = "XoyoGameHard";
PVEMatch.ROOM_XOYO_VERY_HARD = "XoyoGameVeryHard";

-- c2s funcs
------------------------------------------------------------------------------------
-- 玩家进行排队，nInstanceIndex 0表示随机，大于0表示选择排的副本索引，其他值无效
function PVEMatch:c2s_EnterInstanceWaitQueue(pPlayer, nInstanceIndex)
	-- for debug
	-- pPlayer.SysMsg("申请进入的副本索引是："..nInstanceIndex.." 玩家ID："..pPlayer.dwId);
	-- pPlayer.CallClientScript({"YoulongApp.UI.QUICallClientScript:ShowConfirmEnterMission",});
	-- local tbPlayerIds = {pPlayer.dwId, pPlayer.dwId + 1, pPlayer.dwId + 2, pPlayer.dwId + 3, pPlayer.dwId + 4};
	-- local tbFactions = {1, 2, 3, 1, 4};
	-- pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PVEMatchFinished", tbPlayerIds, tbFactions}); do -- return end;
	-- Instance:TryEnterInstance({pPlayer.dwId}, nInstanceIndex);

	assert(pPlayer);

	local nTeamId = pPlayer.GetTeamId();
	local tbTeamMem, nTeamNum = nil, 0;
	if nTeamId > 0 then
		tbTeamMem, nTeamNum = KTeam.GetTeamMemberList(nTeamId);
	end

	if nTeamId > 0 and nTeamNum >= 2 then -- 组队的情况，一人队伍当成单人处理
		if pPlayer.IsCaptain() ~= 1 then
			print("c2s_EnterInstanceWaitQueue: player must be a captain, id is "..pPlayer.dwId);
			return;
		end

		KGameBase.CenterExecute({"PVEMatch:VeryfyTeamMember_GC", tbTeamMem, pPlayer.dwId, nInstanceIndex});
	else -- 单人情况
		-- 已经在副本中不能够排队
		local nMapId = pPlayer.dwSceneTemplateId;
		if nMapId ~= PetIsland.nPetIslandMapId and KScene.GetSceneTypeName(nMapId) ~= "city" then
			--print("c2s_EnterInstanceWaitQueue: player already in mapcopy, playerid is "..pPlayer.dwId);
			return;
		end

		local tbInstanceIndex = {};
		local nMinLevel = MultiInstance:GetMultiMinLevel(nInstanceIndex);
		assert(pPlayer.GetLevel() >= nMinLevel);
		
		table.insert(tbInstanceIndex, nInstanceIndex);
		
		--[[
		local tbSuitable = self:GetPlayerSuitableInstanceIndexes(pPlayer.GetLevel());

		if nInstanceIndex > 0 then
			for _, nIndex in pairs(tbSuitable) do
				if nIndex == nInstanceIndex then
					table.insert(tbInstanceIndex, nIndex);
				end
			end
		elseif nInstanceIndex == 0 then -- 随机副本
			tbInstanceIndex = tbSuitable;
		else
			assert(0);
			print("c2s_EnterInstanceWaitQueue: parm nInstanceIndex error");
			return;
		end
		--]]

		local tbPlayer = {
			dwId = pPlayer.dwId,
			szType = self.tbRoute2Type[pPlayer.GetPrimaryFaction()][pPlayer.GetCurrentRoute()],
			time = KGameBase.GetTime(),
			nAccept = 0,
			nFaction = pPlayer.GetPrimaryFaction(),
			nRoute = pPlayer.GetCurrentRoute(),
			szName = pPlayer.szName,
		};

		if (#tbInstanceIndex == 0) then
			self:OutSysMsg(pPlayer, Lang.gcscript.str3[Lang.Idx]);
			-- pPlayer.SysMsg("系统没找到合适的副本");
			return;
		end

		KGameBase.CenterExecute{"PVEMatch:SingleRequestEnterInstance_GC", tbPlayer, tbInstanceIndex};
	end
end

function PVEMatch:c2s_LeaveWaitQueue(pPlayer)
	KGameBase.CenterExecute{"PVEMatch:PlayerLeaveWaitQueue", pPlayer.dwId};
end

function PVEMatch:VeryfyTeamMember_GS(tbTeamIds, nCaptainId, nInstanceIndex)
	local tbMemberStageMoping = {};
	local tbMemberNoDegree = {};
	local tbMemberInMapCopy = {};
	local tbMemberVeryfied = {};
	local tbTeam = {};
	local tbTeamLevel = {};
	for _, nPlayerId in pairs(tbTeamIds) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if (pPlayer ~= nil) then
			tbMemberVeryfied[pPlayer.dwId] = pPlayer.szName;
			if (nInstanceIndex >= 0) then
				if Player.tbDegree:GetDegree(pPlayer, "MultiInstance") <= 0 then
					table.insert(tbMemberNoDegree, pPlayer.dwId);
				end
			else
				if Player.tbDegree:GetDegree(pPlayer, "RushStage") <= 0 then
					table.insert(tbMemberNoDegree, pPlayer.dwId);
				end
			end
			
			if nInstanceIndex < 0 then
				-- 正在扫荡中
				if KStage.GetMopState(pPlayer) == Player.emMOP_STATE_DOING then
					table.insert(tbMemberStageMoping, pPlayer.dwId);
				end
			end

			local nMapId = pPlayer.dwSceneTemplateId;
			if nMapId ~= PetIsland.nPetIslandMapId and KScene.GetSceneTypeName(nMapId) ~= "city" then
				table.insert(tbMemberInMapCopy, pPlayer.dwId);
			end

			table.insert(tbTeam,
				{
					dwId = pPlayer.dwId,
					szType = self.tbRoute2Type[pPlayer.GetPrimaryFaction()][pPlayer.GetCurrentRoute()],
					bAccept = 0,
					nFaction = pPlayer.GetPrimaryFaction(),
					nRoute = pPlayer.GetCurrentRoute(),
					szName = pPlayer.szName,
				}
			);
			tbTeamLevel[nPlayerId] = pPlayer.GetLevel();
		end
	end

	KGameBase.CenterExecute({"PVEMatch:VeryfyTeamMemberResult_GC",
		nCaptainId, tbMemberVeryfied, tbMemberNoDegree, tbMemberInMapCopy, tbMemberStageMoping, tbTeam, tbTeamLevel});
end

function PVEMatch:VeryfyTeamLevel_GS(nCaptainId, nInstanceIndex, tbTeamLevel, tbTeam, tbMemberName)
	local pPlayer = KGameBase.GetPlayerById(nCaptainId);
	if (pPlayer == nil) then
		return;
	end
	
	assert(nInstanceIndex ~= 0);

	local tbInstanceIndex = {};
	if (nInstanceIndex > 0) then
		for nPlayerId, nMemberLevel in pairs(tbTeamLevel) do
			local nMinLevel = MultiInstance:GetMultiMinLevel(nInstanceIndex);
			if nMemberLevel < nMinLevel then
				if (nPlayerId == nCaptainId) then

					self:OutSysMsg(pPlayer, string.format(Lang.gcscript.str4[Lang.Idx],nMemberLevel,nMinLevel));
					--self:OutSysMsg(pPlayer, "您的等级为"..nMemberLevel.."级, 不能进入等级要求为"..nMinLevel.."级的副本");
				else
					local szName = "";
					if (tbMemberName[nPlayerId] ~= nil) then
						szName = tbMemberName[nPlayerId]["Name"];
					end
					self:OutSysMsg(pPlayer, string.format(Lang.gcscript.str5[Lang.Idx],szName,nMemberLevel, nMinLevel));
					--self:OutSysMsg(pPlayer, "您的队友"..szName.."的等级为"..nMemberLevel.."级，不能进入等级要求为"..nMinLevel.."级的副本");
				end

				return;
			end
		end
		table.insert(tbInstanceIndex, nInstanceIndex);
	elseif (nInstanceIndex < 0) then
		local tbMinLevel = {[-1] = 20, [-2] = 30, [-3] = 40, [-4] = 50};
		local tbRoomName = {[-1] = self.ROOM_XOYO_EASY, [-2] = self.ROOM_XOYO_NORMAL, [-3] = self.ROOM_XOYO_HARD, [-4] = self.ROOM_XOYO_VERY_HARD};

		for nPlayerId, nMemberLevel in pairs(tbTeamLevel) do
			if (nMemberLevel < tbMinLevel[nInstanceIndex]) then
				if (nPlayerId == nCaptainId) then
					self:OutSysMsg(pPlayer, string.format(Lang.gcscript.str6[Lang.Idx],nMemberLevel,tbMinLevel[nInstanceIndex]));
					--self:OutSysMsg(pPlayer, "您的等级为"..nMemberLevel.."级，不能进入等级要求为"..tbMinLevel[nInstanceIndex].."级的关卡");
				else
					local szName = "";
					if (tbMemberName[nPlayerId] ~= nil) then
						szName = tbMemberName[nPlayerId]["Name"];
					end
					self:OutSysMsg(pPlayer, string.format(Lang.gcscript.str7[Lang.Idx],szName,nMemberLevel,tbMinLevel[nInstanceIndex]));
					--self:OutSysMsg(pPlayer, "您的队友"..szName.."的等级为"..nMemberLevel.."级，不能进入等级要求为"..tbMinLevel[nInstanceIndex].."级的关卡");
				end

				return;
			end
		end

		table.insert(tbInstanceIndex, tbRoomName[nInstanceIndex]);
	else
		assert(0);
		print("PVEMatch:VeryfyTeamLevel_GS nInstanceIndex error");
	end

	if (#tbInstanceIndex == 0) then
		pPlayer.OutSysMsg(Lang.gcscript.str8[Lang.Idx]);
		return;
	end

	KGameBase.CenterExecute({"PVEMatch:TeamRequestEnterInstance_GC", tbTeam, tbInstanceIndex});
end

function PVEMatch:NotifyTeamInfo_GC(nCaptainId, szMsg)
	local pPlayer = KGameBase.GetPlayerById(nCaptainId);
	if (pPlayer) then
		self:OutSysMsg(pPlayer, szMsg);
	end
end

PVEMatch.tbStageTimeIds = {};

function PVEMatch:c2s_EnterInstanceRequest(pPlayer, nMapId)
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	assert(pPlayer.IsCaptain() == 1);

	local tbTeamMem, nTeamNum = KTeam.GetTeamMemberList(nTeamId);

	KGameBase.CenterExecute({"PVEMatch:VeryfyTeamMember_GC", tbTeamMem, pPlayer.dwId, nMapId});
end

function PVEMatch:DelayEnterMultiInstanceRequest_GS(tbTeam, nMapId)
	local tbPlayers = {};
	for _, tbPlayer in ipairs(tbTeam) do
		table.insert(tbPlayers, tbPlayer.dwId);
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		if pPlayer then
			-- 播进度条
			KInstance.SetMultiRushMapId(pPlayer, nMapId);
			pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowEnterInstanceProcess", 10});
		end
	end
end

function PVEMatch:c2s_DoEnterInstance(pPlayer, nMapId)
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	assert(pPlayer.IsCaptain() == 1);
	local tbMembers = KTeam.GetTeamMemberList(nTeamId);
	
	KGameBase.CenterExecute{"PVEMatch:RequestEnterMultiInstance_GC", tbMembers, nMapId};
end

-- 关卡入口
function PVEMatch:c2s_EnterStageRequest(pPlayer, nDifficulty)
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	assert(pPlayer.IsCaptain() == 1);

	local tbTeamMem, nTeamNum = KTeam.GetTeamMemberList(nTeamId);

	local tbDiff2InstanceIndex = {[1] = -1, [2] = -2, [3] = -3, [4] = -4};

	pPlayer.SetLevelDifficuty(nDifficulty);

	KGameBase.CenterExecute({"PVEMatch:VeryfyTeamMember_GC", tbTeamMem, pPlayer.dwId, tbDiff2InstanceIndex[nDifficulty]});
end

function PVEMatch:DelayEnterStageRequest_GS(tbTeam)
	local tbPlayers = {};
	for _, tbPlayer in ipairs(tbTeam) do
		table.insert(tbPlayers, tbPlayer.dwId);
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		if pPlayer then
			-- 播进度条
			pPlayer.CallClientScript({"YoulongApp.Represent.QRepresentClientScript:ShowEnterStageProcess", 10});
		end
	end
	
end

function PVEMatch:c2s_DoEnterStage(pPlayer)
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	assert(pPlayer.IsCaptain() == 1);
	local tbMembers = KTeam.GetTeamMemberList(nTeamId);
	
	local nDifficulty = pPlayer.GetLevelDifficuty();
	
	KGameBase.CenterExecute{"PVEMatch:RequestEnterStage_GC", tbMembers, nDifficulty};
end

-- 1-19级不能选，20-39级只能选简单，40-59能选困难，60级全能选
function PVEMatch:c2s_EnterRoomWaitQueue(pPlayer, ndifficutity)
	assert(pPlayer);
	ndifficutity = tonumber(ndifficutity);
	
	KStage.SetRushDifficulty(pPlayer, ndifficutity);

	local tbMinLevel = {[1] = 20, [2] = 30, [3] = 40, [4] = 50};
	local tbRoomName = {[1] = self.ROOM_XOYO_EASY, [2] = self.ROOM_XOYO_NORMAL, [3] = self.ROOM_XOYO_HARD, [4] = self.ROOM_XOYO_VERY_HARD};

	-- 对应简单困难，炼狱三个难度
	if tbMinLevel[ndifficutity] == nil then
		print("PVEMatch:c2s_EnterRoomWaitQueue ndifficutity error");
		return;
	end

	local nTeamId = pPlayer.GetTeamId();
	local tbTeamMem, nTeamNum = nil, 0;
	if nTeamId > 0 then
		tbTeamMem, nTeamNum = KTeam.GetTeamMemberList(nTeamId);
	end

	local bAllAboveLevel = 1;
	if nTeamId > 0 and nTeamNum >= 2 then -- 组队的情况，一人队列当单人处理
		if pPlayer.IsCaptain() ~= 1 then
			print("c2s_EnterRoomWaitQueue: player must be a captain, id is "..pPlayer.dwId);
			return;
		end

		local tbDiff2InstanceIndex = {[1] = -1, [2] = -2, [3] = -3, [4] = -4};

		KGameBase.CenterExecute({"PVEMatch:VeryfyTeamMember_GC", tbTeamMem, pPlayer.dwId, tbDiff2InstanceIndex[ndifficutity]});
	else
		-- 已经在副本中不能够排队
		local nMapId = pPlayer.dwSceneTemplateId;
		if nMapId ~= PetIsland.nPetIslandMapId and KScene.GetSceneTypeName(nMapId) ~= "city" then
			--print("c2s_EnterInstanceWaitQueue: player already in mapcopy, playerid is "..pPlayer.dwId);
			return;
		end

		if (pPlayer.GetLevel() < tbMinLevel[ndifficutity]) then
			self:OutSysMsg(pPlayer, "级别太低，无法进入选择难度的关卡");
			-- pPlayer.SysMsg("级别太低，无法进入选择难度的关卡");
			return;
		end

		local tbPlayer = {
			dwId = pPlayer.dwId,
			szType = "Dps",
			time = KGameBase.GetTime(),
			nAccept = 0,
			nFaction = pPlayer.GetPrimaryFaction(),
			nRoute = pPlayer.GetCurrentRoute(),
			szName = pPlayer.szName,
		};

		KGameBase.CenterExecute{"PVEMatch:SingleRequestEnterInstance_GC", tbPlayer, {[1] = tbRoomName[ndifficutity]}};
	end
end

function PVEMatch:NotifyPlayerLeaveQueue_GS(tbPlayer, nInstanceIndex, bNotifyPlayer)
	bNotifyPlayer = bNotifyPlayer or 1;
	local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);

	if (pPlayer == nil) then
		return;
	end

	if nInstanceIndex == self.ROOM_XOYO_EASY or
		nInstanceIndex == self.ROOM_XOYO_NORMAL or
		nInstanceIndex == self.ROOM_XOYO_HARD or
		nInstanceIndex == self.ROOM_XOYO_VERY_HARD
	then
		pPlayer.SetMissionState(PVEMatch.STATE_NONE);
		--pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetCurMissionQueueState", 0});
		if bNotifyPlayer == 1 then
			self:OutSysMsg(pPlayer, ang.gcscript.str9[Lang.Idx]);
		end
	else
		pPlayer.SetMissionState(PVEMatch.STATE_NONE);
		KInstance.SetMultiRushMapId(pPlayer, 0);
		--pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetCurQueueState", 0});
		if bNotifyPlayer == 1 then
			self:OutSysMsg(pPlayer, Lang.gcscript.str10[Lang.Idx]);
		end
	end
end

function PVEMatch:NotifyTeamLeaveQueue_GS(tbTeam, nInstanceIndex, bNotifyPlayer)
	for _, tbPlayer in pairs(tbTeam) do
		self:NotifyPlayerLeaveQueue_GS(tbPlayer, nInstanceIndex, bNotifyPlayer);
	end
end

-- 通知玩家进入队列
function PVEMatch:NotifyPlayerEnterQueue_GS(tbPlayer, nInstanceIndex, bNotifyPlayer)
	bNotifyPlayer = bNotifyPlayer or 1;
	local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);

	if (pPlayer == nil) then
		return;
	end

	if nInstanceIndex == self.ROOM_XOYO_EASY or
		nInstanceIndex == self.ROOM_XOYO_NORMAL or
		nInstanceIndex == self.ROOM_XOYO_HARD or
		nInstanceIndex == self.ROOM_XOYO_VERY_HARD
	then
		pPlayer.SetMissionState(PVEMatch.STATE_STAGE_WATING);
		--pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetCurMissionQueueState", 1});
		if (bNotifyPlayer == 1) then
			self:OutSysMsg(pPlayer, Lang.gcscript.str11[Lang.Idx]);
		end
	else
		pPlayer.SetMissionState(PVEMatch.STATE_INSTANCE_WATING);
		KInstance.SetMultiRushMapId(pPlayer, nInstanceIndex);
		--pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetCurQueueState", 1});
		if (bNotifyPlayer == 1) then
			self:OutSysMsg(pPlayer, Lang.gcscript.str12[Lang.Idx]);
		end
	end
end

function PVEMatch:NotifyTeamEnterQueue_GS(tbTeam, nInstanceIndex, bNotifyPlayer)
	for _, tbPlayer in pairs(tbTeam) do
		self:NotifyPlayerEnterQueue_GS(tbPlayer, nInstanceIndex, bNotifyPlayer);
	end
end

-- 通知玩家被匹配上
function PVEMatch:NotifyPlayerMatchSuccess_GS(tbPlayer, nInstanceIndex)
	local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);

	if (pPlayer == nil) then
		return;
	end

	-- self:OutSysMsg(pPlayer, "you have matched successfully, instance index is "..nInstanceIndex);
	-- self:OutSysMsg(pPlayer, "您已经匹配上，请等待系统为您匹配其他玩家");
end

-- 通知玩家被匹配上
function PVEMatch:NotifyTeamMatchSuccess_GS(tbTeam, nInstanceIndex)
	for _, tbPlayer in pairs(tbTeam) do
		self:NotifyPlayerMatchSuccess_GS(tbPlayer, nInstanceIndex);
	end
end

-- 通知玩家匹配完成，弹出对话框，让玩家选择是否进入副本
-- nTimerOut是以秒为单位超时时间，玩家在超时时间内没有作出进入或者拒绝副本的选择则视为拒绝进入副本
function PVEMatch:NotifyMatchFinished_GS(tbPlayers, nInstanceIndex, nTimeOut)
	local tbPlayerIds = {};
	local tbFactions = {};
	for i = 1, #tbPlayers do
		tbPlayerIds[i] = tbPlayers[i].dwId;
		tbFactions[i] = tbPlayers[i].nFaction;
	end
	for i = 1, #tbPlayers do
		local pPlayer = KGameBase.GetPlayerById(tbPlayers[i].dwId);
		if pPlayer then
			pPlayer.SetMoveState(Character.MOVE_STATE_ON_STAND);
			-- self:OutSysMsg(pPlayer, "所有人都匹配成功, 副本ID是"..nInstanceIndex);
			self:OutSysMsg(pPlayer, string.format(Lang.gcscript.str13[Lang.Idx],nTimeOut));
			--self:OutSysMsg(pPlayer, "匹配成功，您有"..nTimeOut.."秒时间选择进入还是离开");
			if nInstanceIndex == self.ROOM_XOYO_EASY or
				nInstanceIndex == self.ROOM_XOYO_NORMAL or
				nInstanceIndex == self.ROOM_XOYO_HARD or
				nInstanceIndex == self.ROOM_XOYO_VERY_HARD
			then
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PVEMatchFinished", tbPlayerIds, tbFactions, 2, nTimeOut});
			else
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PVEMatchFinished", tbPlayerIds, tbFactions, 1, nTimeOut});
			end
		end
	end
end

-- 玩家匹配完成后同意进入副本，可能是单人也可能是组队成员
function PVEMatch:c2s_AcceptEnterInstance(pPlayer)
	-- self:OutSysMsg(pPlayer, " 同意进入");
	KGameBase.CenterExecute({"PVEMatch:AcceptEnterInstance_GC", pPlayer.dwId});
end

-- 玩家匹配完成后拒绝进入副本，可能是单人也可能组队成员
function PVEMatch:c2s_RejectEnterInstance(pPlayer)
	-- self:OutSysMsg(pPlayer, " 不同意进入");
	KGameBase.CenterExecute({"PVEMatch:RejectEnterInstance_GC", pPlayer.dwId});
end

-- 玩家同意进入副本的广播信息
function PVEMatch:NotifyAcceptEnterInstance_GS(tbPlayers, tbAcceptPlayer)
	for _, tbPlayer in pairs(tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		if pPlayer then
			if (pPlayer.dwId == tbAcceptPlayer.dwId) then
				self:OutSysMsg(pPlayer, Lang.gcscript.str14[Lang.Idx]);
			else
				self:OutSysMsg(pPlayer, tbAcceptPlayer.szName..Lang.gcscript.str15[Lang.Idx]);
			end
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PlayerAcceptEnter", tbAcceptPlayer.dwId});
		end
	end
end

-- 玩家拒绝进入副本的广播信息
function PVEMatch:NotifyRejectEnterInstance_GS(tbPlayers, tbRejectPlayer)
	for _, tbPlayer in pairs(tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		if pPlayer then
			if (pPlayer.dwId == tbRejectPlayer.dwId) then
				self:OutSysMsg(pPlayer, Lang.gcscript.str16[Lang.Idx]);
			else
				self:OutSysMsg(pPlayer, tbRejectPlayer.szName..Lang.gcscript.str17[Lang.Idx]);
			end
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PlayerRejectEnter", tbRejectPlayer.dwId});
		end
	end
end

-- 玩家选择进入副本时超时了
function PVEMatch:NotifyPlayerEnterTimeOut_GS(tbPlayers, tbTimeOutPlayers, nInstanceIndex)
	for _, tbPlayer in pairs(tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		if pPlayer then
			for _, tbPlayer2 in pairs(tbTimeOutPlayers) do
				if (pPlayer.dwId == tbPlayer2.dwId) then
					self:OutSysMsg(pPlayer, Lang.gcscript.str18[Lang.Idx]);
				else
					self:OutSysMsg(pPlayer, tbPlayer2.szName..Lang.gcscript.str19[Lang.Idx]);
				end
				pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:PlayerRejectEnter", tbPlayer2.dwId});
			end
		end
	end
end

-- 进入关卡
function PVEMatch:DoEnterInstance_GS(tbPlayers, nInstanceIndex)
	local tbPlayerIds = {};
	for _, tbPlayer in pairs(tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.dwId);
		table.insert(tbPlayerIds, tbPlayer.dwId);
	end
	if (nInstanceIndex == self.ROOM_XOYO_EASY) then
		Stage:DoEnterGame(tbPlayerIds, 1);
	elseif (nInstanceIndex == self.ROOM_XOYO_NORMAL) then
		Stage:DoEnterGame(tbPlayerIds, 2);
	elseif (nInstanceIndex == self.ROOM_XOYO_HARD) then
		Stage:DoEnterGame(tbPlayerIds, 3);
	elseif (nInstanceIndex == self.ROOM_XOYO_VERY_HARD) then
		Stage:DoEnterGame(tbPlayerIds, 4);
	else
		MultiInstance:DoEnterGame(tbPlayerIds, nInstanceIndex);
		--Instance:TryEnterInstance(tbPlayerIds, nInstanceIndex);
	end
end

function PVEMatch:DoEnterMultiInstance_GS(tbPlayerIds, nInstanceMapId)
	MultiInstance:DoEnterGame(tbPlayerIds, nInstanceMapId);
end

function PVEMatch:DoEnterMultiInstanceRemote_GS(nPlayerId, nInstanceMapId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	pPlayer.BattleZoneTransfer();
end

function PVEMatch:DoBroadcastOverallTeamMsg_GS(nMapId, dwLeaderCombinedIdx, nRequireFightscore, szCreaterName)
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		local nMinLevel = MultiInstance:GetMultiMinLevel(nMapId);
		--if pPlayer.GetLevel() >= nMinLevel then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:BroadcastOverteamMsg", nMapId, dwLeaderCombinedIdx, nRequireFightscore, szCreaterName});
		--end
	end
end

-- listen events
-------------------------------------------------------------------------------
function PVEMatch:OnLogin(nPlayerId)
	KGameBase.CenterExecute({"PVEMatch:PlayerLogin_GC", nPlayerId});
	
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local nState = pPlayer.GetMissionState();
	if nState == PVEMatch.STATE_STAGE_WATING then
		pPlayer.SysMsg(Lang.gcscript.str20[Lang.Idx]);
	elseif nState == PVEMatch.STATE_INSTANCE_WATING then
		pPlayer.SysMsg(Lang.gcscript.str21[Lang.Idx]);
	end
	pPlayer.SetMissionState(PVEMatch.STATE_NONE);
end

function PVEMatch:OnLogout(nPlayerId)
	KGameBase.CenterExecute({"PVEMatch:PlayerLogout_GC", nPlayerId});
end

function PVEMatch:OnEnterScene(nPlayerId)
	local pPlayer = KGameBase.GetCharacterById(nPlayerId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "city" then
		KGameBase.CenterExecute({"PVEMatch:PlayerLeaveCity_GC", nPlayerId});
	end
end

function PVEMatch:OnLeaveTeam(nPlayerId, nTeamId)
	print("PVEMatch:OnLeaveTeam");
	
	KGameBase.CenterExecute({"PVEMatch:PlayerLeaveTeam_GC", nPlayerId});
end

function PVEMatch:OnDisbandTeam(nPlayerId)
	print("PVEMatch:OnDisbandTeam");
	KGameBase.CenterExecute({"PVEMatch:PlayerLeaveTeam_GC", nPlayerId});
end

function PVEMatch:OnCaptainChange(nPlayerId)
	print("PVEMatch:OnCaptainChange");
	KGameBase.CenterExecute({"PVEMatch:PlayerLeaveTeam_GC", nPlayerId});
end

function PVEMatch:OnEnterTeam(nPlayerId)
	print("PVEMatch:OnEnterTeam");
	KGameBase.CenterExecute({"PVEMatch:PlayerEnterTeam_GC", nPlayerId});
end

function PVEMatch:OnEnterMapCopy(nPlayerId)
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, PVEMatch.OnLogin, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, PVEMatch.OnLogout, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_ADD_TEAM, PVEMatch.OnEnterTeam, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_LEAVE_TEAM, PVEMatch.OnLeaveTeam, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_DISBAND_TEAM, PVEMatch.OnDisbandTeam, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CAPTAIN_CHANGE, PVEMatch.OnCaptainChange, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_EXPSCENE_BEGIN, PVEMatch.OnLogout, PVEMatch);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, PVEMatch.OnEnterScene, PVEMatch);
