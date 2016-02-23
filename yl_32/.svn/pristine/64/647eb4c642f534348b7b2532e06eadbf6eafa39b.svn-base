Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.KinBattleField = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.TONGFIGHT_BATTLEFILED] = Activity.KinBattleField; -- 注册到活动工厂

local KinBattleField = Activity.KinBattleField;

local tbRandPos = {
	{1322, 7521, 1048576},
	{1181, 4871, 1048576},
	{4104, 4538, 1048576},
	{6988, 4837, 1048576},
	{6855, 7536, 1048576},
};

-- ?gs me.TeleportTo(me.dwSceneId, 1181, 4871, 1048576);

--local HOLD_WIN_TIME = (20 * 60);		-- hold住20分钟就算赢
local HOLD_WIN_TIME = (20 * 60);			-- hold住20分钟就算赢

function KinBattleField:OnInit()
	-- 初始化当前抢旗家族，抢旗时间等信息
	self.tbCurHoldInfo = 
	{
		nKinId = 0;				--当前抢旗家族id
		dwKinMasterId = 0;		-- 当前抢旗家族族长id
		dwStartTime = 0;		-- 开始抢到旗子的时间
		nEndTimerId = 0;
	};
	
	self.tbIgnoreDefs = {};		-- 记录原来族长的抗控值
	
	self.tbComboWinTimes = {};		-- 记录每个家族当前连胜次数
end

function KinBattleField:OnEnterActivityScene(nPlayerId)

	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if pPlayer.IsKinMaster() == 1 then
		self.tbIgnoreDefs[nPlayerId] = pPlayer.GetIgnoreDef();
		pPlayer.SetIgnoreDef(100);		-- 完全抗控
		
		-- 记录连胜次数
		self.tbComboWinTimes[pPlayer.GetKinId()] = KActivity.GetKinBattleWinTimes(pPlayer);
	end
	
	-- 加头顶攻守特效
	Timer:Register(3 * Env.GAME_FPS, self.DelayCheckBuffOnHead, self);
end

function KinBattleField:DelayCheckBuffOnHead()
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pPlayer then
			self:CheckBuffOnHead(pPlayer);
		end
	end
	
	return 0;
end

function KinBattleField:CheckBuffOnHead(pPlayer)

	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_DEF);
	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_ATTACK);
	
	if self:IsAttackCamp(pPlayer) == 1 then
		BufferApi.AddBufferToCharacter(pPlayer.dwId, 0, pPlayer.dwId, KinBattle.BUFF_ATTACK, 1);
	else
		BufferApi.AddBufferToCharacter(pPlayer.dwId, 0, pPlayer.dwId, KinBattle.BUFF_DEF, 1);
	end
	
	return 0;
end

function KinBattleField:OnStartStep()
	-- 重置报名人数记录
	self:OnInit();
	
	KGameBase.CenterExecute{"Activity:ResetApplyPlayers"};
	
	local nBuffInRegId = CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_BUFF_BUFF_IN, self.OnBuffIn, self);
	self.nBuffInRegId = nBuffInRegId;
	
	local nBuffOutRegId = CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_BUFF_BUFF_OUT, self.OnBuffOut, self);
	self.nBuffOutRegId = nBuffOutRegId;
	
	local nBuffTimerId = Timer:Register(60 * Env.GAME_FPS, self.AddBufferToGround, self);
	self.nBuffTimerId = nBuffTimerId;
end

function KinBattleField:DelayCheckOldWinner(nOldWinnerId)
	self:CheckOldWinnerAward(nOldWinnerId);
	self:CheckOldWinnerMemberAward();
	return 0;
end

function KinBattleField:OnEndStep()
	-- 重置报名人数记录
	KGameBase.CenterExecute{"Activity:ResetApplyPlayers"};
	
	if self.tbCurHoldInfo.nEndTimerId > 0 then
		Timer:Close(self.tbCurHoldInfo.nEndTimerId);
	end
	
	-- 没有人抢到旗子
	if self.tbCurHoldInfo.nKinId == 0 then
		KActivity.SetKinBattleWinnerId(0);
		KActivity.SetLastWinKinInfo(0);
	end
	
	local nOldWinnerId = KActivity.GetKinBattleWinnerId();
	if nOldWinnerId ~= self.tbCurHoldInfo.dwKinMasterId then
		Timer:Register(3 * Env.GAME_FPS, self.DelayCheckOldWinner, self, nOldWinnerId);
	end
	
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pPlayer then
			local nKinId = pPlayer.GetKinId();
			if nKinId == self.tbCurHoldInfo.nKinId then
				-- 胜利奖励
				self:WinnerAward(pPlayer);
			else
				-- 失败奖励
				self:LoserAward(pPlayer);
			end
		end
	end
	
	-- 新增赢家记录
	if self.tbCurHoldInfo.nKinId ~= 0 then
		-- 公告
		local tbKinInfo = GetPlayerKinInfo(self.tbCurHoldInfo.dwKinMasterId);
		KChat.BroadcastNoticeMsg(string.format(Lang.mission.str31[Lang.Idx], tbKinInfo.szKinName));
		
		local nComboWinTimes = self.tbComboWinTimes[self.tbCurHoldInfo.nKinId];
		nComboWinTimes = nComboWinTimes + 1;
		local pWinner = KGameBase.GetPlayerById(self.tbCurHoldInfo.dwKinMasterId)
		if pWinner then
			KActivity.SetKinBattleWinTimes(pWinner, nComboWinTimes);
		end
		Timer:Register(5 * Env.GAME_FPS, self.AddWinnerRecord, self, self.tbCurHoldInfo.dwKinMasterId, nComboWinTimes);
	end
	
	-- 重新加载三界神皇npc
	Timer:Register(10 * Env.GAME_FPS, KinBattle.ReloadKinBattleInfo, KinBattle);
	
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_BUFF_BUFF_IN, self.nBuffInRegId);
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_BUFF_BUFF_OUT, self.nBuffOutRegId);
	
	if self.nBuffTimerId then
		Timer:Close(self.nBuffTimerId);
	end
end

function KinBattleField:OnKickPlayer(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	-- 删除头顶攻守特效buff
	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_ATTACK);
	pPlayer.pBufferEffect.ClearBuff(KinBattle.BUFF_DEF);
	
	-- 删攻守buff
	pPlayer.pBufferEffect.ClearBuff(1160);
	pPlayer.pBufferEffect.ClearBuff(1163);
	
	if pPlayer.IsKinMaster() == 1 then
		local nOldIgnoreDef = self.tbIgnoreDefs[nPlayerId];
		assert(nOldIgnoreDef);
		pPlayer.SetIgnoreDef(nOldIgnoreDef);	-- 设回去
	end
end

function KinBattleField:AddBufferToGround()
	if not self.tbNpcGroup.yinxing then
		return;
	end
	
	local nNpcId = self.tbNpcGroup.yinxing[1];
	local pNpc = KGameBase.GetNpcById(nNpcId);
	
	if not pNpc then
		return;
	end
	
	-- 从给出的若干随机点中随机4个点放buff
	local tbChoosePos = Lib:RandArray(tbRandPos, 4);
	
	BufferApi.AddGroundBuffer(1158, nNpcId, 407, 341, tbChoosePos[1][1], tbChoosePos[1][2], tbChoosePos[1][3], 1);
	BufferApi.AddGroundBuffer(1158, nNpcId, 407, 341, tbChoosePos[2][1], tbChoosePos[2][2], tbChoosePos[2][3], 1);
	BufferApi.AddGroundBuffer(1158, nNpcId, 407, 341, tbChoosePos[3][1], tbChoosePos[3][2], tbChoosePos[3][3], 1);
	BufferApi.AddGroundBuffer(1161, nNpcId, 408, 341, tbChoosePos[4][1], tbChoosePos[4][2], tbChoosePos[4][3], 1);
end

function KinBattleField:AddWinnerRecord(nWinnerId, nComboWinTimes)
	KActivity.SetLastWinKinInfo(nWinnerId);		-- 设置最近一次赢家信息
	
	-- 产生三界神皇
	if nComboWinTimes >= 2 then
		--KGameBase.SavePlayerDelayData(pPlayer);
		KActivity.SetKinBattleWinnerId(nWinnerId);
		KActivity.AddKinBattleRecord(nWinnerId);
	end
	
	return 0;
end

function KinBattleField:WinnerAward(pPlayer)
	pPlayer.SendBlackMsg(Lang.mission.str32[Lang.Idx]);
	pPlayer.SysMsg(Lang.mission.str33[Lang.Idx]);
	KAward.AddPlayerAward(pPlayer, "SJZB", 1);
	
	if pPlayer.IsKinMaster() ~= 1 then
		KActivity.SetKinBattleWinTimes(pPlayer, 0);
	end
end

function KinBattleField:LoserAward(pPlayer)
	pPlayer.SysMsg(Lang.mission.str34[Lang.Idx]);
	KAward.AddPlayerAward(pPlayer, "SJZB", 2);
	KActivity.SetKinBattleWinTimes(pPlayer, 0);		-- 重置连胜次数
end

-- 检查新一届神皇和老神皇是否一致，不一致删掉老神皇的所有特权
function KinBattleField:CheckOldWinnerAward(nOldWinnerId)
	KGameBase.CenterExecute{"KinBattle:CheckOldWinnerAward_GC", nOldWinnerId, self.tbCurHoldInfo.nKinId};
	
	if nOldWinnerId ~= self.tbCurHoldInfo.dwKinMasterId then
		KActivity.SetKinBattleWinnerId(0);
	end
end

function KinBattleField:CheckOldWinnerMemberAward()
	KGameBase.CenterExecute{"KinBattle:CheckOldWinnerMemberAward_GC", self.tbCurHoldInfo.nKinId};
end

function KinBattleField:OnProgressBarFinished(nPlayerId, dwNpcId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	-- 旗帜易主
	local nKinId = pPlayer.GetKinId();
	
	assert(nKinId > 0 and nKinId ~= self.tbCurHoldInfo.nKinId);
	
	assert(pPlayer.IsKinMaster() == 1);
	
	if self.tbCurHoldInfo.nEndTimerId > 0 then
		Timer:Close(self.tbCurHoldInfo.nEndTimerId);
	end
	
	self.tbCurHoldInfo.nKinId = nKinId;
	self.tbCurHoldInfo.dwKinMasterId = nPlayerId;
	self.tbCurHoldInfo.dwStartTime = KGameBase.GetTime();
	self.tbCurHoldInfo.nEndTimerId = Timer:Register(HOLD_WIN_TIME * Env.GAME_FPS, self.OnHoldTimeUp, self);
	
	-- 改变攻守阵营效果
	self:OnChangeHoldKin();
	
	-- 公告
	local tbKinInfo = GetPlayerKinInfo(pPlayer.dwId);
	KChat.BroadcastNoticeMsg(string.format(Lang.mission.str35[Lang.Idx], tbKinInfo.szKinName, pPlayer.szName));
end

-- 坚持了超过20分钟，提前结束活动
function KinBattleField:OnHoldTimeUp()
	self:EndStep();
	
	return 0;
end

function KinBattleField:OnProgressBarCancel(nPlayerId)

end

-- 是否攻方阵营
function KinBattleField:IsAttackCamp(pPlayer)
	local nKinId = pPlayer.GetKinId();
	if nKinId ~= self.tbCurHoldInfo.nKinId then
		return 1;
	end
	
	return 0;
end

-- 夺旗家族变化，重新加攻守称号
function KinBattleField:OnChangeHoldKin()
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		-- 加头顶攻守特效
		self:CheckBuffOnHead(pPlayer);
	end
end

function KinBattleField:CheckAttackBuff(nKinId)
	local nHasBuff = 0;
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pMember and pMember.GetKinId() == nKinId then
			-- 只要有一个人站在攻buff圈圈里就有攻击buff
			if pMember.pBufferEffect.HasBuff(1159) == 1 then
				nHasBuff = 1;
				break;
			end
		end
	end
	
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pMember and pMember.GetKinId() == nKinId then
			if nHasBuff ~= 1 then
				pMember.pBufferEffect.ClearBuff(1160);
			end
		end
	end
	
end

function KinBattleField:CheckDefBuff(nKinId)
	local nHasBuff = 0;
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pMember and pMember.GetKinId() == nKinId then
			-- 只要有一个人站在攻buff圈圈里就有攻击buff
			if pMember.pBufferEffect.HasBuff(1162) == 1 then
				nHasBuff = 1;
				break;
			end
		end
	end
	
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		if pMember and pMember.GetKinId() == nKinId then
			if nHasBuff ~= 1 then
				pMember.pBufferEffect.ClearBuff(1163);
			end
		end
	end
	
end

function KinBattleField:OnBuffIn(nPlayerId, nBuffId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if self:IsAttackCamp(pPlayer) == 1 and pPlayer.pBufferEffect.HasBuff(1160) == 1 then
		return;
	end
	
	if self:IsAttackCamp(pPlayer) ~= 1 and pPlayer.pBufferEffect.HasBuff(1163) == 1 then
		return;
	end
	
	local nKinId = pPlayer.GetKinId();
	
	if self:IsAttackCamp(pPlayer) == 1 and nBuffId == 1159 then
		for _, tbPlayer in pairs(self.tbPlayers) do
			local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
			if pMember and pMember.GetKinId() == nKinId then
				-- 给本家族人加攻buff
				BufferApi.AddBufferToCharacter(pMember.dwId, 407, pMember.dwId, 1160, 1);
			end
		end
	end
	
	if self:IsAttackCamp(pPlayer) ~= 1 and nBuffId == 1162 then
		for _, tbPlayer in pairs(self.tbPlayers) do
			local pMember = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
			if pMember and pMember.GetKinId() == nKinId then
				-- 给本家族人加守buff
				BufferApi.AddBufferToCharacter(pMember.dwId, 408, pMember.dwId, 1163, 1);
			end
		end
	end
end

function KinBattleField:OnBuffOut(nPlayerId, nBuffId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	local nKinId = pPlayer.GetKinId();
	
	if nBuffId == 1159 then
		self:CheckAttackBuff(nKinId);
	end
	
	if nBuffId == 1162 then
		self:CheckDefBuff(nKinId);
	end
end

function KinBattleField:OnPlayerRevive(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if pPlayer then
		self:CheckBuffOnHead(pPlayer);
	end
end
