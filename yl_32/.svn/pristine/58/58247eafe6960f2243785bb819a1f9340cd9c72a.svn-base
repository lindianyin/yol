-- 文件名　：mission.lua
-- 创建者　：FanZai
-- 创建时间：2007-10-09 15:46:53	（金山软件香港上市！）
-- 文件说明：Mission脚本类


-- 从Mission的基类产生新的Mission
function Mission:New()
	return Lib:NewClass(self.tbClassBase);
end

-- Mission基础类，提供默认操作以及基础处理函数
local tbBase	= {};

--==++ 配置参数 ++==--
-- 说明：
--		在调用tbMission:Open()之前，可以在tbMission.tbMisCfg中设定一些配置参数，
--	以便这个Mission具有一些扩展功能。
--		如无特殊说明，变量为nil表示不使用此功能，否则均表示启用。

-- tbMisCfg.tbEnterPos		入口坐标：	[nGroupId] = {nMapId, nX, nY} 或 [nGroupId] = {tbPos, tbPos, ...}
--		玩家加入时到达指定的地图坐标；如果是一组坐标，会随机选取一个。

-- tbMisCfg.tbLeavePos		出口坐标：	[nGroupId] = {nMapId, nX, nY} 或 [nGroupId] = {tbPos, tbPos, ...}
--		玩家离开时到达指定的地图坐标（中途下线也会到这里）；如果是一组坐标，会随机选取一个。

-- tbMisCfg.tbChannel		聊天频道：	[nGroupId] = {szChannel, nIconId}

-- tbMisCfg.nFightState		设定战斗状态：加入时设定为战斗，离开时设定为非战斗；

-- tbMisCfg.nPkState		PK状态：
--		加入时设定为nPkState状态，禁止切换；离开时设定为练功，允许切换
--		PK状态枚举参见\gameres\script\player\define.lua

-- tbMisCfg.nDeathPunish	无死亡惩罚：加入时禁止死亡惩罚，离开时允许死亡惩罚

-- tbMisCfg.tbDeathRevPos	死亡重生点：[nGroupId] = {nMapId, nX, nY} 或 [nGroupId] = {tbPos, tbPos, ...}
--		玩家进入时设定玩家死亡重生点为特定点（可能从一组中随机抽取）

-- tbMisCfg.tbCamp			改变当前阵营：	[nGroupId]	= nCamp
--		加入时阻止当前阵营改变，如果存在tbCamp[nGroupId] or tbCamp[0]，则设定为指定的当前阵营。
--		离开时恢复当前阵营，允许当前阵营改变。

-- tbMisCfg.nForbidTeam		禁止组队：
--		0、加入时设定为组队不变色，离开时设定为组队变色；
--		1、加入时退出队伍、设定为禁止组队且组队不变色，离开时设定为允许组队、组队变色；

-- tbMisCfg.nInBattleState			战场模式（影响组队、小地图显示、切磋等, 同队伍间, 同帮会,同家族可以pk）
-- tbMisCfg.nInLeagueState  		联赛模式 (同队伍间不能pk, 同帮会,同家族可以pk)
-- tbMisCfg.nButcheStamina			屠杀不掉体力模式开启
-- tbMisCfg.nOnDeath				开启玩家死亡回调：	tbMission:OnDeath
-- tbMisCfg.nOnKillNpc				开启玩家杀怪回调：	tbMission:OnKillNpc
-- tbMisCfg.nOnMovement				标记是否为参加某项活动，用于做登录用
-- tbMisCfg.nForbidTrade			禁止交易
-- tbMisCfg.nForbidStall			禁止摆摊
-- tbMisCfg.nDisableOffer			
-- tbMisCfg.nForbidSwitchFaction	禁止切换门派
-- tbMisCfg.nDisableTeamPlane		禁止组队界面
-- tbMisCfg.nDisableFriendPlane		禁止好友界面
-- tbMisCfg.nDisableStallPlane		禁止交易界面
-- tbMisCfg.nLogOutRV				设置恢复状态，防止宕机出现状态锁，类型定义查看logout_rv.lua

--== ==--

--== 时间事件表配置 ==--
-- tbMisEventList			Mission中时间事件表，格式为 { {状态标识, 状态持续时间, 状态回调函数[, 参数]}， }
--== ==--

--==++ 基本操作函数 ++==--

-- 开启Mission
function tbBase:Open()
	Dbg:Output("Mission", "Open", self);
	
	self.tbGroups	= {};	-- [nGroupId]	= {nPlayerId, nPlayerId, ...}
								-- 特别的，nGroupId 为 0 表示全部
								
	self.tbPlayers	= {};	-- [nPlayerId]	= tbPlayerInfo;
	
	self.tbTimers	= {};	-- [nTimerId]	= tbTimer;

	self.nStateJour = 0;
	
	-- 根据配置项进行额外操作
	local tbMisCfg	= self.tbMisCfg or {};
	
	if (tbMisCfg.tbChannel) then
		for nGroupId, tbChannelId in pairs(tbMisCfg.tbChannel) do
			KChatChannel.CreatChannel(tbChannelId[1], tbChannelId[2],tbChannelId[3], tbChannelId[4]);
		end
	end

	self:OnOpen();
end

function tbBase:GoNextState()
	if (self.tbNowStateTimer) then
		self.tbNowStateTimer:Close();
		self.tbNowStateTimer = nil;
	end
	local nRet = self:OnActiveEventTimer();
	if (0 == nRet) then
		return;
	else
		local tbNowStateTimer = self:CreateTimer(nRet, self.OnActiveEventTimer, self);
		if (tbNowStateTimer) then
			self.tbNowStateTimer = tbNowStateTimer;
		end
	end
end

function tbBase:OnActiveEventTimer()
	local nRet = 0;
	if (0 < self.nStateJour and #self.tbMisEventList >= self.nStateJour) then
		local fncExcute = self[self.tbMisEventList[self.nStateJour][3]];
		if fncExcute then
			nRet = fncExcute(self);
			if nRet and nRet == 0 then	
				return 0;
			end
		end
	end
	-- 状态转换
	self.nStateJour = self.nStateJour + 1;
	if (#self.tbMisEventList < self.nStateJour) then	-- 未必开启或者已经结束
		return 0;
	end
	-- 下一阶段定时
	local tbTimer = self.tbMisEventList[self.nStateJour];
	if not tbTimer then
		return 0;
	end

	return tbTimer[2];
end

function tbBase:GetStateLastTime()
	local tbTimer = self.tbNowStateTimer;
	if (not tbTimer) then
		return 0;
	end
	return tbTimer:GetRestTime();
end

-- 关闭Mission
function tbBase:Close()
	Dbg:Output("Mission", "Close", self);
	
	self:_MustOpen();
	
	Lib:CallBack({self.OnClose, self});

	local tbPlayer, nCount	= self:GetPlayerList(nGroupId);
	for _, pPlayer in ipairs(tbPlayer) do
		Lib:CallBack({self.KickPlayer, self, pPlayer});
	end

	for nRegisterId, tbTimer in pairs(self.tbTimers) do
		Lib:CallBack({tbTimer.Close, tbTimer});
	end

	-- 根据配置项进行额外操作
	local tbMisCfg	= self.tbMisCfg or {};
	
	if (tbMisCfg.tbChannel) then
		for nGroupId, tbChannelId in pairs(tbMisCfg.tbChannel) do
			KChatChannel.DeleteChannel(tbChannelId[1]); 
			-- KDialog.DeleteChannel(tbChannelId[1]);
		end
	end
	
	self.tbGroups			= nil;
	self.tbPlayers			= nil;
	self.tbTimers			= nil;
	self.tbMisEventList 	= nil;
	self.tbNowStateTimer	= nil;
	self.nStateJour			= 0;
end

-- 创建Timer
--	参数：nWaitTime（从现在开始的桢数）, fnCallBack, varParam1, varParam2, ...
--	返回：tbTimer
function tbBase:CreateTimer(nWaitTime, ...)
	self:_MustOpen();
	
	local tbTimer		= Lib:NewClass(Mission.tbTimerBase);
	tbTimer.nWaitTime	= nWaitTime;
	tbTimer.tbCallBack	= arg;
	tbTimer.szRegInfo	= debug.traceback("Register MissionTimer", 2);
	tbTimer.tbMission	= self;
	function tbTimer:OnDestroy(nRegisterId)
		local tbMission	= self.tbMission;
		local tbTimers	= tbMission.tbTimers;
		Dbg:PrintEvent("Mission", "OnTimerDestroy", nRegisterId, tbMission, tbTimers);
		if (tbTimers) then	-- 如果tbTimers不存在说明此Mission已经关闭了（OnDestroy是有可能延后的）
			if (tbTimers[nRegisterId] ~= self) then
				print(debug.traceback("Mission Error: Mission已经关闭了"));
			end
			tbTimers[nRegisterId]	= nil;
		end
		self.nRegisterId	= nil;
	end

	-- 调用公用Timer控件，注册Timer
	local nRegisterId	= Timer:RegisterEx(tbTimer);
	tbTimer.nRegisterId	= nRegisterId;

	-- 将注册情况记录在本Mission中
	self.tbTimers[nRegisterId]	= tbTimer;
	
	Dbg:PrintEvent("Mission", "CreateTimer", nRegisterId, nWaitTime, self);
	
	return tbTimer;
end

--设置玩家进入mission飞往的地图和坐标
function tbBase:SetGroupJoinPos(nGroupId, nDyMapId, nPosX, nPosY)
	self.tbMisCfg = self.tbMisCfg or {};
	self.tbMisCfg.tbEnterPos = self.tbMisCfg.tbEnterPos or {};
	self.tbMisCfg.tbEnterPos[nGroupId] = {nDyMapId, nPosX, nPosY};
end

--设置玩家离开mission飞往的地图和坐标
function tbBase:SetGroupLeavePos(nGroupId, nMapId, nPosX, nPosY)
	self.tbMisCfg = self.tbMisCfg or {};
	self.tbMisCfg.tbLeavePos = self.tbMisCfg.tbLeavePos or {};	
	self.tbMisCfg.tbLeavePos[nGroupId] = {nMapId, nPosX, nPosY};
end

-- 加入玩家
function tbBase:JoinPlayer(pPlayer, nGroupId)
	self:_MustOpen();
	
	local pPlayerOld	= me;
	me	= pPlayer;

	local nPlayerId		= pPlayer.dwId;
	
	if self.tbPlayers[nPlayerId] then
		print(debug.traceback("Mission Error: 玩家重复加入"));
		return 0;
	end
	--assert(not self.tbPlayers[nPlayerId]);	-- 重复加入
	
	local tbPlayerInfo	= {
		nGroupId		= nGroupId,
		nPlayerId       = nPlayerId;
		--nLogoutRegId	= PlayerEvent:Register("OnLogout", self._OnLogout, self), -- zzx
		--nSwitchSvrRegId	= PlayerEvent:Register("OnSwitchServer", self._OnLogout, self),
	}
	self.tbPlayers[nPlayerId]	= tbPlayerInfo;
    
	-- 根据配置项进行额外操作
	local tbMisCfg	= self.tbMisCfg or {};

	self:_GoPos(tbMisCfg.tbEnterPos, pPlayer, nGroupId);
	
	if (tbMisCfg.nFightState) then
		pPlayer.SetFightState(1);
	end
	
	if (tbMisCfg.nForbidTeam) then
		if (tbMisCfg.nForbidTeam == 1) then
			pPlayer.TeamDisable(1);
			pPlayer.TeamApplyLeave();
		end
	end
	
	if (tbMisCfg.nInLeagueState) then
		--pPlayer.nInBattleState	= 1; --zzx
		--pPlayer.SetCheckTeamInBattle(1); --zzx
	end
	
	--[[if (tbMisCfg.nInBattleState) then --zzx
		pPlayer.nInBattleState	= 1;
	end]]
	
	local tbCamp	= tbMisCfg.tbCamp;
	if (tbCamp) then
		local nCamp	= tbCamp[nGroupId] or tbCamp[0]
		if (nCamp) then
			pPlayer.SetCurCamp(nCamp);
		end
		pPlayer.DisableChangeCurCamp(1);
	end
	
	local tbDeathRevPos	= tbMisCfg.tbDeathRevPos;
	if (tbDeathRevPos) then
		local tbPos	= tbDeathRevPos[nGroupId] or tbDeathRevPos[0];
		if (tbPos) then
			if (type(tbPos[1]) == "table") then	-- 多个坐标需要随即选取
				tbPos	= tbPos[MathRandom(#tbPos)];
			end
			--tbPlayerInfo.nRevMapId, tbPlayerInfo.nRevPointId	= pPlayer.GetRevivePos(); -- zzx
			--pPlayer.SetTmpDeathPos(unpack(tbPos)); -- zzx
		end
	end
	
	local tbChannel	= tbMisCfg.tbChannel;
	if (tbChannel) then
		local tbChannelId	= tbChannel[nGroupId] or tbChannel[0];
		pPlayer.EnterChannel(tbChannelId[1]);
	end
	
	if (tbMisCfg.nPkState) then
		pPlayer.nPkModel		= tbMisCfg.nPkState;
		pPlayer.nForbidChangePK	= 1;
	end
	
	if (tbMisCfg.nDeathPunish) then
		--pPlayer.SetNoDeathPunish(1); -- zzx
	end
	
	if (tbMisCfg.nOnDeath) then
		assert(self.OnDeath);
		tbPlayerInfo.nOnDeathRegId	= PlayerEvent:Register("OnDeath", self.OnDeath, self);
	end
	
	if (tbMisCfg.nOnKillNpc) then
		assert(self.OnKillNpc);
		tbPlayerInfo.nOnKillNpcRegId	= PlayerEvent:Register("OnKillNpc", self.OnKillNpc, self);
	end
	
	if (tbMisCfg.nOnMovement) then
		pPlayer.SetMovementState(1);
	end
	
	if (tbMisCfg.nForbidTrade) then
		ForbitTrade(1);
	end

	if (tbMisCfg.nForbidStall) then
		pPlayer.DisabledStall(1);
	end
	
	if (tbMisCfg.nDisableOffer) then
		pPlayer.DisableOffer(1);
	end
	
	if (tbMisCfg.nForbidSwitchFaction) then
		Faction:SetForbidSwitchFaction(pPlayer, 1);
	end
	if (tbMisCfg.nDisableTeamPlane) then
		pPlayer.SetDisableTeam(1);
	end
	
	if (tbMisCfg.nDisableFriendPlane) then
		pPlayer.SetDisableFriend(1);
	end
	
	if (tbMisCfg.nDisableStallPlane) then
		pPlayer.SetDisableStall(1);
	end		
	
	if (tbMisCfg.nLogOutRV) then
		pPlayer.SetLogOutState(tbMisCfg.nLogOutRV);
	end
	
	if (tbMisCfg.nButcheStamina) then
		pPlayer.SetDisableButcherReduceStamina(1);
	end
	
	self:_AddGroupPlayer(0, nPlayerId);	-- 加入总表
	if (nGroupId ~= 0) then
		self:_AddGroupPlayer(nGroupId, nPlayerId);
	end

	self:OnJoin(nGroupId);

	me	= pPlayerOld;
end

-- 改变玩家Group
function tbBase:ChangePlayerGroup(pPlayer, nNewGroupId)
	self:_MustOpen();
	
	local nPlayerId		= pPlayer.dwId;
	local tbPlayerInfo	= self.tbPlayers[nPlayerId];
	if not tbPlayerInfo then
		print("Mission Error: 尚未加入Mission", pPlayer.szName);
		print(debug.traceback("Mission Error: 尚未加入Mission"));
		return 0;
	end
	--assert(tbPlayerInfo);	-- 尚未加入
	local nOldGroupId	= tbPlayerInfo.nGroupId;
	
	-- 调试输出
	Dbg:Output("Mission", "ChangePlayerGroup", self, pPlayer.szName, nOldGroupId.."=>"..nNewGroupId);
	
	-- 无改变直接返回
	if (nOldGroupId == nNewGroupId) then
		return;
	end
	
	-- 根据配置项进行额外操作
	local tbMisCfg	= self.tbMisCfg or {};
	
	if (tbMisCfg.tbEnterPos and tbMisCfg.tbEnterPos[nNewGroupId]) then
		self:_GoPos(tbMisCfg.tbEnterPos, pPlayer, nNewGroupId);
	end
	
	local tbChannel	= tbMisCfg.tbChannel;
	if (tbChannel) then
		local tbChannelId	= tbChannel[nOldGroupId] or tbChannel[0];
		pPlayer.LeaveChannel(tbChannelId[1]);
		tbChannelId			= tbChannel[nNewGroupId] or tbChannel[0];
		pPlayer.EnterChannel(tbChannelId[1]);
	end
	
	local tbCamp	= tbMisCfg.tbCamp;
	if (tbCamp and tbCamp[nNewGroupId]) then
		pPlayer.SetCurCamp(tbCamp[nNewGroupId]);
	end
	
	if (nOldGroupId ~= 0) then
		self:_RemoveGroupPlayer(nOldGroupId, nPlayerId);
	end
	if (nNewGroupId ~= 0) then
		self:_AddGroupPlayer(nNewGroupId, nPlayerId);
	end
	tbPlayerInfo.nGroupId	= nNewGroupId;
end

-- 踢出玩家
-- 参数：pPlayer, [szReason]（szReason可选，用于OnLeave中做特殊处理）
function tbBase:KickPlayer(pPlayer, szReason)
	self:_MustOpen();
	
	local nPlayerId		= pPlayer.dwId;
	local tbPlayerInfo	= self.tbPlayers[nPlayerId];
	if not tbPlayerInfo then
		return 0;
	end
	local nGroupId		= tbPlayerInfo.nGroupId;
	local pPlayerOld	= me;
	
	me	= pPlayer;
	
	Lib:CallBack({self.BeforeLeave, self, nGroupId, szReason or ""});
	
	self.tbPlayers[nPlayerId]	= nil;
	
	self:_RemoveGroupPlayer(0, nPlayerId);	-- 移出总表
	if (nGroupId ~= 0) then
		self:_RemoveGroupPlayer(nGroupId, nPlayerId);
	end

	if (tbPlayerInfo.nLogoutRegId) then
		--PlayerEvent:UnRegister("OnLogout", tbPlayerInfo.nLogoutRegId); --zzx
	end
	
	--if (tbPlayerInfo.nSwitchSvrRegId) then
		--PlayerEvent:UnRegister("OnSwitchServer", tbPlayerInfo.nSwitchSvrRegId);
	--end
	
	-- 根据配置项进行额外操作
	local tbMisCfg	= self.tbMisCfg or {};

	if (tbMisCfg.nFightState) then
		--pPlayer.SetFightState(0); --zzx
	end
	
	--[[if (tbMisCfg.nForbidTeam) then --zzx
		if (tbMisCfg.nForbidTeam == 1) then
			pPlayer.TeamDisable(0);
		end
	end
	
	if (tbMisCfg.nForbidTrade) then
		ForbitTrade(0);
	end
	
	if (tbMisCfg.nForbidStall) then
		pPlayer.DisabledStall(0);
	end
	
	if (tbMisCfg.nDisableOffer) then
		pPlayer.DisableOffer(0);
	end	
	
	if (tbMisCfg.nForbidSwitchFaction) then
		Faction:SetForbidSwitchFaction(pPlayer, 0);
	end


	if (tbMisCfg.nDisableTeamPlane) then
		pPlayer.SetDisableTeam(0);
	end

	if (tbMisCfg.nDisableFriendPlane) then
		pPlayer.SetDisableFriend(0);
	end
	
	if (tbMisCfg.nDisableStallPlane) then
		pPlayer.SetDisableStall(0);
	end	
	
	if (tbMisCfg.nInLeagueState) then
		pPlayer.nInBattleState	= 0;
		pPlayer.SetCheckTeamInBattle(0);
	end
	
	if (tbMisCfg.nInBattleState) then
		pPlayer.nInBattleState	= 0;
	end

	if (tbMisCfg.tbCamp) then
		pPlayer.SetCurCamp(pPlayer.GetCamp());
		pPlayer.DisableChangeCurCamp(1);
	end
	
	if (tbPlayerInfo.nRevMapId) then
		if (pPlayer.IsDead() == 1) then
			pPlayer.ReviveImmediately(0);
		end
		pPlayer.SetRevivePos(tbPlayerInfo.nRevMapId, tbPlayerInfo.nRevPointId);
	end
	
	local tbChannel	= tbMisCfg.tbChannel;
	if (tbChannel) then
		local tbChannelId	= tbChannel[nGroupId] or tbChannel[0];
		pPlayer.LeaveChannel(tbChannelId[1]);
	end
	
	if (tbMisCfg.nPkState) then
		pPlayer.nPkModel		= Player.emKPK_STATE_PRACTISE;
		pPlayer.nForbidChangePK	= 0;
	end
	
	if (tbMisCfg.nDeathPunish) then
		if (pPlayer.IsDead() == 1) then
			pPlayer.ReviveImmediately(1);
		end
		pPlayer.SetNoDeathPunish(0);
	end
	
	if (tbMisCfg.nOnMovement) then
		pPlayer.SetMovementState(0);
	end
	
	if (tbMisCfg.nButcheStamina) then
		pPlayer.SetDisableButcherReduceStamina(0);
	end]]
	
	if (tbMisCfg.nLogOutRV) then
		--Mission:LogOutRV(); --zzx
	end
	
	if (tbPlayerInfo.nOnDeathRegId) then
		--PlayerEvent:UnRegister("OnDeath", tbPlayerInfo.nOnDeathRegId); --zzx
		tbPlayerInfo.nOnDeathRegId	= nil;
	end
	
	if (tbPlayerInfo.nOnKillNpcRegId) then
		--PlayerEvent:UnRegister("OnKillNpc", tbPlayerInfo.nOnKillNpcRegId); --zzx
		tbPlayerInfo.nOnKillNpcRegId	= nil;
	end
	
	--self:_GoPos(XoyoGame.tbPlayerLeavePos, pPlayer, nGroupId); --zzx
	--self:_GoPos(self.tbMisCfg.tbLeavePos, pPlayer, 1);
	--pPlayer.ReturnToLastCity();
	
	Lib:CallBack({self.OnLeave, self, nGroupId, szReason or ""});

	me	= pPlayerOld;
end

-- 广播发消息
-- 参数：
--	[nGroupId]	发送给哪个Group，默认0表示全部
--	szMsg		发送的消息
--	[szName]	发送者名称，默认“系统”
function tbBase:BroadcastMsg(nGroupId, szMsg, szName)
	self:_MustOpen();
	
	if (type(nGroupId) == "string") then
		szName		= szMsg;
		szMsg		= nGroupId;
		nGroupId	= 0;
	end
	local tbPlayer, nCount	= self:GetPlayerList(nGroupId);
	if (nCount > 0) then
		KDialog.Msg2PlayerList(tbPlayer, szMsg, szName or Lang.mission.str1[Lang.Idx]);
	end
end

-- 群体玩家执行指令
-- 参数：
--	[nGroupId]	为哪个Group的玩家执行，默认0表示全部
--	varFunc		回调函数(函数体或函数名)
--	...			调用时的参数
-- 说明：回调函数会被调用多次，每次调用时全局变量me会被设置为不同的Group内玩家
function tbBase:ForEachCall(nGroupId, ...)
	self:_MustOpen();
	
	if (type(nGroupId) ~= "number") then
		table.insert(arg, 1, nGroupId);
		nGroupId	= 0;
	end
	local tbPlayer, nCount	= self:GetPlayerList(nGroupId);
	local pPlayerOld	= me;
	for _, pPlayer in ipairs(tbPlayer) do
		me	= pPlayer;
		Lib:CallBack(arg);
	end
	me	= pPlayerOld;
end

-- 获取是否已开启
function tbBase:IsOpen()
	return (self.tbPlayers and 1) or 0;
end

-- 获取玩家数量
-- 参数：[nGroupId]	默认0表示全部
function tbBase:GetPlayerCount(nGroupId)
	self:_MustOpen();
	
	local tbGroup	= self:_GetGroup(nGroupId);
	return tbGroup.n;
end

-- 获取玩家列表
-- 参数：[nGroupId]	默认0表示全部
-- 返回：tbPlayer, nCount
function tbBase:GetPlayerList(nGroupId)
	self:_MustOpen();
	
	local tbGroup	= self:_GetGroup(nGroupId);
	local tbPlayer	= {};
	for nIndex, nPlayerId in ipairs(tbGroup) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			tbPlayer[nIndex]	= pPlayer;
		else
			--print(debug.traceback("Mission Error:玩家不存在"));
		end
	end
	
	return tbPlayer, tbGroup.n;
end

-- 获取玩家Id列表
-- 参数：[nGroupId]	默认0表示全部
-- 返回：tbPlayerId
function tbBase:GetPlayerIdList(nGroupId)
	self:_MustOpen();
	
	local tbPlayer = self:GetPlayerList(nGroupId);
	local tbPlayerId = {};
	for _, pPlayer in pairs(tbPlayer) do
		table.insert(tbPlayerId, pPlayer.dwId);
	end
	return tbPlayerId;
end

-- 获取特定玩家所属Group
-- 返回：nGroupId	-1表示此玩家不在Mission中
function tbBase:GetPlayerGroupId(pPlayer)
	self:_MustOpen();
	if not pPlayer then
		return -1;
	end
	local tbPlayerInfo	= self.tbPlayers[pPlayer.dwId];
	
	return (tbPlayerInfo and tbPlayerInfo.nGroupId) or -1;
end


--==++ 默认回调函数 ++==--

-- 当Mission被开启“后”被调用
function tbBase:OnOpen()
	Dbg:Output("Mission", "OnOpen", "(Empty)", self);
end
-- 在Mission被关闭“前”被调用
function tbBase:OnClose()
	Dbg:Output("Mission", "OnClose", "(Empty)", self);
end
-- 当玩家加入Mission“后”被调用
function tbBase:OnJoin(nGroupId)
	Dbg:Output("Mission", "OnJoin", "(Empty)", self, me.szName, nGroupId);
end
-- 当玩家离开Mission“前”被调用
function tbBase:BeforeLeave(nGroupId, szReason)
	Dbg:Output("Mission", "BeforeLeave", "(Empty)", self, me.szName, nGroupId, szReason);
end
-- 当玩家离开Mission“后”被调用
function tbBase:OnLeave(nGroupId, szReason)
	Dbg:Output("Mission", "OnLeave", "(Empty)", self, me.szName, nGroupId, szReason);
end


--==++ 底层操作函数 ++==--

function tbBase:_MustOpen()
	assert(self.tbPlayers, debug.traceback("Mission is not opened!", 2));
end

function tbBase:_GetGroup(nGroupId)
	return self.tbGroups[nGroupId or 0] or {n = 0};
end

function tbBase:_AddGroupPlayer(nGroupId, nPlayerId)
	local tbGroup	= self.tbGroups[nGroupId];
	if (not tbGroup) then
		tbGroup	= {n = 0};
		self.tbGroups[nGroupId]	= tbGroup;
	end
	tbGroup.n			= tbGroup.n + 1;
	tbGroup[tbGroup.n]	= nPlayerId;
end

function tbBase:_RemoveGroupPlayer(nGroupId, nPlayerId)
	local tbGroup	= self.tbGroups[nGroupId];
	if not tbGroup then
		print(debug.traceback("Mission Error:Group不存在"));
		return 0;
	end
	--assert(tbGroup);	-- Group不存在
	for nIndex, nId in ipairs(tbGroup) do
		if (nId == nPlayerId) then
			table.remove(tbGroup, nIndex);
			tbGroup.n	= tbGroup.n - 1;
			return;
		end
	end
	print(debug.traceback("Mission Error:未找到 nPlayerId 的匹配项"));
	--assert(nil);	-- 未找到 nPlayerId 的匹配项
end

function tbBase:_OnLogout(nPlayerId)
    if self.tbPlayers ~= nil then
        if self.tbPlayers[nPlayerId] ~= nil then
	        self:KickPlayer(me, "Logout");
	    end
	end
end

function tbBase:_GoPos(tbGroupPos, pPlayer, nGroupId)
	if (not tbGroupPos) then	-- 没有定义传送，无需执行
		return;
	end
	local tbPos	= tbGroupPos[nGroupId][pPlayer.dwId] or tbGroupPos[1];
	if (not tbPos) then	-- 没有定义此Group传送，无需执行
		return;
	end
	--[[if (type(tbPos[1]) == "table") then	-- 多个坐标需要随即选取 --zzx
		tbPos	= tbPos[MathRandom(#tbPos)];
	end]]
	
	me.TeleportTo(tonumber(tbPos.dwSceneId), tonumber(tbPos.nX), tonumber(tbPos.nY), tonumber(tbPos.nZ));
	--pPlayer.NewWorld(unpack(tbPos));
end

Mission.tbClassBase	= tbBase;

--==++ Timer类 ++==--
local tbTimerBase	= {};

function tbTimerBase:Close()
	Dbg:PrintEvent("Mission", "CloseTimer", self.nRegisterId, self.tbMission);
	if self.nRegisterId and self.nRegisterId > 0 then
		Timer:Close(self.nRegisterId);
	end
end

function tbTimerBase:GetRestTime()
	if not self.nRegisterId or self.nRegisterId <= 0 then
		return 0
	end
	return Timer:GetRestTime(self.nRegisterId);
end

Mission.tbTimerBase	= tbTimerBase;

