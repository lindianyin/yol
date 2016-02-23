
MakeUp.tbMakeUp = {1, {{ year=2014, month=9, day=12}, { year=2014, month=9, day=13}}};

function MakeUp:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	--self:CheckCanGiveAward(pPlayer);
end

function MakeUp:CheckCanGiveAward(pPlayer)
	local tLastTakeTime = pPlayer.GetLastTakeMakeupTime();
	local tStartTime = Lib:GetSecByTimeTable(self.tbMakeUp[2][1]);		-- 领补偿开始时间点
	local tEndTime = Lib:GetSecByTimeTable(self.tbMakeUp[2][2]);			-- 领补偿结束时间点
	
	if tLastTakeTime >= tStartTime and tLastTakeTime <= tEndTime then
		return;
	end
	
	if pPlayer.GetLevel() <= 1 then
		return;
	end
	
	--KAward.AddPlayerAward(pPlayer, "MakeUp", MakeUp.tbMakeUp[1]);
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, 5000);
	
	pPlayer.SysMsg(Lang.makeup.str1[Lang.Idx]);
	
	local tNow = KGameBase.GetTime();
	pPlayer.SetLastTakeMakeupTime(tNow);
end


CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, MakeUp.OnLogin, MakeUp);
