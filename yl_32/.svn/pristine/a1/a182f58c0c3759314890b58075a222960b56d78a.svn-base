
local tb	= Task:GetTarget("BagSpace");
tb.szTargetName	= "BagSpace";
tb.REFRESH_FRAME	= 18;	-- 一秒检测一次

function tb:Init(nBagType, nNeedSpaceCount)
	self.nBagType			= nBagType;
	self.nNeedSpaceCount	= nNeedSpaceCount;
end;

function tb:Start()
	-- nothing
end;

function tb:Save(nGroupId, nStartTaskId)
	self.nGroupId = nGroupId;
	return 0;
end;

function tb:Load(nGroupId, nStartTaskId)
	self:Register();
	self.nGroupId = nGroupId;
	return 0;
end;

function tb:IsDone()
	return self:GetBagSpaceCount() >= self.nNeedSpaceCount;
end;




function tb:GetDesc()
	local szMsg	= string.format(Lang.task.target.str10[Lang.Idx], Lang.task.target.str12[Lang.Idx], self:GetBagSpaceCount(), self.nNeedSpaceCount);
	return szMsg;
end;

function tb:GetStaticDesc()
	local szMsg	= string.format(Lang.task.target.str11[Lang.Idx], Lang.task.target.str12[Lang.Idx], self.nNeedSpaceCount);
	return szMsg;
end;

function tb:Close(szReason)
	self:UnRegister();
end;

function tb:Register()
	-- 客户端实时检查物品数量
	if (MODULE_GAMECLIENT) then
		if (not self.nRegisterId) then
			self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTimer, self);
			self.nCount			= self:GetBagSpaceCount();
		end;
	end;
end;

function tb:UnRegister()
	-- 客户端关闭实时监视
	if (MODULE_GAMECLIENT) then
		if (self.nRegisterId) then
			Timer:Close(self.nRegisterId);
			self.nRegisterId	= nil;
		end;
	end;
end;


function tb:OnTimer(nTickCount)
	if (self:GetBagSpaceCount() ~= self.nCount) then
		self.nCount = self:GetBagSpaceCount();
	else
		return self.REFRESH_FRAME;
	end;
	
	if (MODULE_GAMESERVER) then	-- 自行同步到客户端，要求客户端刷新
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, self.nGroupId);
	end;
	
	-- UNDONE:背包满自动触发不合常理
--[[	
	if (self:IsDone()) then
		self.tbTask:OnFinishOneTag();
	end
--]]
	
	return self.REFRESH_FRAME;
end;


function tb:GetBagSpaceCount()
	if (MODULE_GAMESERVER) then	-- 服务端每次重新计算
		return me.pItem.CalcFreeItemCellCount();
	else	-- 客户端检查过频，使用缓冲
		return self.nCount;
	end;
end;
