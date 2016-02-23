
-- 篝火炬

------------------------------------------------------------------------------------------
-- initialize

local tbBonfire = Item:GetClass("bonfire");

------------------------------------------------------------------------------------------

function tbBonfire:OnUse()

	local pItem = it;
	--家族信息
	local tbPlayerKinInfo = GetPlayerKinInfo(me.dwId);
	local tbBonfire = Activity.tbBonfireInfo[me.GetKinId()];

	if tbPlayerKinInfo == nil or tbBonfire == nil then
		me.SendBlackMsg(Lang.item.str1[Lang.Idx]);
		return;
	end
	
	local nMapId = me.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "city" then
		me.SendBlackMsg(Lang.item.str2[Lang.Idx]);
		return;
	elseif KGameBase.GetIsSwimByPlayerId(me.dwId) == 1 then
		me.SendBlackMsg(Lang.item.str3[Lang.Idx]);
		return;
	elseif tbBonfire.onGoing == 0 then
		me.SendBlackMsg(Lang.item.str4[Lang.Idx]);
		return;
	elseif me.dwId ~= tbPlayerKinInfo.dwKinMasterID then
		me.SendBlackMsg(Lang.item.str5[Lang.Idx]);
		return;
	elseif tbBonfire.useTorch == 1 then
		me.SendBlackMsg(Lang.item.str6[Lang.Idx]);
		return;
	end

	local tbPos = me.GetPosition();
	local npc = KGameBase.CallNpc(2655, tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ);
	me.pItem.ConsumeItemInBags(1, pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel);
	Activity.tbBonfireInfo[me.GetKinId()] = {onGoing = 1, sendTorch = 1, useTorch = 1, mapId = tbPos.dwSceneId, npcId = npc.dwId, tbPos = {tbPos.nX, tbPos.nY, tbPos.nZ}};
	KChat.BroadcastKinNoticeMsg(me.GetKinId(), Lang.item.str7[Lang.Idx]);
	KGameBase.CenterExecute{"Activity:UpdateBonfireInfo_GC", me.GetKinId(), Activity.tbBonfireInfo[me.GetKinId()]};
end





