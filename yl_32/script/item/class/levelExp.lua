
-- 经验丹

------------------------------------------------------------------------------------------
-- initialize

local tbLevelExp = Item:GetClass("levelExp");

------------------------------------------------------------------------------------------

function tbLevelExp:OnUse()
	local maxLevel = tonumber(it.GetExtParam(1)); --Maxlevel 
	local Exp = tonumber(it.GetExtParam(2)); --ExpNum 
	local level = me.GetLevel();
	if maxLevel > level then
		me.SetLevel(level+1);
	else
		me.AddExp(Exp);
	end
	
	KItem.DelPlayerItem(me, it);
end
