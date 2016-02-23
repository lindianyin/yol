-------------------------------------------------------
-- unlockgod.lua
-- 文件描述：解锁大神开始
-- 创建者　：zhengbozhou
-- 创建时间：2014-6-27
-------------------------------------------------------

local tb	= Task:GetTarget("UnlockGod");
tb.szTargetName	= "unlockgod";
tb.REFRESH_FRAME	= 18;	-- 一秒检测一次

function tb:Init(szStaticDesc, szDynamicDesc, nEffectTime)
	self.szStaticDesc	= szStaticDesc;
	self.szDynamicDesc	= szDynamicDesc;
	--self.nEffectId = nEffectId;
	self.nEffectTime = nEffectTime;
	self.nCurrEffectTime = nEffectTime;
end

function tb:Start()
	self.bDone = 0;
	
	if (self:IsDone()) then	-- 本目标是一旦达成后不会失效的
		self.tbTask:OnFinishOneTag();
		return;
	end
	self:Register();
end

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone, 1);
	return 1;
end

function tb:Load(nGroupId, nStartTaskId)	
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	
	if (self.bDone ~= 1) then
		self:Register();
	end
	
	return 1;
end

function tb:IsDone()
	return self.bDone == 1;
end

function tb:GetDesc()
	return self.szDynamicDesc;
end

function tb:GetStaticDesc()
	return self.szStaticDesc;
end

function tb:Close(szReason)
	self:UnRegister();
end

function tb:Register()
	if (MODULE_GAMESERVER and not self.nRegisterId) then
		self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTimer, self);
	end
end

function tb:UnRegister()
	if (MODULE_GAMESERVER and self.nRegisterId) then
		Timer:Close(self.nRegisterId);
		self.nRegisterId	= nil;
	end
end

function tb:OnTimer()
	self.bDone = 0;
	self.nCurrEffectTime = self.nCurrEffectTime - 1;
	if self.nCurrEffectTime <= 0 then
	    self.bDone = 1
	end
	
	if (self:IsDone()) then
		self.me.pTask.DirectSetTask(Instance.UNLOCK_GOD_GID, Instance.UNLOCK_GOD_START, 1);
		local tbSaveTask	= self.tbSaveTask;
		if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
			self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
			KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
		end
	
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	
		self.tbTask:OnFinishOneTag();
	end
end
