--=================================================
-- 文件名　：blacklist.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：黑名单逻辑
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbBlackList = {};

-- if (not MODULE_GAMECENTER and not MODULE_GAMESERVER) then
--	return;
-- end

if (MODULE_GAMECENTER) then
	
end

if (MODULE_GAMESERVER) then
	
end

Relation:Register(Relation.emKPLAYERRELATION_TYPE_BLACKLIST, tbBlackList)
