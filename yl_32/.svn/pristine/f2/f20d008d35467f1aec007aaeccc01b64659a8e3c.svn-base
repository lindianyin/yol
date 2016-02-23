BufferMgr = BufferMgr or {};

function BufferMgr:OnLeaveScene(dwCharacterId)
	--BufferApi.RemoveGroundBufferByCasterId(dwCharacterId);
end

function BufferMgr:OnLogout(dwCharacterId)
	BufferApi.RemoveGroundBufferByCasterId(dwCharacterId);
end

function BufferMgr:OnUnloadScene(dwSceneId)
	BufferApi.RemoveGroundBufferBySceneId(dwSceneId);
end

function BufferMgr:OnEnterScene(dwCharacterId)
	local pCharacter = KGameBase.GetCharacterById(dwCharacterId);
	
	if pCharacter.IsNpc() == 1 then
		return;
	end
	
	-- 加载角色光环被动技能
	--BufferApi.ReloadPassiveSkill(dwCharacterId);
end


CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, BufferMgr.OnLogout, BufferMgr);
