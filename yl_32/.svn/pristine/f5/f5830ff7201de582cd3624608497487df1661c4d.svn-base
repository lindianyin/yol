Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")
Include("script/mission/activity/setting/activitysetting_zbt.lua")

Activity.ZHANBAOTANG_SAICHANG = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG] = Activity.ZHANBAOTANG_SAICHANG -- 注册到活动工厂

local ZHANBAOTANG_SAICHANG = Activity.ZHANBAOTANG_SAICHANG
local tbSteps = Activity.ActivitySetting[2].STEP

local BossStartTemplateId = 2530;	--boss起始ID，主要用于奖励结算
local BossAwardRank = 3;			--组队奖励最多3个人

function ZHANBAOTANG_SAICHANG:OnInit()
	self.nBossTemplateId = nil;
	self.nBossNpcId = nil;
	if self.nActivityIndex == 2 and tbSteps[self.nStepIndex] ~= nil and 
		tbSteps[self.nStepIndex].BossTemplateId ~= nil then
		self.nBossTemplateId = tbSteps[self.nStepIndex].BossTemplateId;
	end
	self.nWinnerId = nil;
end

--nTempalteId暂时无用
function ZHANBAOTANG_SAICHANG:OnAddNpc(tbNpcId, nTemplateId)
	assert(nTemplateId);
	for _, nNpcId in pairs(tbNpcId) do
		local pNpc = KGameBase.GetNpcById(nNpcId);
		if pNpc and pNpc.dwTemplateId == self.nBossTemplateId and self.nBossNpcId == nil then
			self.nBossNpcId = nNpcId;
			break; 
		end
	end
end

function ZHANBAOTANG_SAICHANG:OnJoinPlayer(nPlayerId, nGroup)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
    if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
		KCharacter.Revive(pPlayer, 1000, 1000);
	end
end

-- 清除伤害
function ZHANBAOTANG_SAICHANG:OnKickPlayer(nPlayerId)
	self:SetPlayerActivityInfo(nPlayerId, "", "");
end
 
--角色离开活动
function ZHANBAOTANG_SAICHANG:LeaveGame(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if pPlayer and pPlayer.dwSceneTemplateId == self.tbPos[1] then
	    self:KickPlayer(nCharacterId, 1);
		pPlayer.ReturnToLastCity();
		pPlayer.LeaveTeam();
	end
	print("OnCharacterLeaveActivity", pPlayer.dwSceneTemplateId, self.tbPos[1]);
end

function ZHANBAOTANG_SAICHANG:OnPlayerDamage(nCasterId, nInjuredId, nDamage)
	local pBossNpc = KGameBase.GetNpcById(nInjuredId);
	if pBossNpc and nInjuredId == self.nBossNpcId then
		local nBossCurrentLife = pBossNpc.GetCurrentLife();
		if nDamage >= nBossCurrentLife then
			nDamage = nBossCurrentLife;
			self.nWinnerId = nCasterId;
		end
	end
end


function ZHANBAOTANG_SAICHANG:ResetData()
	self.nWinnerId = nil;
	if not self.nBossNpcId then
		return;
	end
	local pBossNpc = KGameBase.GetNpcById(self.nBossNpcId);
	if pBossNpc and pBossNpc.IsDead() ~= 1 then
		KGameBase.RemoveNpc(pBossNpc.dwId);
	end
	self.nBossNpcId = nil;
end

function ZHANBAOTANG_SAICHANG:OnEndStep()
	self:ResetData();
end;

--角色死亡传出地图
function ZHANBAOTANG_SAICHANG:OnPlayerDeath(nDeathCharId, nKillerCharId)
	local pPlayer = KGameBase.GetPlayerById(nDeathCharId);
    if pPlayer.IsDead() == 1 then  -- 如果角色死亡，传出地图
		self:LeaveGame(nDeathCharId);
		--KCharacter.Revive(pPlayer, 300, 300);
	end
end

--发奖励
function ZHANBAOTANG_SAICHANG:OnSendAward()
	if not self.nBossNpcId  or not self.nWinnerId then 
		return;
	end
	local pPlayer = KGameBase.GetPlayerById(self.nWinnerId);
	if not pPlayer then
		return;
	end
	
	local nTeamId = pPlayer.GetTeamId();
	-- 发放奖励
	if nTeamId > 0 then
		local tbTeamMember = KTeam.GetTeamMemberList(nTeamId) or {};
				local tbTeamMember = KTeam.GetTeamMemberList(nTeamId) or {};
		local tbRankPlayer = {};
		for _, nPlayerId in pairs(tbTeamMember) do
			local pPlayer = KGameBase.GetPlayerById(nPlayerId);
			if pPlayer then
				tbRankPlayer[nPlayerId] = pPlayer.GetLevel();
			end
		end
		--等级排序
		local sortFunc = function(a,b) return b < a end;
		table.sort(tbRankPlayer, sortFunc);
		--发放队伍奖励
		local rank = 1;
		for key, value in pairs(tbRankPlayer) do
			local awardLevel = (self.nBossTemplateId - BossStartTemplateId) * BossAwardRank + rank;
		--	KAward.AddPlayerAward(key, "ZBTBoss", awardLevel);
			rank = rank + 1;
		end
	else
		local awardLevel = (self.nBossTemplateId - BossStartTemplateId) * BossAwardRank + 1;
		--KAward.AddPlayerAward(pPlayer,"ZBTBoss", awardLevel);
	end
end

function ZHANBAOTANG_SAICHANG:OnNpcDeath(pKiller, pNpc)
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
	
	local szSceneName = KScene.GetNameById(pKiller.dwSceneId)
	-- 公告
	if self.nBossTemplateId == pNpc.dwTemplateId then
		--KChat.BroadcastNoticeMsg(string.format("<"..pKiller.szName..">小队经历一番血战，将"..szSceneName.."的堂主斩落于马下！"));
		--KChat.BroadcastNoticeMsg(string.format(Lang.mission.str224[Lang.Idx],pKiller.szName,szSceneName));
	end
end

function ZHANBAOTANG_SAICHANG:GetBossName()
	if self.nBossTemplateId == 2530 then
		return Lang.mission.str225[Lang.Idx];
	elseif self.nBossTemplateId == 2531 then
		return Lang.mission.str226[Lang.Idx];
	elseif self.nBossTemplateId == 2532 then
		return Lang.mission.str227[Lang.Idx];
	else
		return Lang.mission.str228[Lang.Idx];
	end
end