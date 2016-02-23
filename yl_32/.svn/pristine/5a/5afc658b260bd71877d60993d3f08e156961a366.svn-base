
-- 宝石镶嵌功能脚本

------------------------------------------------------------------------------------------
-- private

local function CheckStone(pStone)		-- 检查宝石的合法性
	if (not pStone) or (pStone.IsStone() ~= 1) then
		return 0;
	end

	if (pStone.nLevel >= Item.MAX_STONE_LV) then
		me.SysMsg(Lang.item.str190[Lang.Idx]);
		return 0;			-- 已合成到最高级不能合成
	end
	
	return 1;
end

local function CalcNeedCoin(tbStoneList)
	assert(#tbStoneList >= 1 and #tbStoneList < Item.MAX_STONE_AMOUNT);
	
	local nLevel = tbStoneList[1].nLevel;
	
	local nPrice = 3 ^ (nLevel - 1) * 10;		-- 每个初级宝石10元宝
	
	local nNeedCoin = nPrice * (Item.MAX_STONE_AMOUNT - #tbStoneList);
	
	return nNeedCoin;
end

local function CheckStoneList(pPlayer, tbStoneList)
	local nAmount = #tbStoneList;
	
	if nAmount < 1 then
		assert(false);
	end
	
	for _, pStone in ipairs(tbStoneList) do
	
		if (CheckStone(pStone) ~= 1) then 					-- 检查宝石合法性
			return 0; 
		end
		
		if(pStone.nLevel ~= tbStoneList[1].nLevel) then		-- 检查等级是否一致
			me.SendBlackMsg(Lang.item.str191[Lang.Idx]);
			return 0;
		end
		
	end
	
	if nAmount < Item.MAX_STONE_AMOUNT then
		local nNeedCoin = CalcNeedCoin(tbStoneList);
		local text = string.format(Lang.item.str192[Lang.Idx], nNeedCoin);
		Dialog:_AskForSure(pPlayer.dwId, text, ({"Item:OnBuyStones", pPlayer, tbStoneList}), "StoneMix", 0);
		return 0;
	end
	
	return 1;
end

function Item:OnBuyStones(pPlayer, tbStoneList)

	local nNeedCoin = CalcNeedCoin(tbStoneList);
	
	local szPriceTips = Item:GetPriceTips(nNeedCoin);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedCoin then
		pPlayer.SendBlackMsg(string.format(Lang.item.str188[Lang.Idx], szPriceTips))
		return -1;
	end
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedCoin);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedCoin,123);
	
	self:DoStoneMix(pPlayer, tbStoneList);
end

------------------------------------------------------------------------------------------
--public

function Item:CalcStoneMixCost(nStoneAmount, nLv)
	return nStoneAmount * nLv * 1000;
end

function Item:StoneMix(tbStoneList)
	if (CheckStoneList(me, tbStoneList) ~= 1) then
		return -1;
	end
	
	self:DoStoneMix(me, tbStoneList);
end

function Item:DoStoneMix(pPlayer, tbStoneList)

	local nLv = tbStoneList[1].nLevel + 1;
	local nGe = tbStoneList[1].nGenre;
	local nDe = tbStoneList[1].nDetail;
	local nPa = tbStoneList[1].nParticular;
	
	local szName = KItem.GetNameById(nGe, nDe, nPa, nLv);
	local szTips = string.format(Lang.item.str193[Lang.Idx], nLv, string.sub(szName,6));
	
	-- 扣除宝石
	for _, pStone in ipairs(tbStoneList) do
		local bRet = KItem.DelPlayerItem(pPlayer, pStone);
		--assert(bRet);
		if(not bRet) then
			print("..Item:DoStoneMix error");
			return;
		end
	end
	
	-- 给新宝石
	local pStone = KItem.AddPlayerItem(pPlayer, nGe, nDe, nPa, nLv);
	
	if nLv == 7 then
		KChat.BroadcastNoticeMsg(string.format(Lang.item.str194[Lang.Idx], pPlayer.szName, pStone.szName));
	end
	
	pPlayer.SendBlackMsg(szTips);
	
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d",nGe, nDe, nPa, nLv);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,56,szStr);
end

function Item:GetFitStoneType(pEquip)
	local nFitType = 0;
	if 	
		pEquip.nDetail == Item.EQUIP_SWORD or
		pEquip.nDetail == Item.EQUIP_SPEAR or
		pEquip.nDetail == Item.EQUIP_PENDANT or
		pEquip.nDetail == Item.EQUIP_RING or
		pEquip.nDetail == Item.EQUIP_NECKLACE then
		
		nFitType = 1;
		
	elseif 	
		pEquip.nDetail == Item.EQUIP_HEAD or
		pEquip.nDetail == Item.EQUIP_ARMOR or
		pEquip.nDetail == Item.EQUIP_BELT or
		pEquip.nDetail == Item.EQUIP_CLOAK or
		pEquip.nDetail == Item.EQUIP_FOOT then
		
		nFitType = 2;
	end
	
	return nFitType;
		
end

-- 宝石镶嵌
function Item:StoneMount(pEquip, tbStoneList)
	local tbStoneInfo = pEquip.GetStoneInfo();
	
	if self:GetFitStoneType(pEquip) ~= tbStoneList[1].GetStoneType() then
		assert(false);
	end
	
	for _, pStone in ipairs(tbStoneList) do
		local nStoneId = pStone.nParticular;
		local nStoneLevel = pStone.nLevel;
		-- 扣除宝石
		KItem.DelPlayerItem(me, pStone);
		
		for _, tbInfo in ipairs(tbStoneInfo) do
			assert(tbInfo.nStoneId ~= -1);
			if tbInfo.nStoneId == 0 then
				tbInfo.nStoneId = nStoneId;
				tbInfo.nStoneLevel = nStoneLevel;
				break;
			end
		end
	end
	
	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel,
		pEquip.nEnhTimes,
		tbStoneInfo,
		pEquip.GetAppendAttr(),
		0,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	)
	assert(nRet);
	
	me.pItem.ReApplyEquipRes();
	

	--me.SysMsg("镶嵌成功！");
end

function Item:RemoveStone(pEquip, tbSlotList)
	
	if #tbSlotList <= 0 then
		return;
	end
	
	for _, nSlot in ipairs(tbSlotList) do
		if (nSlot < 1 or nSlot > pEquip.nSlotSize) then
			return -1;
		end
		-- 这个孔没有镶嵌宝石
		if pEquip.HasStone(nSlot) == 0 then
			return -1;
		end
	end
	
	local tbStoneInfo = pEquip.GetStoneInfo();
	
	for _, nSlot in ipairs(tbSlotList) do
		local nStoneId = tbStoneInfo[nSlot].nStoneId;
		local nStoneLevel = tbStoneInfo[nSlot].nStoneLevel;
	
		tbStoneInfo[nSlot].nStoneId = 0;
		tbStoneInfo[nSlot].nStoneLevel = 0;
		
		if me.pItem.CountFreeBagCell() < 1 then
			me.SendBlackMsg(Lang.item.str195[Lang.Idx]);
			return;
		end
		
		-- 给宝石
		KItem.AddPlayerItem(me, Item.SCRIPTITEM, Item.SCRIPT_STONE, nStoneId, nStoneLevel);
	end
	
	local nRet = pEquip.Regenerate(
		pEquip.nGenre,
		pEquip.nDetail,
		pEquip.nParticular,
		pEquip.nLevel,
		pEquip.nEnhTimes,
		tbStoneInfo,
		pEquip.GetAppendAttr(),
		0,
		pEquip.dwRandSeed,
		pEquip.dwRandSeedEx
	)
	assert(nRet);
	
	me.pItem.ReApplyEquipRes();
	
	--pEquip.StoneClean();
	--pEquip.Sync();
	--me.SysMsg("移除成功！");
end

function Item:GetPunchCost(nSlot)
	local nCost = 0;
	assert(nSlot >= 0 and nSlot < 3);
	if nSlot == 0 then
		nCost = 20;
	elseif nSlot == 1 then
		nCost = 40;
	elseif nSlot == 2 then
		nCost = 80;
	end
	return nCost;
end

function Item:StonePunch(pEquip, nSlot)
	local nNeedGold = self:GetPunchCost(nSlot);
	
	local nBindCoinNum = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_BIND_COIN)
	
	if nBindCoinNum >= nNeedGold * 2 then  -- 优先消耗绑定元宝
	  local text = string.format(Lang.item.str196[Lang.Idx], nNeedGold * 2);
	  Dialog:_AskForSure(me.dwId, text, ({"Item:DoStonePunchByBindCoin", me, pEquip, nSlot}), "StonePunch", 0);
		return 1;
	end
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if me.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		me.SendBlackMsg(string.format(Lang.item.str197[Lang.Idx], szPriceTips))
		return 0;
	end
	
	local text = string.format(Lang.item.str198[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(me.dwId, text, ({"Item:DoStonePunch", me, pEquip, nSlot}), "StonePunch", 0);
end

function Item:DoStonePunchByBindCoin(pPlayer, pEquip, nSlot)
  local nNeedGold = self:GetPunchCost(nSlot);
	
	local nBindCoinNum = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_BIND_COIN)
  if nBindCoinNum < nNeedGold * 2 then  -- 优先消耗绑定元宝
	  pPlayer.SendBlackMsg(Lang.item.str199[Lang.Idx])
		return 0;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_BIND_COIN, -nNeedGold * 2);
	
	pEquip.StonePunch(nSlot);
	pEquip.Sync();
	
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d,nSlot=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel,pEquip.nEnhTimes,nSlot);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,55,szStr);
	
	
end

function Item:DoStonePunch(pPlayer, pEquip, nSlot)
	local nNeedGold = self:GetPunchCost(nSlot);
	
	local szPriceTips = Item:GetPriceTips(nNeedGold);
	if pPlayer.pPlayerPurse.GetMoney(Item.EM_MONEY_COIN) < nNeedGold then
		pPlayer.SendBlackMsg(string.format(Lang.item.str197[Lang.Idx], szPriceTips))
		return 0
	end
	
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -nNeedGold);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, -nNeedGold,122);
	
	pEquip.StonePunch(nSlot);
	pEquip.Sync();
	--统计点  ldy
	local szStr = string.format("nGenre=%d,nDetail=%d,nParticular=%d,nLevel=%d,nEnhTimes=%d,nSlot=%d",pEquip.nGenre,pEquip.nDetail,pEquip.nParticular,pEquip.nLevel,pEquip.nEnhTimes,nSlot);
	pPlayer.pPlayerPurse.LogMoney(Item.EM_MONEY_COIN, 0,55,szStr);
	
end