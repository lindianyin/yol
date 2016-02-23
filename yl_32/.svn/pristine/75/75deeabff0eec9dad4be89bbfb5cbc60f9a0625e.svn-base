if (not CallCenter.tbGlobalEvent) then
	CallCenter.tbGlobalEvent	= {};
	CallCenter.tbGlobalFun	= {};
end

-- 注册全局事件回调
function CallCenter:RegisterGlobalEvent(szEvent, varCallBack, ...)
	local tbEvent	= self.tbGlobalEvent[szEvent];
	if (not tbEvent) then
		tbEvent	= {};
		self.tbGlobalEvent[szEvent]	= tbEvent;
	end;
	local nRegisterId	= #tbEvent + 1;
	tbEvent[nRegisterId]= {varCallBack, arg};
	return nRegisterId;
end;

-- 注销全局玩家事件回调
function CallCenter:UnRegisterGlobalEvent(szEvent, nRegisterId)
	local tbEvent	= self.tbGlobalEvent[szEvent];
	if (not tbEvent or not tbEvent[nRegisterId]) then
		return;
	end;
	tbEvent[nRegisterId]	= nil;
	return 1;
end;

-- 注销全局玩家事件回调(以注册的callback或者Id来注销)
function CallCenter:UnRegisterGlobalEventEx(szEvent, param)
	if (type(param) == "number") then
		return self:UnRegisterGlobalEvent(szEvent, param)
	end
	if (type(param) == "function") then
		local tbEvent	= self.tbGlobalEvent[szEvent]
		if (tbEvent) then
			for i, v in pairs(tbEvent) do
				if (v[1] == param) then
					tbEvent[i] = nil
					break
				end
			end
		end
	end
end

-- 注册全局事件回调，区别在于这种回调有返回值，一个事件只能注册一次
function CallCenter:RegisterGlobalFun(szFun, varCallBack, ...)
	local tbFun	= self.tbGlobalFun[szFun];
	if (not tbFun) then
		tbFun	= {};
		self.tbGlobalFun[szFun]	= tbFun;
	end;
	local nRegisterId	= #tbFun + 1;
	tbFun[nRegisterId]= {varCallBack, arg};
	return nRegisterId;
end;

-- 注销全局玩家事件回调
function CallCenter:UnRegisterGlobalFun(szFun, nRegisterId)
	local tbFun	= self.tbGlobalFun[szFun];
	if (not tbFun or not tbFun[nRegisterId]) then
		return;
	end;
	tbFun[nRegisterId]	= nil;
	return 1;
end;

-- 被系统调用，某事件发生
function CallCenter:OnEvent(szEvent, ...)
	-- 先检查全局注册回调
	self:_CallBack(true, self.tbGlobalEvent[szEvent], arg);
end
CallCenter.FireEvent = CallCenter.OnEvent

-- 被系统调用，调用函数
function CallCenter:CallFun(szEvent, ...)
	-- 先检查全局注册回调
	return self:_CallBack(false, self.tbGlobalFun[szEvent], arg);
end;

CallCenter.nRunTime = 0;
function CallCenter:_CallBack(bIsEvent, tbEvent, tbArg)
--local nTime = KGRLInterface.GetTickCount();	
	if (not tbEvent) then
		return;
	end
	--为了防止循环中出现新注册导致出错，采用Copy方式
	for nRegisterId, tbCallFunc in pairs(Lib:CopyTB1(tbEvent)) do
		if (tbEvent[nRegisterId]) then	-- 检测是否未被删除
			local varCallBack	= tbCallFunc[1];
			local varParam	= tbCallFunc[2];
			local tbCallBack	= nil;
			if (varParam.n ~= 0) then		-- 如果传入了自定义的参数
				tbCallBack	= {varCallBack, unpack(varParam)};
				Lib:MergeTable(tbCallBack, tbArg);
			else
				tbCallBack	= {varCallBack, unpack(tbArg)};
			end
			if (bIsEvent) then
				Lib:CallBack(tbCallBack);
			else
				local ret = {Lib:CallBack(tbCallBack)};
--CallCenter.nRunTime = CallCenter.nRunTime + KGRLInterface.GetTickCount() - nTime;
				return unpack(ret);
			end
		end
	end

--CallCenter.nRunTime = CallCenter.nRunTime + KGRLInterface.GetTickCount() - nTime;
end
