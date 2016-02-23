
------------------------------------------------------------------------------------------
-- public

local function GetRefineSilver(pEquip, pLock)
	local nBaseNum = 1000;
	for i = 1, 5 do
		if pLock[i] == 1 then
			nBaseNum = nBaseNum + 1000;
		end
	end
	return nBaseNum;
end

local function GetRefineGold(pEquip, pLock)
	local nBaseNum = 5;
	for i = 1, 5 do
		if pLock[i] == 1 then
			nBaseNum = nBaseNum + 5;
		end
	end
	return nBaseNum;
end

local function CheckRefineMoney(pPlayer, pEquip, nMoneyType, pLock)
	local nNeed;
	
	if nMoneyType == Purse.EM_MONEY_SILVER then
	
		nNeed = GetRefineSilver(pEquip, pLock);
		local szPriceTips = Item:GetPriceTips(nNeed);
		if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_SILVER) < nNeed then
			pPlayer.SendBlackMsg(string.format(Lang.item.str120[Lang.Idx], szPriceTips))
			return 0;
		end
		pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_SILVER, -nNeed);
		
	elseif nMoneyType == Purse.EM_MONEY_COIN then
	
		nNeed = GetRefineGold(pEquip, pLock);
		local szPriceTips = Item:GetPriceTips(nNeed);
		if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeed then
			pPlayer.SendBlackMsg(string.format(Lang.item.str188[Lang.Idx], szPriceTips))
			return 0;
		end
		pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeed);
		pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeed,42);
		
	end
	
	return 1;
end

function Item:Refine(pEquip, nMoneyType, ...)
	
	if nMoneyType == Purse.EM_MONEY_COIN and Vip:IsOpen(me, Vip.ID_EQUIP_REFINE) ~= 1 then
		return;
	end
	
	local pLock = arg;
	
	if CheckRefineMoney(me, pEquip, nMoneyType, pLock) ~= 1 then
		return;
	end
	
	pEquip.SetGenerateMoneyType(nMoneyType);
	
	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel,
		pEquip.nEnhTimes,
		pEquip.GetStoneInfo(),
		0,
		pLock,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	);
	--Í³¼Æµã  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel,pEquip.nEnhTimes);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,57,szStr);

	--DailyTasks:commitTaskValue(me,table.concat({pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel},"#"),DailyTasks.TYPE_REFINE_ITEM,1);
	DailyTasks:commitTaskValue(me,"",DailyTasks.TYPE_REFINE_ITEM,1);

	return nRet;
end

function Item:SaveAppendAttr(pEquip, nSaveOri)
	local nRet = 0;
	if nSaveOri == 1 then
		nRet = pEquip.Regenerate(
			pEquip.nGenre,
			pEquip.nDetail,
			pEquip.nParticular,
			pEquip.nLevel,
			pEquip.nEnhTimes,
			pEquip.GetStoneInfo(),
			pEquip.GetAppendAttr(),
			0,
			pEquip.dwRandSeed,
			pEquip.dwRandSeedEx
		);
	else
		nRet = pEquip.Regenerate(
			pEquip.nGenre,
			pEquip.nDetail,
			pEquip.nParticular,
			pEquip.nLevel,
			pEquip.nEnhTimes,
			pEquip.GetStoneInfo(),
			pEquip.GetSaveAppendAttr(),
			0,
			pEquip.dwRandSeed,
			pEquip.dwRandSeedEx
		);
	end
        print("..nSaveOri=".. nSaveOri .. " nRet=" .. nRet)
	if nRet == 1 then
		pEquip.ClearSaveAppendAttr(nSaveOri);
		pEquip.Sync();
	end
	
	return nRet;
end

function Item:OnEquipRefined(pItem, nMaxRefineQuality)
	if nMaxRefineQuality == 9 then
		KChat.BroadcastNoticeMsg(string.format(Lang.item.str189[Lang.Idx], me.szName, pItem.szName));
	end
end

------------------------------------------------------------------------------------------
