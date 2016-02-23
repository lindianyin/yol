-- 协程简单封装，使用步骤
-- 1. 使用Lib:NewClass继承CCoroutineBase，得到需要使用协程的类CDerv
-- 2. 编写一个协程函数CDerv:CoroutineFun，覆盖基类的函数，相当于协程主函数了，逻辑执行都在该函数内部
-- 3. 使用CDerv:BeginCoroutine来启动执行协程
-- 4. 在函数内使用WaitForEvents来使函数暂停，等待的事件名是字符串形式，可以等待多个，逗号隔开
-- 5. 在其他地方调用CDerv:SetEvent来激活协程继续执行,必须的参数是事件名，
-- 如果有参数可以在这个时候传进去，WaitForEvents会返回这些参数
-- 6. TODO 考虑加入等待超时的事件，但这个事件需要有定时器的支持，而我们目前的定时器机制比较混乱，
-- 不容易脱离游戏环境存在

CCoroutineBase = CCoroutineBase or {};

local CURDBGMODE = "Coroutine";
-- 初始化
function CCoroutineBase:init()
	self.m_Co = nil;
end


-- 创建协程
function CCoroutineBase:CreateCoroutine(...)
	self.m_Co = coroutine.create(function ()
									self:CoroutineFun(unpack(arg));
									self:ResetCo();
								end
							);
end

-- 恢复运行
function CCoroutineBase:Resume(...)
	if (self.m_Co == nil) then
		return 0;
	end
	local tbRet = {coroutine.resume(self.m_Co, unpack(arg))};
	if (tbRet[1] == true) then
		return 1;
	else
		Dbg:Output(CURDBGMODE, "---------协程调用出错开始--------------");
		Dbg:Output(CURDBGMODE, unpack(tbRet, 2));
		Dbg:Output(CURDBGMODE, "---------协程调用出错结束--------------");
		return 0;
	end
end

-- 重置co
function CCoroutineBase:ResetCo()
	self.m_Co = nil;
end

-- 开始执行协程
function CCoroutineBase:BeginCoroutine(...)
	if (self.m_Co == nil) then
		self:CreateCoroutine(unpack(arg));
	else
		-- 如果已经有了，怎么处理？
		return 0;
	end
	return self:Resume();
end

-- 等待事件，有一个事件到达就可以满足
-- 等待的事件格式为字符串格式，事件名不可重复，可以等待多个事件，用逗号隔开
function CCoroutineBase:WaitForEvents(...)
	if (self.m_Co == nil) then
		Dbg:Output(CURDBGMODE, "协程没有创建", unpack(arg));
		return nil;
	end
	Dbg:Output(CURDBGMODE, "等待事件：", unpack(arg));
	local tbMsg = {};
	for k, v in ipairs(arg) do --会忽略arg.n
		tbMsg[v] = 1;
	end	
	while(true) do
		local tbRet = {coroutine.yield()};
		if (tbMsg[tbRet[1]] or tbRet[1] == "ForceExit") then
			return unpack(tbRet);
		else
			Dbg:Output(CURDBGMODE, "警告，接收到非等待事件！", tbRet[1]);
		end
	end
end

-- 等待事件，所有事件都到达了才可以
-- 等待的事件格式为字符串格式，事件名不可重复，可以等待多个事件，用逗号隔开
-- 返回最后一个事件的名字和设置事件时带的参数
function CCoroutineBase:WaitForAllEventsSigned(...)
	if (self.m_Co == nil) then
		return nil;
	end
	local tbMsg = {};
	for k, v in ipairs(arg) do --会忽略arg.n
		tbMsg[v] = 1;
	end	
	while(true) do
		local tbRet = {coroutine.yield()};
		tbMsg[tbRet[1]] = nil;
		if (Lib:CountTB(tbMsg) == 0 or tbRet[1] == "ForceExit") then
			return unpack(tbRet);
		end
	end
end

-- 设置事件
-- 设置事件的时候，必须指定事件名，同时可以传递不定参数（最多5个），这些参数会返回到WaitForEvents那里
-- 由上层逻辑判断决定传递和处理参数
function CCoroutineBase:SetEvent(szEventName, ...)
	Dbg:Output(CURDBGMODE, "设置事件：" .. szEventName);
	return self:Resume(szEventName, unpack(arg));
end

-- 协程函数，需要派生类覆盖，这只是个例子
function CCoroutineBase:CoroutineFun(...)
	local szEvent = self:WaitForEvents("Wait", "Wait2");
	--if (szEvent == "Wait") then
		print("very good!");
	--end
end

--local oTest = Lib:NewClass(CCoroutineBase);

--oTest:BeginRun();
--oTest:SetEvent("Wait");
--oTest:SetEvent("Wait2");

