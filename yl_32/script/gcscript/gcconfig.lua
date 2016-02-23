if not (MODULE_GAMESERVER or MODULE_GC_SERVER) then
	return;
end

local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

KGCMODULE_DEF = {
	emKGCMODULE_LOGICBASE	= IdAccumulator(1),
	emKGCMODULE_EXCHANGE	= IdAccumulator(),
	emKGCMODULE_CHAT		= IdAccumulator(),
	emKGCMODULE_GROUP		= IdAccumulator(),
	emKGCMODULE_RELATION	= IdAccumulator(),
	emKGCMODULE_IBSHOP		= IdAccumulator(),
	emKGCMODULE_GLOBALDATA	= IdAccumulator(),
	emKGCMODULE_LEAGUE      = IdAccumulator();
	emKGCMODULE_SNS_SUBSTITUTE = IdAccumulator();
	emKGCMODULE_KIN         = IdAccumulator();
	emKGCMODULE_CUP         = IdAccumulator();
}
