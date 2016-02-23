-- buffµÀ¾ß

------------------------------------------------------------------------------------------
-- initialize

local tbCurrency = Item:GetClass("currency");

------------------------------------------------------------------------------------------

function tbCurrency:CheckUsable()
	return 1;
end

function tbCurrency:OnUse()
	local nCurrencyType = tonumber(it.GetExtParam(1));
	local nCurrencyNum = tonumber(it.GetExtParam(2));
	
	me.pPlayerPurse.AddMoney(nCurrencyType, nCurrencyNum)
	
	return 1;
end