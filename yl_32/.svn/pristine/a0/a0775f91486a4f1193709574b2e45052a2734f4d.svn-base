--
-- FileName: mail_c.lua
-- Author: 	 xiewenzhe
-- Time: 	 2011/5/25 9:40
-- Comment:	 邮件(Client)
--

if not MODULE_GAMECLIENT then
	return
end

-------------------------------------------------- 功能API -------------------------------------
-----------------------------------------
--  发送玩家邮件
--	tbMailInfo格式：
--	tbMailInfo.tbReceiver	收件人列表
-- 	tbMailInfo.szCaption	邮件标题
--  tbMailInfo.szContent	邮件内容
--  tbMailInfo.tbItem		邮件附件（玩家邮件不能发送附件）
------------------------------------------
function Mail:SendMail(tbMailInfo)
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "发送邮件信息为空!")
		return
	end
	
	-- 检测收件人
	local bCheckReceiver = 1
	local nReceiverCount = #tbMailInfo.tbReceiver
	if (not tbMailInfo.tbReceiver or nReceiverCount == 0) then
		bCheckReceiver = 0
	else
		for _, szReceiver in pairs(tbMailInfo.tbReceiver) do
			if (not szReceiver or type(szReceiver) ~= "string") then
				bCheckReceiver = 0
				break
			end
		end
	end
	if (bCheckReceiver == 0) then
		return self:OnSendResult(0, self.FAILREASON_RECEIVER)
	end
	tbMailInfo.tbReceiver.nCount = nReceiverCount
	
	-- 检测银两是否够
	local nMoney = me.pPlayerPurse.GetMoney(Purse.EM_MONEY_SILVER)
	if (nMoney < nReceiverCount * self.MAIL_COST) then
		return self:OnSendResult(0, self.FAILREASON_MONEY)
	end
	
	-- 发送
	me.pMail.SendMail(self.ICONID_PLAYER, Mail.TYPE_PLAYER, tbMailInfo.tbReceiver, tbMailInfo.szCaption, tbMailInfo.szContent)
end

-----------------------------------------
--  发送急件
--	tbMailInfo格式：
--	tbMailInfo.tbReceiver	收件人列表
-- 	tbMailInfo.szCaption	邮件标题
--  tbMailInfo.szContent	邮件内容
--  tbMailInfo.tbItem		邮件附件（急件不能发送附件）
------------------------------------------
function Mail:DeliveryMail(tbMailInfo)
	if (not tbMailInfo) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "急件信息为空!")
		return
	end
	
	-- 检测收件人
	local bCheckReceiver = 1
	local nReceiverCount = #tbMailInfo.tbReceiver
	if (not tbMailInfo.tbReceiver or nReceiverCount == 0) then
		bCheckReceiver = 0
	else
		for _, szReceiver in pairs(tbMailInfo.tbReceiver) do
			if (not szReceiver or type(szReceiver) ~= "string") then
				bCheckReceiver = 0
				break
			end
		end
	end
	if (bCheckReceiver == 0) then
		return self:OnSendResult(0, self.FAILREASON_RECEIVER)
	end
	tbMailInfo.tbReceiver.nCount = nReceiverCount
	
	-- 发送
	me.pMail.SendMail(self.ICONID_PLAYER, Mail.TYPE_DELIVERY, tbMailInfo.tbReceiver, tbMailInfo.szCaption, tbMailInfo.szContent)
end

-- 请求邮件列表
function Mail:RequestMailList(nType)
	me.pMail.RequestMailList(nType)
end

-- 请求邮件具体内容
function Mail:RequestMail(nMailId)
	if (not nMailId) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "请求邮件Id为空!")
		return
	end
	
	me.pMail.RequestMail(nMailId)
end

-- 请求获取附件
function Mail:FetchItem(nMailId, nItemIndex)
	if (not nMailId or
		not  nItemIndex or
		type(nItemIndex) ~= "number") then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "请求邮件附件参数不正确!")
		return
	end
	
	me.pMail.FetchItem(nMailId, nItemIndex)
end

-- 请求删除邮件
function Mail:DeleteMail(nMailId)
	if (not nMailId) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "请求删除邮件Id为空!")
		return
	end
	
	me.pMail.DeleteMail(nMailId)
	if (self.tbDelivery and self.tbDelivery[nMailId]) then
		self.tbDelivery[nMailId] = nil
	end
end

-------------------------------------------------- GS反馈响应 ----------------------------------
function Mail:Init()
	self.tbMailList = {}
	self.tbMailContent = {}
end

-- 邮箱加载完成
function Mail:OnMailBoxLoadOK(bShowMsg)
	if (bShowMsg and bShowMsg == 1) then
--		me.SysMsg("信件加载完成！")
	end
	
	self:Init()
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_LOADOK)
end

-- 同步邮件列表
function Mail:OnFetchMailList(tbMailList)
	if (not tbMailList) then
		return
	end
	
	-- 按时间先后排序，未读邮件优先排
	self.tbMailList = {}
	for _, tbMailInfo in pairs(tbMailList) do
		if (not self.tbMailList[1]) then
			table.insert(self.tbMailList, tbMailInfo)
		else
			local bFind = false
			for nIndex, tbInsert in ipairs(self.tbMailList) do
				if (tbMailInfo.nState == tbInsert.nState) then
					if (tbMailInfo.nTime > tbInsert.nTime) then
						table.insert(self.tbMailList, nIndex, tbMailInfo)
						bFind = true
						break
					end
				else
					if (tbMailInfo.nState == 0) then
						table.insert(self.tbMailList, nIndex, tbMailInfo)
						bFind = true
						break
					end
				end
			end
			
			if (not bFind) then
				table.insert(self.tbMailList, tbMailInfo)
			end
		end
	end
	
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SYNCLIST)
	
	-- 测试脚本
	--me.SysMsg("同步邮件列表")
	--print("~~~~~~~~~~~~~~~~~~~~ 邮件列表：")
	--for i, tbInfo in ipairs(self.tbMailList) do
		--print(string.format("[%d] mail", i))
		--print("nMailId: " .. tbInfo.nMailId)
		--print("nState: " .. tbInfo.nState)
		--print("nType: " .. tbInfo.nType)
		--print("nIconId: " .. tbInfo.nIconId)
		--print("szCatpion: " .. tbInfo.szCaption)
		--print("szReceiver: " .. tbInfo.szReceiver)
		--print("szSender: " .. tbInfo.szSender)
		--print("nTime: " .. tbInfo.nTime)
	--end
end

function Mail:GetMailList()
	return self.tbMailList
end

-- 更新缓存的邮件状态
function Mail:ChangeState(nMailId, nState)
	local tbMailInfo = nil
	local bFind = 0
	for _, tbOne in pairs(self.tbMailList) do
		if (tbOne.nMailId == nMailId) then
			tbMailInfo = tbOne
			bFind = 1
			break
		end
	end
	if (bFind == 0) then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "Mail", "指定邮件不存在当前邮件列表中，无法改变状态!")
		return
	end
	
	tbMailInfo.nState = nState
end

function Mail:GetDeliveryMailById(nMailId)
	return self.tbDelivery[nMailId]
end

-- 同步邮件内容
function Mail:OnFetchMail(tbMailContent)
	if (not tbMailContent) then
		return
	end
	
	if (not self.tbDelivery) then
		self.tbDelivery = {}
	end
	
	if (tbMailContent.nType == Mail.TYPE_DELIVERY) then
		self.tbDelivery[tbMailContent.nMailId] = tbMailContent
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_NEWDELIVERY, tbMailContent.nMailId)
	else
		self.tbMailContent = tbMailContent
		CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SYNCCONTENT, tbMailContent.nMailId)
	end
	
	-- 测试脚本
	--me.SysMsg("同步邮件内容")
	--print("~~~~~~~~~~~~~~~~~~~~ 邮件内容：")
	--print("nMailId: " .. tbMailContent.nMailId)
	--print("nIconId: " .. tbMailContent.nIconId)
	--print("szContent: " .. tbMailContent.szContent)
	--if (tbMailContent.tbItem) then
		--print("tbItem: ")
		--for i, tbInfo in ipairs(tbMailContent.tbItem) do
			--print(string.format("[%d] item", i))
			--print("nItemIndex: " .. tbInfo.nItemIndex)
			--print("nClassId: " .. tbInfo.nClassId)
			--print("nCount: " .. tbInfo.nCount)
			--print("nPrice: " .. tbInfo.nPrice)
			--print("nStateFlags: " .. tbInfo.nStateFlags)
			--print("nTimeoutType: " .. tbInfo.nTimeoutType)
			--print("nTimeout: " .. tbInfo.nTimeout)
		--end
	--end
end

function Mail:GetMailContent()
	return self.tbMailContent
end

--急件发送结果
function Mail:OnSendDeliveryResult(nResult)
	Shop:OnSendClaimResult(nResult or 0)
end

-- 邮件发送结果
function Mail:OnSendResult(nResult, nReasonType, szFailReason)
	local szMsg = ""
	if (nResult == 1) then
		-- 发送成功
		szMsg = Lang.mail.str1[Lang.Idx]
		me.SysMsg(szMsg)
	else
		-- 发送失败
		szMsg = Lang.mail.str2[Lang.Idx]
		if (nReasonType == self.FAILREASON_BOXFULL) then
			szMsg = szMsg .. Lang.mail.str3[Lang.Idx]
		elseif (nReasonType == self.FAILREASON_RECEIVER) then
			if (not szFailReason or type( szFailReason) ~= "string") then
				szMsg = szMsg .. Lang.mail.str4[Lang.Idx]
			else
				szMsg = szMsg .. szFailReason
			end
		elseif (nReasonType == self.FAILREASON_MONEY) then
			szMsg = szMsg .. Lang.mail.str5[Lang.Idx]
		elseif (nReasonType == self.FAILREASON_ITEM) then
			szMsg = szMsg .. Lang.mail.str6[Lang.Idx]
		elseif (nReasonType == self.FAILREASON_FORBID) then
			if (not  szFailReason or type( szFailReason) ~= "string") then
				szMsg = szMsg .. Lang.mail.str7[Lang.Idx]
			else
				szMsg = szMsg .. szFailReason
			end
		else
			szMsg = szMsg .. Lang.mail.str8[Lang.Idx]
		end
		
		me.SysMsg(szMsg)
	end
	
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_SENDRESULT, nResult, szMsg)
end

-- 新邮件通知
function Mail:OnNewMail()
	me.SysMsg(Lang.mail.str9[Lang.Idx])
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_NEW)
	
	-- 有新邮件时，请求一次邮件列表
	self:RequestMailList(self.MAILLIST_SYSTEM)
end

-- 获取附件成功
function Mail:OnFetchItemOK(nMailId, nItemIndex)
	CallCenter:OnEvent(KOBJEVENT.emKOBJEVENTTYPE_MAIL_FETCHITEMOK, nMailId, nItemIndex)
end
