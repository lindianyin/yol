--=================================================
-- 文件名　：enermy.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：仇人关系逻辑
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbEnermy = {};

-- if (not MODULE_GC_SERVER and not MODULE_GAMESERVER) then
--	return;
-- end

if (MODULE_GAMECENTER) then
	
end

if (MODULE_GAMESERVER) then
	
end

Relation:Register(Relation.emKPLAYERRELATION_TYPE_ENEMEY, tbEnermy)
