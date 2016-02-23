if not (MODULE_GAMECENTER) then
	return
end

Activity.MHZD_MAP_MAX_PLAYER_COUNT = 20;

-- 神魔战场报名
function Activity:ApplyJoinMHZD_GC(nPlayerId, nMapId)
	local nPlayerCount = GetMapPlayerCount(nMapId);
	local isFull = 1;
	if nPlayerCount < Activity.MHZD_MAP_MAX_PLAYER_COUNT then
		isFull = 0;
		GlobalExecute({"Activity:DoJoinMHZD", nPlayerId, nMapId, isFull});
	else
		GlobalExecute({"Activity:DoJoinMHZD", nPlayerId, nMapId, isFull});
	end

end