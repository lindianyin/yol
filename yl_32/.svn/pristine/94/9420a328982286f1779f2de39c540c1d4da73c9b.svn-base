
-- …‚√‚¡Ó

------------------------------------------------------------------------------------------
-- initialize

local tbBonfire = Item:GetClass("shemianling");

------------------------------------------------------------------------------------------

function tbBonfire:OnUse()

	local pItem = it;
	local nEvilValue = me.pTask.GetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.EVIL_VALUE_ID) or 0;
	local nMinusValue = me.pTask.GetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.MINUS_ATTACK_ID) or 0;
	if nEvilValue > 0 then
		local nCount = tonumber(pItem.GetExtParam(1));
		--me.pItem.ConsumeItemInBags(1, pItem.nGenre, pItem.nDetail, pItem.nParticular, pItem.nLevel);
		KItem.DelPlayerItem(me, pItem);
		
		if nEvilValue > nCount then
			me.pTask.DirectSetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.EVIL_VALUE_ID, nEvilValue - nCount);
			
			Activity.MANHUANGZHIDI:SetActivityInfo(me.dwId, nEvilValue - nCount, nMinusValue);
		else
			local nAttack = me.GetAttack();
			me.SetAttack(nMinusValue + nAttack);
			
			me.pTask.DirectSetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.MINUS_ATTACK_ID, 0);
			me.pTask.DirectSetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.KILL_PLAYER_COUNT_ID, 0);
			me.pTask.DirectSetTask(Activity.MANHUANGZHIDI.KILL_PLAYER_GID, Activity.MANHUANGZHIDI.EVIL_VALUE_ID, 0);
			Activity.MANHUANGZHIDI:SetActivityInfo(me.dwId, 0, 0);
			me.SetChangeNameColor(0);
		end
		
		me.SendBlackMsg(string.format("∂Ò»À÷µ-%d", math.min(nEvilValue, nCount)));
	else
		me.SendBlackMsg(Lang.item.str69[Lang.Idx]);
	end
end





