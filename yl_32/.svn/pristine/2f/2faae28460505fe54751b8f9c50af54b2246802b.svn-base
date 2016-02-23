-- 文件名　：relation_gc.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：gamecenter的人际关系逻辑

if not (MODULE_GAMECENTER) then
	return
end

-- 判断是否可以添加关系
function Relation:CheckCanAddRelation(nAppId, nDstId, nType, nRole)
	-- 对方不在线
	--[[
	local nDstOnline = KGCPlayer.OptGetTask(nDstId, KGCPlayer.TSK_ONLINESERVER);
	if (nDstOnline <= 0) then
		self:SetInfoMsg("对方玩家不在线。");
		return self.emKEPLAYER_NOTONLINE;
	end ]]
	
	-- 数据没有加载完
	local nAppIsLoadEnd = CheckIsLoadEnd(nAppId);
	local nDstIsLoadEnd = CheckIsLoadEnd(nDstId);
	if (nAppIsLoadEnd == 0 or nDstIsLoadEnd == 0) then
		--self:SetInfoMsg("玩家数据没有加载完成。");
		self:NotifyPlayerOffline_GC(nAppId, nDstId);
		return self.emKEADDRELATION_FAIL;
	end
	
	-- 检查冲突和依赖关系
	local nCheckDepRep = CheckCreateDepRep(nType, nRole, nAppId, nDstId);
	if (0 == nCheckDepRep) then
		return self.emKEADDRELATION_FAIL;
	end	
	
	-- 是否可以添加关系
	local nCanAddRelation = CanCreateRelation(nType, nRole, nAppId, nDstId);
	if (nCanAddRelation == 0) then
		return self.emKEADDRELATION_FAIL;
	end
	
	return self.emKEADDRELATION_SUCCESS;
end

function Relation:SetLastTalkTime_GC(nAppId, szDstName, dwTime)
	if (not nAppId or not szDstName or nAppId <= 0) then
		return;
	end

	local nDstId = GetPlayerIdByName(szDstName);
	if (not nDstId or nDstId == 0 or nDstId == nAppId) then
		return;
	end
	
	SetFriendTalkTime(nAppId, nDstId, dwTime);
end

-- 通知对方不在线或者不存在
function Relation:NotifyPlayerOffline_GC(nAppId, nDstId)
	if (not nAppId or nAppId <= 0) then
		return;
	end
	if (not nDstId or nDstId <= 0) then
		return;
	end
	
	local szDstName = GetRoleName(nDstId);
	if szDstName then
		GlobalExecute{"Relation:NotifyPlayerOffline_GS", nAppId, szDstName};
	end
end

-- 通知已被列入黑名单
function Relation:NotifyAddedBlack_GC(nAppId, szDstName)
	if (not nAppId or not szDstName or nAppId <= 0) then
		return;
	end
	GlobalExecute{"Relation:NotifyAddedBlack_GS", nAppId, szDstName};
end

-- 通知已达上限值
function Relation:NotifyReachMaxCount_GC(nAppId, nType)
	if (not nAppId or nAppId <= 0) then
		return;
	end
	GlobalExecute{"Relation:NotifyReachMaxCount_GS", nAppId, nType};
end

-- 同步关系列表
function Relation:SyncRelation_GC(nAppId)
	if (not nAppId or nAppId <= 0) then
		return;
	end
	SyncRelation(nAppId);
end
-- 添加关系
function Relation:AddRelation_GC(nAppId, szDstName, nType, nRole)
	if (not nAppId or not szDstName or nAppId <= 0) then
		return;
	end

	self:ClearInfoMsg();
	
	local nDstId = GetPlayerIdByName(szDstName);
	if (not nDstId or nDstId == 0) then
		return;
	elseif nDstId == nAppId then
		self:SetInfoMsg(Lang.gcscript.str49[Lang.Idx]);
		return;
	end
	
	local nResult = self:CheckCanAddRelation(nAppId, nDstId, nType, nRole);
	if (nResult == self.emKEADDRELATION_SUCCESS) then
		nResult = CreateRelation(nType, nAppId, nDstId, nRole);
		if (nResult == 1) then
			Relation:ProcessAfterAddRelation_GC(nType, nRole, nAppId, nDstId);
			
			-- 添加人际成功后的提示信息
			local szType = self.tbRelationName[nType] or "";

			-- local szMsg = "您已经成功将 "..szDstName .. "添加到"..szType.."列表中。"; print(szMsg);
			-- self:SetInfoMsg(szMsg);
		end
	end
	
	self:TellPlayerMsg_GC(nAppId);
end

function Relation:AfterAddRelation_GC(nAppId, nDstId, nType)
	if (not nAppId or not nDstId or not nType or
		nAppId <= 0 or nDstId <= 0) then
		return;
	end
	
	GlobalExecute{"Relation:AfterAddRelation_GS", nAppId, nDstId, nType};
end

-- 判断是否可以删除关系
function Relation:CheckCanDelRelation(nAppId, nDstId, nType, nRole)
	-- 数据是否加载完成
	local nAppIsLoadEnd = CheckIsLoadEnd(nAppId);
	if (nAppIsLoadEnd == 0) then
		return 0;
	end
	
	-- 检查冲突和依赖关系
	local nCheckDepRep = CheckDelDepRep(nType, nRole, nAppId, nDstId);
	if (0 == nCheckDepRep) then
		return 0;
	end
	
	return 1;
end

-- 删除关系
function Relation:DelRelation_GC(nAppId, szDstName, nType, nRole)
	local nDstPlayerId = GetPlayerIdByName(szDstName);
	if (nDstPlayerId == 0) then
		return;
	end
	self:ClearInfoMsg();
	
	local nCanDelRelation = self:CheckCanDelRelation(nAppId, nDstPlayerId, nType, nRole);
	if (1 == nCanDelRelation) then
		if (1 == DelRelation(nType, nAppId, nDstPlayerId, nRole)) then
			Relation:ProcessAfterDelRelation_GC(nType, nRole, nAppId, nDstPlayerId);
		end
	end
	
	self:TellPlayerMsg_GC(nAppId);
end

-- 检查是否可以增加亲密度
function Relation:CheckCanAddFavor(nAppId, nDstId, nFavor, nMethod)
	-- 某一方不在线
	local nDstOnline = KGCPlayer.OptGetTask(nDstId, KGCPlayer.TSK_ONLINESERVER);
	local nAppOnline = KGCPlayer.OptGetTask(nAppId, KGCPlayer.TSK_ONLINESERVER);
	if (nDstOnline <= 0 or nAppOnline <= 0) then
		return 0;
	end
	
	-- 数据没有加载完
	local nAppIsLoadEnd = KRelation.CheckIsLoadEnd(nAppId);
	local nDstIsLoadEnd = KRelation.CheckIsLoadEnd(nDstId);
	if (nAppIsLoadEnd == 0 or nDstIsLoadEnd == 0) then
		return 0;
	end
	
	return 1;
end

-- 增加亲密度
function Relation:AddFriendFavor_GC(szAppName, szDstName, nFavor, nMethod)
	if (not szAppName or not szDstName or szAppName == szDstName or nFavor <= 0) then
		return;
	end
	
	self:ClearInfoMsg();
	
	local nAppId = KGCPlayer.GetPlayerIdByName(szAppName);
	local nDstId = KGCPlayer.GetPlayerIdByName(szDstName);
	
	local nCanAddFavor = self:CheckCanAddFavor(nAppId, nDstId, nFavor, nMethod);
	if (1 == nCanAddFavor) then
		KRelation.ResetLimtWhenCrossDay(nAppId, nDstId);
		KRelation.AddFriendFavor(nAppId, nDstId, nFavor, nMethod);
		KRelation.SyncFriendInfo(nAppId, nDstId);
	end
	
	self:TellPlayerMsg_GC(nAppId);
end

-- 给玩家返还提示信息
function Relation:TellPlayerMsg_GC(nPlayerId)
	-- todo
	do
		return;
	end
	if (not nPlayerId or nPlayerId <= 0) then
		return;
	end
	
	-- 没有信息的话，返回
	if (self:CheckInfoMsg() == 0) then
		return;
	end
	local szMsg = self:GetInfoMsg();
	local nOnline = KGCPlayer.OptGetTask(nPlayerId, KGCPlayer.TSK_ONLINESERVER);
	if (nOnline > 0) then
		GSExcute(nOnline, {"Relation:TellPlayerMsg_GS", nPlayerId, szMsg});
	end
end

-- 在玩家上线的时候获取密友关系即将一年到期的信息，并且给出玩家提示
function Relation:GetCloseFriendTimeInfo_GC(nPlayerId)
	if (nPlayerId <= 0) then
		return;
	end
	
	local tbTimeInfo = KRelation.GetCloseFriendTimeInfo(nPlayerId);
	if (not tbTimeInfo or Lib:CountTB(tbTimeInfo) == 0) then
		return;
	end
	
	local tbInfo = {};
	for i, v in pairs(tbTimeInfo) do
		-- 如果剩余时间是0，就表示该关系需要删除
		if (v.nTime == 0) then
			KRelation.DelOverTimeRelation(v.nType, nPlayerId, v.nPlayerId, v.bAsMaster);
		end
		
		-- 把剩余时间的信息通知玩家
		local szPlayerName = KGCPlayer.GetPlayerName(v.nPlayerId);
		if (szPlayerName) then
			local tbTemp = {};
			tbTemp.szPlayerName = szPlayerName;
			tbTemp.nTime = v.nTime;
			tbTemp.nType = v.nType;
			table.insert(tbInfo, tbTemp);
		end
	end
	
	GlobalExecute{"Relation:GetCloseFriendTimeInfo_GS2", nPlayerId, tbInfo};
end

--===================================================

-- 亲密度等级提升之后的回调
function Relation:OnFavorLevelUp(nPlayerAppId, nPlayerDstId, nFavorLevel)
	if (not nPlayerAppId or not nPlayerDstId or not nFavorLevel or
		nPlayerAppId <= 0 or nPlayerDstId <= 0 or nFavorLevel <= 0) then
		return;
	end
	GlobalExecute{"Relation:OnFavorLevelUp_GS", nPlayerAppId, nPlayerDstId, nFavorLevel};
end

-- 增加人际关系之后的通用回调
function Relation:ProcessAfterAddRelation_GC(nType, nRole, nAppId, nDstId)
	if (not nType) then
		return;
	end
	
	GlobalExecute{"Relation:ProcessAfterAddRelation_GS", nType, nRole, nAppId, nDstId};
	
	if (not self.tbClass[nType]) then
		return;
	end
	if (not self.tbClass[nType]["ProcessAfterAddRelation_GC"]) then
		return;
	end
	self.tbClass[nType]:ProcessAfterAddRelation_GC(nRole, nAppId, nDstId);
end

-- 删除人际关系之后的通用回调
function Relation:ProcessAfterDelRelation_GC(nType, nRole, nAppId, nDstId)
	if (not nType) then
		return;
	end
	
	GlobalExecute{"Relation:ProcessAfterDelRelation_GS", nType, nRole, nAppId, nDstId};
	
	if (not self.tbClass[nType]) then
		return;
	end
	if (not self.tbClass[nType]["ProcessAfterDelRelation_GC"]) then
		return;
	end
	self.tbClass[nType]:ProcessAfterDelRelation_GC(nRole, nAppId, nDstId);
end

--=============================================

-- 通用的人际gc调用gs的一个接口
-- 主要是给class下面的各种具体的人际关系调用
function Relation:CallServerScript_Relation(nType, szFunName, tbParam)
	if (not nType or not szFunName or type(szFunName) ~= "string") then
		return;
	end
	
	GlobalExecute{"Relation:ServerScript_Relation", nType, szFunName, tbParam};
end

-- 人际部分，通用的从gs调用的gc函数
function Relation:GCScript_Relation(nType, szFunName, tbParam)
	if (not nType or not szFunName or type(szFunName) ~= "string") then
		return;
	end
	
	if (not self.tbClass[nType]) then
		return;
	end
	if (not self.tbClass[nType][szFunName]) then
		return;
	end
	
	self.tbClass[nType][szFunName](self.tbClass[nType], tbParam);
end
