
local tbNpc = Npc:GetClass("battledelivery_zst_out")

local tbDeliveryPoint = {6365,1847,1062952};--’Ω…ÒÃ√

function tbNpc:OnDialog()
	local nPlayerId = me.dwId;
	local nGroupId = Activity:GetPlayerGroup(nPlayerId);
	if nGroupId == 1 then
		tbNpc:GetDelivery();
	else
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		pPlayer.SysMsg(Lang.npc.str11[Lang.Idx]);
		return;
	end
end

function tbNpc:GetDelivery()
	local dwSceneId = me.GetPosition().dwSceneId;
	me.TeleportTo(dwSceneId,tbDeliveryPoint[1],tbDeliveryPoint[2],tbDeliveryPoint[3]);
end
