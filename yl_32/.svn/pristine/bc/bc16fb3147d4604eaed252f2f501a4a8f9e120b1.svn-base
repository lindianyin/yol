-- File       : doodaddef.lua
-- Creator    : simon
-- Date       : 2010-03-18 17:31:56
-- Description: 

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER or MODULE_PLOTEDITOR) then
	return
end

local nId4Accumulator = 0;
local function IdAccumulator(nSegment)
	nId4Accumulator = nSegment or nId4Accumulator + 1;
	return nId4Accumulator;
end

-- 与kdoodad.h中的KEDOODAD_STATE保持一致
Doodad.STATE_NONE		= IdAccumulator(0);
Doodad.STATE_NORMAL		= IdAccumulator();
Doodad.STATE_OPING		= IdAccumulator();
Doodad.STATE_EMPTY		= IdAccumulator();
Doodad.STATE_DISABLE	= IdAccumulator();

Doodad.STATE_GATHER		= IdAccumulator();

