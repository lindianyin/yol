-- 
-- 闯关基础房间逻辑
--

-- 应用了家族关卡的基础锁逻辑
Include("script/mission/baselock.lua")
Include("script/mission/level/xoyogame_def.lua");
local tbEventLock = Lib:NewClass(Lock.tbBaseLock);

XoyoGame.BaseRoom = {}
local BaseRoom = XoyoGame.BaseRoom;

BaseRoom.EVENT_PROC  =
{
	[XoyoGame.ADD_NPC]		= "AddNpc",
	[XoyoGame.DEL_NPC]		= "DelNpc",
	[XoyoGame.CHANGE_TRAP]	= "ChangeTrap",
	[XoyoGame.DO_SCRIPT]	= "ExcuteScript",
	[XoyoGame.CHANGE_FIGHT]	= "ChangeFight",
	[XoyoGame.TARGET_INFO]	= "SetTargetInfo",
	[XoyoGame.TIME_INFO]	= "SetTimeInfo",
	[XoyoGame.CLOSE_INFO]	= "CloseInfo",
	[XoyoGame.MOVIE_DIALOG] = "MovieDialog",
	[XoyoGame.BLACK_MSG]	= "BlackMsg",
	[XoyoGame.CHANGE_NPC_AI]= "ChangeNpcAi",
	[XoyoGame.ADD_GOUHUO]	= "AddGouHuo",
	[XoyoGame.SEND_CHAT]	= "SendNpcChat",
	[XoyoGame.ADD_TITLE]	= "AddTeamTitle",
	[XoyoGame.TRANSFORM_CHILD] = "TransformChild",
	[XoyoGame.TRANSFORM_CHILD_2] = "TransformChild2",
	[XoyoGame.SHOW_NAME_AND_LIFE] = "ShowNameAndLife",
	[XoyoGame.NPC_CAN_TALK] = "NpcCanTalk",
	[XoyoGame.CHANGE_CAMP] = "ChangeCamp",
	[XoyoGame.SET_SKILL] = "SetSkill",
	[XoyoGame.DISABLE_SWITCH_SKILL] = "DisableSwitchSkill",
	[XoyoGame.FINISH_ACHIEVE]	= "FinishAchieve",
	[XoyoGame.NPC_BLOOD_PERCENT] = "RegNpcHurtCallback",
	[XoyoGame.NEW_WORLD_PLAYER] = "TeleportGroup",
	[XoyoGame.NPC_CAST_SKILL] = "NpcCastSkill",
	[XoyoGame.NPC_SET_LIFE] = "SetNpcLife",
	[XoyoGame.PLAYER_ADD_EFFECT] = "PlayerAddEffect",
	[XoyoGame.PLAYER_REMOVE_EFFECT] = "PlayerRemoveEffect",
	[XoyoGame.NPC_ADD_BUFF_TO_PLAYER] = "NpcAddBuffToPlayer",
	[XoyoGame.NPC_BUBBLE] 	= "NpcBubble",
	[XoyoGame.SET_NPC_NICKNAME] = "SetNpcNickName",
	[XoyoGame.CLEAR_BUFF] = "ClearPlayerBuff",
	[XoyoGame.SYS_MSG] = "SendSysMsg",
	[XoyoGame.REVIVE_INFO] = "ReviveInfo",
};

BaseRoom.ZhenYuanExpAward = 
{
	[3]  = 10,
	[4]  = 5,
	[5]  = 5,
}

function tbEventLock:InitEventLock(tbRoom, nTime, nMultiNum, tbStartEvent, tbUnLockEvent)
	self:InitLock(nTime, nMultiNum);
	self.tbRoom 		= tbRoom;
	self.tbUnLockEvent 	= tbUnLockEvent;
	self.tbStartEvent 	= tbStartEvent;
end

function tbEventLock:OnUnLock()
	if self.tbRoom and self.tbUnLockEvent then
		for i = 1, #self.tbUnLockEvent do
			self.tbRoom:OnEvent(unpack(self.tbUnLockEvent[i]));
		end
	end
	local tbRoom = self.tbRoom;
	if tbRoom then
		if (tbRoom.tbTeam[1].bIsWiner == 1) then
			tbRoom:RoomLevelUp();
		elseif (tbRoom.tbTeam[1].bIsWiner == 0) then
			tbRoom:RoomFailed();
		end
	end
end

function tbEventLock:OnStartLock()
	if self.tbRoom and self.tbStartEvent then
		for i = 1, #self.tbStartEvent do
			self.tbRoom:OnEvent(unpack(self.tbStartEvent[i]));
		end
	end
end

function BaseRoom:GetMonsterLevel(nLevel)
	--[[if nLevel < 50 then
		return nLevel;
	end]]
	if self.nDifficuty == 1 then
		return tonumber(XoyoGame.tbNpcLevel[nLevel].nAdd1);
	elseif self.nDifficuty == 3 then
		return tonumber(XoyoGame.tbNpcLevel[nLevel].nAdd2);
	elseif self.nDifficuty == 5 then
		return tonumber(XoyoGame.tbNpcLevel[nLevel].nAdd3);
	end
end

function BaseRoom:ChangeNpcByDifficuty()
	if self.nDifficuty == 5 then
		return 1;
	else
		return 0;
	end
end

function BaseRoom:RoomLevelUp()
	if self.nTimerId then
		local fnExcute = function (pPlayer)
			local timeInfo = Lang.mission.str792[Lang.Idx];
			pPlayer.SysMsg(string.format(timeInfo, XoyoGame.DELAY_ENDTIME));
			pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", timeInfo, XoyoGame.DELAY_ENDTIME * Env.GAME_FPS});
		end
		local tbGame = self.tbGame;
		self:GroupPlayerExcute(fnExcute, -1);
		Timer:Close(self.nTimerId);
		self.nTimerId = nil;

		local nRoomType = self.tbSetting.nType;
		if (nRoomType ~= nil and nRoomType == 1) then
			Timer:Register(XoyoGame.DELAY_ENDTIME * Env.GAME_FPS, tbGame.EndRoomDTTTime, tbGame, self.nRoomId);
		elseif (nRoomType == nil or nRoomType == 0) then
			Timer:Register(XoyoGame.DELAY_ENDTIME * Env.GAME_FPS, tbGame.EndRoomTime, tbGame, self.nRoomId);
		end
	end
end

function BaseRoom:RoomFailed()
	if self.nTimerId then
		local fnExcute = function (pPlayer)
			local timeInfo = Lang.mission.str793[Lang.Idx];
			pPlayer.SysMsg(string.format(timeInfo, XoyoGame.DELAY_ENDTIME));
			pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", timeInfo, XoyoGame.DELAY_ENDTIME * Env.GAME_FPS});
		end
		local tbGame = self.tbGame;
		self:GroupPlayerExcute(fnExcute, -1);
		Timer:Close(self.nTimerId);
		self.nTimerId = nil;

		local nRoomType = self.tbSetting.nType;
		if (nRoomType ~= nil and nRoomType == 1) then
			Timer:Register(XoyoGame.DELAY_ENDTIME * Env.GAME_FPS, tbGame.EndRoomDTTTime, tbGame, self.nRoomId);
		elseif (nRoomType == nil or nRoomType == 0) then
			Timer:Register(XoyoGame.DELAY_ENDTIME * Env.GAME_FPS, tbGame.EndRoomTime, tbGame, self.nRoomId);
		end
	end
end

function BaseRoom:ChangeCamp(nGroup, nCamp)
	self.USE_CHANGE_CAMP = 1;
	local f = function(pPlayer, nGroupId)
		self.tbGroupOriginalCamp[nGroupId] = pPlayer.GetCurCamp();
		pPlayer.SetCurCamp(nCamp);
	end
	self:GroupPlayerExcute(f, nGroup);
end

function BaseRoom: TestNpc()
	local nMapId, nX, nY = me.GetWorldPos();
	local pMeNpc = me.GetNpc();
	--for i = 1, 3 do
		local pProtectNpc = KNpc.Add2(2183, 80, -1, nMapId, nX, nY, 0, 0, 1);
		pProtectNpc.SetNpcAI(9, 0, 1, 1, 25, 25, 25, 1, pMeNpc.nIndex, pMeNpc.dwId, 0);
	--end
end

function BaseRoom:GetRoomLevel(nRoomId)
	local nLevel = XoyoGame.RoomSetting.tbRoom[nRoomId].nRoomLevel;
	return nLevel;
end

function BaseRoom:InitRoom(tbGame, tbRoomSetting, nMapId, nRoomId, tbAward)
	self.tbGame = tbGame;
	self.tbSetting = tbRoomSetting;
	self.nMapId = nMapId;
	self.nRoomId = nRoomId;
	self.tbLock = {};
	self.tbNpcGroup = {}; -- [szGroupName]-->{dwId1, dwId2, ...} 
	self.tbNpcHurtCallback = {};
	
	-- tbTeam[1] 对应 tbPlayerGroup[1], tbTeam[2] 对应 tbPlayerGroup[2], 如此类推
	self.tbTeam = {};    -- {{nTeamId1, nPlayerCount, ...}, {nTeamId2, nPlayerCount}, ...} 这是一个数组
	self.tbPlayerGroup = {}; -- {{nPlayerId, nPlayerId...}, {nPlayerId, nPlayerId, ...}, ...} 玩家分组，按1组2组3组这样索引, 不直接用队伍机制
	self.tbGroupOriginalCamp = {}; -- 分组原来的阵型
	
	self.tbPlayer = {}; -- [nPlayerId] -->{nTeam, ...} nTeam是self.tbTeam的索引
	self.tbAddNpcTimer = {};
	self.tbTeamId2Group = {};
	self.tbAward = tbAward;
	self.fnExc = nil;
	
	if (self.tbSetting.nType == nil or self.tbSetting.nType == 0) then
		local nLevel =  self:GetRoomLevel(nRoomId);
		self.nEndTime = KGameBase.GetTime() + XoyoGame.ROOM_TIME[nLevel];
	elseif (self.tbSetting.nType == 1) then
		self.nEndTime = KGameBase.GetTime() + XoyoGame.ROOM_DTT_TIME;
	end
	
	if not tbRoomSetting.LOCK then
		return ;
	end
	for i, tbLockSetting in pairs(tbRoomSetting.LOCK) do
		self.tbLock[i] = Lib:NewClass(tbEventLock);
		self.tbLock[i].nLockId = i;
		self.tbLock[i]:InitEventLock(self, tbLockSetting.nTime * Env.GAME_FPS, tbLockSetting.nNum, tbLockSetting.tbStartEvent, tbLockSetting.tbUnLockEvent);
	end
	for i, tbLockSetting in ipairs(tbRoomSetting.LOCK) do -- 保证解锁顺序
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
	
	if self.OnInitRoom then
		self:OnInitRoom();
	end
end

-- 添加NPC
-- varIndex: nTemplateId or {id1,id2,id3...}, 可以是一个或者一组模板id
-- nNum: 刷几个
-- nLock
-- szGroup: 新刷出来的npc所属的组的名字
-- szPointName: 刷怪组别
-- nTimes, nFrequency, szTimerName: 循环刷怪用，参数为：次数，隔多久刷一次，timer名字
function BaseRoom:AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName)
	if (nTimes or nFrequency or szTimerName) and not (nTimes > 1 and nFrequency > 0 and szTimerName) then
		assert(false);
	end
	
	if type(varIndex) == "number" then
		varIndex = {varIndex};
	end
	for _, id in ipairs(varIndex) do
		local tbNpcInfo = self.tbSetting.NPC[id];
		assert(tbNpcInfo);
	end

	if nFrequency and not self.tbAddNpcTimer[szTimerName] then
		self.tbAddNpcTimer[szTimerName] = {};
		self.tbAddNpcTimer[szTimerName].tbNpcId = {};
	end

	local ret = self:__AddNpc(varIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName);

	if ret > 0 then
		CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_SELFHURT, self.OnNpcHurt, self, szGroup);
	end
end

-- 计算玩家等级平均值
function BaseRoom:GetAverageLevel()
	local nLevel;
	-- 计算房间内玩家平均等级
	assert(self.tbTeam)
	local nTotalLevel = 0;
	local nTotalPlayer = 0;
	for _, tbTeamInfo in pairs(self.tbTeam) do
		local tbTeamer, nCount = KTeam.GetTeamMemberList(tbTeamInfo.nTeamId);
		for i=1, nCount do
			local pPlayer = KGameBase.GetPlayerById(tbTeamer[i]);
			if pPlayer then
				nTotalLevel = nTotalLevel + pPlayer.GetLevel();
				nTotalPlayer = nTotalPlayer + 1
			end
		end
	end
	if (nTotalPlayer ~= 0) then
		nLevel = math.ceil(nTotalLevel / nTotalPlayer);
	else
		nLevel = 50;
	end
	return nLevel;
end

-- 把npc刷到不同位置上
-- nNum: 数量
-- tbIndex: npc索引表
function BaseRoom:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)
	local pNpc;
	local tbNpcId = {};
	local nIndex;
	if #tbIndex == 1 then
		nIndex = tbIndex[1];
	end
	
	local tbNpcInfo;
	if nIndex then
	    tbNpcInfo = self.tbSetting.NPC[nIndex];
	else
	    tbNpcInfo = self.tbSetting.NPC[tbIndex[MathRandom(#tbIndex)]];
	end
	-- 用九天的刷怪方式
	local tbNpcT = Scene.tbNpcCfg[self.nMapId][tbNpcInfo.nTemplate];
	local tbNpc = {};
	tbNpc.tbPosList = tbNpcT.tbPosList;
	tbNpc.nTemplateId = tbNpcT.nTemplateId;
	tbNpc.nRegenerationTime = tbNpcT.nRegenerationTime;
	tbNpc.nNormalAI = tbNpcT.nNormalAI;
	tbNpc.nCount = nNum;
	
	local tbRetNpcs = Scene:_CallOneNpc(tbNpc, self.nSceneId, self.nMapId, nil, szPointName);
	for _, pNpc in ipairs(tbRetNpcs) do
	    KCharacter.ResetNpcIntensity(pNpc, nPropId, nLevel);
	    self:AddNpcInLock(pNpc, nLock);
		self:AddNpcInGroup(pNpc, szGroup);
		table.insert(tbNpcId, pNpc.dwId);
		local tbTmp = pNpc.GetTempTable("XoyoGame")
		tbTmp.tbRoom = self;
	end
	--[[for i = 1, nNum do --zzx
		local nRand;
		local x,y;
		local tbNpcInfo;
		if nIndex then
			tbNpcInfo = self.tbSetting.NPC[nIndex];
		else
			tbNpcInfo = self.tbSetting.NPC[tbIndex[MathRandom(#tbIndex);
]]		--[[end
		if point_ge_num then -- 点多怪少，要保证一个点只刷一个怪
			nRand = MathRandom(#tbPoint - i + 1) - 1;
			if tbPoint[i + nRand] then
				tbPoint[i], tbPoint[i + nRand] = tbPoint[i + nRand], tbPoint[i];
			end
			x, y = tbPoint[i][1], tbPoint[i][2];
		else
			nRand = MathRandom(#tbPoint);
			x, y = tbPoint[nRand][1], tbPoint[nRand][2];
		end
		if tbNpcInfo.nTemplate2 and self:ChangeNpcByDifficuty() == 1 then
			pNpc = KNpc.Add2(tbNpcInfo.nTemplate2, nLevel, tbNpcInfo.nSeries, self.nMapId, x,y,0,0,0,1)
		else
			pNpc = KNpc.Add2(tbNpcInfo.nTemplate, nLevel, tbNpcInfo.nSeries, self.nMapId, x,y,0,0,0,1)
		end
		if pNpc then
			self:AddNpcInLock(pNpc, nLock);
			self:AddNpcInGroup(pNpc, szGroup);
			table.insert(tbNpcId, pNpc.dwId);
			local tbTmp = pNpc.GetTempTable("XoyoGame")
			tbTmp.tbRoom = self;
		else
			print("Add Npc Failed", tbNpcInfo.nTemplate, nLevel, tbNpcInfo.nSeries, self.nMapId, x,y,0,0,0,1)
		end
	end]]
	return tbNpcId;
end

function BaseRoom:__AddNpc(tbIndex, nNum, nLock, szGroup, szPointName, nTimes, nFrequency, szTimerName)
	if not self.tbSetting or not self.tbSetting.NPC then
		return 0;
	end
	
	local nLevel = -1;	-- 默认值
	local tbNpcInfo = self.tbSetting.NPC[tbIndex[1]];
	assert(tbNpcInfo.nLevel ~= nil);
	local nPropId = KCharacter.GetNpcPropId(tbNpcInfo.nTemplate);

	if type(tbNpcInfo.nLevel) == "table" then
		assert(self.nDifficuty >= 1 and self.nDifficuty <= 3);
		nPropId = tbNpcInfo.nLevel[self.nDifficuty];
		nLevel = self:GetAverageLevel();
		nLevel = KCharacter.GetNpcMatchLevel(nPropId, nLevel);
	elseif tbNpcInfo.nLevel <= 0 then
		nPropId = KCharacter.GetNpcPropId(tbNpcInfo.nTemplate);
		nLevel = self:GetAverageLevel();
		nLevel = KCharacter.GetNpcMatchLevel(nPropId, tbNpcInfo.nLevel);
	else
		nLevel = tbNpcInfo.nLevel;
	end
		
	local tbNpcId = self:__AllocNpc(nNum, tbIndex, nLock, szGroup, nPropId, nLevel, szPointName)
	
	if nFrequency then
		for _, id in ipairs(self.tbAddNpcTimer[szTimerName].tbNpcId) do -- 删除上一轮的npc
			local pNpc = KGameBase.GetNpcById(id);
			if pNpc then
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
	
	return 1;
end

-- 把NPC加到锁里
function BaseRoom:AddNpcInLock(pNpc, nLock)
	if not nLock or nLock <= 0 then
		return 0;
	end
	local tbTmp = pNpc.GetTempTable("XoyoGame")
	tbTmp.nLock = nLock
end

-- 把NPC加到组里
function BaseRoom:AddNpcInGroup(pNpc, szGroup)
	if not self.tbNpcGroup[szGroup] then
		self.tbNpcGroup[szGroup] = {};
	end
	if pNpc then
		table.insert(self.tbNpcGroup[szGroup], pNpc.dwId);
	end
end

-- 删除特定组的NPC
function BaseRoom:DelNpc(szGroup)
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

function BaseRoom:RegNpcHurtCallbackEx(szGroup, nPercent, EventType, ...)
	assert(nPercent > 0 and nPercent < 100);
	local callback = self.tbNpcHurtCallback;
	if not callback[szGroup] then
		callback[szGroup] = {};
	end
	if not callback[szGroup][EventType] then
		callback[szGroup][EventType] = {};
	end
	if not callback[szGroup][EventType][nPercent] then
		callback[szGroup][EventType][nPercent] = {};
	end
	table.insert(callback[szGroup][EventType][nPercent], arg);
end

function BaseRoom:RegNpcHurtCallback(szGroup, ...)
	for _, tbArg in pairs(arg) do
		if(type(tbArg) == "table") then
			self:RegNpcHurtCallbackEx(szGroup, unpack(tbArg));
		end
	end
end

function BaseRoom:OnNpcHurt(szGroup)
	if not self.tbNpcGroup[szGroup] or not self.tbNpcHurtCallback[szGroup] then
		return;
	end

	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
		
			local nPercent = pNpc.GetCurrentLife() / pNpc.GetMaxLife() * 100;

			for EventType, typeinfo in pairs(self.tbNpcHurtCallback[szGroup]) do
				for percent, arginfo in pairs(typeinfo) do
					for i = 1, #arginfo do
						local arg = arginfo[i];
						if nPercent < percent then
							self:OnEvent(EventType, unpack(arg));
							-- 调用一次后删除
							self.tbNpcHurtCallback[szGroup][EventType][percent][i] = nil;
							--[[
							if #self.tbNpcHurtCallback[szGroup][EventType][percent] < 1 then
								self.tbNpcHurtCallback[szGroup][EventType][percent] = nil
							end
							--]]
						end
					end
				end
			end
		
		end
	end

end

--[[
-- 改变Trap点传送位置
function BaseRoom:ChangeTrap(szClassName, tbPoint)
	if self.tbGame.tbTrap then
		self.tbGame.tbTrap[szClassName] = tbPoint;
	end
end
--]]

-- 改变Trap点传送位置
function BaseRoom:ChangeTrap(szClassName, ...)
	assert(Scene.tbTrapInfo[szClassName]);
	local tbTrapInfo = Scene.tbTrapInfo[szClassName];
	tbTrapInfo.Param1 = arg[1];
	tbTrapInfo.Param2 = arg[2];
	tbTrapInfo.Param3 = arg[3];
	tbTrapInfo.Param4 = arg[4];
end

-- 成就
function BaseRoom:FinishAchieve(nPlayerGroup, nAchieveId)
	local fnExcute = function (pPlayer)
		Achievement:FinishAchievement(pPlayer, nAchieveId);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 成就2
function BaseRoom:FinishAchieve2(tbTeam, nAchieveId)
	local fnExcute = function (pPlayer)
		Achievement:FinishAchievement(pPlayer, nAchieveId);
	end
	self:TeamPlayerExcute(fnExcute, tbTeam);
end

function BaseRoom:SetNpcNickName(szGroup,szName)
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId); 
		pNpc.szNickName = szName;
	end
end

function BaseRoom:ClearPlayerBuff(nPlayerGroup)
	local tbClearBuff =  XoyoGame.RoomSetting.tbRoom[self.nRoomId].tbClearBuff;
	if not tbClearBuff then
		return;
	end
	
	local fnExcute = function (pPlayer)
		for _, nBuffId in ipairs(tbClearBuff) do
			pPlayer.pBufferEffect.ClearBuff(nBuffId);
		end
	end
	
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end


-- 执行脚本 -- TODO
function BaseRoom:ExcuteScript(szCmd)
	XoyoGame.tbExcuteTable = self;
	local fnExc = loadstring("local self = XoyoGame.tbExcuteTable;"..szCmd);
	if fnExc then
		xpcall(fnExc, Lib.ShowStack);
	end
	XoyoGame.tbExcuteTable = nil;
end

-- 遍历房间所有玩家
function BaseRoom:GroupPlayerExcute(fnExcute, nPlayerGroup)
	local tbGroup;
	if nPlayerGroup and nPlayerGroup > 0 then
		if (not self.tbPlayerGroup) or (not self.tbPlayerGroup[nPlayerGroup]) then
			return 0;
		end
		tbGroup = {[nPlayerGroup] = self.tbPlayerGroup[nPlayerGroup]};
	else
		tbGroup = self.tbPlayerGroup;
	end
	for nGroupId, tbCurGroup in pairs(tbGroup) do
		for _, nPlayerId in pairs(tbCurGroup) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId);
			if pPlayer then
				fnExcute(pPlayer, nGroupId);
			end
		end
	end
end

-- 传送组id为nPlayerGroup的所有玩家到指定位置
function BaseRoom:TeleportGroup(nPlayerGroup, nX, nY, nZ)
	assert(self.tbPlayerGroup[nPlayerGroup]);
	for _, nPlayerId in pairs(self.tbPlayerGroup[nPlayerGroup]) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer and self.tbPlayer[nPlayerId] then
			pPlayer.TeleportTo(pPlayer.dwSceneId, nX, nY, nZ);
		end
	end
end

-- 遍历队伍所有完家执行
function BaseRoom:TeamPlayerExcute(fnExcute, tbTeam)
	if not tbTeam then
		return;
	end
	for _, nTeamId in ipairs(tbTeam) do
		local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
		for i = 1, nCount do
			local pPlayer = KGameBase.GetPlayerById(tbMember[i]);
			if pPlayer then
				fnExcute(pPlayer);
			end
		end
	end
end

-- 队伍横幅
function BaseRoom:TeamBlackMsg(tbTeam, szMsg)
	local fnExcute = function (pPlayer)
		Dialog:SendBlackBoardMsg(pPlayer, szMsg);
	end
	self:TeamPlayerExcute(fnExcute, tbTeam);
end

-- 队伍奖励，目前只有经验，钱、道具，每个队员都有，不存在分配规则
function BaseRoom:TeamAward(tbTeam, bIsWinner, nCloseLock)
	local fnExcute = function(pPlayer)
		if bIsWinner and bIsWinner == 1 then
			XoyoGame.XoyoChallenge:PassRoomForCard(pPlayer, self.nRoomId, nCloseLock); -- 给通关卡片
		end
	end
	self:TeamPlayerExcute(fnExcute, tbTeam);
end

function BaseRoom:AddFriendFavor(tbTeamId)
	if (not tbTeamId) then
		return ;
	end
	for _, nTeamId in ipairs(tbTeamId) do
		local tbMember, nCount = KTeam.GetTeamMemberList(nTeamId);
		if (nCount >= 2) then
			local pPlayer = KGameBase.GetPlayerById(tbMember[1]);
			if (pPlayer) then
				local tbTeamPlayer = pPlayer.GetTeamMemberList();
				self:AddFriendFavorInTeam(tbTeamPlayer);
			end	
		end
	end	
end

function BaseRoom:AddFriendFavorInTeam(tbTeamPlayer)
	if (not tbTeamPlayer) then
		return ;
	end
	for i = 1, #tbTeamPlayer do
		for j = i + 1, #tbTeamPlayer do
			if (tbTeamPlayer[i] and tbTeamPlayer[j] and 1 == tbTeamPlayer[i].IsFriendRelation(tbTeamPlayer[j].szName)) then
				Relation:AddFriendFavor(tbTeamPlayer[i].szName, tbTeamPlayer[j].szName, 16);
				tbTeamPlayer[i].Msg(string.format(Lang.mission.str794[Lang.Idx], tbTeamPlayer[j].szName, 16));
				tbTeamPlayer[j].Msg(string.format(Lang.mission.str794[Lang.Idx], tbTeamPlayer[i].szName, 16));
			end	
		end
	end		
end

-- 改变PK、战斗模式
function BaseRoom:ChangeFight(nPlayerGroup, nFightState, nPkModel, nCamp)
	local fnExcute = function (pPlayer)
		pPlayer.SetFightState(nFightState);
		pPlayer.nPkModel = nPkModel;
		if nCamp and nCamp > 0 and nCamp <=3 then
			pPlayer.SetCurCamp(nCamp);
		end
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 同步即时战报信息给所有玩家,   附加参数增添模式, 模式0默认， 模式1，附加nNum/nInitNum （杀敌数量）
function BaseRoom:SetTargetInfo(nPlayerGroup, szInfo, ...)
	local varLock; -- 读取varLock， 哪个锁
	if #arg == 0 then varLock = -1;   -- 没有参数
	else varLock = unpack(arg);	end -- 获取第一个
	
	self.szTargetInfo = szInfo;  -- 设置targetInfo
	if (varLock ~= -1 and self.tbLock[varLock]) then  -- 存在该锁
		local nCurrentNum = self.tbLock[varLock].nMultiNum; -- 当前杀敌数
		local nInitNum = self.tbLock[varLock].nInitMultiNum; -- 总杀敌数
		szInfo = string.format("%s %d/%d", self.szTargetInfo, nInitNum - nCurrentNum, nInitNum);
	end
	
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTargetInfo", szInfo});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	
	--成功LOG统计 比较差的实现
	if XoyoGame.LOG_ATTEND_OPEN == 1 and szInfo == Lang.mission.str795[Lang.Idx] and self.nTimerId then
		--查找所在Group
		local tbGroup;
		if nPlayerGroup and nPlayerGroup > 0 then
			if (not self.tbPlayerGroup) or (not self.tbPlayerGroup[nPlayerGroup]) then
				return;
			end
			tbGroup = {[nPlayerGroup] = self.tbPlayerGroup[nPlayerGroup]};
		else
			tbGroup = self.tbPlayerGroup;
		end		
		local nLastFrameTime = tonumber(Timer:GetRestTime(self.nTimerId));			
		local nLevel =  XoyoGame.RoomSetting.tbRoom[self.nRoomId].nRoomLevel;
		local nPlayTime = XoyoGame.ROOM_TIME[nLevel]  - math.floor(nLastFrameTime/Env.GAME_FPS);
		
		for nGroupId, tbCurGroup in pairs(tbGroup) do
			for _, nPlayerId in pairs(tbCurGroup) do
				local pPlayer = KGameBase.GetPlayerById(nPlayerId);
				if pPlayer and self.tbPlayer[nPlayerId] then
					--DataLog:WriteELog(pPlayer.szName, 1, 2, self.nRoomId, pPlayer.nTeamId, nPlayTime); -- zzx
				end
			end
		end
	end	
end

-- TODO: BUG: 向玩家同步关卡信息,  这里是否应该获取当前进行的TimeInfo发送？
function BaseRoom:SendRoomInfo(pPlayer)
	local nSec = self.nEndTime - KGameBase.GetTime();
	pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", self.szTimeInfo, nSec * Env.GAME_FPS});
	if self.szTargetInfo then
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTargetInfo", self.szTargetInfo});
	end
end

function BaseRoom:OnLogin(pPlayer)
	self:SendRoomInfo(pPlayer);
end

-- 同步时间信息
-- varLock: nLock  第几个锁的时间获取
function BaseRoom:SetTimeInfo(nPlayerGroup, szTimeInfo, ...)
	local varLock; -- 读取varLock， 哪个锁
	
	if #arg == 0 then varLock = -1; 
	else varLock = unpack(arg);	end
	
	if type(varLock) ~= "number" then
		print("[Lua:BaseRoom:SetTimeInfo]varLock非Number");
		assert(false);
	end

	-- 从当前room中获取锁
	if (not self.tbLock) then
		assert(false)
	end
	
	local nLastFrameTime; --保存锁的时间
	if (varLock ~= -1 and self.tbLock[varLock]) then  -- 存在该锁
		nLastFrameTime = tonumber(Timer:GetRestTime(self.tbLock[varLock].nTimerId));
		if nLastFrameTime == -1 then -- timer还没开启
			nLastFrameTime = self.tbSetting.LOCK[varLock].nTime*Env.GAME_FPS;
		end
	else
		nLastFrameTime = 0;  -- 时间已经到底了 
	end
	
	self.szTimeInfo = szTimeInfo
	local fnExcute = function (pPlayer)
		-- 本关剩余时间
		pPlayer.CallClientScript({"QClientScript.Mission:SetMissionTimeInfo", szTimeInfo, nLastFrameTime});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 关闭即时战报信息
function BaseRoom:CloseInfo(nPlayerGroup)
	local fnExcute = function (pPlayer)
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:BroadcastMissionComplete", "UI_TASKTIPS", "Begin", "CloseMission"});
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseRoom:PlayerAddEffect(nPlayerGroup, nBuffId, bBroadCast)
	assert(self.tbPlayerGroup[nPlayerGroup]);
	for _, nPlayerId in pairs(self.tbPlayerGroup[nPlayerGroup]) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer and self.tbPlayer[nPlayerId] then
			BufferApi.AddBufferToCharacter(nPlayerId, 0, nPlayerId, nBuffId, bBroadCast);
		end
	end
end

function BaseRoom:PlayerRemoveEffect(nPlayerGroup, nBuffId)
	assert(self.tbPlayerGroup[nPlayerGroup]);
	for _, nPlayerId in pairs(self.tbPlayerGroup[nPlayerGroup]) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer and self.tbPlayer[nPlayerId] then
			pPlayer.pBufferEffect.ClearBuff(nBuffId);
		end
	end
end

-- 电影模式对话
function BaseRoom:MovieDialog(nPlayerGroup, szMovie, delay)
	if delay and delay > 0 then
		Timer:Register(delay * Env.GAME_FPS, self.MovieDialog, self, nPlayerGroup, szMovie);
		return;
	end
	
	local fnExcute = function (pPlayer)
		if pPlayer.dwSceneTemplateId == XoyoGame.GAME_IN_POS.nMapId then
			return;
		end
		TaskAct:NormalTalk(pPlayer, szMovie);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
	return 0;
end

-- 黑条消息
function BaseRoom:BlackMsg(nPlayerGroup, szMsg)
	local fnExcute = function (pPlayer)
		Dialog:SendBlackBoardMsg(pPlayer, szMsg)
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 篝火
-- nMinute: ?
-- nBaseMultip: ?
-- 篝火npc的group
-- varPoint: {x,y} or 点的名字
function BaseRoom:AddGouHuo(nMinute, nBaseMultip, szGroup, varPoint)
	if not self.tbTeam[1] then
		return 0;
	end
	
	local pos;
	
	if type(varPoint) == "table" then
		pos = varPoint;
	else
		local szPointName = varPoint
		local tbPoint 	= XoyoGame.tbNpcPoint[szPointName];
		if not tbPoint then
			return 0;
		end
		local nRand = MathRandom(#tbPoint)
		pos = tbPoint[nRand];
	end
	
	local nTeamId	= self.tbTeam[1].nTeamId;
	local tbNpc		= Npc:GetClass("gouhuonpc");
	local pNpc		= KNpc.Add2(tbNpc.nNpcId, 1, -1, self.nMapId, pos[1], pos[2]);		-- 获得篝火Npc
	if pNpc then
		tbNpc:InitGouHuo(pNpc.dwId, 1,	nMinute * 60, 5, 40, nBaseMultip, 1)
		tbNpc:SetTeamId(pNpc.dwId, nTeamId);
		tbNpc:StartNpcTimer(pNpc.dwId)
		self:AddNpcInGroup(pNpc, szGroup);
		KTeam.Msg2Team(nTeamId, Lang.mission.str796[Lang.Idx]);
	else
		print("XoyoGame", "AddGouHuo Failed");
	end
end

-- 公告
function BaseRoom:SendPlayerMsg(nPlayerGroup, szMsg)
	local f = function(pPlayer)
		pPlayer.Msg(szMsg);
	end
	
	self:GroupPlayerExcute(f, nPlayerGroup);
end

function BaseRoom:AddTeamTitle(nPlayerGroup, nGenre, nDetail, nLevel, nParam)
	local fnExcute = function (pPlayer)
		pPlayer.AddTitle(nGenre, nDetail, nLevel, nParam);
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

-- 变身做小孩
function BaseRoom:TransformChild(__pPlayer) -- 只用于双队房
	self.USE_TRANSFORM_CHILD = 1;
	local fnExcute = function (pPlayer, nGroupId)
		local nSkillLevel; -- 1-4级依次变成3605-3608号npc
		-- 变身
		if (pPlayer.nSex == 0) then
			if (nGroupId == 1) then
				nSkillLevel = 5;
			else
				nSkillLevel = 7;
			end
		else
			if (nGroupId == 1) then
				nSkillLevel = 6;
			else
				nSkillLevel = 8;
			end
		end
		local mapId, x, y = pPlayer.GetWorldPos();
		pPlayer.CastSkill(1326, nSkillLevel, x, y);
		
		--pPlayer.LevelUpFightSkill(1, 1451);
		FightSkill:SaveRightSkillEx(pPlayer, 1451);
	end
	
	if __pPlayer then
		local nGroupId = self.tbTeamId2Group[__pPlayer.nTeamId];
		fnExcute(__pPlayer, nGroupId);
	else
		self:GroupPlayerExcute(fnExcute, 1);
		self:GroupPlayerExcute(fnExcute, 2);
	end
end

function BaseRoom:NpcCastSkill(szGroup, nSkillId, nX, nY, nZ)
	if not self.tbNpcGroup[szGroup] then
		return;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
			if nX ~= nil then
				pNpc.pFightSkill.CastSkill(nSkillId, 0, nX, nY, nZ);
			else
				pNpc.pFightSkill.CastSkill(nSkillId, 0);
			end
		end
	end
end

function BaseRoom:NpcAddBuffToPlayer(szGroup, nBuffId, nPlayerGroup, bBroadCast)
	if not self.tbNpcGroup[szGroup] then
		return;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
			if nPlayerGroup == 0 then
				local mPos = pNpc.GetPosition();
				-- TODO 现在没有绑定技能，以后可能要加上技能参数
				BufferApi.AddGroundBuffer(nBuffId, 0, 0, pNpc.dwSceneId, mPos.nX, mPos.nY, mPos.nZ, bBroadCast);
			else
				assert(self.tbPlayerGroup[nPlayerGroup]);
				for _, nPlayerId in pairs(self.tbPlayerGroup[nPlayerGroup]) do
					local pPlayer = KGameBase.GetPlayerById(nPlayerId);
					if pPlayer and self.tbPlayer[nPlayerId] then
						BufferApi.AddBufferToCharacter(pPlayer.dwId, 0, nPlayerId, nBuffId, bBroadCast);
					end
				end
			end
		end
	end
end

function BaseRoom:SetNpcLife(szGroup, nPercent)
	if not self.tbNpcGroup[szGroup] then
		return;
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
			local nCurrentLife = pNpc.GetMaxLife() * nPercent / 100;
			pNpc.SetCurrentLife(nCurrentLife);
		end
	end
end

-- 变牧童
function BaseRoom:TransformChild2()
	self.USE_TRANSFORM_CHILD = 1;
	local fnExcute = function (pPlayer, nGroupId)
		local mapId, x, y = pPlayer.GetWorldPos();
		pPlayer.CastSkill(1326, 9, x, y);
	end
	self:GroupPlayerExcute(fnExcute, 1);
end

function BaseRoom:SetSkill(nGroupId, nSkillId, szLeftRight)
	local fnExcute = function(pPlayer, nGroupId)
		FightSkill:SaveRightSkillEx(pPlayer, nSkillId);
		FightSkill:SaveLeftSkillEx(pPlayer, nSkillId);
	end
	self:GroupPlayerExcute(fnExcute, nGroupId);
end

function BaseRoom:DisableSwitchSkill(var, nDisable)
	self.USE_DISABLE_SWITCH_SKILL = 1;
	local szVal;
	if nDisable == 1 then
		szVal = "1";
	else
		szVal = "nil";
	end
	
	local sz = string.format([[
		UiManager:CloseWindow(Ui.UI_SKILLTREE, "LEFT");
		UiManager:CloseWindow(Ui.UI_SKILLTREE, "RIGHT");
		Ui(Ui.UI_SKILLBAR)._disable_switch_skill = %s;
	]], szVal);
	local fnExcute = function(pPlayer)
		pPlayer.CallClientScript({"GM:DoCommand",sz});
	end
	
	if type(var) == "number" then
		self:GroupPlayerExcute(fnExcute, var);
	else
		fnExcute(var);
	end
end

function BaseRoom:ShowNameAndLife(nShow)
	assert(nShow == 0 or nShow == 1)
	local sz = string.format(
		[[
		UiManager:CloseWindow(Ui.UI_SYSTEM);
		me.nShowLife =%d;
		me.nShowName =%d;
		Ui(Ui.UI_SYSTEM)._no_name_or_life=%d;
		]], nShow, nShow, nShow);
		
	local fnExcute = function(pPlayer)
		pPlayer.CallClientScript({"GM:DoCommand",sz});
	end
	self:GroupPlayerExcute(fnExcute);
end

function BaseRoom:SetNpcMove(pNpc, szRoad, nLockId, nAttact, bRetort, bArriveDel)
	if not XoyoGame.tbRoad or not XoyoGame.tbRoad[szRoad] then
		return 0;
	end
	pNpc.AI_ClearPath();
	for _,Pos in pairs(XoyoGame.tbRoad[szRoad]) do
		if (Pos[1] and Pos[2]) then
			pNpc.AI_AddMovePos(Pos[1]*32, Pos[2]*32);
		end
	end
	pNpc.SetNpcAI(9, nAttact or 0, bRetort or 1, -1, 25, 25, 25, 0, 0, 0, 0);
	pNpc.SetActiveForever(1);
	pNpc.GetTempTable("Npc").tbOnArrive = {self.OnArrive, self, nLockId, bArriveDel};
end

function BaseRoom:SetNpcReMove(pNpc, szRoad, nAttact, bRetort, nTimes)
	if not XoyoGame.tbRoad or not XoyoGame.tbRoad[szRoad] then
		return 0;
	end
	pNpc.AI_ClearPath();
	for _,Pos in pairs(XoyoGame.tbRoad[szRoad]) do
		if (Pos[1] and Pos[2]) then
			pNpc.AI_AddMovePos(Pos[1]*32, Pos[2]*32);
		end
	end
	pNpc.SetNpcAI(9, nAttact or 0, bRetort or 1, -1, 25, 25, 25, 0, 0, 0, 0);
	pNpc.SetActiveForever(1);
	pNpc.GetTempTable("Npc").tbOnArrive = {self.OnReMoveArrive, self, szRoad, nAttact, bRetort, nTimes};
end

function BaseRoom:SetNpcAttack(pNpc, szNpc, nCamp, bRetort)
	pNpc.SetCurCamp(nCamp)
	if szNpc and self.tbNpcGroup[szNpc] then
		if #self.tbNpcGroup[szNpc] >= 1 then
			local nRandom = MathRandom(#self.tbNpcGroup[szNpc]);
			local pTagetNpc = KNpc.GetById(self.tbNpcGroup[szNpc][nRandom])
			if pTagetNpc then
				pNpc.SetNpcAI(9, 100, bRetort or 1, -1, 25, 25, 25, 1, 0, self.tbNpcGroup[szNpc][nRandom], 0);
			end
		end
	end
end

function BaseRoom:OnArrive(nLockId, bArriveDel)
	-- 要先删除NPC~
	if bArriveDel == 1 then
		him.Delete();
	end
	if not nLockId then
		return 0;
	end
	
	if not self.tbLock[nLockId] then
		return 0;
	end
	self.tbLock[nLockId]:UnLockMulti();
end

function BaseRoom:OnReMoveArrive(szRoad, nAttact, bRetort, nTimes)
	if not XoyoGame.tbRoad or not XoyoGame.tbRoad[szRoad] then
		return 0;
	end
	him.AI_ClearPath();
	for _,Pos in pairs(XoyoGame.tbRoad[szRoad]) do
		if (Pos[1] and Pos[2]) then
			him.AI_AddMovePos(Pos[1]*32, Pos[2]*32);
		end
	end
	him.SetNpcAI(9, nAttact or 0, bRetort or 1, -1, 25, 25, 25, 0, 0, 0, 0);
end

BaseRoom.AI_MODE_PROC = 
{
	[XoyoGame.AI_MOVE] 			= "SetNpcMove";
	[XoyoGame.AI_RECYLE_MOVE]	= "SetNpcReMove";
	[XoyoGame.AI_ATTACK]		= "SetNpcAttack";
}

function BaseRoom:OnEvent(nEventType, ...)
	if self.EVENT_PROC[nEventType] and self[self.EVENT_PROC[nEventType]] then
		self[self.EVENT_PROC[nEventType]](self, ...);
	else
		print("Undefind EventType ", nEventType, ...);
	end
end

function BaseRoom:ChangeNpcAi(szNpcGroup, nAiMode, ...)
	if not self.tbNpcGroup[szNpcGroup] then
		print("NpcGroup is not In", szNpcGroup);
		return 0
	end
	if self.AI_MODE_PROC[nAiMode] and self[self.AI_MODE_PROC[nAiMode]] then
		for i, nNpcId in pairs(self.tbNpcGroup[szNpcGroup]) do
			local pNpc = KNpc.GetById(nNpcId);
			if pNpc then
				self[self.AI_MODE_PROC[nAiMode]](self, pNpc, ...);
			end
		end
	else
		print("Undefin AiModeType ", nAiMode, ...);
	end
end


-- 加入房间 nTeamId1, nTeamId2, ....
function BaseRoom:JoinRoom(...)
	if self.tbSetting.fnPlayerGroup then
		self.tbSetting.fnPlayerGroup(self, unpack(arg));
	else 
		self:DefaultGroup(unpack(arg));
	end
	for i = 1, #arg do
		local tbMember, nNum = KTeam.GetTeamMemberList(arg[i])
		table.insert(self.tbTeam, { nTeamId = arg[i], nPlayerCount = 0 });
		self.tbGame.tbTeam[arg[i]].nRoomId = nRoomId;
		for j = 1, nNum do
			local pPlayer = KGameBase.GetPlayerById(tbMember[j]);
			-- TODO   
			if (pPlayer) and self.tbGame == XoyoGame:GetPlayerGame(pPlayer.dwId)
				and (not self.tbPlayer[tbMember[j]])
			then
				self.tbPlayer[tbMember[j]] = {};
				self.tbPlayer[tbMember[j]].nTeam = #self.tbTeam;
				self.tbTeam[#self.tbTeam].nPlayerCount = self.tbTeam[#self.tbTeam].nPlayerCount + 1;
				-- 设定玩家在本房间内
				self.tbGame:SetPlayerInRoom(tbMember[j], self.nRoomId);
				
				-- 事件的注册全部先关闭(zzx)
				--Setting:SetGlobalObj(pPlayer);
				--if self.tbSetting.fnDeath then
					--self.tbPlayer[tbMember[j]].nOnDeathRegId = PlayerEvent:Register("OnDeath", self.tbSetting.fnDeath, self);
				--else
					--self.tbPlayer[tbMember[j]].nOnDeathRegId = PlayerEvent:Register("OnDeath", self.DefaultDeath, self);
				--end
				--Setting:RestoreGlobalObj();
			end
		end		
	end
end

-- 默认死亡函数
function BaseRoom:DefaultDeath(pKiller)
	-- 自动重生
	me.ReviveImmediately(0);
	-- 战斗状态
	me.SetFightState(0);
	-- PK状态
	me.nPkModel = Player.emKPK_STATE_PRACTISE;
	
	-- 剔除房间逻辑
	self:PlayerLeaveRoom(me.dwId);
	
	-- 提示信息
	if self.GetDeathMsg then
		Dialog:SendBlackBoardMsg(me, self:GetDeathMsg(me));
	else
		Dialog:SendBlackBoardMsg(me, Lang.mission.str797[Lang.Idx]);
	end
	
	-- 提示时间最后做~万一定时器被以外关了也不至于逻辑错误
	if self.nTimerId then
		local nLastFrameTime = tonumber(Timer:GetRestTime(self.nTimerId));
		Dialog:SetBattleTimer(me, Lang.mission.str798[Lang.Idx], nLastFrameTime);
		Dialog:SendBattleMsg(me, Lang.mission.str799[Lang.Idx]);
	end
end

function BaseRoom:SendPlayer2RestRoom(pPlayer)
	-- 战斗状态
	pPlayer.SetFightState(0);
	-- PK状态
	pPlayer.nPkModel = Player.emKPK_STATE_PRACTISE;
	-- 剔除房间逻辑
	self:PlayerLeaveRoom(pPlayer.dwId);
	
	-- 提示时间最后做~万一定时器被以外关了也不至于逻辑错误
	if self.nTimerId then
		local nLastFrameTime = tonumber(Timer:GetRestTime(self.nTimerId));
		Dialog:SetBattleTimer(pPlayer, Lang.mission.str798[Lang.Idx], nLastFrameTime);
		Dialog:SendBattleMsg(pPlayer, Lang.mission.str799[Lang.Idx]);
	end
end

-- 动态场景加载完毕后。。。开始关卡
function BaseRoom:DynamicSceneLoaded(nTemplateId)
    --self.nSceneId = nSceneId;
    for nGroupId, tbCurGroup in pairs(self.tbPlayerGroup) do
		for _, nPlayerId in pairs(tbCurGroup) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId)
            if pPlayer then
                local tbPos;
                if type(self.tbSetting.tbBeginPoint[1]) == "number" then
                    tbPos = self.tbSetting.tbBeginPoint;
                else
                    tbPos = self.tbSetting.tbBeginPoint[MathRandom(#self.tbSetting.tbBeginPoint)];
                end
				
				local nTeamId = pPlayer.GetTeamId();
				if KTeam.GetTeamLeader(nTeamId) == pPlayer.dwId then
					KScene.LoadScene(nTemplateId, pPlayer.dwId);
				end
                
                pPlayer.TeleportTo(nTemplateId, tonumber(tbPos[1]), tonumber(tbPos[2]), tonumber(tbPos[3]));
            end
		end
	end
	
end

-- 默认分组函数
function BaseRoom:DefaultGroup(...)
	for i = 1, #arg do
		local tbMemberList, nNum = KTeam.GetTeamMemberList(arg[i]);
		self.tbTeamId2Group[arg[i]] = i;
		if tbMemberList then
			for j = 1, nNum do
				if not self.tbPlayerGroup[i] then
					self.tbPlayerGroup[i] = {};
				end
				table.insert(self.tbPlayerGroup[i], tbMemberList[j]);
			end
		end
	end
	
	if #self.tbPlayerGroup > 0 then
		self:DynamicSceneLoaded(self.nMapId);
	end
end

-- 删除队伍信息
function BaseRoom:DelTeamInfo(nTeamId)
	for i = 1, #self.tbTeam do
		if self.tbTeam[i].nTeamId == nTeamId then
			self.tbTeam[i] = nil;
		end
	end
end

-- 获得队伍信息
function BaseRoom:GetTeamInfo(nTeamId)
	for i = 1, #self.tbTeam do
		if self.tbTeam[i].nTeamId == nTeamId then
			return self.tbTeam[i];
		end
	end
end

function BaseRoom:NpcCanTalk(szGroup, nCanTalk)
	local var = nil;
	if nCanTalk == 0 then
		var = 1;
	end
	for _, dwId in ipairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KNpc.GetById(dwId);
		if pNpc then
			pNpc.GetTempTable("XoyoGame").nDontTalk = var;
		end
	end	
end

-- 玩家离开房间; (恢复状态)
function BaseRoom:PlayerLeaveRoom(nPlayerId)
	if self.tbPlayer[nPlayerId] then
		local pPlayer = KGameBase.GetPlayerById(nPlayerId)
		if pPlayer then
			self.tbTeam[self.tbPlayer[nPlayerId].nTeam].nPlayerCount = self.tbTeam[self.tbPlayer[nPlayerId].nTeam].nPlayerCount - 1;
		end
		
		self.tbPlayer[nPlayerId] = nil;
		
		if self.OnPlayerLeaveRoom then
			self:OnPlayerLeaveRoom(nPlayerId);
		end
	end
end

-- 检查胜方
function BaseRoom:CheckWinner()
	if not self.tbWinner or not self.tbLoser then
		if self.tbSetting.fnWinRule then
			self.tbWinner, self.tbLoser = self.tbSetting.fnWinRule(self);
		else
			self.tbWinner, self.tbLoser = self:DefaultWinRule();
		end
	end
	return self.tbWinner, self.tbLoser;
end

-- 默认胜利规则
function BaseRoom:DefaultWinRule()
	local tbWinnerTeam = {};
	local tbLostTeam = {};
	for _, tbTeamInfo in pairs(self.tbTeam) do
		if tbTeamInfo.bIsWiner == 1 then
			table.insert(tbWinnerTeam, tbTeamInfo.nTeamId);
		else
			table.insert(tbLostTeam, tbTeamInfo.nTeamId);
		end
	end
	return tbWinnerTeam, tbLostTeam;
end


-- 开启房间
function BaseRoom:Start()
	if self.OnBeforeStart then
		self:OnBeforeStart();
	end
	self.tbLock[1]:StartLock();
end

function BaseRoom:OnCharacterEnterScene(pPlayer)
	-- 场景加载完毕开始本房间关卡
	self.nSceneId = pPlayer.dwSceneId;
	self:Start();
end

-- 关闭房间
function BaseRoom:Close(nCloseLock)
	
	self:SetTargetInfo(-1, "");  -- 清空TargetInfo
	
	local nOwnerId;
	-- 删除剩余的NPC
	for szGroup, _ in pairs(self.tbNpcGroup) do
		self:DelNpc(szGroup);
	end
	for nPlayerId, _ in pairs(self.tbPlayer) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			nOwnerId = KScene.GetMapOwnerId(pPlayer.dwSceneId);
			--Dialog:ShowBattleMsg(pPlayer,  0,  0); --关闭界面 --zzx
		end
		self:PlayerLeaveRoom(nPlayerId);
	end
	for _, data in pairs(self.tbAddNpcTimer) do
		if data.nTimerId then
			Timer:Close(data.nTimerId);
		end
	end
	
	for _, tbLock in ipairs(self.tbLock) do
		tbLock:Close();
	end
	
	if self.tbWinner then
		self:TeamAward(self.tbWinner, 1, nCloseLock);
	end
	if self.tbLoser then
		self:TeamAward(self.tbLoser, 0, nCloseLock);
	end
	
	-- 延迟回收地图，防止地图里还有玩家
	Timer:Register(10 * Env.GAME_FPS, self.ResetRoomMapCopy, self, self.nMapId, self.nSceneId);
	
	if self.OnClose then
		self:OnClose();
	end
end

function BaseRoom:ResetRoomMapCopy(nMapId, nSceneId)
	KScene.ResetMap(nMapId, nSceneId);
	return 0;
end

--------------------------------- PK 房间逻辑 -------------------------------------

function BaseRoom:PKGroup(...)
	if #arg ~= 2 then
		print("not enough team in PK Room", arg[1], arg[2]);
		return 0;
	end
	for i = 1, #arg do
		local tbMemberList, nNum = KTeam.GetTeamMemberList(arg[i]);
		if tbMemberList then
			for j = 1, nNum do
				local pPlayer = KGameBase.GetPlayerById(tbMemberList[j])
				if pPlayer then
					pPlayer.NewWorld(self.nMapId, unpack(self.tbSetting.tbBeginPoint[i]));
					pPlayer.SetCurCamp(i);
					pPlayer.nInBattleState = 1
				end
				if not self.tbPlayerGroup[i] then
					self.tbPlayerGroup[i] = {};
				end
				table.insert(self.tbPlayerGroup[i], tbMemberList[j]);
			end
		end
	end
end

function BaseRoom:PKDeath(pKillNpc)
	-- 马上原地复活
	me.ReviveImmediately(1);
	-- 战斗状态
	me.SetFightState(0);
	local pWinPlayer = pKillNpc.GetPlayer();
	if not pWinPlayer then -- 不是被玩家所杀
		return 0;
	end
	local nWinPlayerId = pWinPlayer.dwId;
	local nPlayerId = me.dwId;
	if not self.tbPlayer[nWinPlayerId] or not self.tbPlayer[nPlayerId] then		-- 玩家不在房间数据内
		return 0;
	end
	local nWinTeam = self.tbPlayer[nWinPlayerId].nTeam;
	local nKilledTeam = self.tbPlayer[nPlayerId].nTeam;
	if not self.tbTeam[nWinTeam].nCount then
		self.tbTeam[nWinTeam].nCount = 0;
	end
	if not self.tbTeam[nKilledTeam].nCount then
		self.tbTeam[nKilledTeam].nCount = 0;
	end
	-- 重投战斗定时
	self.tbPlayer[nPlayerId].nTimerId = Timer:Register(XoyoGame.PK_REFIGHT_TIME * Env.GAME_FPS, self.ReFight, self, nPlayerId)
	self.tbTeam[nWinTeam].nCount = self.tbTeam[nWinTeam].nCount + 1;
	self:SetTagetInfo(nWinTeam, 
		string.format(Lang.mission.str800[Lang.Idx], self.tbTeam[nKilledTeam].nCount, self.tbTeam[nWinTeam].nCount));
	self:SetTagetInfo(nKilledTeam,
		string.format(Lang.mission.str800[Lang.Idx], self.tbTeam[nWinTeam].nCount, self.tbTeam[nKilledTeam].nCount));
end

function BaseRoom:ReFight(nPlayerId)
	if not self.tbPlayer[nPlayerId] then
		return 0;
	end
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		pPlayer.SetFightState(1);
	end
	return 0;
end

-- PK胜负规则
function BaseRoom:PKWinRule()
	if (not self.tbTeam[1]) and (not self.tbTeam[2]) then		-- 双方都不在场
		return {},{};
	elseif not self.tbTeam[1] then					-- 一方在场
		return {self.tbTeam[2].nTeamId}, {};
	elseif not self.tbTeam[2] then
		return {self.tbTeam[1].nTeamId}, {};
	elseif not self.tbTeam[1].nCount then			-- 双方无杀人
		return {}, {self.tbTeam[1].nTeamId, self.tbTeam[2].nTeamId};
	else
		local tbLevel = {}
		for i = 1, 2 do
			local tbMemberList, nCount = KTeam.GetTeamMemberList(self.tbTeam[i].nTeamId);
			tbLevel[i] = 0;
			for j = 1, nCount do
				local pPlayer = KGameBase.GetPlayerById(tbMemberList[j]);
				if pPlayer then
					tbLevel[i] = tbLevel[i] + pPlayer.nLevel;
				end
			end
			tbLevel[i] = tbLevel[i] / (nCount * 1000);
		end
		if self.tbTeam[1].nCount - tbLevel[1] > self.tbTeam[2].nCount - tbLevel[2] then	-- 一方胜利
			return {self.tbTeam[1].nTeamId}, {self.tbTeam[2].nTeamId}
		elseif self.tbTeam[2].nCount - tbLevel[2] > self.tbTeam[1].nCount - tbLevel[1] then
			return {self.tbTeam[2].nTeamId}, {self.tbTeam[1].nTeamId};
		else		-- 平局
			return {}, {self.tbTeam[1].nTeamId, self.tbTeam[2].nTeamId};
		end
	end
end

-- PK胜负规则 忽略等级差别
function BaseRoom:PKWinRule2()
	if (not self.tbTeam[1]) and (not self.tbTeam[2]) then		-- 双方都不在场
		return {},{};
	elseif not self.tbTeam[1] then					-- 一方在场
		return {self.tbTeam[2].nTeamId}, {};
	elseif not self.tbTeam[2] then
		return {self.tbTeam[1].nTeamId}, {};
	elseif self.tbTeam[1].nCount > self.tbTeam[2].nCount then	-- 一方胜利
		return {self.tbTeam[1].nTeamId}, {self.tbTeam[2].nTeamId}
	elseif self.tbTeam[2].nCount > self.tbTeam[1].nCount then
		return {self.tbTeam[2].nTeamId}, {self.tbTeam[1].nTeamId};
	else	-- 平局
		return {}, {self.tbTeam[1].nTeamId, self.tbTeam[2].nTeamId};
	end
end

----------------- 猜谜房间逻辑--------------------------------------------
-- 获得队伍答题者
function BaseRoom:GetTeamGuessPlayer(nTeamId)
	for _, tbTeamInfo in ipairs(self.tbTeam) do
		if tbTeamInfo.nTeamId == nTeamId then
			if not tbTeamInfo.nGuessPlayer then
				return 0;
			end
			return tbTeamInfo.nGuessPlayer;
		end 
	end
end

-- 设定队伍答题者
function BaseRoom:SetTeamGuessPlayer(nTeamId, nPlayerId)
	for _, tbTeamInfo in ipairs(self.tbTeam) do
		if tbTeamInfo.nTeamId == nTeamId then
			if not tbTeamInfo.nGuessPlayer then
				tbTeamInfo.nGuessPlayer = nPlayerId
			end
		end 
	end
end

function BaseRoom:AskQuestion(nTeamId, nNpcId)
	local pNpc = KNpc.GetById(nNpcId);
	if not pNpc then
		return 0;
	end
	local tbTeamInfo = self:GetTeamInfo(nTeamId);
	if not tbTeamInfo then
		return ;
	end
	if not tbTeamInfo.nQuestCount then
		tbTeamInfo.nQuestCount = 0;
		tbTeamInfo.nGuessScore = 0;
		tbTeamInfo.nTime = GetTime();
	end
	local szMsg;
	local szMemberMsg = "你的队长被问到如下问题：\n  ";
	local tbOpt = {};
	if tbTeamInfo.nQuestCount >= XoyoGame.GUESS_QUESTIONS then
		if tbTeamInfo.nGuessScore >= XoyoGame.MIN_CORRECT then
			szMsg = string.format(Lang.mission.str802[Lang.Idx],tbTeamInfo.nGuessScore,Lib:TimeDesc(tbTeamInfo.nTime));
			--szMsg = "你答对了"..tbTeamInfo.nGuessScore.."道题，用时"..Lib:TimeDesc(tbTeamInfo.nTime).."，干得不错，不过还得等时间到才能分出胜负哦";
		else
			szMsg = string.format(Lang.mission.str802[Lang.Idx],tbTeamInfo.nGuessScore)
			--szMsg = "你只答对了"..tbTeamInfo.nGuessScore.."道题，看来你对这个江湖还不够熟悉啊"
		end
		Dialog:Say(szMsg);
		return 0;
	end
	local nRandom = MathRandom(#XoyoGame.tbGuessQuestion);
	szMsg = XoyoGame.tbGuessQuestion[nRandom].szQuestion;
	szMemberMsg = szMemberMsg..szMsg;
	table.insert(tbOpt, {XoyoGame.tbGuessQuestion[nRandom].szAnswer, self.ResultAnswer, self, nTeamId, 1, nNpcId})
	for i = 1, #XoyoGame.tbGuessQuestion[nRandom].tbSelect do
		if XoyoGame.tbGuessQuestion[nRandom].tbSelect[i] ~= "" then
			table.insert(tbOpt, {XoyoGame.tbGuessQuestion[nRandom].tbSelect[i], self.ResultAnswer, self, nTeamId, 0, nNpcId})
		end
	end
	local nOptCount = #tbOpt;
	for i = 1, nOptCount do
		local nSwap = MathRandom(nOptCount);
		tbOpt[i], tbOpt[nSwap] = tbOpt[nSwap], tbOpt[i];
	end
	for i = 1, nOptCount do
		szMemberMsg = szMemberMsg..Lang.mission.str804[Lang.Idx]..i.."："..tbOpt[i][1]
	end
	table.insert(tbOpt, {Lang.mission.str805[Lang.Idx]});
	tbTeamInfo.nQuestCount = tbTeamInfo.nQuestCount + 1;
	Dialog:Say(string.format(Lang.mission.str806[Lang.Idx],tbTeamInfo.nQuestCount)..szMsg, tbOpt);
	--Dialog:Say("你的第"..tbTeamInfo.nQuestCount.."道问题如下：\n  "..szMsg, tbOpt);
	local tbMemberList, nCount = KTeam.GetTeamMemberList(me.nTeamId);
	for i = 2, nCount do
		local pPlayer = KGameBase.GetPlayerById(tbMemberList[i]);
		if pPlayer then
			Setting:SetGlobalObj(pPlayer);
			Dialog:Say(szMemberMsg);
			Setting:RestoreGlobalObj();
		end
	end
end

function BaseRoom:ResultAnswer(nTeamId, nResult, nNpcId)
	local pNpc = KNpc.GetById(nNpcId);
	if not pNpc then
		return 0;
	end
	local tbTeamInfo = self:GetTeamInfo(nTeamId);
	if not tbTeamInfo then
		return ;
	end
	if nResult == 1 then
		tbTeamInfo.nGuessScore = tbTeamInfo.nGuessScore + 1;
	end
	if tbTeamInfo.nQuestCount == XoyoGame.GUESS_QUESTIONS then
		tbTeamInfo.nTime = GetTime() - tbTeamInfo.nTime;
	end
	self:AskQuestion(nTeamId, nNpcId);
end

-- 答题胜负规则
function BaseRoom:GuessRule()
	local tbWinner = {};
	local tbLoser = {};
	local tbTemp = {};
	for i = 1, 2 do
		if self.tbTeam[i] then
			if not self.tbTeam[i].nQuestCount or self.tbTeam[i].nQuestCount < XoyoGame.GUESS_QUESTIONS or self.tbTeam[i].nGuessScore < XoyoGame.MIN_CORRECT then
				table.insert(tbLoser, self.tbTeam[i].nTeamId);
			else
				table.insert(tbTemp, self.tbTeam[i]);
			end
		end
	end
	if #tbTemp < 2 then
		if tbTemp[1] then
			table.insert(tbWinner, tbTemp[1].nTeamId);
		end
	else
		if tbTemp[1].nGuessScore - tbTemp[1].nTime / 600 > tbTemp[2].nGuessScore - tbTemp[2].nTime / 600 then
			tbWinner[1] = tbTemp[1].nTeamId;
			tbLoser[1] = tbTemp[2].nTeamId;
		elseif tbTemp[1].nGuessScore - tbTemp[1].nTime / 600 < tbTemp[2].nGuessScore - tbTemp[2].nTime / 600 then
			tbWinner[1] = tbTemp[2].nTeamId;
			tbLoser[2] = tbTemp[1].nTeamId;
		else
			tbWinner[1] = tbTemp[1].nTeamId;
			tbWinner[2] = tbTemp[2].nTeamId;
		end
	end
	return tbWinner, tbLoser;
end

-- npc头顶冒字
function BaseRoom:SendNpcChat(szGroup, delay, szChat)
	if delay <= 0 then
		self:SendNpcChatNow(szGroup, szChat)
	else
		Timer:Register(delay, self.SendNpcChatNow, self, szGroup, szChat);
	end
end

function BaseRoom:SendNpcChatNow(szGroup, szChat)
	local tbTeamMember = nil;
	if not szGroup or not self.tbNpcGroup[szGroup] then
		print("error", "No Npc Group!");
		return 0;
	end
	local pPlayer = KGameBase.GetPlayerById(self.tbPlayerGroup[1]);
	local nTeamId = pPlayer.GetTeamId();
	if nTeamId > 0 then
		tbTeamMember = KTeam.GetTeamMemberList(nTeamId);
	else
		tbTeamMember = { pPlayer.dwId }
	end
	for _, nId in pairs(self.tbNpcGroup[szGroup]) do
		local pNpc = KGameBase.GetNpcById(nId);
		if pNpc then
			for _, _Id in pairs(tbTeamMember) do
				local pMember = KGameBase.GetPlayerById(_Id);
				pMember.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowBuble", pNpc.dwId, szChat});
			end
		end
	end
	return 0
end

function BaseRoom:NpcBubble(nPlayerGroup, szGroup, szMsg, delay)
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

function BaseRoom:SendSysMsg(nPlayerGroup, szMsg)
	local fnExcute = function (pPlayer)
		pPlayer.SysMsg(string.format("<color=red>%s", szMsg)); 
	end
	self:GroupPlayerExcute(fnExcute, nPlayerGroup);
end

function BaseRoom:ReviveInfo(bWait, bUseItem, bReturn, nScene, ...)
	local fnExcute = function (pPlayer)
		Player:SetTempRevive(pPlayer, bWait, bUseItem, bReturn, nScene, unpack(arg));
	end
	self:GroupPlayerExcute(fnExcute, -1);	
end

------------------------------------------------------------------------------------------------------------------------
-- 生存模式规则

function BaseRoom:SurviveRule()
	if self.tbTeam[1].bIsLost == 1 then
		return {}, {self.tbTeam[1].nTeamId};
	end
	local tbTemp = {}
	--Lib:ShowTB(self.tbPlayer);
	for _, tbPlayerInfo in pairs(self.tbPlayer) do
		if tbPlayerInfo.nTeam then
			return {tbPlayerInfo.nTeam}, {};
		end
	end
	return {}, {self.tbTeam[1].nTeamId};
end
