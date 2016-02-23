
local tb	= Task:GetTarget("GiveItem");
tb.szTargetName	= "GiveItem";

-- Npc模板Id, 对话选项，对话内容，物品列表,
function tb:Init(nNpcTempId, nMapId, szOption, szMsg, szRepeatMsg, szItemSet, szBeforePop, szLaterPop, szDesc, nCheckItem)
	self.nNpcTempId = nNpcTempId;
	self.nMapId		= nMapId;
	self.szMapName	= Task:GetMapName(nMapId);
	self.szNpcName	= KGameBase.GetNameByTemplateId(nNpcTempId);
	self.szOption	= szOption;
	self.szMsg		= szMsg;
	if (szRepeatMsg ~= "") then
		self.szRepeatMsg	= szRepeatMsg;
	end;
	self.ItemList	= self:ParseItem(szItemSet);
	self.szBeforePop	= szBeforePop;
	self.szLaterPop		= szLaterPop;
	self.szDesc			= szDesc;
	self.nCheckItem		= nCheckItem;
end;


function tb:ParseItem(szItemSet)
	local tbRet = {};
	local tbItem = Lib:SplitStr(szItemSet, "\n")
	self.szItemDesc = "";
	for i=1, #tbItem do
		if (tbItem[i] and tbItem[i] ~= "") then
			-- 每行的物品格式：{物品名, nGenre, nDetail, nParticular, nLevel, nSeries, nItemNum}
			local tbTemp = loadstring(string.gsub(tbItem[i],"{.+,(.+),(.+),(.+),(.+),(.+),(.+)}", "return {tonumber(%1),tonumber(%2),tonumber(%3),tonumber(%4),tonumber(%5),tonumber(%6)}"))()
			local tbTempEx =  Lib:SplitStr(tbItem[i]);
			if tbTempEx[8] then
				tbTemp[7] = 1;
			end			
			table.insert(tbRet, tbTemp);
			local szItemName = KItem.GetNameById(unpack(tbTemp));
			local szItemNum	 = tostring(tbTemp[6]);
			self.szItemDesc = self.szItemDesc..szItemNum.."个"..szItemName.."、";
		end
	end
	
	if (string.len(self.szItemDesc) > 0) then
		self.szItemDesc = string.sub(self.szItemDesc, 1, string.len(self.szItemDesc)-2); 
	end
	return tbRet;
end;


function tb:Start()
	self.bDone		= 0;
	self:Register();
end;


function tb:Save(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.me.pTask.SetTask(nGroupId, nStartTaskId, self.bDone);
	return 1;
end;

function tb:Load(nGroupId, nStartTaskId)
	self.tbSaveTask	= {	-- 这里保存下来，以后随时可以自行同步客户端
		nGroupId		= nGroupId,
		nStartTaskId	= nStartTaskId,
	};
	self.bDone		= self.me.pTask.GetTask(nGroupId, nStartTaskId);
	if (not self:IsDone()  or self.szRepeatMsg) then	-- 本目标是一旦达成后不会失效的
		self:Register();
	end;
	return 1;
end;

function tb:IsDone()
	return self.bDone == 1;
end;

function tb:GetDesc()
	return self:GetStaticDesc();
end;

function tb:GetStaticDesc()
	local szMsg = "";
	if (self.szItemDesc) then
		szMsg = Lang.task.target.str28[Lang.Idx]..self.szItemDesc.. Lang.task.target.str29[Lang.Idx];
	else
		szMsg	= Lang.task.target.str30[Lang.Idx];
	end

	if (self.nMapId ~= 0) then
		szMsg	= szMsg..self.szMapName..Lang.task.target.str17[Lang.Idx];
	end;
	szMsg	= szMsg..string.format("%s", self.szNpcName);
	return szMsg;
end;


function tb:Close(szReason)
	self:UnRegister();
end;


function tb:Register()
	self.tbTask:AddNpcMenu(self.nNpcTempId, self.nMapId, self.szOption, self.OnTalkNpc, self);
	if not self.nCheckItem then
		self:ResetItemCount();
	end
end;


-- 确保物品数目是正确的
function tb:ResetItemCount()
	for _,tbItem in ipairs(self.ItemList) do
		local nCount = Task:GetItemCount(self.me, {tbItem[1], tbItem[2], tbItem[3], tbItem[4], tbItem[5]});
		local nAddCount = tbItem[6] - nCount;
		if (nAddCount > 0) then
			if ((not tbItem[7]) and TaskCond:CanAddCountItemIntoBag({tbItem[1], tbItem[2], tbItem[3], tbItem[4], tbItem[5]}, nAddCount)) then
				Task:AddItems(self.me, {tbItem[1], tbItem[2], tbItem[3], tbItem[4], tbItem[5]}, nAddCount);
			end
		elseif ((not tbItem[7]) and nAddCount < 0) then
			Task:DelItem(self.me, {tbItem[1], tbItem[2], tbItem[3], tbItem[4], tbItem[5]}, -nAddCount);
		end
	end
end


function tb:UnRegister()
	self.tbTask:RemoveNpcMenu(self.nNpcTempId);
end;


function tb:OnTalkNpc()
	if (self.nMapId ~= 0 and self.nMapId ~= self.me.GetMapId()) then
		return;
	end;
	if (self:IsDone()) then
		if (self.szRepeatMsg) then
			TaskAct:Talk(self.szRepeatMsg);
		end;
		return;
	end;
	
	TaskAct:Talk(self.szMsg, self.OnTalkFinish, self);
end;

function tb:OnTalkFinish()
	Task.GiveItemTag.tbGiveForm:SetRegular(self, self.me);
	-- 弹给与界面
	Dialog:Gift("Task.GiveItemTag.tbGiveForm");
	
end;

function tb:OnFinish()
	self.bDone	= 1;
	local tbSaveTask	= self.tbSaveTask;
	if (MODULE_GAMESERVER and tbSaveTask) then	-- 自行同步到客户端，要求客户端刷新
		self.me.pTask.SetTask(tbSaveTask.nGroupId, tbSaveTask.nStartTaskId, self.bDone, 1);
		KTask.SendRefresh(self.me, self.tbTask.nTaskId, self.tbTask.nReferId, tbSaveTask.nGroupId);
	end;
	
	if (not self.szRepeatMsg) then
		self:UnRegister()	-- 本目标是一旦达成后不会失效的
	end;
	
	self.tbTask:OnFinishOneTag();
end;
