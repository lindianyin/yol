-----------------------------------------------------
--文件名		：	relation_gs.lua
--创建者		：
--创建时间		：
--功能描述		：	人际关系服务器脚本
------------------------------------------------------
if not (MODULE_GAMESERVER) then
	return
end

-- Relation.tbc2sFun = {}

function Relation:AddAwared(pPlayerApp, szDstName, tbData)

	if (not tbData or not szDstName or not pPlayerApp) then
		return 0;
	end
	local nBindCoin = 0;
	local szMsg = "";
	if (tbData.nAwardApp == 1) then
		szMsg = self:GetShowMsg(szDstName, tbData.nLevel, tbData.nAwardCoin, tbData.nAppGiveCount, tbData.nAppLeftCount, tbData.nAppHaveCount);
		pPlayerApp.Msg(szMsg);
		nBindCoin = (tbData.nAppGiveCount + 1)* tbData.nAwardCoin;
		pPlayerApp.AddBindCoin(nBindCoin, Player.emKBINDCOIN_ADD_RELATION);

		-- 加好友，亲密度等级提升需要记录的相应任务变量
		-- 因为目前还没有统一的，人际操作完成之后的回调，先暂时写在这里，很挫，等人际关系重构的时候，加上这种回调，可以把这段代码放到那里去

		-- 亲密度等级1，表示这个好友时刚加的
		if (tbData.nLevel == 1) then
			pPlayerApp.SetTask(Player.TSKGROUP_NEWPLAYER_GUIDE, Player.TSKID_NEWPLAYER_FRIEND, 1);
		end

		-- 亲密度等级到2
		if (tbData.nLevel == 2) then
			pPlayerApp.SetTask(Player.TSKGROUP_NEWPLAYER_GUIDE, Player.TSKID_NEWPLAYER_FRIEND, 2);
		end
	end

	return 1;
end

function Relation:GetShowMsg(szDstName, nLevel, nAward, nGiveCount, nLeftCount, nHaveCount)
	local szMsg = string.format(Lang.gcscript.str50[Lang.Idx], szDstName, nLevel, nAward, IVER_g_szCoinName);

	local szMsgInfo = string.format(Lang.gcscript.str51[Lang.Idx], nGiveCount, nLeftCount);
	local szMsgOtherInfo = string.format(Lang.gcscript.str52[Lang.Idx], nLevel, nHaveCount, nLevel, nLeftCount);
	local szLastMsg = "";
	if (nGiveCount == 0) then
		szLastMsg = szMsg ..szMsgOtherInfo;
	else
		szLastMsg = szMsg .. szMsgInfo;
	end
	return szLastMsg;
end



function Relation:CanDoRelationOpt(szAppName)
	do
		return 1;
	end
	local szErrMsg = "";
	if (not szAppName) then
		return 0, szErrMsg;
	end
	local pAppPlayer = KPlayer.GetPlayerByName(szAppName);
	if (pAppPlayer.IsAccountLock() ~= 0) then
		szErrMsg = Lang.gcscript.str53[Lang.Idx];
		return 0, szErrMsg;
	end
	return 1;
end

function Relation:SetLastTalkTime_GS(nAppId, szDstName, dwTime)
	if (not nAppId or not szDstName or not nType) then
		return;
	end

	KGameBase.CenterExecute{"Relation:SetLastTalkTime_GC", nAppId, szDstName, dwTime};
end

function Relation:c2s_SyncRelation(pPlayer)
	assert(pPlayer);
	local nAppId = pPlayer.dwId;
	KGameBase.CenterExecute{"Relation:SyncRelation_GC", nAppId};
end

-- 添加人际关系
-- 参数分别为申请人姓名，对方姓名，关系类型，是否是主位（0表示次位，1表示主位，默认为1）
function Relation:AddRelation_GS(nAppId, szDstName, nType, nRole)
	if (not nAppId or not szDstName or not nType) then
		return;
	end

	KGameBase.CenterExecute{"Relation:AddRelation_GC", nAppId, szDstName, nType, nRole};

--[[
	if (not nAppId or not nDstId or not nType or nAppId == nDstId or
		0 == self:CheckRelationType(nType)) then
		return;
	end
	local pAppPlayer = KPlayer.GetPlayerByName(szAppName);
	if (not pAppPlayer) then
		return;
	end
	local nCanOpt, szErrMsg = self:CanDoRelationOpt(szAppName);
	if (0 == nCanOpt) then
		if ("" ~= szErrMsg) then
			pAppPlayer.Msg(szErrMsg);
		end
		return;
	end
	nRole = nRole or 1;
	if (0 ~= nRole and 1 ~= nRole) then
		nRole = 1;
	end
	if (KPlayer.CheckRelation(szAppName, szDstName, nType) == 1) then
		pAppPlayer.Msg(string.format("添加失败，您与[%s]已经有这种人际关系了，不必再次添加。", szDstName));
		return;
	end

	KGameBase.CenterExecute{"Relation:AddRelation_GC", szAppName, szDstName, nType, nRole};
	]]
end

function Relation:c2s_AddRelation(pPlayer, szDstName, nType, nRole)
	assert(pPlayer);
	if pPlayer.IsBZonePlayer() == 1 then
		pPlayer.SendBlackMsg(Lang.gcscript.str54[Lang.Idx]);
	end

	if not szDstName then
		pPlayer.SysMsg(Lang.gcscript.str55[Lang.Idx]);
		return;
	end
--[[
	-- 跨服状态加好友，将调用加大区好友
	if (IsGlobalServer() and nType == Player.emKPLAYERRELATION_TYPE_TMPFRIEND) then
		Player.tbGlobalFriends:gs_ApplyAddFriend(me.szName, szDstName);
		return;
	end
	local szAppName = me.szName;
	if (not szAppName or not szDstName or not nType or szAppName == szDstName or
		0 == self:CheckRelationType(nType)) then
		return;
	end
	]]
	nRole = nRole or 1;
	if (0 ~= nRole and 1 ~= nRole) then
		nRole = 1;
	end
	nType = nType or 0;
	if nType < 0 or nType >= self.emKPLAYERRELATION_TYPE_COUNT then
		nType = 0;
	end

	self:AddRelation_GS(pPlayer.dwId, szDstName, nType, nRole);
end
-- Relation.tbc2sFun["AddRelation_C2S"] = Relation.AddRelation_C2S;

-- 删除人际关系
-- 参数分别为申请人姓名，对方姓名，关系类型，是否是主位（0表示次位，1表示主位，默认为1）
function Relation:DelRelation_GS(pPlayer, szDstName, nType, nRole)
	local nCanOpt, szErrMsg = self:CanDoRelationOpt();
	if (0 == nCanOpt) then
		if ("" ~= szErrMsg) then
			pPlayer.SysMsg(szErrMsg);
		end
		return;
	end

	nRole = nRole or 1;
	if (0 ~= nRole and 1 ~= nRole) then
		nRole = 1;
	end

--	if (CheckRelation(pPlayer.szName, szDstName, nType) == 0) then
--		return;
--	end

	KGameBase.CenterExecute{"Relation:DelRelation_GC", pPlayer.dwId, szDstName, nType, nRole};
end

function Relation:c2s_DelRelation(pPlayer, szDstName, nType, nRole)
	if (not pPlayer or not szDstName) then
		return;
	end;

	nRole = nRole or 1;
	if (0 ~= nRole and 1 ~= nRole) then
		nRole = 1;
	end

	-- 某些特殊的关系，不允许通过客户端发送指令来删除
	if (nType == Relation.emKPLAYERRELATION_TYPE_COUPLE or
		nType == Relation.emKPLAYERRELATION_TYPE_TRAINED or
		nType == Relation.emKPLAYERRELATION_TYPE_TRAINING) then
		return;
	end

	self:DelRelation_GS(pPlayer, szDstName, nType, nRole);
end
-- Relation.tbc2sFun["DelRelation_C2S"] = Relation.DelRelation_C2S;

-- 增加亲密度
-- nMethod 增加亲密度途径 0正常途径，1通过ib道具（默认是0）
function Relation:AddFriendFavor(szAppName, szDstName, nFavor, nMethod)
	nMethod = nMethod or 0;
	if (nMethod ~= 0 and nMethod ~= 1) then
		nMethod = 1;
	end
	if (not szAppName or not szDstName or szAppName == szDstName or nFavor <= 0) then
		return;
	end
	local pAppPlayer = KPlayer.GetPlayerByName(szAppName);
	if (not pAppPlayer) then
		return;
	end

	-- 不是好友关系，不添加亲密度
	if (KPlayer.CheckRelation(szAppName, szDstName, Player.emKPLAYERRELATION_TYPE_BIDFRIEND, 1) == 0) then
		return;
	end

	KGameBase.CenterExecute{"Relation:AddFriendFavor_GC", szAppName, szDstName, nFavor, nMethod};
end

function Relation:GetMsgById(nPlayerId, szDstName, nType, nMsgId)
	if (nMsgId < 0) then
		return;
	end

	local szMsg = "";
	if (nMsgId == self.emKEADDRELATION_SUCCESS) then
		if (nType == Player.emKPLAYERRELATION_TYPE_ENEMEY) then
			szMsg = string.format(Lang.gcscript.str56[Lang.Idx], szDstName);
		elseif (nType == Player.emKPLAYERRELATION_TYPE_BLACKLIST) then
			szMsg = string.format(Lang.gcscript.str57[Lang.Idx], szDstName);
		elseif (nType == Player.emKPLAYERRELATION_TYPE_TMPFRIEND) then
			szMsg = string.format(Lang.gcscript.str58[Lang.Idx], szDstName);
		end
	elseif (nMsgId == self.emKEHAVE_RELATION) then
		szMsg = string.format(Lang.gcscript.str59[Lang.Idx], szDstName);
	elseif (nMsgId == self.emKEPLAYER_NOTONLINE) then
		szMsg = Lang.gcscript.str60[Lang.Idx];
	else
		if (nType == Player.emKPLAYERRELATION_TYPE_BINDFRIEND or
			nType == Player.emKPLAYERRELATION_TYPE_SIBLING) then
			szMsg = string.format(Lang.gcscript.str61[Lang.Idx], szDstName);
		else
			szMsg = Lang.gcscript.str62[Lang.Idx];
		end
	end

	return szMsg;
end

function Relation:TellPlayerMsg_GS(nPlayerId, szMsg)
	if (not nPlayerId or not szMsg or nPlayerId <= 0 or "" == szMsg) then
		return;
	end
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer) then
		pPlayer.Msg(szMsg);
	end
end

-- 在玩家上线的时候获取密友关系即将一年到期的信息，并且给出玩家提示
function Relation:GetCloseFriendTimeInfo_GS(bExchangeServerComing)
	if (not bExchangeServerComing or 1 == bExchangeServerComing) then
		return;
	end

	local nPlayerId = me.nId;
	if (nPlayerId <= 0) then
		return;
	end

	local tbCloseFrientList = me.GetRelationList(Player.emKPLAYERRELATION_TYPE_BUDDY);
	local tbTrainedList = me.GetRelationList(Player.emKPLAYERRELATION_TYPE_TRAINED, 1);
	local tbIntroduceList = me.GetRelationList(Player.emKPLAYERRELATION_TYPE_INTRODUCTION, 1);
	if (#tbCloseFrientList == 0 and #tbTrainedList == 0 and #tbIntroduceList == 0) then
		return;
	end

	KGameBase.CenterExecute{"Relation:GetCloseFriendTimeInfo_GC", nPlayerId};
end

-- tbTimeInfo = {{"nPlayerId" = XXX, "nTime" = XXX}, {}, ...}
function Relation:GetCloseFriendTimeInfo_GS2(nPlayerId, tbTimeInfo)
	if (Lib:CountTB(tbTimeInfo) == 0 or nPlayerId <= 0) then
		return;
	end

	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (not pPlayer) then
		return;
	end

	local tbRelationName = {
		[Player.emKPLAYERRELATION_TYPE_BUDDY] = Lang.gcscript.str63[Lang.Idx],
		[Player.emKPLAYERRELATION_TYPE_INTRODUCTION] = Lang.gcscript.str64[Lang.Idx],
		[Player.emKPLAYERRELATION_TYPE_TRAINED] = Lang.gcscript.str65[Lang.Idx]
		};

	local szMsg = "";
	for _, tbInfo in pairs(tbTimeInfo) do
		local nTime = tbInfo.nTime;
		local nRemainDay = math.floor(nTime / (24 * 3600));
		if (nTime >= 0 and nRemainDay >= 0 and nRemainDay < self.TIME_NOTIFYONEYEAR and
			tbRelationName[tbInfo.nType]) then
			szMsg = szMsg .. string.format(Lang.gcscript.str66[Lang.Idx], tbInfo.szPlayerName,
				tbRelationName[tbInfo.nType]);
			pPlayer.Msg(szMsg);
			szMsg = "";
		end
	end
end

-- 开除当前弟子
function Relation:c2s_DelTrainingStudent(szStudentName)
	local tbNpc	= Npc:GetClass("renji");
	tbNpc:DelTrainingStudent1(szStudentName);
end
-- Relation.tbc2sFun["DelTrainingStudent"] = Relation.DelTrainingStudent;

-- 成就
function Relation:__FinishAchievement(nPlayerId)
	if (not nPlayerId or nPlayerId <= 0) then
		return;
	end
	-- local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pPlayer = nil;
	if (not pPlayer) then
		return;
	end

	-- 好友
	local tbFriend = pPlayer.GetRelationList(Player.emKPLAYERRELATION_TYPE_BIDFRIEND, 1);
	local nFriendNum = #tbFriend;
	if (nFriendNum >= 1) then
		Achievement:FinishAchievement(pPlayer, 1);
	end
	if (nFriendNum >= 10) then
		Achievement:SetFinishNum(pPlayer, 2, nFriendNum);
		Achievement:FinishAchievement(pPlayer, 2);
	end
	if (nFriendNum >= 20) then
		Achievement:SetFinishNum(pPlayer, 3, nFriendNum);
		Achievement:FinishAchievement(pPlayer, 3);
	end
	if (nFriendNum >= 50) then
		Achievement:SetFinishNum(pPlayer, 4, nFriendNum);
		Achievement:FinishAchievement(pPlayer, 4);
	end

	-- 密友
	local tbBuddy = pPlayer.GetRelationList(Player.emKPLAYERRELATION_TYPE_BUDDY, 1);
	local tbTrained = pPlayer.GetRelationList(Player.emKPLAYERRELATION_TYPE_TRAINED, 1);
	local tbIntroduction = pPlayer.GetRelationList(Player.emKPLAYERRELATION_TYPE_INTRODUCTION, 1);
	local nBuddyNum = #tbBuddy;
	local nTrainedNum = #tbTrained;
	local nIntroduction = #tbIntroduction;
	local nMiyouNum = nBuddyNum + nTrainedNum + nIntroduction;
	if (nMiyouNum >= 1) then
		Achievement:FinishAchievement(pPlayer, 11);
	end
	if (nMiyouNum >= 2) then
		Achievement:SetFinishNum(pPlayer, 12, nMiyouNum);
		Achievement:FinishAchievement(pPlayer, 12);
	end
	if (nMiyouNum >= 4) then
		Achievement:SetFinishNum(pPlayer, 13, nMiyouNum);
		Achievement:FinishAchievement(pPlayer, 13);
	end

	-- 出师弟子
	if (nTrainedNum >= 1) then
		Achievement:FinishAchievement(pPlayer, 22);
	end
	if (nTrainedNum >= 3) then
		Achievement:SetFinishNum(pPlayer, 23, nTrainedNum);
		Achievement:FinishAchievement(pPlayer, 23);
	end
	if (nTrainedNum >= 10) then
		Achievement:SetFinishNum(pPlayer, 24, nTrainedNum);
		Achievement:FinishAchievement(pPlayer, 24);
	end
	if (nTrainedNum >= 20) then
		Achievement:SetFinishNum(pPlayer, 25, nTrainedNum);
		Achievement:FinishAchievement(pPlayer, 25);
	end

	-- 未出师师徒
	local tbTraining = pPlayer.GetRelationList(Player.emKPLAYERRELATION_TYPE_TRAINING, 1);
	local nTrainingNum = #tbTraining;
	if (nTrainingNum >= 1) then
		Achievement:FinishAchievement(pPlayer, 15);
	end
	if (nTrainingNum >= 3) then
		Achievement:SetFinishNum(pPlayer, 16, nTrainingNum);
		Achievement:FinishAchievement(pPlayer, 16);
	end
end

function Relation:AfterAddRelation_GS(nAppId, nDstId, nType)
	self:__FinishAchievement(nAppId);
	self:__FinishAchievement(nDstId);
end

-- 修复成就
function Relation:RepairAchievement()
	-- 修复好友数量的成就
	self:__FinishAchievement(me.nId);

	-- 修复亲密度等级的成就
	local nMaxFavor = 0;
	local tbFavorLevel_Achieve = {[2] = 5, [3] = 6, [4] = 7, [5] = 8, [6] = 9, [7] = 10,};
	local tbFriend = me.GetRelationList(Player.emKPLAYERRELATION_TYPE_BIDFRIEND, 1);
	if (not tbFriend or #tbFriend <= 0) then
		return;
	end
	for _, szName in pairs(tbFriend) do
		local nFavor = me.GetFriendFavor(szName);
		nMaxFavor = nMaxFavor > nFavor and nMaxFavor or nFavor;
	end
	local nFavorLevel = math.ceil(math.sqrt(math.ceil(nMaxFavor/100)));
	if (nFavorLevel < 2) then
		return;
	end
	for i = 2, nFavorLevel do
		if (tbFavorLevel_Achieve[i]) then
			Achievement:FinishAchievement(me, tbFavorLevel_Achieve[i]);
		end
	end
end

function Relation:__FinishAchievement_FavorLevel(pPlayer, nFavorLevel)
	if (not pPlayer or not nFavorLevel or nFavorLevel < 1) then
		return;
	end

	-- 成就，亲密度等级提升
	if (nFavorLevel >= 2) then
		Achievement:FinishAchievement(pPlayer, 5);
	end
	if (nFavorLevel >= 3) then
		Achievement:FinishAchievement(pPlayer, 6);
	end
	if (nFavorLevel >= 4) then
		Achievement:FinishAchievement(pPlayer, 7);
	end
	if (nFavorLevel >= 5) then
		Achievement:FinishAchievement(pPlayer, 8);
	end
	if (nFavorLevel >= 6) then
		Achievement:FinishAchievement(pPlayer, 9);
	end
	if (nFavorLevel >= 7) then
		Achievement:FinishAchievement(pPlayer, 10);
	end
end

-- 拜师或收徒的应答
function Relation:c2s_TrainingResponse(nTOrSFlag, szPlayerName, bAnswer)
	if (not self.tbClass or not self.tbClass[Player.emKPLAYERRELATION_TYPE_TRAINING]) then
		return;
	end
	if (self.tbClass[Player.emKPLAYERRELATION_TYPE_TRAINING].TrainingResponse) then
		self.tbClass[Player.emKPLAYERRELATION_TYPE_TRAINING]:TrainingResponse(nTOrSFlag, szPlayerName, bAnswer);
	end
end
-- Relation.tbc2sFun["TrainingResponse_C2S"] = Relation.TrainingResponse_C2S;

--==============================================

-- 亲密度等级提升之后gs的回调
function Relation:OnFavorLevelUp_GS(nPlayerAppId, nPlayerDstId, nFavorLevel)
	if (not nPlayerAppId or not nPlayerDstId or not nFavorLevel or
		nPlayerAppId <= 0 or nPlayerDstId <= 0 or nFavorLevel <= 0) then
		return;
	end
	local pPlayerApp = KGameBase.GetPlayerById(nPlayerAppId);
	local pPlayerDst = KGameBase.GetPlayerById(nPlayerDstId);
	if (pPlayerApp) then
		self:__FinishAchievement_FavorLevel(pPlayerApp, nFavorLevel);
	end
	if (pPlayerDst) then
		self:__FinishAchievement_FavorLevel(pPlayerDst, nFavorLevel);
	end
end

-- 添加人际关系之后gs的回调
function Relation:ProcessAfterAddRelation_GS(nType, nRole, nAppId, nDstId)
	if (not nType) then
		return;
	end

	-- 增加人际关系记录log
	Relation:WriteRelationLog(self.LOG_TYPE_ADDRELATION, nType, nAppId, nDstId);

	if (not self.tbClass[nType]) then
		return;
	end
	if (not self.tbClass[nType]["ProcessAfterAddRelation_GS"]) then
		return;
	end
	self.tbClass[nType]:ProcessAfterAddRelation_GS(nRole, nAppId, nDstId);
end

-- 删除人际关系之后gs的回调
function Relation:ProcessAfterDelRelation_GS(nType, nRole, nAppId, nDstId)
	if (not nType) then
		return;
	end

	-- 删除人际关系记录log
	Relation:WriteRelationLog(self.LOG_TYPE_DELRELATION, nType, nAppId, nDstId);

	if (not self.tbClass[nType]) then
		return;
	end
	if (not self.tbClass[nType]["ProcessAfterDelRelation_GS"]) then
		return;
	end
	self.tbClass[nType]:ProcessAfterDelRelation_GS(nRole, nAppId, nDstId);
end
-- 通知在对方黑名单中
function Relation:NotifyAddedBlack_GS(nAppId, szDstName)
	if (not nAppId or not szDstName or nAppId <= 0) then
		return;
	end
	local pPlayer = KGameBase.GetPlayerById(nAppId);
	if pPlayer then
		pPlayer.SendBlackMsg(string.format("您已被 %s 屏蔽",szDstName));
	end
end

-- 通知关系类型超过上限值
function Relation:NotifyReachMaxCount_GS(nAppId, nType)
	if (not nAppId or nAppId <= 0) then
		return;
	end
	local pPlayer = KGameBase.GetPlayerById(nAppId);
	if pPlayer then
		if nType == Relation.emKPLAYERRELATION_TYPE_BIDFRIEND or nType == Relation.emKPLAYERRELATION_TYPE_TMPFRIEND then
			pPlayer.SendBlackMsg(string.format(Lang.gcscript.str67[Lang.Idx]));
		elseif nType == Relation.emKPLAYERRELATION_TYPE_BLACKLIST then
			pPlayer.SendBlackMsg(string.format(Lang.gcscript.str68[Lang.Idx]));
		end
	end
end

function Relation:NotifyPlayerOffline_GS(nAppId, szDstName)
	if (not nAppId or not szDstName or nAppId <= 0) then
		return;
	end
	local pPlayer = KGameBase.GetPlayerById(nAppId);
	if pPlayer then
		pPlayer.SysMsg(string.format(Lang.gcscript.str69[Lang.Idx],szDstName));
	end
end
--==============================================

-- 人际部分，通用的从从gc调用的gs函数
function Relation:ServerScript_Relation(nType, szFunName, tbParam)
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

-- 通用的人际gs调用gc的一个接口
-- 主要是给class下面的各种具体的人际关系调用
function Relation:CallGCScript_Relation(nType, szFunName, tbParam)
	if (not nType or not szFunName or type(szFunName) ~= "string") then
		return;
	end

	KGameBase.CenterExecute{"Relation:GCScript_Relation", nType, szFunName, tbParam};
end

--==============================================

-- 注册通用上线事件
-- PlayerEvent:RegisterGlobal("OnLogin", Relation.GetCloseFriendTimeInfo_GS, Relation);
