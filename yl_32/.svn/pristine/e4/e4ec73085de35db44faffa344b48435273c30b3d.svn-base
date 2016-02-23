
function Rank:RefreshRoleRank()
	KRank.RefreshRoleRank();
end

function Rank:RefreshPetLvRank()
	KRank.RefreshPetLvRank();
end

function Rank:RefreshPetFSRank()
	KRank.RefreshPetFSRank();
end

function Rank:RefreshRideRank()
	KRank.RefreshRideRank();
end

function Rank:RefreshMpjjRank()
	KRank.RefreshMpjjRank();
end

function Rank:RefreshLGRank()
	KRank.RefreshLGRank();
end

function Rank:RefreshSmzcRank()
	KRank.RefreshSmzcRank();
end

function Rank:RefreshTongFSRank()
	KRank.RefreshTongFSRank();
end

function Rank:RefreshTongSrRank()
	KRank.RefreshTongSrRank();
end	

function Rank:SaveAllPlayers()
	KGameBase.SaveAllPlayers();
end

function Rank:RefreshAllRank()
	KRank.RefreshAllRank();
end

function Rank:RefreshAllRankByOneMinute() 
	local nNow = os.time()
	if(nNow % 60  == 0) then 
		--print("..Rank:RefreshAllRankByOneMinute()")
		Rank:RefreshRoleRank();
		Rank:RefreshPetLvRank();
		Rank:RefreshPetFSRank();
		Rank:RefreshRideRank();
		Rank:RefreshMpjjRank();
		Rank:RefreshLGRank();
		Rank:RefreshSmzcRank();
		Rank:RefreshTongFSRank();
		Rank:RefreshTongSrRank();
		Rank:RefreshAllRank(); -- 开服活动排行榜
	end
end
function Rank:GiveRoleRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.ROLE_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return;
	end
	
	Title:AddTitleToCharacter(pPlayer, 15, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.ROLE_RANK, tNow);
end

function Rank:GivePetLvRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.PET_LV_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 13, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.PET_LV_RANK, tNow);
end

function Rank:GivePetFSRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.PET_FIGHTSCORE_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 14, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.PET_FIGHTSCORE_RANK, tNow);
end

function Rank:GiveRideRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.RIDE_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 16, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.RIDE_RANK, tNow);
end

function Rank:GiveSmzcRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.SMZC_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 12, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.SMZC_RANK, tNow);
end

function Rank:GiveMpjjRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 16 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.MPJJ_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 17, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.MPJJ_RANK, tNow);
end

function Rank:GiveLGRankAward(nPlayerId, nRankIdx)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if nRankIdx < 0 or nRankIdx >= 10 then
		return;
	end
	
	local tNow = KGameBase.GetTime();
	local tLastTakeAwardTime = KRank.GetLastTakeAwardTime(pPlayer, Rank.LG_RANK);
	
	if tLastTakeAwardTime == 0 then
		tLastTakeAwardTime = KGameBase.GetServerStartTime();
	end
	
	if tNow - tLastTakeAwardTime < Rank.RANK_AWARD_SPEN then
		return	
	end
	
	Title:AddTitleToCharacter(pPlayer, 11, nRankIdx + 1, 0, Lib:GetLocalDayTime());
	KRank.SetLastTakeAwardTime(pPlayer, Rank.LG_RANK, tNow);
end

function Rank:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	-- 给各个排行榜相关奖励
	for nRankId = Rank.ROLE_RANK, Rank.SMZC_RANK do
		local nRankIdx = KRank.GetMyRank(pPlayer, nRankId);
		if nRankId == Rank.ROLE_RANK then
			Rank:GiveRoleRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.PET_LV_RANK then
			Rank:GivePetLvRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.PET_FIGHTSCORE_RANK then
			Rank:GivePetFSRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.RIDE_RANK then
			Rank:GiveRideRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.MPJJ_RANK then
			Rank:GiveMpjjRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.LG_RANK then
			Rank:GiveLGRankAward(nPlayerId, nRankIdx);
		elseif nRankId == Rank.SMZC_RANK then
			Rank:GiveSmzcRankAward(nPlayerId, nRankIdx);
		end
	end
end

function Rank:CheckRefreshRanks()
	--print("...............Rank:CheckRefreshRanks()")
	local tNow = KGameBase.GetTime();
	for nIdx = self.ROLE_RANK, self.TONG_SALARY_RANK do
		local tbInfo = self.tbRefreshTime[nIdx];
		if tbInfo then
			local tRefreshTime = Lib:GetSecByTimeTable(tbInfo[1]);
			if tRefreshTime == tNow then
				Lib:CallBack({tbInfo[2]});
			end
		end
	end
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Rank.OnLogin, Rank);

Timer:Register(Env.GAME_FPS, Rank.CheckRefreshRanks, Rank)
Timer:Register(Env.GAME_FPS, Rank.RefreshAllRankByOneMinute, Rank)
