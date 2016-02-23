--=======================================================================
-- 文件名　：limit.txt
-- 创建者　：yulei(yulei1@kingsoft.com)
-- 修改列表：
-- 创建时间：2011-08-20 16:12:45
-- 功能描述：防沉迷系统相关
--=======================================================================

if not MODULE_GAMESERVER then
	return
end

--用于提醒玩家的定时器Id列表
Player.tbRegLimitMsg = {}

--玩家进入防沉迷监控
function Player:EnterLimitControl(pPlayer, dwPlayerId, nFlag, dwOnlineSecond, dwOfflineSecond)
	local nAwardLimitFlag = Player.AwardFlagNoLimit
	if not nFlag then
		local nLimitFlag = pPlayer.GetLimitPlayTimeFlag()
		nFlag = Player.tbLimitFlag[nLimitFlag]
	end 
	if nFlag == 1 then
		if not dwOnlineSecond then
			dwOnlineSecond = pPlayer.GetLimitOnlineSecond()
		end
		if not dwOfflineSecond then
			dwOfflineSecond = pPlayer.GetLimitOfflineSecond()
		end
		pPlayer.SysMsg(Lang.player.str37[Lang.Idx])
		local nHour = math.floor(dwOnlineSecond / 3600)
		local nMin = math.floor((dwOnlineSecond - 3600 * nHour) / 60)
		local nSec = dwOnlineSecond % 60
		pPlayer.SysMsg(string.format(Lang.player.str38[Lang.Idx], nHour, nMin, nSec))
		
		nHour = math.floor(dwOfflineSecond / 3600)
		nMin = math.floor((dwOfflineSecond - 3600 * nHour) / 60)
		nSec = dwOfflineSecond % 60
		pPlayer.SysMsg(string.format(Lang.player.str39[Lang.Idx], nHour, nMin, nSec))
		if dwOfflineSecond >= Player.UnLimitOffLineTime then
			dwOnlineSecond = 0
		end
		--不健康时间
		if dwOnlineSecond >= Player.WholeLimitOnlineTime then
			nAwardLimitFlag = Player.AwardFlagZero
			pPlayer.SysMsg(Lang.player.str40[Lang.Idx])
			if dwOnlineSecond == Player.WholeLimitOnlineTime then
				pPlayer.CallClientScript({"Login:ShowLimitMessage", math.floor(Player.WholeLimitOnlineTime / 3600)})
			else
				pPlayer.CallClientScript({"Login:ShowLimitMessage", Player.LIMIT_FOURHOUR_MESSAGE})
			end
			Player.tbRegLimitMsg[dwPlayerId] = {}
			Player.tbRegLimitMsg[dwPlayerId].nRegId = Timer:Register(Player.WholeLimitInterval * 16, self.HandleLimitTimer, self, dwPlayerId, 0, 0, Player.WholeLimitInterval)
			Player.tbRegLimitMsg[dwPlayerId].nRestTime = 0
		else
			pPlayer.SysMsg(Lang.player.str41[Lang.Idx])
			nAwardLimitFlag = Player.AwardFlagNoLimit
			Player:RegisterMsg(dwPlayerId, dwOnlineSecond, Player.WholeLimitOnlineTime, Player.NoLimitInterval)
		end
	else
		nAwardLimitFlag = Player.AwardFlagNoLimit
	end
	pPlayer.SetAwardFlag(nAwardLimitFlag)
end

--注册定时器，定时提醒玩家
--参数（玩家Id，目前累积在线时间，这一阶段最大时间，提示间隔时间）单位：秒
function Player:RegisterMsg(dwPlayerId, dwOnlineSecond, dwMaxSecond, dwIntervalSecond)
	local nMaxTime = math.floor(dwMaxSecond / dwIntervalSecond)		--最大提示的次数
	local nPastTime = math.floor(dwOnlineSecond / dwIntervalSecond)	--已经提示过的次数
	local nRestTime = nMaxTime - nPastTime							--还需要提示的次数
	
	Player.tbRegLimitMsg[dwPlayerId] = {}
	Player.tbRegLimitMsg[dwPlayerId].nRestTime = nRestTime
	local nCurRegisterTime = dwIntervalSecond - (dwOnlineSecond - nPastTime * dwIntervalSecond)
	if nCurRegisterTime == 0 then
		nCurRegisterTime = dwIntervalSecond
	end
	Player.tbRegLimitMsg[dwPlayerId].nRegId = Timer:Register(nCurRegisterTime * 16, self.HandleLimitTimer, self, dwPlayerId, dwOnlineSecond, dwMaxSecond, dwIntervalSecond)
end

function Player:HandleLimitTimer(dwPlayerId, dwOnlineSecond, dwMaxSecond, dwIntervalSecond)
	local pPlayer = KGameBase.GetPlayerById(dwPlayerId)
	--如果玩家已经离线，则关闭定时器，并清空相关数据
	if not pPlayer then
		Player.tbRegLimitMsg[dwPlayerId] = nil
		return 0
	end
	Player.tbRegLimitMsg[dwPlayerId].nRestTime = Player.tbRegLimitMsg[dwPlayerId].nRestTime - 1
	local nAwardFlag = pPlayer.GetAwardFlag()
	if Player.tbRegLimitMsg[dwPlayerId].nRestTime > 0 then
		if nAwardFlag == Player.AwardFlagNoLimit then --健康时间阶段，1小时提示1次
			local nPastTime = math.floor(dwMaxSecond / Player.NoLimitInterval) - Player.tbRegLimitMsg[dwPlayerId].nRestTime
			pPlayer.CallClientScript({"Login:ShowLimitMessage", nPastTime})
			--pPlayer.SysMsg(string.format("您累计在线时间已满%d小时", nPastTime))
			return dwIntervalSecond * 16
		elseif nAwardFlag == Player.AwardFlagZero then --不健康时间阶段，15分钟提示1次
			pPlayer.CallClientScript({"Login:ShowLimitMessage", Player.LIMIT_FOURHOUR_MESSAGE})
			--pPlayer.SysMsg("您已进入不健康游戏时间，为了您的健康，请您立即下线休息。如不下线，您的身体将受到损害，您的收益已降为零，直到您的累计下线时间满5小时后，才能恢复正常")
			return dwIntervalSecond * 16
		end
	else
		if nAwardFlag == Player.AwardFlagNoLimit then
			local nHour = math.floor(dwMaxSecond / Player.NoLimitInterval)
			--pPlayer.SysMsg(string.format("您累计在线时间已满%d小时,请您下线休息，做适当身体活动。", nHour))
			pPlayer.CallClientScript({"Login:ShowLimitMessage", nHour})
			Player.tbRegLimitMsg[dwPlayerId].nRegId = Timer:Register(Player.WholeLimitInterval * 16, self.HandleLimitTimer, self, dwPlayerId, Player.WholeLimitOnlineTime, -1, Player.WholeLimitInterval)
			pPlayer.SetAwardFlag(Player.AwardFlagZero)
			return 0
		elseif nAwardFlag == Player.AwardFlagZero then
			pPlayer.CallClientScript({"Login:ShowLimitMessage", Player.LIMIT_FOURHOUR_MESSAGE})
			--pPlayer.SysMsg("您已进入不健康游戏时间，为了您的健康，请您立即下线休息。如不下线，您的身体将受到损害，您的收益已降为零，直到您的累计下线时间满5小时后，才能恢复正常")
			return dwIntervalSecond * 16
		end
	end
	return 0
end

function Player:IsLimitTime(pPlayer)
	local bRet = 0
	--防沉迷判断
	local nWallowFlag = pPlayer.GetAwardFlag()
	if nWallowFlag == Player.AwardFlagZero then
		bRet = 1
	end	
	return bRet
end


function Player:OnDestroyLimitTimer(dwPlayerId)
	if Player.tbRegLimitMsg[dwPlayerId] and Player.tbRegLimitMsg[dwPlayerId].nRegId then
		Timer:Close(Player.tbRegLimitMsg[dwPlayerId].nRegId)
		Player.tbRegLimitMsg[dwPlayerId] = nil
	end
end

if Player.nRegLogout then
	CallCenter:UnRegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Player.nRegLogout);
	Player.nRegLogout = nil
end	
if not Player.nRegLogout then
	Player.nRegLogout = CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, Player.OnDestroyLimitTimer, Player);
end
