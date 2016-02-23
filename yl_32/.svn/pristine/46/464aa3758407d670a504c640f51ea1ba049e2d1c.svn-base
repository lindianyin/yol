
-- œ‰

------------------------------------------------------------------------------------------
-- initialize

local tbBox = Item:GetClass("box_award");

------------------------------------------------------------------------------------------

function tbBox:CheckUsable()
	return 1;
end

function tbBox:CanTakeAward(pPlayer, tbAward)
	assert(tbAward);
	
	local nItemCellCount = Reward:GetNeedBagCell(tbAward);
	
	if pPlayer.pItem.CountFreeBagCell() < nItemCellCount then
		pPlayer.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], nItemCellCount));
		return 0;
	end
	
	return 1;
end


function tbBox:OnUse()
	local nListId = tonumber(it.GetExtParam(1)); --¡–±Ìid
	
	local tbAwardData = KAward.GetAssetAwardData("BoxAward", nListId);
	assert(tbAwardData);
	
	if self:CanTakeAward(me, tbAwardData) ~= 1 then
		return;
	end
	
	KItem.DelPlayerItem(me, it);
	
	KAward.AddPlayerAward(me, "BoxAward", nListId);
end
