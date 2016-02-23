
if not MODULE_GAMESERVER then
	return
end

function Player:OnFirstLogin(pPlayer, nHairId, nBodyId)--, nShirtId, nShoeId)
	for i=1,10 do
		local pItem = KItem.AddPlayerItem(pPlayer, 1, i, 1, 1);  --新手武器
		pPlayer.pItem.AutoEquip(pItem);
	end
end
function Player:ForTestUse(name)
	local pPlayer = KGameBase.GetPlayerByName(name)
	local pItem = KItem.AddPlayerItem(pPlayer, 1, 1, 1, 1);  --新手武器
	pPlayer.pItem.AutoEquip(pItem);
	
end
function Player:GiveEquip(pPlayer, nItemId)
	local nItemIdx = Item:AddPlayerItem(pPlayer, nItemId, 1)
	if nItemIdx ~= 0 then
		Item:EquipItem(pPlayer, nItemIdx)
	end
end

function Player:GiveExp(pPlayer, nExp)
	Player.tbExpSystem:AddPlayerFinalExp(pPlayer.dwId, nExp)
	pPlayer.SysMsg(string.format(Lang.player.str36[Lang.Idx], nExp))
end
