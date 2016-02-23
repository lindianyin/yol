--=================================================
-- 文件名　：sibling.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：结拜关系逻辑（目前还没有这个关系，不过先把这个东动写上吧）
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbSibling = {};

-- if (not MODULE_GC_SERVER and not MODULE_GAMESERVER) then
--	return;
-- end

if (MODULE_GAMECENTER) then
	
end

if (MODULE_GAMESERVER) then
	
end

Relation:Register(Relation.emKPLAYERRELATION_TYPE_SIBLING, tbSibling)
