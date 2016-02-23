Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.KIN_PANTAOSHENGYAN = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.KIN_PANTAOSHENGYAN] = Activity.KIN_PANTAOSHENGYAN -- 注册到活动工厂

local KIN_PANTAOSHENGYAN = Activity.KIN_PANTAOSHENGYAN

function KIN_PANTAOSHENGYAN:OnProgressBarFinished(nPlayerId, dwNpcId)
  if KGameBase.GetNpcById(dwNpcId) ~= nil then
		KGameBase.RemoveNpc(dwNpcId);
	end
	
	if self.IsOpen == 0 then
		return;
	end
	
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer and pPlayer.GetKinId() ~= 0 and self:IsPlayerIn(nPlayerId) ~= 0 then
		local tbKinInfo = GetPlayerKinInfo(nPlayerId);
		if tbKinInfo then
			KAward.AddPlayerAward(pPlayer, "PTSY", tbKinInfo.nKinLevel);
		end
		pPlayer.SendBlackMsg(Lang.mission.str36[Lang.Idx]);
	end
end

function KIN_PANTAOSHENGYAN:OnProgressBarCancel(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if pPlayer then
		pPlayer.SendBlackMsg(Lang.mission.str37[Lang.Idx]);
	end
end

function KIN_PANTAOSHENGYAN:OnEndStep()
	--发放蟠桃盛宴参与奖励
	for _, tbPlayer in pairs(self.tbPlayers) do
		local pPlayer = KGameBase.GetPlayerById(tbPlayer.nPlayerId);
		local tbPlayerKinInfo = GetPlayerKinInfo(pPlayer.dwId);
		if tbPlayerKinInfo ~= nil and tbPlayerKinInfo.nKinLevel > 0 then
			KAward.AddPlayerAward(pPlayer, "JoinPTSY", tbPlayerKinInfo.nKinLevel);
		end
	end
end
