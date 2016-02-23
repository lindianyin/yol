
-- 篝火炬

------------------------------------------------------------------------------------------
-- initialize

local tbYuQiongJiang = Item:GetClass("yuqiongjiang");

------------------------------------------------------------------------------------------

function tbYuQiongJiang:OnUse()
	local pItem = it;
	local nPercent = (tonumber(pItem.GetExtParam(1)) or 0) / 100;
	local nExp = nPercent * KGameBase.GetNextLevelExp(me.GetLevel()+1);
	
	--家族信息
	local tbPlayerKinInfo = GetPlayerKinInfo(me.dwId);
	local tbBonfire = Activity.tbBonfireInfo[me.GetKinId()];

	if tbPlayerKinInfo == nil or tbBonfire == nil then
		me.SendBlackMsg("篝火活动开始才能使用玉琼浆");
		return;
	end
	
	local nMapId = me.dwSceneTemplateId;
	if KScene.GetSceneTypeName(nMapId) ~= "city" then
		me.SendBlackMsg("玉琼浆只能在主城使用");
		return;
	elseif KGameBase.GetIsSwimByPlayerId(me.dwId) == 1 then
		me.SendBlackMsg("玉琼浆不能在水域中使用");
		return;
	elseif tbBonfire.onGoing == 0 then
		me.SendBlackMsg("篝火活动开始才能使用玉琼浆");
		return;
	end

	-- local tbPos = me.GetPosition();
	-- local npc = KGameBase.CallNpc(2655, tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ);

	-- 获取经验并消耗道具	
	me.pItem.ConsumeItemInBags(1, pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel);
	me.AddExp(nExp);
	
	-- Activity.tbBonfireInfo[me.GetKinId()] = {onGoing = 1, sendTorch = 1, useTorch = 1, mapId = tbPos.dwSceneId, npcId = npc.dwId, tbPos = {tbPos.nX, tbPos.nY, tbPos.nZ}};
	-- KChat.BroadcastKinNoticeMsg(me.GetKinId(), "家族的篝火已经熊熊燃起啦，童鞋们快快打开家族活动面板参加吧！");
	-- KGameBase.CenterExecute{"Activity:UpdateBonfireInfo_GC", me.GetKinId(), Activity.tbBonfireInfo[me.GetKinId()]};
end





