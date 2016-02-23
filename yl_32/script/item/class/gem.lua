
-- 丹药

------------------------------------------------------------------------------------------
-- initialize

local tbGem= Item:GetClass("gem");

------------------------------------------------------------------------------------------

function tbGem:CheckUsable()
	return 1;
end

-- function tbGem:CanTakeAward(pPlayer, tbAward)
	-- assert(tbAward);
	
	-- local nItemCellCount = Reward:GetNeedBagCell(tbAward);
	
	-- if pPlayer.pItem.CountFreeBagCell() < nItemCellCount then
		-- pPlayer.SendBlackMsg(string.format("包裹里需要有%d个空位，先整理下包裹吧", nItemCellCount));
		-- return 0;
	-- end
	
	-- return 1;
-- end


function tbGem:OnUse()
	local pItem = it;
	me.pGem.OnUse(pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel);
	me.pItem.ConsumeItemInBags(1, pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel);
end
