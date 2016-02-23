-- 装备相关

function Item:OnEquipOps(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	local nGreen = 0;		-- 全身绿装数量
	local nBlue = 0;
	local nPurple = 0;
	local nOrange = 0;
	local nRed = 0;
	local nYellow = 0;
	
	for i = Item.EQUIP_SWORD, Item.EQUIP_FOOT do
		local pEquip = pPlayer.pItem.GetEquipByDetail(i);
		if pEquip then
			if pEquip.nQuality == 2 then
				nGreen = nGreen + 1;
			elseif pEquip.nQuality == 3 then
				nBlue = nBlue + 1;
			elseif pEquip.nQuality == 4 then
				nPurple = nPurple + 1;
			elseif pEquip.nQuality == 5 then
				nOrange = nOrange + 1;
			elseif pEquip.nQuality == 6 then
				nRed = nRed + 1;
			elseif pEquip.nQuality == 7 then
				nYellow = nYellow + 1;
			end
		end
	end
	
	local tNow = KGameBase.GetTime();
	if nGreen == 10 then
		Target:DoTarget(pPlayer, 2);
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_GREEN) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str122[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_GREEN, tNow);
		end
	elseif nBlue == 10 then
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_BLUE) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str123[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_BLUE, tNow);
		end
	elseif nPurple == 10 then
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_PURPLE) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str124[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_PURPLE, tNow);
		end
	elseif nOrange == 10 then
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_ORANGE) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str125[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_ORANGE, tNow);
		end
	elseif nRed == 10 then
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_RED) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str126[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_RED, tNow);
		end
	elseif nYellow == 10 then
		if KChat.GetLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_GOLD) == 0 then
			KChat.BroadcastNoticeMsg(string.format(Lang.item.str127[Lang.Idx], pPlayer.szName));
			KChat.ModifyLastBroadcastTime(pPlayer, Chat.BROADCAST_ID_EQUIP_GOLD, tNow);
		end
	end
end

--[[
KChat.BroadcastNoticeMsg(string.format("<%s>穿上一身蓝装，顿时感觉自己实力大增！", pPlayer.szName));
	elseif nPurple == 10 then
		KChat.BroadcastNoticeMsg(string.format("紫装加身的<%s>，真是集高贵与强大于一身啊！", pPlayer.szName));
	elseif nOrange == 10 then
		KChat.BroadcastNoticeMsg(string.format("<%s>那一身的橙装，竟然隐隐散发出一股危险的气息！", pPlayer.szName));
	elseif nRed == 10 then
		KChat.BroadcastNoticeMsg(string.format("<%s>身上的，究竟是敌人鲜血染红的战袍，还是月老附身的红装！", pPlayer.szName));
	elseif nYellow == 10 then
		KChat.BroadcastNoticeMsg(string.format("是什么如此闪亮？<%s>身上的竟然是一身金装吗？我快要被亮瞎啦！", pPlayer.szName));
	end--]]


CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_EQUIP_OPS, Item.OnEquipOps, Item);