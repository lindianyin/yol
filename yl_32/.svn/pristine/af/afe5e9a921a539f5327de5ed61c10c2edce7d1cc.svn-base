Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")
Include("script/mission/activity/setting/activitysetting_boss.lua")

Activity.KUAFUBOSS_SAICHANG = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.KUAFUBOSS_SAICHANG] = Activity.KUAFUBOSS_SAICHANG -- 注册到活动工厂

local WorldBossAwardRank = 3;	--世界boss奖励数量，目前只奖励组队的，最多有3个
local WorldBossStartTemplateId = 2507;	--世界boss起始ID，主要永远奖励结算
local KUAFUBOSS_SAICHANG = Activity.KUAFUBOSS_SAICHANG
local tbSteps = Activity.ActivitySetting[15].STEP

function KUAFUBOSS_SAICHANG:OnInit()
	self.tbPlayerDamage = {};
	self.nBossTemplateId = nil;
	self.nBossNpcId = nil;
	self.nBossDamagedValue = 0;
	if self.nActivityIndex == 15 and tbSteps[self.nStepIndex] ~= nil and 
		tbSteps[self.nStepIndex].WorldBossTemplateId ~= nil then
		self.nBossTemplateId = tbSteps[self.nStepIndex].WorldBossTemplateId;
	end
	--self.nWinnerId = -1
end

--nTempalteId暂时无用
function KUAFUBOSS_SAICHANG:OnAddNpc(tbNpcId, nTemplateId)
	assert(nTemplateId);
	for _, nNpcId in pairs(tbNpcId) do
		local pNpc = KGameBase.GetNpcById(nNpcId);
		if pNpc and pNpc.dwTemplateId == self.nBossTemplateId and self.nBossNpcId == nil then
			self.nBossNpcId = nNpcId;
			self.nBossDamagedValue = 0;
			break; 
		end
	end
end

function KUAFUBOSS_SAICHANG:OnEnterActivityScene(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
    if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
		KCharacter.Revive(pPlayer, 1000, 1000);
	end
	self.tbPlayerDamage[nPlayerId] = 0;
	self:SetPlayerActivityInfo(nPlayerId, nil, string.format(Lang.mission.str38[Lang.Idx], self.tbPlayerDamage[nPlayerId], self.tbPlayerDamage[nPlayerId]).."%");
	-- 覆盖复活场景：从准备场传送至赛场时，还在准备场时已经执行过一次，复活时场景不对
	self:ExcCacheEvent(nPlayerId);
end

--[[
function KUAFUBOSS_SAICHANG:OnJoinPlaeyr(nPlayerId, nGroup)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
    if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
		KCharacter.Revive(pPlayer, 1000, 1000);
	end
	self.tbPlayerDamage[nPlayerId] = 0;
	self:SetPlayerActivityInfo(nPlayerId, nil, string.format("\r个人伤害: <color=red>%d\r<color=white>获奖概率: <color=red>%.2f", self.tbPlayerDamage[nPlayerId], self.tbPlayerDamage[nPlayerId]).."%");
end
]]--

-- 清除伤害
function KUAFUBOSS_SAICHANG:OnKickPlayer(nPlayerId)
	self:SetPlayerActivityInfo(nPlayerId, "", "");
	self:ClearPlayerDamage(nPlayerId);
end
 
--角色离开场景
function KUAFUBOSS_SAICHANG:LeaveGame(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if pPlayer and pPlayer.dwSceneTemplateId == self.tbPos[1] then
		self:ClearPlayerDamage(nCharacterId);
	    self:KickPlayer(nCharacterId, 1);

		pPlayer.ReturnToLastCity();
		pPlayer.LeaveTeam();
	end
	print("LeaveGame", pPlayer.dwSceneTemplateId, self.tbPos[1]);
end

function KUAFUBOSS_SAICHANG:OnPlayerDamage(nCasterId, nInjuredId, nDamage)
	local pBossNpc = KGameBase.GetNpcById(nInjuredId);
	if pBossNpc and nInjuredId == self.nBossNpcId and pBossNpc.IsDead() ~= 1 then
		local nBossCurrentLife = pBossNpc.GetCurrentLife();
		if nDamage > nBossCurrentLife then
			nDamage = nBossCurrentLife;
		end

		self.tbPlayerDamage[nCasterId] = self.tbPlayerDamage[nCasterId] + nDamage;
		self.nBossDamagedValue = self.nBossDamagedValue + nDamage;

		--通知所有活动玩家概率改变
		for nPlayerId, nDamage in pairs(self.tbPlayerDamage) do
			if nDamage > 0 then
				local giftPercent = 0.0;
				if self.nBossDamagedValue > 0 then
					giftPercent = nDamage * 1.0 / self.nBossDamagedValue * 100.0;
				end
				self:SetPlayerActivityInfo(nPlayerId, nil, string.format(Lang.mission.str39[Lang.Idx], nDamage, giftPercent).."%");
			end
		end
	end
end

function KUAFUBOSS_SAICHANG:ClearPlayerDamage(nPlayerId)
	if self.tbPlayerDamage[nPlayerId] and self.tbPlayerDamage[nPlayerId] > 0 then
		if self.nBossDamagedValue and self.nBossDamagedValue >= self.tbPlayerDamage[nPlayerId] then
			self.nBossDamagedValue = self.nBossDamagedValue - self.tbPlayerDamage[nPlayerId];
		end
		self.tbPlayerDamage[nPlayerId] = 0;
	end
end

function KUAFUBOSS_SAICHANG:ResetData()
	local pBossNpc = KGameBase.GetNpcById(self.nBossNpcId);
	if pBossNpc and pBossNpc.IsDead() ~= 1 then
		KGameBase.RemoveNpc(pBossNpc.dwId);
	end
	self.nBossNpcId = nil;
	self.nBossDamagedValue = 0;
	self.tbPlayerDamage = {};
end

function KUAFUBOSS_SAICHANG:OnEndStep()
	self:ResetData();
end;

--发奖励
function KUAFUBOSS_SAICHANG:OnSendAward()
	if not self.nBossNpcId then 
		return;
	end
	
	local nRandom = KUnify.MathRandom(0, self.nBossDamagedValue);
	local nTargetId = nil;
	local nCurDamageValue = 0;
	for nPlayerId, nDamage in pairs(self.tbPlayerDamage) do
		if nDamage > 0 then
			if nRandom >= nCurDamageValue and nRandom <= nCurDamageValue + nDamage then
				nTargetId = nPlayerId;
				break;
			end
			nCurDamageValue = nCurDamageValue + nDamage;
		end
	end
	
	if not nTargetId then 
		return 
	end;
	
	local pPlayer = KGameBase.GetPlayerById(nTargetId);
	
	if not pPlayer then	
		return 
	end;
	
	local nTeamId = pPlayer.GetTeamId();
	-- 发放奖励
	if nTeamId > 0 then
		local tbTeamMember = KTeam.GetTeamMemberList(nTeamId) or {};
		local tbRankPlayer = {};
		for _, nPlayerId in pairs(tbTeamMember) do
			if self.tbPlayerDamage[nPlayerId] then --and self.tbPlayerDamage[nPlayerId] > 0 then
			  local tbRank = {};
			  tbRank.nPlayerId = nPlayerId;
			  tbRank.nHarm = self.tbPlayerDamage[nPlayerId];
			  table.insert(tbRankPlayer, tbRank);
				--tbRankPlayer[nPlayerId] = self.tbPlayerDamage[nPlayerId];
			end
		end
		--伤害值排序
		local sortFunc = function(a,b) return b.nHarm < a.nHarm end;
		table.sort(tbRankPlayer, sortFunc);
		--发放队伍奖励
		local rank = 1;
		for key, tbRank in pairs(tbRankPlayer) do
			print(string.format("世界boss第%d名:%d, 伤害值:%d", rank, key, tbRank.nHarm));
			local pPlayer = KGameBase.GetPlayerById(tbRank.nPlayerId);
			local awardLevel = (self.nBossTemplateId - WorldBossStartTemplateId) * WorldBossAwardRank + rank;
			KAward.AddPlayerAward(pPlayer, "WorldBoss", awardLevel);
			rank = rank + 1;
		end
	else
		local awardLevel = (self.nBossTemplateId - WorldBossStartTemplateId) * WorldBossAwardRank + 1;
		KAward.AddPlayerAward(pPlayer,"WorldBoss", awardLevel);
	end
	
	KChat.BroadcastNoticeMsg(string.format(Lang.mission.str40[Lang.Idx],pPlayer.szName,self:GetBossName()));
end

function KUAFUBOSS_SAICHANG:GetBossName()
	if self.nBossTemplateId == 2507 then
		return Lang.mission.str41[Lang.Idx];
	elseif self.nBossTemplateId == 2508 then
		return Lang.mission.str42[Lang.Idx];
	elseif self.nBossTemplateId == 2509 then
		return Lang.mission.str42[Lang.Idx];
	elseif self.nBossTemplateId == 2510 then
		return Lang.mission.str43[Lang.Idx];
	else
		return Lang.mission.str45[Lang.Idx];
	end
end

function KUAFUBOSS_SAICHANG:OnNpcDeath(pKiller, pNpc)
	assert(pKiller and pNpc);
	if pNpc.IsNpc() ~= 1 then
		return;
	end
	
	local tbPlayerIds = {};
	for _, tbPlayer in pairs(self.tbPlayers) do
		table.insert(tbPlayerIds, tbPlayer.nPlayerId);
	end
	
	local mPos = pNpc.GetPosition();
	Npc:DropItems(pNpc.dwTemplateId, pKiller.dwId, mPos, tbPlayerIds);
end
