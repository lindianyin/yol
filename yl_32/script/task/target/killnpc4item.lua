
local tb	= Task:GetTarget("KillNpc4Item");
tb.szTargetName	= "KillNpc4Item";
tb.REFRESH_FRAME	= Env.GAME_FPS * 2;	-- zhengyuhua:降低刷新频率(原：一秒检测一次)


function tb:Init(nNpcTempId, nMapId, tbTaskItemId, nPercent, nNeedCount, bDelete, bShare, szLaterPop)
	self.nNpcTempId	= nNpcTempId;
	self.szNpcName	= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.nMapId		= nMapId;
	self.szMapName	= Task:GetMapName(nMapId);
	self.tbItemId	= {20,1,tbTaskItemId[1],1,-1,0};
	self.szItemName	= KItem.GetNameById(unpack(self.tbItemId)) or "";
	self.nPercent	= nPercent;
	self.nNeedCount	= nNeedCount;
	self.bDelete	= bDelete;
	self.bShare		= tonumber(bShare) or 1;
	self.szLaterPop	= szLaterPop;
end;


function tb:Start()
	self:Register();
end;

function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	
	
	return 1;
end;

function tb:Load(nGroupId, nStartTaskId)
		self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self:Register();
	
	return 1;
end;

function tb:IsDone()
	return self:GetCount() >= self.nNeedCount;
end;

function tb:GetDesc()
	local szMsg	= Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	local nCurCount = self:GetCount();
	if (nCurCount > self.nNeedCount) then
		nCurCount = self.nNeedCount;
	end
	
	szMsg	= szMsg..string.format(Lang.task.target.str37[Lang.Idx], self.szNpcName, self.szItemName, nCurCount, self.nNeedCount);
	return szMsg;
end;

function tb:GetStaticDesc()
	local szMsg	=  Lang.task.target.str16[Lang.Idx];
	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format(Lang.task.target.str39[Lang.Idx], self.szNpcName, self.szItemName, self.nNeedCount);
	return szMsg;
end;


function tb:Close(szReason)
	if (MODULE_GAMESERVER) then	-- 服务端看情况删除物品
		if (szReason == "finish" and self.bDelete) then
			Task:DelItem(self.me, self.tbItemId, self.nNeedCount);
		end
	end
	
	self:UnRegister();
end;


function tb:Register()
	if (MODULE_GAMESERVER) then
		if (not self.nRegisterId) then
			self.tbTask:AddInCollectList(self.tbItemId);
			self.bCanCollect = 1;
			self.nRegisterId	= Timer:Register(self.REFRESH_FRAME, self.OnTimer, self);
		end
		if (not self.nRegPickId) then
			--Setting:SetGlobalObj(self.me);
			--self.nRegPickId		= PlayerEvent:Register("OnPickUp", self.OnPickUp, self);
			--Setting:RestoreGlobalObj()
		end
	end;
end


function tb:UnRegister()
	if (MODULE_GAMESERVER) then
		if (self.nRegisterId) then
			Timer:Close(self.nRegisterId);
			self.nRegisterId	= nil;
		end
		if (self.nRegPickId) then
			Setting:SetGlobalObj(self.me);
			--PlayerEvent:UnRegister("OnPickUp", self.nRegPickId);
			Setting:RestoreGlobalObj()
			self.nRegPickId = nil;
		end
	end;
end


function tb:OnPickUp()
	if (self:IsDone()) then
		if (self.bCanCollect == 1) then
			self.tbTask:RemoveInCollectList(self.tbItemId);
			self.bCanCollect = 0;
		end
	else
		if (self.bCanCollect == 0) then
			self.tbTask:AddInCollectList(self.tbItemId);
			self.bCanCollect = 1;
		end
	end
end

function tb:OnTimer()
	local nCount	= self:GetCount();
	if (not self.nCount or self.nCount ~= nCount) then
		self.nCount	= nCount;
		local tbSaveTask	= self.tbSaveTask;
		if (MODULE_GAMESERVER) then	-- 自行同步到客户端，要求客户端刷新
			KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
		end;
		
		if (self:IsDone()) then
			if (self.bCanCollect == 1) then
				self.tbTask:RemoveInCollectList(self.tbItemId);
				self.bCanCollect = 0;
			end
			self.tbTask:OnFinishOneTag();
		else
			if (self.bCanCollect == 0) then
				self.tbTask:AddInCollectList(self.tbItemId);
				self.bCanCollect = 1;
			end
		end
	end;
	
	return self.REFRESH_FRAME;
end


function tb:OnKillNpcDropItem(pPlayer, pNpc)
	assert(MODULE_GAMESERVER);
	if (self:IsDone()) then
		return 0;
	end
	
	if (self.nNpcTempId ~= pNpc.nTemplateId) then
		return 0;
	end;
	
	if (self.nMapId ~= 0 and self.nMapId ~= pPlayer.GetMapId()) then
		return 0;
	end;
	
	
	if (MathRandom() >= self.nPercent) then
		return 0;
	end

--	local nPosX, nPosY = pNpc.GetMpsPos();
--	local nMapIdx = 0;
--	if (self.nMapId <= 0) then
--		nMapIdx	= SubWorldID2Idx(pPlayer.GetMapId()); 
--	else
--		nMapIdx	= SubWorldID2Idx(self.nMapId);
--	end
--	
--	Task:AddObjAtPos(pPlayer, self.tbItemId, nMapIdx, nPosX, nPosY);
	
	Task:AddItem(pPlayer, self.tbItemId)
	return 1;
end


-- 返回已有物品数量
function tb:GetCount()
	return Task:GetItemCount(self.me, self.tbItemId);
end;
