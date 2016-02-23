-- Ride脚本类

Include("/script/pet/define.lua");

Pet.tbRide = {};

local function PreloadRideTemplate()
	local tbFileData = Lib:LoadTabFile("\\setting\\pet\\ride.txt")
	for _, tb in pairs(tbFileData) do
		local nPa = tonumber(tb.Particular);
		local nLv = tonumber(tb.Level);
		if nPa then
			if not Pet.tbRide[nPa] then
				Pet.tbRide[nPa] = {};
			end
			if not Pet.tbRide[nPa][nLv] then
				Pet.tbRide[nPa][nLv] = {};
			end

			local tbInfo = Pet.tbRide[nPa][nLv];
			tbInfo.RankConsume = tonumber(tb.RankConsume);
			tbInfo.MaxLevel = tonumber(tb.MaxLevel);
		end
	end
end

PreloadRideTemplate();

function Pet:TakeOffRide(pPlayer, pRide)
	local nGenre = pRide.nGenre;
	local nDetail = pRide.nDetail;
	local nParticular = pRide.nParticular;
	local nLevel = pRide.nLevel;

	if me then
		pPlayer = me;
	end

	if pPlayer.pItem.CountFreeBagCell() <= 0 then
		pPlayer.SysMsg(Lang.pet.str58[Lang.Idx]);
		return 0;
	end

	local nHungry = pRide.nHungry;
	if nHungry == 0 then
		nHungry = -1;
	end
	
	local nMagicRate = pRide.nMagicRate;

	local bRet = KPet.TakeOffRide(pPlayer, pRide.dwId);
	assert(bRet > 0);

	local pItem = KItem.AddPlayerItem(pPlayer, nGenre, nDetail, nParticular, nLevel);
	pItem.SetExtParam(1, nHungry);
	pItem.SetExtParam(2, nMagicRate);
end

function Pet:AddRideMagicRate(pRide, nCostNum)
	local nPa = pRide.nParticular;
	local nLv = pRide.nLevel;

	local nCurrentRate = pRide.nMagicRate + nCostNum;
	local tbNumber = KAward.GetNumericalAwardData("horse_level", 1, pRide.nLevel + 1);
	if (nCurrentRate > tbNumber.nCost) then
	    return;
	end

	-- 消耗物品
	local tbCostGDPL = { 18, 1, 30 };	-- 幻化果
	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nCount = Item:GetItemAmount(tbFind);
	
	if (nCount < nCostNum) then
		local szErrMsg = string.format(Lang.pet.str59[Lang.Idx] , nCostNum);
		me.SysMsg(szErrMsg);
		return;
	end
	
	me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));
	
	pRide.SetMagicRate(nCurrentRate);
	
	return pRide.Sync();
end

function Pet:RideUpRank(pRide)
	local nPa = pRide.nParticular;
	local nLv = pRide.nLevel;
  local tbNumber = KAward.GetNumericalAwardData("horse_level", 1, pRide.nLevel + 1);
  local nPa = pRide.nParticular;
	local nLv = pRide.nLevel;
  local tbInfo = Pet.tbRide[nPa][nLv];

	assert(pRide.nLevel < tbInfo.MaxLevel);
	assert(pRide.nMagicRate == tbNumber.nCost);
	local needNum = pRide.nLevel + 1
	if (pRide.nLevel + 1 > 12) then
		needNum = 12
	end
	local tbCostGDPL = Pet.Up_Ride_Need_Item[needNum];
	local nNeedCount =	1 ;
	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nCount = Item:GetItemAmount(tbFind);
	local szName = KItem.GetNameById(unpack(tbCostGDPL));
	if (nCount < nNeedCount) then
		local szErrMsg = string.format(Lang.pet.str60[Lang.Idx],nNeedCount,szName);
		--local szErrMsg = string.format("请随身携带至少"..nNeedCount.."个" .. szName);
		me.SysMsg(szErrMsg);
		return;
	end
	
	me.pItem.ConsumeItemInBags(nNeedCount, unpack(tbCostGDPL));
	
	local szOldRideName = pRide.szName

	local bRet = pRide.Regenerate(
		pRide.nGenre,
		pRide.nDetail,
		pRide.nParticular,
		pRide.nLevel + 1);

	assert(bRet > 0);
	
	local nIndex = pRide.nLevel % 6;
	if nIndex == 0 then
	    nIndex = 1;
	end
	
	local szNewRideName = pRide.szName
	
	local szMsg = string.format(Lang.pet.str61[Lang.Idx],
		szOldRideName, 
		szNewRideName);
	
	me.SendBlackMsg(szMsg);
	
	if (pRide.nLevel >= 10) then
	    KChat.BroadcastNoticeMsg(string.format(Lang.pet.str62[Lang.Idx] ,me.szName,szNewRideName));
	end
	
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d",pRide.nGenre,pRide.nDetail,pRide.nParticular,pRide.nLevel);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,58,szStr);
	
	return pRide.Sync();
end

local PARAM_FEED_FOOD_LV = 1;		-- 坐骑饲料等级

function Pet:FeedRide(pRide, nCostNum)

	-- 消耗物品
	local tbCostGDPL = { 18, 1, 31 };	-- 饲料
	local tbFind = me.pItem.FindItemInBags(unpack(tbCostGDPL));
	
	local nCount = Item:GetItemAmount(tbFind);
	if (nCount < nCostNum) then
		local szErrMsg = string.format(Lang.pet.str63[Lang.Idx] , nCostNum);
		me.SysMsg(szErrMsg);
		return;
	end
	
	local nHungry = tbFind[1].GetExtParam(1);
	me.pItem.ConsumeItemInBags(nCostNum, unpack(tbCostGDPL));
	
	nHungry = nHungry * nCostNum;
	
	pRide.AddHungry(nHungry);
	
	pRide.Sync();
end

function Pet:GetDownRide(pPlayer)
	local pRide = KPet.GetRide(pPlayer);
	if not pRide then
		return;
	end
	-- 下马
	local nState = pRide.nState;
	if nState == Pet.emRIDE_STATE_UP then
		pRide.SetState(Pet.emRIDE_STATE_DOWN);
	end
end

-- 上马
function Pet:GetUpRide(pPlayer)
	local pRide = KPet.GetRide(pPlayer);
	if not pRide then
		return;
	end
	local nState = pRide.nState;
	if nState == Pet.emRIDE_STATE_DOWN then
		pRide.SetState(Pet.emRIDE_STATE_UP);
	end
end
