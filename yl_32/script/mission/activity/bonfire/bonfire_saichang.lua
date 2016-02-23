Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.BONFIRE_SAICHANG = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.BONFIRE_SAICHANG] = Activity.BONFIRE_SAICHANG -- 注册到活动工厂

local BONFIRE_SAICHANG = Activity.BONFIRE_SAICHANG
--local tbBonfireInfo = Activity.tbBonfireInfo or {};

function BONFIRE_SAICHANG:OnInit()
	self.tbPlayerDamage = {};
	self.tbPlayerName = {};
	self.nWinnerId = -1;
	self.bAward = 0;	--是否已发奖励
end

function BONFIRE_SAICHANG:OnStartStep()
	self.nEndTime = KGameBase.GetTime() + self.nTime;
	Timer:Register(self.nTime * Env.GAME_FPS, self.TimeOut, self);

	KChat.BroadcastKinNoticeMsg(self.nKinId, Lang.mission.str25[Lang.Idx]);
	
	local tbBonfireInfo = Activity.tbBonfireInfo or {};
	if tbBonfireInfo == nil or tbBonfireInfo[self.nKinId] == nil then
		return;
	end
	local tbBonfire = tbBonfireInfo[self.nKinId];

	tbBonfire.onGoing = 1
	
	KGameBase.CenterExecute{"Activity:UpdateBonfireInfo_GC", self.nKinId, tbBonfire};

end

function BONFIRE_SAICHANG:OnJoinPlayer(nPlayerId, nGroup)

end

function BONFIRE_SAICHANG:SetGameProgress(nGameProgress, nPanelIndex)

end


function BONFIRE_SAICHANG:OnPlayerDamage(nCasterId, nInjuredId, nDamage)
end

function BONFIRE_SAICHANG:OnKickPlayer(nPlayerId)
end

function BONFIRE_SAICHANG:OnLeaveGame(nCharacterId)

end

function BONFIRE_SAICHANG:OnPlayerKill(nKillerCharId, nDeathCharId)

end

function BONFIRE_SAICHANG:OnEndStep()

	KChat.BroadcastKinNoticeMsg(self.nKinId, Lang.mission.str26[Lang.Idx]);
	
	local tbBonfireInfo = Activity.tbBonfireInfo or {};
	if tbBonfireInfo == nil or tbBonfireInfo[self.nKinId] == nil then
		return;
	end
	
	local tbPlayers = KGameBase.GetAllPlayers();
	for _, tbPlayer in pairs(tbPlayers) do
		local tbPlayerKinInfo = GetPlayerKinInfo(tbPlayer.dwId);
		print("篝火奖励......", tbPlayerKinInfo, tbPlayerKinInfo.nKinLevel, Activity:GetBonfireStatus(tbPlayer), tbPlayer.GetKinId(), self.nKinId)
		if tbPlayerKinInfo ~= nil and tbPlayerKinInfo.nKinLevel > 0 and Activity:GetBonfireStatus(tbPlayer) == 1 and  tbPlayer.GetKinId() == self.nKinId then
			KAward.AddPlayerAward(tbPlayer, "TongFire", tbPlayerKinInfo.nKinLevel + 1);
		end
	end
		
	local tbBonfire = tbBonfireInfo[self.nKinId];
	
	if tbBonfire and KScene.IsDynamicSceneLoad(tbBonfire.mapId) == 1 then
		local pNpc = KGameBase.GetNpcById(tbBonfire.npcId)
		print("npc......", pNpc);
		if pNpc then
			tbBonfire.useTorch = 0;	
			tbBonfire.onGoing = 0;
			KGameBase.RemoveNpc(tbBonfire.npcId);
			KGameBase.CenterExecute{"Activity:UpdateBonfireInfo_GC", self.nKinId, tbBonfire};
		end
	end
	
end

function BONFIRE_SAICHANG:OnDeath(pPlayer)
	
end

--发奖励
function BONFIRE_SAICHANG:SendAwards()

end
