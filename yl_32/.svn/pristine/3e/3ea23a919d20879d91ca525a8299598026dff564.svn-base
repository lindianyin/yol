if not MODULE_GAMESERVER then
	return
end

Include("/script/player/define.lua")

function Login:OnPlayerLogin(dwPlayerId)
	local pPlayer = KGameBase.GetPlayerById(dwPlayerId)
	pPlayer.SysMsg("<color=yellow>server "..KGameBase.GetServerId()..".")
	Player:EnterLimitControl(pPlayer, dwPlayerId)
	
	Scene:AutoRecoverLife(dwPlayerId);
end

-- 关注上线事件
if Login.nPlayerLoginEventId then
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Login.nPlayerLoginEventId)
end
Login.nPlayerLoginEventId =
	CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Login.OnPlayerLogin, Login)
