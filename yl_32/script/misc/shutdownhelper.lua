-- File       : shutdownhelper
-- Creator    : simon
-- Date       : 2010-12-08 20:48:46
-- Description: 安全关闭服务器助手

KGameBase.ShutdownHelper = {}
local tbHelper = KGameBase.ShutdownHelper

-- 在以下时间点发出告示
local tbRemindWhen =
{
	[1] = 1,
	[2] = 1,
	[3] = 1,
	[4] = 1,
	[5] = 1,
	[6] = 1,
	[7] = 1,
	[8] = 1,
	[9] = 1,
	[10] = 1,
	[15] = 1,
	[20] = 1,
	[30] = 1,
	[45] = 1,
	[60] = 1,
	[90] = 1,
	[120] = 1,
	[180] = 1,
	[300] = 1,
	[600] = 1,
	[900] = 1,
	[1800] = 1,
	[3600] = 1,
}

local function funGetTimeString(nSeconds)
	local nHour = math.floor(nSeconds / 3600)
	nSeconds = nSeconds - nHour * 3600
	local nMin = math.floor(nSeconds / 60)
	nSeconds = nSeconds - nMin * 60
	local nSec = nSeconds
	
	local szTime = ""
	if nHour > 0 then
		szTime = szTime..nHour..Lang.misc.str167[Lang.Idx]
	end
	if nMin > 0 then
		szTime = szTime..nMin..Lang.misc.str168[Lang.Idx]
	end
	if nSec > 0 then
		szTime = szTime..nSec..Lang.misc.str169[Lang.Idx]
	end
	return szTime
end

function tbHelper:ShutdownServer(nCountDownSeconds)
	self:Cancel()
	print(Lang.misc.str170[Lang.Idx]..nCountDownSeconds)
	
	self.nTimerId = Timer:Register(Env.GAME_FPS, self._CheckShutdown, self)
	self.nTimeLeft = nCountDownSeconds
end

function tbHelper:Cancel()
	if self.nTimerId then
		print(Lang.misc.str171[Lang.Idx])
		Timer:Close(self.nTimerId)
		self.nTimerId = nil
	end
end

function tbHelper:_Notify(nSeconds)
	KChat.BroadcastSysMsg(string.format(Lang.misc.str172[Lang.Idx],funGetTimeString(nSeconds)))
	--KChat.BroadcastSysMsg("<bgclr=yellow>服务器将在"..funGetTimeString(nSeconds).."内关闭")
end

function tbHelper:_CheckShutdown()
	if self.nTimeLeft <= 0 then
		KGameBase.Shutdown()
		return 0
	elseif tbRemindWhen[self.nTimeLeft] then
		self:_Notify(self.nTimeLeft)
	end
	self.nTimeLeft = self.nTimeLeft - 1
end
