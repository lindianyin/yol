Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.MANHUANGZHIDI = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.MANHUANGZHIDI] = Activity.MANHUANGZHIDI -- 注册到活动工厂

local MANHUANGZHIDI = Activity.MANHUANGZHIDI

local nModeSwitchCD = 180;	--模式切换

local tbManHuangZhiDiBoss = Activity.tbManHuangZhiDiBoss or {};

local nBossStartId = 2807;
local nBossEndId = 2813;

local tbMsg = {
	[2807] = Lang.mission.str46[Lang.Idx], 
	[2808] = Lang.mission.str47[Lang.Idx], 
	[2809] = Lang.mission.str48[Lang.Idx], 
	[2810] = Lang.mission.str49[Lang.Idx], 
	[2811] = Lang.mission.str50[Lang.Idx], 
	[2812] = Lang.mission.str51[Lang.Idx], 
	[2813] = Lang.mission.str52[Lang.Idx], 
	
};

local tbKillBossMsg = {
	[2807] = Lang.mission.str53[Lang.Idx], 
	[2808] = Lang.mission.str54[Lang.Idx], 
	[2809] = Lang.mission.str55[Lang.Idx], 
	[2810] = Lang.mission.str56[Lang.Idx], 
	[2811] = Lang.mission.str57[Lang.Idx], 
	[2812] = Lang.mission.str58[Lang.Idx], 
	[2813] = Lang.mission.str59[Lang.Idx], 
	
};

local tbExpId = {
	[353] = 10,
	[354] = 11,
	[355] = 12,
	[356] = 13,
}

MANHUANGZHIDI.KILL_PLAYER_GID = 5005;
MANHUANGZHIDI.KILL_PLAYER_COUNT_ID = 1;
MANHUANGZHIDI.MINUS_ATTACK_ID = 2;
MANHUANGZHIDI.EVIL_VALUE_ID = 3;

MANHUANGZHIDI.ADD_SPIRIT_NUM = 1000;

function MANHUANGZHIDI:OnInit()
	self.tbPlayerUpdateTime = {};
end

function MANHUANGZHIDI:OnJoinPlayer(nPlayerId, nGroup)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	--Player:SetTempReviveFunc(pPlayer, {"Activity.MANHUANGZHIDI:OnDeath", pPlayer});
    if pPlayer.IsDead() == 1 then  -- 如果角色死亡，自动重生
		--进入擂台赛，自动满血
		KCharacter.Revive(pPlayer, 1000, 1000);
	else
		pPlayer.SetCurrentCamp(1);	
	end
	
	local nKillPlayerCount = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID) or 0;
	local nEvilValue = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID) or 0;
	local nMinusPercent = 0;
	local nChangeNameColor = pPlayer.GetChangeNameColor();
	if nKillPlayerCount >= 10 and (nEvilValue >= 1000 or nChangeNameColor > 0) then
		nMinusPercent = math.min(50, (nKillPlayerCount - 9) * 5);
		pPlayer.SetChangeNameColor(1);
	end
	local nMinusValue = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID) or 0;
	local nAttack = pPlayer.GetAttack() + nMinusValue;
	local nBladeAttack = 0;
	local nAngerLeftTime = pPlayer.GetAngerLeftTime(); -- 大神状态下
	if nAngerLeftTime > 0 then
		local tbBladeInfo = pPlayer.GetBladeInfo();
		if tbBladeInfo ~= nil then
			nBladeAttack = tbBladeInfo.nAttack;
			nAttack = nAttack - nBladeAttack;
		end
	end
	nMinusValue = nAttack * nMinusPercent / 100;
	pPlayer.SetAttack(nMinusValue * (-1) + (nAttack + nBladeAttack));
	pPlayer.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID, nMinusValue);
	
	self:SetActivityInfo(nPlayerId, nEvilValue, nMinusValue);
	
	--self:SetPlayerActivityInfo(nPlayerId, nil, string.format("\\r<color=yellow>模  式: <color=green>和平模式 \\r<color=yellow>恶名值: <color=red>%d \\r<color=yellow>攻击力: <color=red>减少%d攻击力", nEvilValue,nMinusValue));
end

function MANHUANGZHIDI:SetActivityInfo(nPlayerId, nEvilValue, nMinusValue)
	local msg = Lang.mission.str60[Lang.Idx];
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local nCurrentCamp = pPlayer.GetCurrentCamp();
	local nEnableEnemyCondition = pPlayer.GetEnableEnemyCondition();
	local nEnemyCondition = pPlayer.GetEnemyCondition1()-- + Activity.Type_Not;
	if nCurrentCamp == 1 and nEnableEnemyCondition == 1 then
		if nEnemyCondition == Activity.Type_Kin + Activity.Type_Team + Activity.Type_Kin_Enable + Activity.Type_Team_Enable - Activity.Type_Not then
			msg = Lang.mission.str61[Lang.Idx];
		--elseif nEnemyCondition == Activity.Type_Self + Activity.Type_Self_Enable - Activity.Type_Not then
		--	msg = "<color=green>pk模式";
		end
	elseif nCurrentCamp == 8 then
		msg = Lang.mission.str62[Lang.Idx];
	elseif nCurrentCamp == 9 then
		msg = Lang.mission.str63[Lang.Idx];
	end
	
	local nChangeNameColor = pPlayer.GetChangeNameColor();
	if nChangeNameColor ~= 255 then
		if nMinusValue > 0 then
			pPlayer.SetChangeNameColor(1);
		else
			pPlayer.SetChangeNameColor(0);
		end
	end
	self:SetPlayerActivityInfo(nPlayerId, nil, string.format(Lang.mission.str64[Lang.Idx], msg, nEvilValue,nMinusValue));
end


function MANHUANGZHIDI:OnKickPlayer(nPlayerId)
	--恢复和平模式
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		pPlayer.SetEnableEnemyCondition(0);
		pPlayer.SetEnemyCondition1(0);
		pPlayer.SetCurrentCamp(1);	
	end
	self.tbPlayerUpdateTime[nPlayerId] = nil;
	
	local nAttack = pPlayer.GetAttack();
	local nMinusValue = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID) or 0;
	pPlayer.SetAttack(nMinusValue + nAttack);
	pPlayer.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID, 0);
	local nChangeNameColor = pPlayer.GetChangeNameColor();
	if nChangeNameColor == 1 then
		pPlayer.SetChangeNameColor(255);
	end
	pPlayer.LeaveTeam();
	self:SetPlayerActivityInfo(nPlayerId, nil, "");
end

--设置单个玩家的战斗关系
function MANHUANGZHIDI:SetPlayerFightMode(pPlayer, nMode)
	local dwNow = KGameBase.GetTime();
	
	if pPlayer.GetIsSetFightMode() == 0 then
		pPlayer.SendBlackMsg(Lang.mission.str65[Lang.Idx]);
		return;
	end
	
	if self.tbPlayerUpdateTime[pPlayer.dwId] and self.tbPlayerUpdateTime[pPlayer.dwId]  + nModeSwitchCD > dwNow then
		pPlayer.SendBlackMsg(Lang.mission.str66[Lang.Idx]);
		return;
	end
	
	
	if nMode == 1 then	--和平模式
		pPlayer.SetCurrentCamp(1);	
		pPlayer.SetEnableEnemyCondition(0);
		pPlayer.SetEnemyCondition1(0);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetPlayerFightMode", nMode});
	elseif nMode == 2 then --家族模式
		if pPlayer.GetKinId() ~= 0 then
			local nEnemy = Activity.Type_Kin + Activity.Type_Team + Activity.Type_Kin_Enable + Activity.Type_Team_Enable + Activity.Type_Not;
			pPlayer.SetCurrentCamp(1);	
			pPlayer.SetEnableEnemyCondition(1);
			pPlayer.SetEnemyCondition1(nEnemy);
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetPlayerFightMode", nMode});
		else
			pPlayer.SendBlackMsg(Lang.mission.str67[Lang.Idx]);
		end
	elseif nMode == 3 then --pk模式
		local nEnemy = Activity.Type_Self + Activity.Type_Self_Enable + Activity.Type_Not;
		pPlayer.SetCurrentCamp(9);	
		pPlayer.SetEnableEnemyCondition(1);
		pPlayer.SetEnemyCondition1(nEnemy);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetPlayerFightMode", nMode});
	elseif nMode == 4 then --屠杀模式
		local nEnemy = Activity.Type_All + Activity.Type_Self_Enable + Activity.Type_Not;
		pPlayer.SetCurrentCamp(8); -- 屠杀阵营
		pPlayer.SetEnableEnemyCondition(1);
		pPlayer.SetEnemyCondition1(nEnemy);
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetPlayerFightMode", nMode});
		
		print("Player CurrentCamp   ", pPlayer.GetCurrentCamp());
	end
	
	local nMinusValue = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID) or 0;
	local nEvilValue = pPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID) or 0;
	self:SetActivityInfo(pPlayer.dwId, nEvilValue, nMinusValue);
	self.tbPlayerUpdateTime[pPlayer.dwId] = dwNow;
end

function MANHUANGZHIDI:OnEndStep()
	self.tbPlayerUpdateTime = {};
	tbManHuangZhiDiBoss = {};
end

function MANHUANGZHIDI:OnAddNpc(tbNpcId, nTemplateId)
	assert(nTemplateId);
	
	--boss
	if nBossStartId <= nTemplateId and nTemplateId <= nBossEndId then
		local nSceneId = 0;
		for _, nNpcId in pairs(tbNpcId) do
			local pNpc = KGameBase.GetNpcById(nNpcId);
			nSceneId = pNpc.dwSceneTemplateId;
			break;
		end
		
		if tbManHuangZhiDiBoss[nSceneId] == nil then
			tbManHuangZhiDiBoss[nSceneId] = {bossCount = 0};
		end
		
		tbManHuangZhiDiBoss[nSceneId].bossCount = tbManHuangZhiDiBoss[nSceneId].bossCount + 1;
		KGameBase.CenterExecute{"Activity:UpdateManHuangZhiDi_GC", nSceneId, tbManHuangZhiDiBoss[nSceneId]};
		
		KChat.BroadcastNoticeMsg(tbMsg[nTemplateId]);
	end
end

function MANHUANGZHIDI:OnPlayerDeath(nDeathCharId, nKillerCharId)
	local pKiller = KGameBase.GetPlayerById(nKillerCharId);
	if pKiller == nil then
		return;
	end
	
	local pDeathPlayer = KGameBase.GetPlayerById(nDeathCharId);
	
	local nDeathKillCount = pDeathPlayer.pTask.GetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID) or 0;
	local nDeathCurrentCamp = pDeathPlayer.GetCurrentCamp();
	local nKillerKillCount = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID) or 0;
	local nKillerCurrentCamp = pKiller.GetCurrentCamp();
	
	if nDeathKillCount >= 10 and nKillerKillCount < 10 and nKillerCurrentCamp ~= 8 then
		pKiller.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, self.ADD_SPIRIT_NUM);
	end
	
	if nDeathCurrentCamp ~= 8 and nKillerCurrentCamp == 8 then
		local nKillerEvilValue = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID) or 0;
		nKillerEvilValue = math.min(1000, nKillerEvilValue + 100);
		pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID, nKillerEvilValue);
		
		local nKillPlayerCount = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID) or 0;
		--local nEvilValue = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID) or 0;
		local nMinusPercent = 0;
		nKillPlayerCount = nKillPlayerCount + 1;
		local nChangeNameColor = pKiller.GetChangeNameColor();
		if nKillPlayerCount >= 10 and (nKillerEvilValue >= 1000 or nChangeNameColor > 0) then
			nMinusPercent = math.min(50, (nKillPlayerCount - 9) * 5);
		end
		local nMinusValue = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID) or 0;
		local nAttack = pKiller.GetAttack() + nMinusValue;
		local nAngerLeftTime = pKiller.GetAngerLeftTime(); -- 大神状态下
		local nBladeAttack = 0;
		if nAngerLeftTime > 0 then
			local tbBladeInfo = pKiller.GetBladeInfo();
			if tbBladeInfo ~= nil then
				nBladeAttack = tbBladeInfo.nAttack;
				nAttack = nAttack - nBladeAttack;
			end
		end
		nMinusValue = nAttack * nMinusPercent / 100;
		pKiller.SetAttack(nMinusValue * (-1) + (nAttack + nBladeAttack));
		pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID, nMinusValue);
		pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID, nKillPlayerCount);
		
		self:SetActivityInfo(nKillerCharId, nKillerEvilValue, nMinusValue);
	end
end

function MANHUANGZHIDI:OnNpcDeath(pKiller, pNpc)
	print("..MANHUANGZHIDI:OnNpcDeath", pKiller.dwSceneTemplateId, pNpc.dwSceneTemplateId)
	local nSceneId = pNpc.dwSceneTemplateId;
	local nLevel = pKiller.GetLevel();
	--local nExp = KAward.GetExpByLevel(tbExpId[nSceneId], nLevel + 1);
	local nPropId = KCharacter.GetNpcPropId(pNpc.dwTemplateId);
	local nExp = KCharacter.GetNpcExp(nPropId,pNpc.GetLevel())
	--local nExp = pNpc.GetExp();
	--KChat.BroadcastNoticeMsg("nExp:" .. nExp)
	--nExp = math.floor(nExp / 100);
	
	--组队加成5%的经验
	local tbTeamMember = {};
	local nTeamId = pKiller.GetTeamId();
	local nTeamPlayerCount = 0;
	if nTeamId > 0 then
		tbTeamMember = KTeam.GetTeamMemberList(nTeamId);
		for _, nPlayerId in pairs(tbTeamMember) do
			nTeamPlayerCount = nTeamPlayerCount + 1;
		end
		if nTeamPlayerCount > 1 then
			nExp = nExp * 1.05;
		end
	end
	
	pKiller.AddExp(nExp);
	
	local tbPlayerIds = {};
--	if nTeamId > 0 and nBossStartId <= pNpc.dwTemplateId and pNpc.dwTemplateId <= nBossEndId then
--		for _, nPlayerId in pairs(tbTeamMember) do
--			table.insert(tbPlayerIds, nPlayerId);
--		end
--	else
--		table.insert(tbPlayerIds, pKiller.dwId);
--	end
    local tbPlayers = KGameBase.GetAllPlayers()
    local nCount = 0;
	for _, pPlayer in pairs(tbPlayers) do
		local nMapId = pPlayer.dwSceneTemplateId;
		if(nMapId == pNpc.dwSceneTemplateId) then 
			table.insert(tbPlayerIds, pPlayer.dwId);
			nCount = nCount + 1;
		end
	end
	print("nCount",nCount);
	

	local mPos = pNpc.GetPosition();
	Npc:DropItems(pNpc.dwTemplateId, pKiller.dwId, mPos, tbPlayerIds);
	
	if nBossStartId <= pNpc.dwTemplateId and pNpc.dwTemplateId <= nBossEndId then
		if tbManHuangZhiDiBoss[nSceneId] == nil then
			tbManHuangZhiDiBoss[nSceneId] = {bossCount = 0};
		end
		
		tbManHuangZhiDiBoss[nSceneId].bossCount = math.max(0,tbManHuangZhiDiBoss[nSceneId].bossCount - 1);
		KGameBase.CenterExecute{"Activity:UpdateManHuangZhiDi_GC", nSceneId, tbManHuangZhiDiBoss[nSceneId]};
		
		KChat.BroadcastNoticeMsg(string.gsub(tbKillBossMsg[pNpc.dwTemplateId], "name", pKiller.szName));
	end
	
	local nEvilValue = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID) or 0;
	nEvilValue = math.max(0, nEvilValue - 1);
	pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.EVIL_VALUE_ID, nEvilValue);
	local nMinusValue = pKiller.pTask.GetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID) or 0;
	if nEvilValue <= 0 then
		local nAttack = pKiller.GetAttack();
		pKiller.SetAttack(nMinusValue + nAttack);
		pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.MINUS_ATTACK_ID, 0);
		pKiller.pTask.DirectSetTask(self.KILL_PLAYER_GID, self.KILL_PLAYER_COUNT_ID, 0);
		nMinusValue = 0;
	end
	
	self:SetActivityInfo(pKiller.dwId, nEvilValue, nMinusValue);
end


