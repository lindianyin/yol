local tb	= Task:GetTarget("AddTaskItem");
tb.szTargetName	= "AddTaskItem";


function tb:Init(tbTaskItemId)
	self.tbItemId	= {20, 1, tbTaskItemId[1], 1, 0,0};
end;

function tb:Start()
	if (MODULE_GAMESERVER) then	-- 服务端看情况添加物品
		if (Task:GetItemCount(self.me, self.tbItemId) <= 0) then
			Task:AddItem(self.me, self.tbItemId);
		end
	end
end;

function tb:Save(nGroupId, nStartTaskId)
	return 0;
end;

function tb:Load(nGroupId, nStartTaskId)
	if (MODULE_GAMESERVER) then	-- 服务端看情况添加物品
		if (Task:GetItemCount(self.me, self.tbItemId) <= 0) then
			Task:AddItem(self.me, self.tbItemId);
		end
	end
	
	return 0;
end;


function tb:IsDone()
	return 1;
end;

function tb:GetDesc()
	return "";
end;

function tb:GetStaticDesc()
	return "";
end;

function tb:Close(szReason)
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品
		if (szReason == "finish" or szReason == "failed" or szReason == "giveup") then
			Task:DelItem(self.me, self.tbItemId, 1);
		end;
	end;
end;

