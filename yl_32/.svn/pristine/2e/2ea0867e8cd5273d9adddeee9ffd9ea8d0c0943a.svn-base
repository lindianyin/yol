--=================================================
-- 文件名　：couple.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：夫妻关系逻辑
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbCouple = {};

-- if (not MODULE_GC_SERVER and not MODULE_GAMESERVER) then
--	return;
-- end





--=================================================
--============= MODULE_GC_SERVER ==================
--=================================================


if (MODULE_GAMECENTER) then

-- 创建人际关系判断
function tbCouple:CanCreateRelation(nRole, nAppId, nDstId)
	if (not nRole or not nAppId or not nDstId) then
		return 0;
	end
	
	if (nRole == 0) then
		nAppId, nDstId = nDstId, nAppId;
	end
	
	-- 配偶只能有一个
	if (KRelation.GetOneRelationCount(nAppId, Player.emKPLAYERRELATION_TYPE_COUPLE, 1) >= 1 or
		KRelation.GetOneRelationCount(nDstId, Player.emKPLAYERRELATION_TYPE_COUPLE, 0) >= 1) then
		Relation:SetInfoMsg(Lang.gcscript.str33[Lang.Idx]);
		return 0;
	end
	
	-- 性别一直不能结婚
	if (KGCPlayer.OptGetTask(nAppId, KGCPlayer.SEX) == KGCPlayer.OptGetTask(nDstId, KGCPlayer.SEX)) then
		Relation:SetInfoMsg(Lang.gcscript.str34[Lang.Idx]);
		return 0;
	end
	
	return 1;
end

end













--=================================================
--============= MODULE_GAMESERVER =================
--=================================================

if (MODULE_GAMESERVER) then
	
end

Relation:Register(Relation.emKPLAYERRELATION_TYPE_COUPLE, tbCouple)
