
------------------------------------------------------------------------------------------

local function CheckHasItem(tbItem)
	local tbFind = me.pItem.FindItemInBags(tbItem.nGenre, tbItem.nDetail, tbItem.nParticular, 0);
	local nCount = Item:GetItemAmount(tbFind);
	return nCount - tbItem.nAmount;
end

-- 在交元宝的情况下重新计算所需材料
local function RecalcNeedItems(tbNeedItems)
	for i = 2, #tbNeedItems do
		local tbItem = tbNeedItems[i];
		local tbFind = me.pItem.FindItemInBags(tbItem.nGenre, tbItem.nDetail, tbItem.nParticular, 0);
		local nCount = Item:GetItemAmount(tbFind);
		tbItem.nAmount = nCount;
	end
end

-- 检查升级所需材料
local function CheckNeedItems(pPlayer, pEquip)
	local tbNeedItems = pEquip.GetUpgradeItems();
	local nNeedCoin = 0;
	
	for i = 1, #tbNeedItems do
		local tbItem = tbNeedItems[i];
		local nCheck = CheckHasItem(tbItem);
		if i == 1 then
			if nCheck < 0 then
				-- 材料书必须要有
				assert(false);
			end
		else
			if nCheck < 0 then
				local tbProp = KItem.GetOtherBaseProp(tbItem.nGenre, tbItem.nDetail, tbItem.nParticular);
				local nPrice = tbProp.nPrice;
				nNeedCoin = nNeedCoin + (-nCheck) * nPrice;
			end
		end
	end
	
	-- 材料不足
	if nNeedCoin > 0 then
		RecalcNeedItems(tbNeedItems);
		local text = string.format(Lang.item.str192[Lang.Idx], nNeedCoin);
		Dialog:_AskForSure(pPlayer.dwId, text, ({"Item:OnBuyUpgradeItems", pPlayer, pEquip, nNeedCoin, tbNeedItems}), "EquipUpgrade", 0);
		return 0;
	end
	
	-- 消耗材料
	for _, tbItem in ipairs(tbNeedItems) do
		local nCheck = CheckHasItem(tbItem);
		assert(nCheck >= 0);
		pPlayer.pItem.ConsumeItemInBags(tbItem.nAmount, tbItem.nGenre, tbItem.nDetail, tbItem.nParticular);
	end
	
	return 1;
end

function Item:OnBuyUpgradeItems(pPlayer, pEquip, nNeedCoin, tbNeedItems)
	local szPriceTips = Item:GetPriceTips(nNeedCoin);
	if me.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedCoin then
		me.SendBlackMsg(string.format(Lang.item.str188[Lang.Idx], szPriceTips))
		return;
	end
	me.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedCoin);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedCoin,41);
	-- 消耗剩余材料
	for _, tbItem in ipairs(tbNeedItems) do
		if tbItem.nAmount > 0 then
			local nCheck = CheckHasItem(tbItem);
			assert(nCheck >= 0);
			pPlayer.pItem.ConsumeItemInBags(tbItem.nAmount, tbItem.nGenre, tbItem.nDetail, tbItem.nParticular);
		end
	end
	
	self:DoUpgrade(pEquip);
end

local function CanUpgrade(pPlayer, pEquip)
	local tbSaveAppendAttr = pEquip.GetSaveAppendAttr();
	
	if KItem.IsEquipExist(pEquip.nGenre, pEquip.nDetail, pEquip.nParticular, pEquip.nLevel + 1) ~= 1 then
		me.SendBlackMsg(string.format(Lang.item.str200[Lang.Idx], pEquip.szName));
		return 0;
	end
	
	-- 必须没有在洗炼状态下
	if tbSaveAppendAttr[1].nAttr ~= 0 then
		assert(false);
	end

	if pPlayer.GetLevel() < pEquip.GetNextReqLevel() then
		me.SendBlackMsg(string.format(Lang.item.str201[Lang.Idx], pEquip.GetNextReqLevel()));
		return 0;
	end
	
	if CheckNeedItems(pPlayer, pEquip) ~= 1 then
		me.SendBlackMsg(Lang.item.str202[Lang.Idx]);
		return 0;
	end
	
	return 1;
end

function Item:Upgrade(pEquip)
	assert(pEquip.nLevel < Item.MAX_EQUIP_LEVEL);
	
	if CanUpgrade(me, pEquip) ~= 1 then
		return 0;
	end
	
	self:DoUpgrade(pEquip);
	
	return 1;
end

function Item:DoUpgrade(pEquip)

	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel + 1,
		pEquip.nEnhTimes,
		pEquip.GetStoneInfo(),
		0,
		0,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	);
	print("..Item:DoUpgrade pEquip.nLevel",pEquip.nLevel+1,"nRet",nRet);
	if(nRet == 1) then 
		if((pEquip.nLevel + 1) >= 4) then 
			local szMsg = Lang.mission.str1224[Lang.Idx];
			local szPlayerName = me.szName;
			local szQuality = Pet.QUALITY_TIPS[pEquip.nLevel];   --此处修改蓝品变紫品问题
			local szEquipName = KItem.GetNameById(pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel); ----此处修改蓝品变紫品问题
			local szFormatMsg = string.format(szMsg,szPlayerName,szQuality,szEquipName);
			KChat.BroadcastNoticeMsg(szFormatMsg);	
		end
	end
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel + 1,pEquip.nEnhTimes);
	me.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,54,szStr);
	return nRet;
end

------------------------------------------------------------------------------------------
