-- 文件名　：timer.lua
-- 创建者　：FanZai
-- 创建时间：2007-10-08 22:09:05
-- 文件说明：计时器系统


-- 范例见最下面

if (not Timer.tbTimeTable) then
	-- 注：	本系统中使用的uActiveTime变量在GC上表示毫秒，GS和Client上表示桢数。
	--		本系统的对外接口保持一致，使用桢数。
	-- 时刻表：[uActiveTime] = {nRegisterId1, nRegisterId2, ...}
	Timer.tbTimeTable	= {};
	-- 注册表：[nRegisterId] = {uActiveTime=.., nIdx=.., tbCallBack=..}
	Timer.tbRegister	= {};
	
	if (not Timer.nHaveUsedTimerId) then
		Timer.nHaveUsedTimerId = 0;
	end;
	Timer.tbAttach	= {};
end

-- 由程序根据注册情况，需要时调用
function Timer:OnActive(uCurrentTime)
	local tbTime		= self.tbTimeTable[uCurrentTime];
	if (not tbTime) then
		return;
	end
	
	-- 建立一个table用于存放在本次调用中决定关闭的Timer
	self.tbToBeClose	= {};

	local oldme = me;
	local oldhim = him;
	--print("........tbTime:"..#tbTime .. " uCurrentTime:"..uCurrentTime);
	-- 这里不会有新的Timer注册在当前帧，可以不复制tbTime
	for nIdx, nRegisterId in pairs(tbTime) do
		if (not self.tbToBeClose[nRegisterId]) then	-- 没有打算关闭此Timer
			local tbEvent		= self.tbRegister[nRegisterId];
			local tbGRoleArg	= tbEvent.tbGRoleArg;
			me = tbGRoleArg.pPlayer;
			him = tbGRoleArg.pNpc;
			
			local tbCallBack	= tbEvent.tbCallBack;
			local bOK, nRet		= Lib:CallBack(tbCallBack);	-- 调用回调

			--local tbxx = debug.getinfo(tbCallBack[1])
			--for k,v in pairs(tbxx) do 
			--	print(k,v)
			--end


			Dbg:Output("Timer", "OnTimer", nRegisterId, bOK, nRet);	-- 通知调试模块，OnTimer调用完毕
			if (not bOK) then		-- 调用失败，关闭此Timer
				nRet	= 0;
				print("[ERROR]OnTimer", tbEvent.szRegInfo)
			elseif (not nRet) then	-- 无返回值，相等时间后再次调用
				nRet	= tbEvent.nWaitTime;
			end
			
			if (nRet <= 0) then	-- 返回0，关闭此Timer
				self.tbToBeClose[nRegisterId]	= 1;
			else	-- 需要再次触发此Event，时间间隔为nRet
				local uNewTime	= KTimer.RegisterTimerPoint(nRet);	-- 注册并得到新的触发时刻
				--print("........uNewTime:"..uNewTime)
				tbEvent.nWaitTime	= nRet;
				tbEvent.uActiveTime	= uNewTime;
				local tbNewTime		= self.tbTimeTable[uNewTime];
				if (not tbNewTime) then	-- 此时刻尚无注册
					self.tbTimeTable[uNewTime]	= {nRegisterId};
					tbEvent.nIdx				= 1;
				else	-- 此时刻已有注册
					tbEvent.nIdx			= #tbNewTime + 1;
					tbNewTime[tbEvent.nIdx]	= nRegisterId;
				end
			end
		end
	end

	me = oldme;
	him = oldhim;
	
	-- 将累积下来的要关闭的Timer全都关闭
	for nRegisterId in pairs(self.tbToBeClose) do
		local tbEvent	= self.tbRegister[nRegisterId];
		local oldme = me;
		local oldhim = him;
		me = tbEvent.tbGRoleArg.pPlayer;
		him = tbEvent.tbGRoleArg.pNpc;
		self.tbRegister[nRegisterId]	= nil;
		if (tbEvent.uActiveTime ~= uCurrentTime) then
			self.tbTimeTable[tbEvent.uActiveTime][tbEvent.nIdx]	= nil;
		end
		if (tbEvent.OnDestroy) then	-- 需要通知销毁
			tbEvent:OnDestroy(nRegisterId);
		end
		
		--神秘商人调试
		Lib:CallBack({"Timer:_DebugMerchant", nRegisterId});
		
		me = oldme;
		him = oldhim;
	end
	
	self.tbTimeTable[uCurrentTime]	= nil;
	self.tbToBeClose				= nil;
end

--注册新Timer，
--	参数：nWaitTime（从现在开始的桢数）, fnCallBack, varParam1, varParam2, ...
--	返回：nRegisterId
function Timer:Register(nWaitTime, ...)
	local tbEvent	= {
		nWaitTime	= nWaitTime,
		tbCallBack	= arg,
		szRegInfo	= debug.traceback("Register Timer", 2),
	};
	--local tb = debug.getinfo(arg[1])
	--for k,v in pairs(tb) do 
	--	print(k,v)
	--end
	--print("##",tbEvent.szRegInfo);
	return self:RegisterEx(tbEvent);
end

-- 注册新Timer，进阶版
function Timer:RegisterEx(tbEvent)
	assert(tbEvent.nWaitTime > 0);
	tbEvent.tbGRoleArg	= { pPlayer = me, pNpc = him };
	tbEvent.uActiveTime	= KTimer.RegisterTimerPoint(tbEvent.nWaitTime)	-- 注册并得到新的触发时刻
	Timer.nHaveUsedTimerId = Timer.nHaveUsedTimerId + 1; 
	local nRegisterId	= Timer.nHaveUsedTimerId;
	self.tbRegister[nRegisterId] = tbEvent;
	local tbNewTime	= self.tbTimeTable[tbEvent.uActiveTime];
	if (not tbNewTime) then	-- 此时刻尚无注册
		self.tbTimeTable[tbEvent.uActiveTime] = {nRegisterId};
		tbEvent.nIdx 			= 1;
	else	-- 此时刻已有注册
		tbEvent.nIdx			= #tbNewTime + 1;
		tbNewTime[tbEvent.nIdx]	= nRegisterId;
	end
	Dbg:PrintEvent("Timer", "Register", nRegisterId, tbEvent.nWaitTime);	-- 通知调试模块，注册新Timer
	return nRegisterId;
end

--关闭Timer
function Timer:Close(nRegisterId)
	Dbg:PrintEvent("Timer", "Close", nRegisterId);	-- 通知调试模块，关闭Timer
	
	if (self.tbAttach[nRegisterId]) then
		print("Close Timer Error:", debug.traceback());
	end;
	
	local tbEvent	= self.tbRegister[nRegisterId];
	if not tbEvent then
		print("CloseTimerWarring", debug.traceback());
		return;
	end
	local oldme = me;
	local oldhim = him;
	me 	= tbEvent.tbGRoleArg.pPlayer;
	him = tbEvent.tbGRoleArg.pNpc;
	if (self.tbToBeClose) then	-- 正在调用Timer，不能直接关闭
		self.tbToBeClose[nRegisterId]	= 1;
	else
		self.tbTimeTable[tbEvent.uActiveTime][tbEvent.nIdx] = nil;
		self.tbRegister[nRegisterId] = nil;
		if (tbEvent.OnDestroy) then	-- 需要通知销毁
			tbEvent:OnDestroy(nRegisterId);
		end
	end
	
	--神秘商人调试
	Lib:CallBack({"Timer:_DebugMerchant", nRegisterId});
			
	me = oldme;
	him = oldhim;
end

--察看指定的Timer剩余多少桢触发
function Timer:GetRestTime(nRegisterId)
	local tbEvent	= self.tbRegister[nRegisterId];
	if (not tbEvent) then
		return -1;
	else
		if (MODULE_GC_SERVER) then
			local nRestTime	= tbEvent.uActiveTime - GCGetCurElapse();
			if (nRestTime < 0) then	-- 卡的时候有可能出现负数，修正一下，不要引起误解
				nRestTime	= 0;
			end
			-- 这里还没有载入Dev无法使用Env.GAME_FPS常数
			return math.floor(nRestTime * 18 / 1000)
		else
			return tbEvent.uActiveTime - KTimer.GetFrame();
		end
	end
end

--察看指定的Timer启动计时时间
function Timer:GetWaitTime(nRegisterId)
	local tbEvent	= self.tbRegister[nRegisterId];
	if (not tbEvent) then
		return -1;
	else
		return tbEvent.nWaitTime;
	end
end

--将形如（123530，表示12时35分30秒)的数字转成秒数
function Timer:CycleTimeToSeconds(cycleTime)
	 local hour = math.floor(cycleTime / 10000)
	 hour = math.max(0, hour)
	 local minute = math.floor((cycleTime - hour * 10000) / 100)
	 minute = math.max(0, minute)
	 local second = cycleTime - hour * 10000 - minute * 100
	 
	 return (hour % 24) * 3600 + minute * 60 + second
end

-- CycleTimeToSeconds的反向功能
function Timer:SecondsToCycleTime(secondTimes)
  local hours = math.floor(secondTimes / 3600)
  local minutes = math.floor((secondTimes % 3600) / 60);
  local seconds = secondTimes % 3600 - minutes * 60;
  
  return (hours % 24) * 10000 + minutes * 100 + seconds; 
end

--神秘商人调试
function Timer:_DebugMerchant(nRegisterId)
	if Npc._tbDebugFreeAITimer and Npc._tbDebugFreeAITimer[nRegisterId] then
		local nNpcId = Npc._tbDebugFreeAITimer[nRegisterId];
		Npc._tbDebugFreeAITimer[nRegisterId]	= nil;
		Npc._tbDebugFreeAITimer2[nNpcId] 		= nil;
		print(debug.traceback("Timer Error: Merchant Error Close!!!"));
	end	
end

do return end
---------------- 以下是范例 ----------------

function SomeEvent:OnTimer()	-- 时间到，会调用此函数
	if (XXX) then
		-- 返回正数，表示要保持此Timer，等待123桢后再次调用
		return 123;
	elseif (YYY) then
		-- 返回0，表示要关闭此Timer
		return 0;
	else
		-- 返回nil，表示等待时间与上次相同
		return;
	end
end

function SomeEvent:Begin()
	-- 开启计时器，记录nRegisterId
	self.nRegisterId	= Timer:Register(1, self.OnTimer, self);
end

function SomeEvent:Stop()
	-- 查看剩余桢数
	print(Timer:GetRestTime(self.nRegisterId))
	-- 关闭计时器
	Timer:Close(self.nRegisterId);
end
