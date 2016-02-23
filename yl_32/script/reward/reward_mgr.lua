-- 奖励

function Reward:GetNeedBagCell(tbAward)
	local nItemCellCount = 0;
	
	for i = 1, #tbAward.pItem do
		local tbItem = tbAward.pItem[i];
		local nStack, nAmount;
		-- 是装备
		if tbItem.nGenre == 1 then
			nStack = 1;
			nAmount = 1;
		else
			local tbProp = KItem.GetOtherBaseProp(tbItem.nGenre, tbItem.nDetailType, tbItem.nParticular, tbItem.nLevel);
			nStack = tbProp.nStackMax;
			if tbItem.nAmountMax then
			    nAmount = tbItem.nAmountMax;
			else
			    nAmount = tbItem.nAmount;
			end
		end
		
		nItemCellCount = nItemCellCount + math.ceil(nAmount / nStack);
	end
	
	return nItemCellCount;
end

function Reward:CanTakeAward(pPlayer, tbAward)
	assert(tbAward);
	
	local nItemCellCount = self:GetNeedBagCell(tbAward);
	
	if pPlayer.pItem.CountFreeBagCell() < nItemCellCount then
		pPlayer.SendBlackMsg(string.format(Lang.reward.str1[Lang.Idx], nItemCellCount));
		return 0;
	end
	
	return 1;
end

function Reward:CanTakeAwardById(pPlayer, nAwardId)
	local tbAward = KAward.GetAwardData(nAwardId);
	assert(tbAward);
	
	local nCanTake = self:CanTakeAward(pPlayer, tbAward);
	return nCanTake;
end

function Reward:TakeAward(pPlayer, nId)
	local tbAward = KAward.GetAwardData(nId);
	assert(tbAward);
	
	if self:CanTakeAward(pPlayer, tbAward) ~= 1 then
		return 0;
	end
	
	local nPercent = 100;
	-- 皇族神威加成
	if KinBattle:IsWinnerMember(pPlayer) and (tbAward.szModuleName == "Escort" or tbAward.szModuleName == "Tower") then
		nPercent = 110;
	end
	
	-- 银两
	if tbAward.nSilver > 0 then
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, math.floor(tbAward.nSilver * nPercent / 100));
	end
	
	-- 战魂
	if tbAward.nFightingSpirit > 0 then
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SPIRIT, math.floor(tbAward.nFightingSpirit * nPercent / 100));
	end
	
	-- 游龙真气
	if tbAward.nEnergy > 0 then
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_YLENERGY, math.floor(tbAward.nEnergy * nPercent / 100));
	end
	
	-- 声望
	if tbAward.nPrestige > 0 then
	  pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_JINGPO, math.floor(tbAward.nPrestige));
	end
	
	-- 经验
	if tbAward.nExp > 0 and tbAward.bGiveExp == 1 then
		pPlayer.AddExp(math.floor(tbAward.nExp * nPercent / 100));
	end
	
	for i = 1, #tbAward.pItem do
		local tbItem = tbAward.pItem[i];
		
		local pItem = KItem.AddPlayerItem(pPlayer, 
					tbItem.nGenre, 
					tbItem.nDetailType,
					tbItem.nParticular,
					tbItem.nLevel,
					tbItem.nAmount);
		
		-- 设置过期时间
		if tbItem.nTimeOut > 0 then
			pPlayer.pItem.SetItemTimeout(pItem, tbItem.nTimeOut, 1);
			pItem.Sync();	-- 重新同步一次
		end
	end
	
	return 1;
end

function Reward:c2s_TakeAllAwards(pPlayer, szModuleName)
	KAward.TakeAllAward(pPlayer, szModuleName);
end
