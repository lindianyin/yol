-- buffµÀ¾ß

------------------------------------------------------------------------------------------
-- initialize

local tbBuffItem = Item:GetClass("buff_item");

------------------------------------------------------------------------------------------

function tbBuffItem:CheckUsable()
	return 1;
end

function tbBuffItem:OnUse()
	local nBuffId = tonumber(it.GetExtParam(1));
	
	BufferApi.AddBufferToCharacter(me.dwId, 0, me.dwId, nBuffId, 1);
	
	return 1;
end