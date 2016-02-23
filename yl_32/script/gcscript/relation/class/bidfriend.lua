--=================================================
-- 文件名　：bidfriend.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：普通好友逻辑
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbBidFriend = {};

-- if (not MODULE_GC_SERVER and not MODULE_GAMESERVER) then
--	return;
-- end

if (MODULE_GAMECENTER) then
	
end

if (MODULE_GAMESERVER) then
	
end

Relation:Register(Relation.emKPLAYERRELATION_TYPE_BIDFRIEND, tbBidFriend)
