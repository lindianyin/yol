--
-- FileName: mail_gs.lua
-- Author: 	 xiewenzhe
-- Time: 	 2011/5/24 10:46
-- Comment:	 邮件(GS)
--

if (not MODULE_GAMESERVER) then
	return;
end

-- 所有待发送的邮件
Mail.tbSendMail = Mail.tbSendMail or {}

-- 添加待发送邮件
function Mail:AddSendMail(tbMailInfo)
	if (not tbMailInfo) then
		return
	end
	
	local nIndex = 1
	while (true) do
		if (not self.tbSendMail[nIndex]) then
			self.tbSendMail[nIndex] = tbMailInfo
			break
		end
		
		nIndex = nIndex + 1
	end
	
	return nIndex
end

-- 删除待发送邮件（表示已经成功发送）
function Mail:RemoveSendMail(nIndex)
	if (not nIndex) then
		return
	end
	
	self.tbSendMail[nIndex] = nil
end

-- 发送条件判断
function Mail:CanSendMail(pPlayer)
	if (not pPlayer) then
		return 0
	end
	
	local bOverSendLimit = pPlayer.pMail.OverSendLimit(Mail.SEND_LIMIT)
	if (bOverSendLimit == 1) then
		local szFailReason = string.format(Lang.mail.str10[Lang.Idx], Mail.SEND_LIMIT)
		return 0, szFailReason
	end
	
	-- TODO:禁止发送条件
	
	return 1
end

-- 检查附件
function Mail:CheckItem(pPlayer, nCount, tbItem)
	if (not pPlayer) then
		return 0
	end
	if (not tbItem) then
		return 1
	end
	
	local bCheckItem = 1
	for _, nItemId in pairs(tbItem) do
		local pItem = pPlayer.pPlayerItem.FindItemById(nItemId)
		if (not pItem) then
			bCheckItem = 0
			break
		end
		
		-- TODO:检测物品个数
	end
	
	return bCheckItem
end

-----------------------------------------
--  玩家发送邮件
--	tbMailInfo格式：
--	tbMailInfo.nIconId		邮件图标Id
--	tbMailInfo.nType		邮件类型
--  tbMailInfo.nPlayerId 	发件人Id
--	tbMailInfo.tbReceiver	收件人列表
-- 	tbMailInfo.szCaption	邮件标题
--  tbMailInfo.szContent	邮件内容
--  tbMailInfo.tbItem		邮件附件（{{nClassId,nCount,nStateFlags,nTimeoutType,nTimeout,nPrice,szGuid},...}）
------------------------------------------
function Mail:SendMailByPlayer(tbMailInfo)
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str11[Lang.Idx])
		return
	end
	
	local nPlayerId = tbMailInfo.nPlayerId
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if (not pPlayer) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str12[Lang.Idx])
		return
	end
	
	-- 判断邮件类型
	if (tbMailInfo.nType == Mail.TYPE_SYSTEM) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str13[Lang.Idx])
		return
	end
	
	-- 检测当前是否能发送邮件
	local bCanSend, szForbitReason = self:CanSendMail(pPlayer)
	if (bCanSend == 0) then
		if (not szForbitReason) then
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str14[Lang.Idx])
			pPlayer.pMail.SendClientResult(0, self.FAILREASON_FORBID)
		else
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", szForbitReason)
			pPlayer.pMail.SendClientResult(0, self.FAILREASON_FORBID, szForbitReason)
		end
		
		return
	end
	
	-- 标识发送人类型
	tbMailInfo.nSendType = Mail.SEND_TYPE_PLAYER
	
	-- 检测收件人是否合法(实际发送逻辑在检测回馈中做)
	local nIndex = self:AddSendMail(tbMailInfo)
	local szSenderName = pPlayer.szName
	local nCount = #tbMailInfo.tbReceiver
	tbMailInfo.tbReceiver.nCount = nCount
	for i = 1, nCount do
		local szReceiver = tbMailInfo.tbReceiver[i]
		if (szSenderName == szReceiver) then
			self:RemoveSendMail(nIndex)
			
			local szFailReason = Lang.mail.str15[Lang.Idx]
			pPlayer.pMail.SendClientResult(0, self.FAILREASON_RECEIVER, szFailReason)

			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", szFailReason)
			return
		end
	end
	
	local bRet = pPlayer.pMail.CheckReceiver(nIndex, tbMailInfo.nType, tbMailInfo.tbReceiver)
	if (bRet == 0) then
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str16[Lang.Idx])
		pPlayer.pMail.SendClientResult(0, self.FAILREASON_UNKNOWN)
	end
end

-- 实际玩家发送邮件逻辑
function Mail:_SendMailByPlayer(nMailIndex, nResult, tbParam)
	local tbMailInfo = self.tbSendMail[nMailIndex]
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str17[Lang.Idx])
		return
	end
	
	local nPlayerId = tbMailInfo.nPlayerId
	local pPlayer = KGameBase.GetPlayerById(nPlayerId)
	if (not pPlayer) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str18[Lang.Idx])
		return
	end
	
	if (nResult == 0) then
		-- 收件人不合法
		local szFailReason = tbParam
		if (not szFailReason) then
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str19[Lang.Idx])
			pPlayer.pMail.SendClientResult(nResult, self.FAILREASON_RECEIVER)
		else
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", szFailReason)
			pPlayer.pMail.SendClientResult(nResult, self.FAILREASON_RECEIVER, szFailReason)
		end
		
		self:RemoveSendMail(nMailIndex)
		return
	end
	
	-- 记录收件人Id
	if (not tbParam) then
		-- 没有返回Id
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str20[Lang.Idx])
		pPlayer.pMail.SendClientResult(nResult, self.FAILREASON_UNKNOWN)
	else
		tbMailInfo.tbReceiverId = tbParam
		tbMailInfo.tbReceiverId.nCount = Lib:CountTB(tbMailInfo.tbReceiverId)
	end
	
	-- 检查邮资
	local nReceiverCount = tbMailInfo.tbReceiverId.nCount
	local nMoney = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_SILVER)
	if (nMoney < nReceiverCount * self.MAIL_COST) then
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str21[Lang.Idx])
		pPlayer.pMail.SendClientResult(0, self.FAILREASON_MONEY)
		self:RemoveSendMail(nMailIndex)
		return
	end
	
	-- 检查附件
	local bCheckItem = Mail:CheckItem(pPlayer, nReceiverCount, tbMailInfo.tbItem)
	if (bCheckItem == 0) then
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str22[Lang.Idx])
		pPlayer.pMail.SendClientResult(0, self.FAILREASON_ITEM)
		self:RemoveSendMail(nMailIndex)
		return
	end
	
	-- 发送邮件(玩家邮件不发送物品)
	local bRet = pPlayer.pMail.SendMail2GC(
		tbMailInfo.nIconId, tbMailInfo.nType,
		tbMailInfo.tbReceiverId, tbMailInfo.szCaption, tbMailInfo.szContent
	)
	if (bRet == 1) then
		-- 扣钱
		local nCostMoney = 0 - nReceiverCount * self.MAIL_COST
		pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, nCostMoney)
		
		-- 刷新发送次数
		pPlayer.pMail.AddSendTime()
		-- 通知客户端发送成功
		pPlayer.pMail.SendClientResult(1)
	else
		-- 通知客户端发送失败
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str23[Lang.Idx])
		pPlayer.pMail.SendClientResult(0, self.FAILREASON_UNKNOWN)
	end
	
	self:RemoveSendMail(nMailIndex)
end

-----------------------------------------
--  系统发送邮件
--	tbMailInfo格式：
--	tbMailInfo.nIconId		邮件图标Id
--	tbMailInfo.nType		邮件类型
--	tbMailInfo.tbReceiver	收件人列表（为空表示发送所有玩家）
-- 	tbMailInfo.szCaption	邮件标题
--  tbMailInfo.szContent	邮件内容
--  tbMailInfo.tbItem		邮件附件（{{nClassId,nCount,nStateFlags,nTimeoutType,nTimeout,nPrice,szGuid},...}）
--  tbMailInfo.tbCondition	玩家接收条件
------------------------------------------
function Mail:SendMailBySystem(tbMailInfo)
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str24[Lang.Idx])
		return
	end
	
	-- 标识发送人类型
	tbMailInfo.nSendType = Mail.SEND_TYPE_SYSTEM
	local nIndex = self:AddSendMail(tbMailInfo)
	
	if (tbMailInfo.tbReceiver) then
		-- 给指定玩家发系统消息
		-- 检测收件人是否合法(实际发送逻辑在检测回馈中做)
		local nCount = #tbMailInfo.tbReceiver
		tbMailInfo.tbReceiver.nCount = nCount
		
		local bRet = KMail.CheckReceiver(nIndex, tbMailInfo.nType, tbMailInfo.tbReceiver)
		if (bRet == 0) then
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str25[Lang.Idx])
			CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nIndex, Lang.mail.str26[Lang.Idx])
		end
	else
		-- 给全区玩家发送系统邮件
		local tbReceiver = {}
		tbReceiver.nCount = 0
		
		if (not tbMailInfo.tbItem) then
			tbMailInfo.tbItem = {}
			tbMailInfo.tbItem.nItemCount = 0
		else
			local nItemCount = #tbMailInfo.tbItem
			tbMailInfo.tbItem.nItemCount = nItemCount
		end
		
		local bRet = 0
		if (not tbMailInfo.tbCondition) then
			bRet = KMail.SendMail(tbMailInfo.nIconId, tbMailInfo.nType, tbReceiver, tbMailInfo.szCaption, 
								  tbMailInfo.szContent, tbMailInfo.tbItem)
		else
			bRet = KMail.SendMail(tbMailInfo.nIconId, tbMailInfo.nType, tbReceiver, tbMailInfo.szCaption, 
							      tbMailInfo.szContent, tbMailInfo.tbItem, tbMailInfo.tbCondition)
		end
		
		-- 抛发送结果事件
		if (bRet == 1) then
			CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 1, nIndex)
		else
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str27[Lang.Idx])
			CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nIndex, Lang.mail.str28[Lang.Idx])
		end
	end
	
	return nIndex
end

-- 实际系统发送邮件逻辑
function Mail:_SendMailBySystem(nMailIndex, nResult, tbParam)
	local tbMailInfo = self.tbSendMail[nMailIndex]
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str29[Lang.Idx])
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nMailIndex, Lang.mail.str30[Lang.Idx])
		self:RemoveSendMail(nMailIndex)
		return
	end
	
	if (nResult == 0) then
		-- 收件人不合法
		local szFailReason = tbParam
		if (not szFailReason) then
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str31[Lang.Idx])
			CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nMailIndex, Lang.mail.str31[Lang.Idx])
		else
			Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", szFailReason)
			CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nMailIndex, szFailReason)
		end

		self:RemoveSendMail(nMailIndex)
		return
	end
	
	if (not tbParam) then
		-- 没有返回Id
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str32[Lang.Idx])
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nMailIndex, Lang.mail.str33[Lang.Idx])
	else
		tbMailInfo.tbReceiverId = tbParam
		tbMailInfo.tbReceiverId.nCount = Lib:CountTB(tbMailInfo.tbReceiverId)
	end
	
	-- 发送邮件
	if (not tbMailInfo.tbItem) then
		tbMailInfo.tbItem = {}
		tbMailInfo.tbItem.nItemCount = 0
	else
		local nItemCount = #tbMailInfo.tbItem
		tbMailInfo.tbItem.nItemCount = nItemCount
	end
	
	local bRet = 0
	if (not tbMailInfo.tbCondition) then
		bRet = KMail.SendMail(tbMailInfo.nIconId, Mail.TYPE_SYSTEM, tbMailInfo.tbReceiverId, tbMailInfo.szCaption, 
							  tbMailInfo.szContent, tbMailInfo.tbItem)
	else
		bRet = KMail.SendMail(tbMailInfo.nIconId, Mail.TYPE_SYSTEM, tbMailInfo.tbReceiverId, tbMailInfo.szCaption, 
							  tbMailInfo.szContent, tbMailInfo.tbItem, tbMailInfo.tbCondition)
	end
	
	-- 抛发送结果事件
	if (bRet == 1) then
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 1, nMailIndex)
	else
		Dbg:WriteLogEx(Dbg.LOG_INFO, "Mail", Lang.mail.str34[Lang.Idx])
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, 0, nMailIndex, Lang.mail.str28[Lang.Idx])
	end
	self:RemoveSendMail(nMailIndex)
end

-------------------------------------------------- GC反馈响应 ----------------------------------
-- 收件人检测结果
function Mail:OnCheckReceiver(nMailIndex, nResult, tbParam)
 	local tbMailInfo = self.tbSendMail[nMailIndex]
	if (not tbMailInfo or not tbMailInfo.nType) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str17[Lang.Idx])
		return
	end
	
	if (tbMailInfo.nSendType == Mail.SEND_TYPE_PLAYER) then
		self:_SendMailByPlayer(nMailIndex, nResult, tbParam)
	elseif (tbMailInfo.nSendType == Mail.SEND_TYPE_SYSTEM) then
		self:_SendMailBySystem(nMailIndex, nResult, tbParam)
	else
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", Lang.mail.str35[Lang.Idx])
		return
	end
end
