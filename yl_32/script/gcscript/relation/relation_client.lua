-----------------------------------------------------
--文件名		：	relation_client.lua
--创建者		：	
--创建时间		：	
--功能描述		：	人际关系client_core脚本。
------------------------------------------------------


Relation.SORTTYPE =
{
	Player.emKPLAYERRELATION_TYPE_BIDFRIEND,
	Player.emKPLAYERRELATION_TYPE_TRAINING,
	Player.emKPLAYERRELATION_TYPE_TRAINED,
	Player.emKPLAYERRELATION_TYPE_COUPLE,
	Player.emKPLAYERRELATION_TYPE_INTRODUCTION,
	Player.emKPLAYERRELATION_TYPE_BUDDY,
};

Relation.MAX_NUMBER_FRIEND	= 200;
----------------------------

function Relation:SyncRelationStart()
	me.Relation_InitRelationList();
end

-- 同步人际关系列表
function Relation:SyncRelationList(tbList)
	for nIndex in pairs(tbList) do
		-- 根据类型，对数据进行分类
		me.Relation_AddRelation(tbList[nIndex])
	end
	CoreEventNotify(UiNotify.emCOREEVENT_RELATION_UPDATEPANEL);
end

function Relation:SyncAddRelationList(tbList)
	for nIndex in pairs(tbList) do
		-- 根据类型，对数据进行分类
		me.Relation_AddRelation(tbList[nIndex])
	end
	CoreEventNotify(UiNotify.emCOREEVENT_RELATION_UPDATEPANEL);
end

function Relation:SyncRelationInfo(tbInfoList)
	if not tbInfoList then
		return 0;
	end
	
	for i = 1, #tbInfoList do
		me.Relation_SetRelationInfo(tbInfoList[i].szPlayer, tbInfoList[i]);
	end
	CoreEventNotify(UiNotify.emCOREEVENT_RELATION_REFRESHTRAIN);
	CoreEventNotify(UiNotify.emCOREEVENT_RELATION_UPDATEPANEL);
end

-- 添加人际关系
function Relation:OnAddRelation(szPlayer, nType)
	if me.HasRelation(szPlayer, 4) == 1 then
		me.Msg(Lang.gcscript.str47[Lang.Idx])
		return
	end
	local tbRelationList, tbInfo, nNum = me.Relation_GetRelationList();
	if ((nType == Player.emKPLAYERRELATION_TYPE_BIDFRIEND or nType == Player.emKPLAYERRELATION_TYPE_TMPFRIEND) and
		nNum and self.MAX_NUMBER_FRIEND <= nNum) then
		me.Msg(Lang.gcscript.str48[Lang.Idx]);
		return;
	end
	
	me.CallServerScript({"RelationCmd", "AddRelation_C2S", szPlayer, nType});
end

-- 删除人际关系
function Relation:DelRelation(szPlayer, nType)
	local tbDel = me.Relation_DelRelation(nType, szPlayer)
	if not tbDel then
		return;
	end
	if (tbDel.nType == Player.emKPLAYERRELATION_TYPE_TRAINED or
		tbDel.nType == Player.emKPLAYERRELATION_TYPE_TRAINING or 
		tbDel.nType == Player.emKPLAYERRELATION_TYPE_BUDDY or
		tbDel.nType == Player.emKPLAYERRELATION_TYPE_INTRODUCTION or
		tbDel.nType ==  Player.emKPLAYERRELATION_TYPE_COUPLE) then
		tbDel.nType = Player.emKPLAYERRELATION_TYPE_BIDFRIEND;
		me.Relation_AddRelation(tbDel);
	end
	CoreEventNotify(UiNotify.emCOREEVENT_RELATION_UPDATEPANEL);
end

function Relation:UpdateTrainingOption(nTeacher, nStudent)
	CoreEventNotify(UiNotify.emCOREEVENT_SYNC_TRAINING_OPTION, nTeacher, nStudent);
end
function Relation:SelfCheckInTime(nCheckTime)
-- 	me.SetMySelfCheckTime(nCheckTime);
--	CoreEventNotify(UiNotify.emCOREEVENT_SELF_CHECKIN_TIME);
end

-- 申请拜师
function Relation:CmdApplyTeacher(szPlayerName)
	me.CallServerScript({"RelationCmd", "ApplyTeacher_C2S", szPlayerName});
end

-- 申请拜师
function Relation:ApplyTeacher_S2C(szPlayerName)
	if (not szPlayerName) then
		return;
	end
	CoreEventNotify(UiNotify.emCOREEVENT_TRAINING_APPLYFORTEACHER, szPlayerName);
end

---------------------------------------------------------------------------------------
-- 以下人际关系判断与relation.lua中保持一致(除了临时好友)

-- 是否为本地玩家的好友 不包括临时好友
function Relation:IsFriend(szPlayer)
	local tbRelationList, _ = me.Relation_GetRelationList();
	if (not tbRelationList) then
		return 0;
	end
	local tbFriendList = tbRelationList[Player.emKPLAYERRELATION_TYPE_BIDFRIEND];
	if (tbFriendList) then
		for szName, tbFriend in pairs(tbFriendList) do
			if (szName == szPlayer) then
				return 1;
			end
		end
	end
	-- 介绍人
	local tbIntroducerList = tbRelationList[Player.emKPLAYERRELATION_TYPE_INTRODUCTION];
	if (tbIntroducerList) then
		for szName, tbIntroducer in pairs(tbIntroducerList) do
			if (szName == szPlayer and tbIntroducer.nRole == 1) then
				return 1;
			end
		end
	end
	return 0;
end

-- 是否为本地玩家的队友 
function Relation:IsTeammate(szPlayer)
	local nAllotModel, tbMemberList = me.GetTeamInfo();
	if (not tbMemberList) then
		return 0;
	end
	for i = 1, #tbMemberList do
		if (szPlayer == tbMemberList[i].szName) then
			return 1;
		end
	end
	return 0;
end

-- 是否与本地玩家有师徒关系
function Relation:IsTrainRelation(szPlayer)
	local tbRelationList, _ = me.Relation_GetRelationList();
	if (not tbRelationList) then
		return 0;
	end
	-- 未出师
	local tbTrainingList = tbRelationList[Player.emKPLAYERRELATION_TYPE_TRAINING];
	if (tbTrainingList) then
		for szName, _ in pairs(tbTrainingList) do
			if (szName == szPlayer) then
				return 1;
			end
		end
	end
	-- 已出师
	local tbTrainedList = tbRelationList[Player.emKPLAYERRELATION_TYPE_TRAINED];
	if (tbTrainedList) then
		for szName, tbTrained in pairs(tbTrainedList) do
			if (szName == szPlayer) then	
				return 1;
			end
		end
	end
	return 0;
end

-- 是否为本地玩家的密友
function Relation:IsBuddy(szPlayer)
	local tbRelationList, _ = me.Relation_GetRelationList();
	if (not tbRelationList) then
		return 0;
	end
	-- 密友
	local tbBuddyList = tbRelationList[Player.emKPLAYERRELATION_TYPE_BUDDY];
	if (tbBuddyList) then
		for szName, _ in pairs(tbBuddyList) do
			if (szName == szPlayer) then
				return 1;
			end
		end
	end
	-- 介绍人
	local tbIntroducerList = tbRelationList[Player.emKPLAYERRELATION_TYPE_INTRODUCTION];
	if (tbIntroducerList) then
		for szName, tbIntroducer in pairs(tbIntroducerList) do
			if (szName == szPlayer and tbIntroducer.nRole == 0) then
				return 1;
			end
		end
	end
	return 0;
end

-- 师傅为本地玩家的侠侣
function Relation:IsCouple(szPlayer)
	local tbRelationList, _ = me.Relation_GetRelationList();
	if (not tbRelationList) then
		return 0;
	end
	-- 侠侣
	local tbCoupleList = tbRelationList[Player.emKPLAYERRELATION_TYPE_COUPLE];
	if (tbCoupleList) then
		for szName, _ in pairs(tbCoupleList) do
			if (szName == szPlayer) then
				return 1;
			end
		end
	end
	return 0;
end
