
local tb	= Task:GetTarget("SearchItemEx");
tb.szTargetName	= "SearchItemEx";
tb.REFRESH_FRAME	= 18;	-- 一秒检测一次

function tb:Init(szItemName, nGenre, nDetail, nParticular, szMagic, nNeedCount, bDelete)
	self.szItemName = szItemName;
	self.nGenre = nGenre;
	self.nDetail = nDetail;
	self.nParticular = nParticular;
	self.szMagic = szMagic;
	self.nNeedCount = nNeedCount;
	self.bDelete = bDelete;
	
	self.tbItemId = {nGenre, nDetail, nParticular, 0};
end;

function tb:Start()
	self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};

	return 0;
end;

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	
	self:Register();
	
	return 0;
end;

function tb:IsDone()
	return self:GetCount() >= self.nNeedCount;
end;

function tb:GetDesc()
	local szMsg	= string.format(Lang.task.target.str53[Lang.Idx].."%s：%d/%d", self.szItemName, self:GetCount(), self.nNeedCount);
	return szMsg;
end;

function tb:GetStaticDesc()
	local szMsg	= string.format(Lang.task.target.str53[Lang.Idx].."%s：%d", self.szItemName, self.nNeedCount);
	return szMsg;
end;

function tb:Close(szReason)
	self:UnRegister();
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品
		if (szReason == "finish" and self.bDelete) then
			Task:DelItem(self.me, self.tbItemId, self.nNeedCount);
		end;
	end;
end;

function tb:Register()
	if (not self.nRegisterId) then
		self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTimer, self);
	end;
end;

function tb:UnRegister()
	if (self.nRegisterId) then
		Timer:Close(self.nRegisterId);
		self.nRegisterId	= nil;
	end;
end;

-- 返回已有物品数量
function tb:GetCount()
	return Task:GetItemCount(self.me, self.tbItemId);
end;

function tb:OnTimer(nTickCount)
	local nCount	= self:GetCount();
	if (not self.nCount or self.nCount ~= nCount) then
		self.nCount	= nCount;
		
		local tbSaveTask	= self.tbSaveTask;
		if (MODULE_GAMESERVER) then	-- 自行同步到客户端，要求客户端刷新
			KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
		end;
		
		if (self:IsDone()) then
			self.tbTask:OnFinishOneTag();
		end
	end;
	
	return self.REFRESH_FRAME;
end;
