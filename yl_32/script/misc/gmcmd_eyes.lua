-- 用于执行KingEyes到游戏指令的映射

GM.tbEyesKey2Fun = {
	["LoadScript"]				= {},										-- 指令
	["QueryServerInfo"]			= {},										-- 查询服务器信息
	
	["SysMsgInChat"]			= {"szContent"},							-- 聊天栏公告
	["SysMsgInNews"]			= {"szContent"},							-- 顶部公告栏
	["KickPlayerOnLine"]		= {"szName"},								-- 在线踢人
	["ArrestPlayer"]			= {"szName"},								-- 抓入天牢
	["SetFreePlayer"]			= {"szName"},								-- 释放玩家
	["PlayerBanChat"]			= {"szPlayerName", "nBanTime"},				-- 禁言
	["TransferToCity"]			= {"szPlayerName"},							-- 传回主城（卡场景时用）
}
local GmCmd = {}

function GM:DoMappedCmdKingEye(szCmdKey, nSession, nAsker,  szData)
	--print(szCmdKey, nSession, nAsker, szData)
	if (type(szCmdKey) ~= "string") then
		return 0, "the cmdkey is not string"
	end
	
	local tbParamDef = self.tbEyesKey2Fun[szCmdKey]
	if (not tbParamDef) then
		print("Wrong Key To Mapped Commod ", szCmdKey)
		return 0, "key is not mapped: " .. szCmdKey
	end
	
	local fnCmdFunc = GmCmd[szCmdKey]
	if type(fnCmdFunc) ~= "function" then
		return 0, "function not exist: " .. szCmdKey
	end
	
	local varData = {szData}
	if (tbParamDef[1]) then	-- 有参数定义
		local fnGetTb, szErrorMsg	= loadstring("return "..szData)
		if (not fnGetTb) then
			return 0, szErrorMsg
		end
		
		varData	= fnGetTb()
		-- 参数检查
		if (type(varData) ~= "table") then
			return 0, "Param format error! Table expected!"
		end
		for i, szParamDef in ipairs(tbParamDef) do
			if (not varData[szParamDef]) then
				return 0, "Param ["..szParamDef.."] is missing!"
			end
			varData[i] = varData[szParamDef]
		end
	end
	
	local szResult = fnCmdFunc(GmCmd, unpack(varData))
	szResult = (szResult == nil and "" or szResult)
	szResult = szResult or ""
	
	if type(szResult) ~= "string" then
		szResult = tostring(szResult)
	end
	KGameserverEyes.PostGmResult(nSession, 1, 1, nAsker, szResult)
	return 1, "Ok"
end

function GmCmd:QueryServerInfo()
	return KScene.Dump()
end

function GmCmd:LoadScript(szFile)
	return DoScript(szFile)
end

function GmCmd:SysMsgInChat(szContent)
	KChat.BroadcastSysMsg(szContent) -- default Chat.emKSYSTEMMSG_NORMAL
end

function GmCmd:SysMsgInNews(szContent)
	KChat.BroadcastSysMsg(szContent, Chat.emKSYSTEMMSG_IMPORTANT)
end

function GmCmd:KickPlayerOnLine(szName)
	local pPlayer = KGameBase.GetPlayerByName(szName)
	if not pPlayer then
		return string.format(Lang.misc.str12[Lang.Idx], szName)
	end
	pPlayer.KickOut()
end

function GmCmd:ArrestPlayer(szName)
	local pPlayer = KGameBase.GetPlayerByName(szName)
	if not pPlayer then
		return string.format(Lang.misc.str12[Lang.Idx], szName)
	end
	pPlayer.TeleportTo(10,2167,1815,1048704) -- TODO: further processing
end

function GmCmd:SetFreePlayer(szName)
	local pPlayer = KGameBase.GetPlayerByName(szName)
	if not pPlayer then
		return string.format(Lang.misc.str12[Lang.Idx], szName)
	end
	-- 送回锦阳
	pPlayer.TeleportTo(5,3864,12086,1059520) -- TODO: need further processing
end

function GmCmd:PlayerBanChat(szPlayerName, nBanTime)
	local pPlayer = KGameBase.GetPlayerByName(szPlayerName)
	if not pPlayer then
		return string.format(Lang.misc.str12[Lang.Idx], szPlayerName)
	end
	pPlayer.SetBanChatEndTime(KGameBase.GetTime() + nBanTime)
end

function GmCmd:TransferToCity(szPlayerName)
	local pPlayer = KGameBase.GetPlayerByName(szPlayerName)
	if not pPlayer then
		return string.format(Lang.misc.str12[Lang.Idx], szPlayerName)
	end
	pPlayer.TeleportTo(5,3864,12086,1059520)
	pPlayer.KickOut()
end
