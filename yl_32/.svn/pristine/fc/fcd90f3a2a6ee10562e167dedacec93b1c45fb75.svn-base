Include("script/mission/activity/activitystep.lua")
Include("script/mission/activity/activitydef.lua")

Activity.KUAFUBOSS_ZHUNBEICHANG = Lib:NewClass(Activity.ActivityStep);

Activity.ACTSTEP_FACTORY[Activity.STEP_TYPE.KUAFUBOSS_ZHUNBEICHANG] = Activity.KUAFUBOSS_ZHUNBEICHANG -- 注册到活动工厂

local KUAFUBOSS_ZHUNBEICHANG = Activity.KUAFUBOSS_ZHUNBEICHANG

--[[
function KUAFUBOSS_ZHUNBEICHANG:OnCharacterLeaveScene(nCharacterId)
	local pPlayer = KGameBase.GetPlayerById(nCharacterId);
	if pPlayer and pPlayer.dwSceneTemplateId == self.tbPos[1] then
	    self:KickPlayer(nCharacterId, 1)
	end
	print("OnCharacterLeaveScene", pPlayer.dwSceneTemplateId, self.tbPos[1]);
	--print("跨服boss中离开场景不做处理", nCharacterId, pPlayer.dwSceneTemplateId, self.tbPos[1]);
end]]--