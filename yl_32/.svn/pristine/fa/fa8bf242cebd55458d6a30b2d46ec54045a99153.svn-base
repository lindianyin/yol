-- 文件名　：relation_logic.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：gameserver和gamecenter共用的人际关系逻辑

--if not (MODULE_GAMECLIENT) then
--	return;
--end

Require("\\script\\gcscript\\relation\\relation_def.lua");

-- 检查指定的玩家关系类型是否在合适的范围内
function Relation:CheckRelationType(nType)
	if (nType < Relation.emKPLAYERRELATION_TYPE_TMPFRIEND or
		nType > Relation.emKPLAYERRELATION_TYPE_BUDDY) then
			return 0;
	end
	return 1;
end

--============================================

function Relation:Register(nType, tbSubRelation)
	if (not nType or not tbSubRelation) then
		return;
	end
	
	Relation.tbClass = Relation.tbClass or {};
	Relation.tbClass[nType] = tbSubRelation;
	Relation.tbClass[nType] = Relation.tbClass[nType] or tbSubRelation;
end

--============================================

-- 检查建立人际关系的依赖和冲突关系
function Relation:CheckCreateDepRep(nType, nRole, nAppId, nDstId)
	return self.tbRelayRuleMgr:CheckCreateDepRep(nType, nRole, nAppId, nDstId);
end

-- 判断是否可以建立指定人际关系
function Relation:CanCreateRelation(nType, nRole, nAppId, nDstId)
	if (0 == nRole) then
		nAppId, nDstId = nDstId, nAppId;
	end
	local bReachMaxCount_App = self:__CheckReachMaxCount(nType, nAppId, 1);
	local bReachMaxCount_Dst = self:__CheckReachMaxCount(nType, nDstId, 0);
	if (not bReachMaxCount_App or not bReachMaxCount_Dst or
		bReachMaxCount_App == 0 or bReachMaxCount_Dst == 0) then
		self:SetInfoMsg(Lang.gcscript.str70[Lang.Idx]);
		return 0;
	end
	
	local bHasRelation = self:__CheckHasRelation(nType, nRole, nAppId, nDstId);
	if (not bHasRelation or bHasRelation == 1) then
		self:SetInfoMsg(Lang.gcscript.str71[Lang.Idx]);
		return 0;
	end
	
	if (not Relation.tbClass[nType]) then
		return 1;
	end
	if (Relation.tbClass[nType].CanCreateRelation) then
		return Relation.tbClass[nType]:CanCreateRelation(nRole, nAppId, nDstId) or 0;
	end
	
	return 1;
end

-- 添加人际关系的时候检查是否已经 有了改关系，有的话返回1，否则为0
function Relation:__CheckHasRelation(nType, nRole, nAppId, nDstId)
	if (not nType or not nRole or nAppId or nDstId) then
		return 0;
	end
	if (0 == nRole) then
		nAppId, nDstId = nDstId, nAppId;
	end
	return KRelation.HasRelation(nAppId, nDstId, nType, nRole);
end

-- 创建人际关系的时候，判断特定类型人际关系的人数是否达到上限
function Relation:__CheckReachMaxCount(nType, nPlayerId, nRole)
	if (not nType or not nPlayerId or not nRole or nType < 0 or nPlayerId <= 0) then
		return 0;
	end
	
	local nMaxCount = self.tbRelayRuleMgr:GetMaxCount(nType);
	if (not nMaxCount) then
		return 1;
	end
	
	local nCurCount = 0;
	if (Player.emKPLAYERRELATION_TYPE_TMPFRIEND == nType or
		Player.emKPLAYERRELATION_TYPE_BIDFRIEND == nType) then
		nCurCount = KRelation.GetOneRelationCount(nPlayerId, Player.emKPLAYERRELATION_TYPE_TMPFRIEND, 1) + 
					KRelation.GetOneRelationCount(nPlayerId, Player.emKPLAYERRELATION_TYPE_BIDFRIEND, 1);
	else
		nCurCount = KRelation.GetOneRelationCount(nPlayerId, nType, nRole);
	end
	
	if (nCurCount >= nMaxCount) then
		return 0;
	end
	
	return 1;
end

-- 检查删除人际关系的依赖和冲突关系
function Relation:CheckDelDepRep(nType, nRole, nAppId, nDstId)
	return self.tbRelayRuleMgr:CheckDelDepRep(nType, nRole, nAppId, nDstId);
end

-- 判断是否可以删除人际关系
function Relation:CanDelRelation(nType, nRole, nAppId, nDstId)
end

--============================================

-- 判断亲密度上限
function Relation:CheckFavorLimit(nPlayerId)
	
end

--============================================

-- 记录人际相关log
function Relation:WriteRelationLog(nOpt, nType, nAppId, nDstId)
	if (not nOpt or not nType or not nAppId or not nDstId or nAppId <= 0 or nDstId <= 0) then
		return;
	end
	
	-- todo
	
	-- 只在AppPlayer 所在服务器记录log
	-- local pAppPlayer = KGameBase.GetPlayerById(nAppId);
	local pAppPlayer = nil;
	if (not pAppPlayer) then
		return;
	end
	
	local szAppName = KGCPlayer.GetPlayerName(nAppId);
	local szDstName = KGCPlayer.GetPlayerName(nDstId);
	if (not szAppName or not szDstName) then
		return;
	end
	
	local szRelationType = self.tbRelationName[nType];
	if (not szRelationType) then
		return;
	end
	
	local szOpt = "";
	if (nOpt == self.LOG_TYPE_ADDRELATION) then
		szOpt = "AddRelation";
	elseif (nOpt == self.LOG_TYPE_DELRELATION) then
		szOpt = "DelRelation"
	end
	
	Dbg:WriteLog("Relation", szOpt, szRelationType, szAppName, szDstName);
end

--============================================

-- 关于人际操作相关错误信息的一些操作
function Relation:ClearInfoMsg()
	self.szInfoMsg = "";
end

function Relation:SetInfoMsg(szInfoMsg)
	if (not szInfoMsg) then
		return;
	end
	self.szInfoMsg = szInfoMsg;
end

function Relation:GetInfoMsg()
	return self.szInfoMsg;
end

-- 检查是否有错误信息
function Relation:CheckInfoMsg()
	local szInfoMsg = self:GetInfoMsg();
	if (not szInfoMsg or "" == szInfoMsg) then
		return 0;
	end
	return 1;
end

--============================================
