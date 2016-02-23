if not MODULE_GAMECENTER then
	return
end

function GlobalData:GiveRankAward()
	print("给排名奖励");
	GlobalExecute({"Arena:WorldGiveRankAwards"});
	
	-- 公告第一名
	local dwRoleId = GetRankIdByIdx(Rank.ARENA_RANK, 0);
  local szName = GetRoleName(dwRoleId);
  if szName ~= nil then
  	local nConnectIndex = GetPlayerConnectIndex(dwRoleId);
  	GlobalExecute({"Arena:BroadcastWinnerInfo", string.format(Lang.gcscript.str1[Lang.Idx],szName)}, nConnectIndex);
  else
    print("竞技场第一名的角色找不到",  dwRoleId, szName)
  end
end

function GlobalData:ResetArena()
	print("重置竞技场");
	GlobalExecute({"Arena:WorldResetArena"});
end