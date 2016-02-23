
function Arena:GiveAward(pPlayer, nAwardId)
	local nAwardId = KAward.AddPlayerAward(pPlayer, "Arena", nAwardId);
	return nAwardId;
end

function Arena:GetArenaRankAwardId(pPlayer)
	local nArenaRank = pPlayer.GetArenaRank();
	
	if nArenaRank <= 0 then
		return 0;
	end
	
	local nAwardId = nArenaRank;
	if nArenaRank > 500 then
		nAwardId = 501;
	end
	
	return nAwardId;
end

function Arena:GetWinComboAwardId(nWinCombo)
	return 3 + math.floor(nWinCombo / 5);
end

function Arena:BroadcastWinnerInfo(szMsg)
    KChat.BroadcastNoticeMsg(szMsg)
end

function Arena:GiveRankAward(pPlayer)
	local nAwardId = self:GetArenaRankAwardId(pPlayer);
	
	if nAwardId == 0 then
		return;
	end
	
	local nArenaRank = pPlayer.GetArenaRank();
	
	KAward.AddRankAward(pPlayer, "ArenaRank", nAwardId, nArenaRank);
	
	local tNow = KGameBase.GetTime();
	pPlayer.pArena.SetTakeAwardTime(tNow);
	
	pPlayer.pArena.SyncArenaInfo();
end

function Arena:WorldGiveRankAwards()
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		self:GiveRankAward(pPlayer);
	end
end

function Arena:WorldResetArena()
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		pPlayer.pArena.ResetData();
	end
end

function Arena:CheckRankAward(pPlayer, dwLastTakeTime)
	local dwNow = KGameBase.GetTime();
	local dwLastLogoutTime = pPlayer.GetLastLogoutTime();
	
	if (pPlayer.GetLevel() < 18) then  -- 未到开放等级，不处理
	    return
	end
	
	local hoursLast = Lib:GetLocalHour(dwLastLogoutTime);
	local hoursNow = Lib:GetLocalHour(dwNow);
	local hourLastLogout = hoursLast % 24;
	local awardNeedHoursDiff = 0;
	
	-- 9点之前下线的话，时间差大于9点与下线时间的小时数即可领取,否则得加上24小时
	if hourLastLogout < 21 then  
		awardNeedHoursDiff = 21 - hourLastLogout;
	else
		awardNeedHoursDiff = 45 - hourLastLogout;
	end

  print("CheckRankAward", hoursNow, hoursLast, hourLastLogout, hoursNow % 24, awardNeedHoursDiff)
	if (hoursNow - hoursLast) >=  awardNeedHoursDiff then
		self:GiveRankAward(pPlayer);
	end

	-- 连胜奖励记录清0
	if Lib:GetLocalDay(dwNow) ~= Lib:GetLocalDay(dwLastLogoutTime) then
		pPlayer.pArena.ResetData();
	end
end

function Arena:c2s_TakeComboAward(pPlayer)
	local nWinCombo = pPlayer.pArena.GetWinCombo();		-- 当前连胜次数
	
	if nWinCombo % 5 == 0 then
		if nWinCombo / 5 == 1 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str5[Lang.Idx],pPlayer.szName))
		    --KChat.BroadcastNoticeMsg("英雄出少年, <"..pPlayer.szName..">在竞技场中已经取得了5连胜！");
		elseif nWinCombo / 5 == 2 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str6[Lang.Idx],pPlayer.szName))
		    --KChat.BroadcastNoticeMsg("英雄出少年, <"..pPlayer.szName..">在竞技场中已经取得了10连胜！");
		elseif nWinCombo / 5 == 3 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str7[Lang.Idx],pPlayer.szName))
		    --KChat.BroadcastNoticeMsg("宝剑锋从磨砺出, <"..pPlayer.szName..">在竞技场中已经取得了15连胜！");
		elseif nWinCombo / 5 == 4 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str8[Lang.Idx],pPlayer.szName))
		    --KChat.BroadcastNoticeMsg("宝剑锋从磨砺出, <"..pPlayer.szName..">在竞技场中已经取得了20连胜！");
		elseif nWinCombo / 5 == 5 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str9[Lang.Idx],pPlayer.szName))
		    --KChat.BroadcastNoticeMsg("<"..pPlayer.szName..">在竞技场如入无人之境,已经取得了惊人的25连胜！");
		elseif nWinCombo / 5 >= 6 then
			 KChat.BroadcastNoticeMsg(string.format(Lang.arena.str10[Lang.Idx],pPlayer.szName,nWinCombo))
		    --KChat.BroadcastNoticeMsg("<"..pPlayer.szName..">在竞技场如入无人之境,已经取得了惊世骇俗的"..nWinCombo.."连胜！");
		end
  end
	
	local nCurComboAward = pPlayer.pArena.GetTakeComboAward();		-- 当前角色已经领过的连胜奖励场次
	if nCurComboAward == self.MAX_WIN_COMBO_AWARD then
		-- 全领完了
		return;
	end
	local nNextComboAward = self.tbComboAward[nCurComboAward];
	assert(nNextComboAward);

	print("Arena:c2s_TakeComboAward", nWinCombo, nNextComboAward, nWinComboAwardId)
	if nWinCombo >= nNextComboAward then
		local nWinComboAwardId = self:GetWinComboAwardId(nNextComboAward);
		assert(nWinComboAwardId);
		self:GiveAward(pPlayer, nWinComboAwardId);
		pPlayer.pArena.SetTakeComboAward(nNextComboAward);
	end
end