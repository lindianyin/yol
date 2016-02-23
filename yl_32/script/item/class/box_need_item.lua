
-- 箱 withitem
-- 2016/1/5 
------------------------------------------------------------------------------------------
-- initialize

local tbBoxByItem = Item:GetClass("box_award_by_item");

------------------------------------------------------------------------------------------
tbBoxByItem.ItemOb = {};

function tbBoxByItem:CheckUsable()
	return 1;
end

function tbBoxByItem:CanTakeAward(pPlayer, tbAward)
	assert(tbAward);
	
	local nItemCellCount = Reward:GetNeedBagCell(tbAward);
	
	if pPlayer.pItem.CountFreeBagCell() < nItemCellCount then
		pPlayer.SendBlackMsg(string.format("包裹里需要有%d个空位，先整理下包裹吧", nItemCellCount));
		return 0;
	end
	
	return 1;
end

function tbBoxByItem:DoGetAwardByBox( pPlayer )
	
	local  it = self.ItemOb[pPlayer.dwId];

	self.ItemOb[pPlayer.dwId] = nil;
	
	local nNeedItem = {it.GetExtParam(2),it.GetExtParam(3),it.GetExtParam(4),it.GetExtParam(5)};
	local nNeedCount = tonumber(it.GetExtParam(6)) or 0;
	local szItemName = KItem.GetNameById(unpack(nNeedItem));

	if Task:GetItemCount(pPlayer,nNeedItem) < nNeedCount then
		pPlayer.SendBlackMsg(string.format("开启该箱子需要%d个%s", nNeedCount, szItemName));
		return 0;
	end
		
	local nListId = tonumber(it.GetExtParam(1)); --列表id

	if self:CanTakeAward(pPlayer, tbAwardData) ~= 1 then
		return;
	end

	pPlayer.pItem.ConsumeItemInBags(nNeedCount, unpack(nNeedItem));

	KItem.DelPlayerItem(pPlayer, it);
	KAward.AddPlayerAward(pPlayer, "BoxAward", nListId);

end

function tbBoxByItem:OnUse()
	local nNeedItem = {it.GetExtParam(2),it.GetExtParam(3),it.GetExtParam(4),it.GetExtParam(5)};
	local nNeedCount = tonumber(it.GetExtParam(6)) or 0;
	local szItemName = KItem.GetNameById(unpack(nNeedItem));

	self.ItemOb[me.dwId] = it;
	local text = string.format("是否花费%d个%s开启该箱子", nNeedCount, szItemName);
	Dialog:_AskForSure(me.dwId, text, ({"tbBoxByItem:DoGetAwardByBox", pPlayer}), "GetAwardByBox", 0);
end
