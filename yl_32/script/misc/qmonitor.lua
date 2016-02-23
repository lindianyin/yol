
if not MODULE_GAMESERVER then
	return
end

QMonitor.FormatString = 
[[
Uptime .......................... %s
连接数(当前/请求/接受) .......... %d,  %d,  %d
发包/收包(次) ................... %d,  %d
发包/收包频率(秒) ............... %.2f,  %.2f
发包/收包(Bytes) ................ %d,  %d
发包/收包速率(Bytes/s) .......... %d,  %d

副本数/总计 ................. %d,  %d
副本开启/结束速率(分钟) ......... %.2f,  %.2f
任务数 .................... %d
Npc/Doodad/Item ................. %d,  %d,  %d
]]

function QMonitor:GetRunStat()
	local strInfo

	local nTotalTask = 0

	local tbSockInfo = KGameserverEyes.GetGameserverSocketInfo()
	
	local fRunSecond = KGameserverEyes.GetRealSecond()
	local fRunMin = fRunSecond / 60

	local fSendPerSecond = tbSockInfo.dwSendPackages / fRunSecond
	local fRecvPerSecond = tbSockInfo.dwRecvPackages / fRunSecond
	
	strInfo = string.format(self.FormatString,
		Lib:TimeFullDesc(fRunSecond),
		tbSockInfo.dwCurrentConnection, tbSockInfo.dwTotalConnection, tbSockInfo.dwProcessedConnection,
		tbSockInfo.dwSendPackages, tbSockInfo.dwRecvPackages,
		fSendPerSecond, fRecvPerSecond,
		tbSockInfo.dwSendBytes, tbSockInfo.dwRecvBytes,
		tbSockInfo.dwSendBytesPerSecond, tbSockInfo.dwRecvBytesPerSecond,
		
		0, 0,
		0, 0,
		nTotalTask,
		KGameBase.GetNpcCount(), KGameBase.GetDoodadCount(), KItem.GetItemCount()
		)
	if KGameBase.IsDevMode() == 1 then
		strInfo = strInfo.."<br><font color=red>警告：运行于开发模式（允许GM指令等）</font><br>"
	end
	return strInfo
end
