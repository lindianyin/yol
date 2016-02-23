	
-- 任务道具，通用功能脚本

------------------------------------------------------------------------------------------
-- initialize

local tbTaskItem = Item:GetClass("taskitem");

------------------------------------------------------------------------------------------
-- public

function tbTaskItem:OnUse()
	Task:OnTaskItem(it);
	return 0;
end

function tbTaskItem:IsPickable()
	for _, tbTask in pairs(Task:GetPlayerTask(me).tbTasks) do
		if (tbTask:IsItemToBeCollect({it.nGenre, it.nDetail, it.nParticular, it.nLevel}) == 1) then
			return 1;
		end
	end
	
	me.Msg(Lang.item.str70[Lang.Idx])
	
	return 0;
end

function tbTaskItem:PickUp()
	Task:SharePickItem(me, it);
	return 1;
end

function tbTaskItem:GetTip()
	local szTip = "";

	if (it.nParticular == 463) then
		szTip = szTip..Lang.item.str71[Lang.Idx];
	elseif(it.nParticular == 464) then
		szTip = szTip..Lang.item.str72[Lang.Idx];
	elseif(it.nParticular == 465) then
		szTip = szTip..Lang.item.str73[Lang.Idx];
	end

	return szTip;

end
