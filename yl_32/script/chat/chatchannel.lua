if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end


function Chat:CheckSendCondition(nChannelId, nSenderId,  nReceiverId, strSenderName, strMessage)
	if (MODULE_GAMECLIENT) then
		return 1;
	end
	if nChannelId == Chat.emKCHATCHANNEL_GM then
		return 1;
	end
	if (not self.tbChatChannelCfg[nChannelId]) then
		return 1;
	end
	
	local pPlayer = KGameBase.GetPlayerById(nSenderId);
	-- 非玩家没有时限限制
	if (not pPlayer) then
		return 1;
	end
	if KGameBase.GetTime() <= pPlayer.GetBanChatEndTime() then
		pPlayer.SysMsg(Lang.chat.str1[Lang.Idx] ..os.date("%c", pPlayer.GetBanChatEndTime()));
		return 0;
	end
	if nChannelId == Chat.emKCHATCHANNEL_GLOBAL and pPlayer.IsGm() ~= 1 then
		pPlayer.SysMsg(Lang.chat.str2[Lang.Idx]);
		return 0;
	end
	
	-- 判断喇叭数
	if (nChannelId == Chat.emKCHATCHANNEL_WORLD) then
		-- 线聊
		local nFreeSpeakerCount = pPlayer.GetWorldFreeSpeakerCount()
		local nSpeakerCount = pPlayer.GetWorldSpeakerCount()
		if (nSpeakerCount + nFreeSpeakerCount <= 0) then
			pPlayer.SysMsg(Lang.chat.str3[Lang.Idx])
			return 0
		end
	elseif (nChannelId == Chat.emKCHATCHANNEL_GLOBAL) then
		-- 区聊
		local nSpeakerCount = pPlayer.GetGlobalSpeakerCount()
		if (nSpeakerCount <= 0) then
			pPlayer.SysMsg(Lang.chat.str4[Lang.Idx])
			return 0
		end
	end
	
	if (self.tbChatChannelCfg[nChannelId].nMsgInterval) then
		local nMsgInterval = tonumber(self.tbChatChannelCfg[nChannelId].nMsgInterval);
		local tbPlayerChat = pPlayer.GetTempTable("Chat");
		tbPlayerChat.tbChannelInterval = tbPlayerChat.tbChannelInterval or {};
		tbPlayerChat.tbChannelInterval[nChannelId] = tbPlayerChat.tbChannelInterval[nChannelId] or {};
		local nLastSendTime = tbPlayerChat.tbChannelInterval[nChannelId].nTime or 0;
		if (KGameBase.GetTime() - nLastSendTime < nMsgInterval) then
			pPlayer.SysMsg(string.format(Lang.chat.str5[Lang.Idx],math.ceil(nMsgInterval + nLastSendTime - KGameBase.GetTime())));
			--pPlayer.SysMsg("您发送得太频繁了，请过"..math.ceil(nMsgInterval + nLastSendTime - KGameBase.GetTime()).."秒再尝试发送。");
			return 0;
		end
		
		tbPlayerChat.tbChannelInterval[nChannelId].nTime = KGameBase.GetTime();
	end
	return 1;
end

function Chat:GetChannelCfg()
	return self.tbChatChannelCfg;
end

-- 禁言
function Chat:c2s_DisablePlayerChat(pPlayer, szName)
	-- 检查权限
	local nHasAuth = KChat.GetChatDisableAuth(pPlayer);
	assert(nHasAuth == 1);
	
	if nDisableId == pPlayer.dwId then
		pPlayer.SendBlackMsg(Lang.chat.str6[Lang.Idx]);
		return;
	end
	
	local tNow = KGameBase.GetTime();
	if tNow - KChat.GetChatDisableStartTime(pPlayer) <= 30 then
		pPlayer.SendBlackMsg(Lang.chat.str7[Lang.Idx]);
		return;
	end
	
	KChat.SetChatDisableStartTime(pPlayer);
	KGameBase.CenterExecute{"Chat:ApplyDisableChat_GC", pPlayer.dwId, szName};
end

function Chat:DoDisableChat_GS(nSenderId, szName)
	
	local pPlayer = KGameBase.GetPlayerByName(szName);
	if not pPlayer then
		return;
	end
	
	KChat.DisablePlayerChat(pPlayer);
end

-- 设置每日赠送线聊喇叭数
function Chat:SetFreeWorldSpeaker(nCount)
	KChat.SetFreeWorldSpeakerCount(nCount)
end

-- 重置线聊
function Chat:ResetWorldSpeaker()
	local tbPlayers = KGameBase.GetAllPlayers()
	local dwTime = KGameBase.GetTime()
	local nFreeWorldSpeakerCount = KChat.GetFreeWorldSpeakerCount()
	for _, pPlayer in pairs(tbPlayers) do
		pPlayer.SetWorldFreeSpeakerCount(nFreeWorldSpeakerCount)
		pPlayer.SetWorldFreeSpeakerResetTime(dwTime)
	end
end
