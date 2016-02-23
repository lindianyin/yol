
-- œ‰

------------------------------------------------------------------------------------------
-- initialize

local tbBoxByMoney = Item:GetClass("box_award_by_money");

------------------------------------------------------------------------------------------

function tbBoxByMoney:CheckUsable()
	return 1;
end


function tbBoxByMoney:OnUse()
	local nNeedGold = tonumber(it.GetExtParam(2)); --¡–±Ìid

	Item.ItemOb[me.dwId] = it;
	local text = string.format(Lang.item.str10[Lang.Idx], nNeedGold);
	Dialog:_AskForSure(me.dwId, text, ({"Item:DoGetAwardByBox", me}), "GetAwardByBox", 0);
end
