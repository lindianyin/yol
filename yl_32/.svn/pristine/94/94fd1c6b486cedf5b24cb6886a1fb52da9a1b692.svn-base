

-- 家族战报名
function Activity:c2s_ApplyJoinKinBattle(pPlayer)
	local nKinId = pPlayer.GetKinId();
	if nKinId <= 0 then
		pPlayer.SendBlackMsg(Lang.mission.str27[Lang.Idx]);
		return;
	end
	
	local tbKinInfo = GetPlayerKinInfo(pPlayer.dwId);
	if tbKinInfo.nKinLevel < 2 then
		pPlayer.SendBlackMsg(Lang.mission.str28[Lang.Idx]);
		return;
	end
	
	if Activity:IfCanEnterActivity(pPlayer, 26, 1) ~= 1 then
		return;
	end
	
	KGameBase.CenterExecute{"Activity:ApplyJoinKinBattle_GC", pPlayer.dwId, nKinId};
end

function Activity:NoticeJoinKinBattle_GS(nPlayerId, nJoinNum, nHasJoined)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	KActivity.SyncKinBattleJoinNum(nPlayerId, nJoinNum, nHasJoined);
end

function Activity:OnGetKinBattleJoinNumRequest(pPlayer)
	local nKinId = pPlayer.GetKinId();
	
	KGameBase.CenterExecute{"Activity:OnGetApplyKinBattleNum_GC", pPlayer.dwId, nKinId};
end

function Activity:OnGetApplyKinBattleNum_GS(nPlayerId, nJoinNum, nHasJoined)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	-- 同步报名人数
	KActivity.SyncKinBattleJoinNum(nPlayerId, nJoinNum, nHasJoined);
end

-- 进入家族战
function Activity:c2s_ApplyEnterKinBattle(pPlayer)
	local nKinId = pPlayer.GetKinId();
	assert(nKinId > 0);
	KGameBase.CenterExecute{"Activity:ApplyEnterKinBattle_GC", pPlayer.dwId, nKinId};
end

function Activity:DoEnterKinBattle(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	Activity:RequestEnterActivity(pPlayer, 26, 1);
end
