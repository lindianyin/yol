if 1 then
	return;
end

local RELATIONTYPE_TRAINING 	= 5		-- 当前师徒关系
local RELATIONTYPE_TRAINED		= 6		-- 出师师徒关系
local RELATIONTYPE_INTRODUCE	= 8		-- 介绍人关系
local RELATIONTYPE_BUDDY		= 9		-- 指定密友关系
local COST_DELTEACHER			= 10000	-- 解除和师父关系的费用
local COST_DELSTUDENT			= 10000	-- 解除和弟子关系的费用

local tbNpc	= Npc:GetClass("renji");

-- 对话
function tbNpc:OnDialog()
	local szMsg = Lang.gcscript.str91[Lang.Idx];
	local tbOpt = {};
	
	table.insert(tbOpt, {  Lang.gcscript.str75[Lang.Idx], 		tbNpc.Training,					self });
	table.insert(tbOpt, {  Lang.gcscript.str76[Lang.Idx],	tbNpc.Introduce,				self });
	table.insert(tbOpt, {  Lang.gcscript.str77[Lang.Idx],		tbNpc.Buddy,					self });
	table.insert(tbOpt, {  Lang.gcscript.str78[Lang.Idx],	tbNpc.GainBindCoin,				self });
	table.insert(tbOpt, {  Lang.gcscript.str79[Lang.Idx],	tbNpc.AchievementDlg,			self });
	table.insert(tbOpt, {  Lang.gcscript.str80[Lang.Idx] });
	
	
	local nCurDate = tonumber(os.date("%Y%m%d", GetTime()));
	if (nCurDate >= 20100920 and nCurDate <= 20101004) then
		local tbTemp = {Lang.gcscript.str81[Lang.Idx], SpecialEvent.Achive_Zhaneyuan.OnDialog,
			SpecialEvent.Achive_Zhaneyuan};
		table.insert(tbOpt, 1, tbTemp);
	end
	
	
	return Dialog:Say(szMsg, tbOpt);
end

function  tbNpc:Training()
	local tbOpt = 	{
		{ Lang.gcscript.str82[Lang.Idx], 		tbNpc.AppTrain,					self },
		{ Lang.gcscript.str83[Lang.Idx],   tbNpc.Graduation, 				self },
		{ Lang.gcscript.str84[Lang.Idx],	tbNpc.DelTrainingRelation,		self },
		{ Lang.gcscript.str85[Lang.Idx],	tbNpc.GetShiTuChuanSongFu, 		self },
		{ Lang.gcscript.str86[Lang.Idx], tbNpc.RetrieveAchievement, 	self},
	};
	if (me.GetTrainingStudentList()) then
		table.insert(tbOpt, #tbOpt, {Lang.gcscript.str87[Lang.Idx], tbNpc.ChangeShituTitle, self});
	end
	
	if (me.GetTrainingTeacher()) then
		table.insert(tbOpt, #tbOpt, {Lang.gcscript.str88[Lang.Idx], tbNpc.GetStudentTitle, self});
	end
	if Esport.Mentor.bVisible then
		if self:GetTeamMission() then
			table.insert(tbOpt, {Lang.gcscript.str89[Lang.Idx], tbNpc.PreStartMentor, self });
		else
			table.insert(tbOpt, {Lang.gcscript.str90[Lang.Idx], tbNpc.PreStartMentor, self });
		end
	end
	
	table.insert(tbOpt, {Lang.gcscript.str92[Lang.Idx]});
	
	Dialog:Say(Lang.gcscript.str93[Lang.Idx], tbOpt);
end

function tbNpc:DelTrainingRelation()
	local pszTeacher = me.GetTrainingTeacher();
	local tbStudent = me.GetTrainingStudentList();
	if (self:CanDoRelationOpt(me.szName) == 0) then
		return;
	end
	if (not pszTeacher and not tbStudent) then
		Dialog:Say(Lang.gcscript.str94[Lang.Idx]);
		return 0;
	else
		Dialog:Say(Lang.gcscript.str93[Lang.Idx],
		{
			{ Lang.gcscript.str95[Lang.Idx], tbNpc.DelTrainingTeacherDialog, self },
			{ Lang.gcscript.str96[Lang.Idx], tbNpc.DelTrainingStudentDialog, self },
			{ Lang.gcscript.str97[Lang.Idx] }
		})
	end
end

function tbNpc:Introduce()
	if (self:CanDoRelationOpt(me.szName) == 0) then
		return;
	end
	Dialog:Say(Lang.gcscript.str98[Lang.Idx],
	{
		{ Lang.gcscript.str100[Lang.Idx], tbNpc.IntroduceDialog, 			self },
		{ Lang.gcscript.str101[Lang.Idx], tbNpc.BuddyDialog, 				self },	
		{ Lang.gcscript.str97[Lang.Idx] }
	})
end

function tbNpc:Buddy()
	if (self:CanDoRelationOpt(me.szName) == 0) then
		return;
	end
	Dialog:Say(Lang.gcscript.str102[Lang.Idx],
	{
		{ Lang.gcscript.str101[Lang.Idx], tbNpc.BuddyDialog, 				self },	
		{ Lang.gcscript.str103[Lang.Idx], 	tbNpc.DelBuddyDialog, 			self },
		{ Lang.gcscript.str97[Lang.Idx] }
	})
end

function tbNpc:GainBindCoin()
	if 1 ~= jbreturn:GainBindCoin() then
		Dialog:Say(Lang.gcscript.str104[Lang.Idx],
		{
			{ string.format(Lang.gcscript.str105[Lang.Idx], IVER_g_szCoinName), tbNpc.GetIbBindCoin,			self },
			{ Lang.gcscript.str97[Lang.Idx] }
		})
	end
end

-- 更换师徒称号
function tbNpc:ChangeShituTitle()
	local tbItem = Item:GetClass("teacher2student");
	tbItem:ChangeShituTitle();
end

-- 获取弟子称号
function tbNpc:GetStudentTitle()
	local tbItem = Item:GetClass("teacher2student");
	tbItem:FetchStudentTitle();
end

-- 检查拜师的条件
function tbNpc:CheckAppTrainCond()
	if (0 == self:CanDoRelationOpt(me.szName)) then
		return 0;
	end

	if (me.GetTrainingTeacher()) then
		Dialog:Say(Lang.gcscript.str106[Lang.Idx]);
		return 0;
	end
	
	if (me.nLevel < Relation.STUDENT_MINILEVEL) then
		Dialog:Say(string.format(Lang.gcscript.str107[Lang.Idx], Relation.STUDENT_MINILEVEL));
		return 0;
	end
	
	if (me.nLevel >= Relation.STUDENT_MAXLEVEL) then
		Dialog:Say(string.format(Lang.gcscript.str108[Lang.Idx], Relation.STUDENT_MAXLEVEL - 1));
		return 0;
	end
	
	local nCurDate = tonumber(os.date("%Y%m%d", GetTime()));
	local nLastAppDate = me.GetTask(Relation.TASK_GROUP, Relation.TASKID_LASTAPPTRAIN_DATE);
	local nAppCount = me.GetTask(Relation.TASK_GROUP, Relation.TASKID_APPTRAIN_COUNT);
	if (nAppCount > Relation.MAX_APPTRAIN_COUNT) then
		Dialog:Say(string.format(Lang.gcscript.str109[Lang.Idx], Relation.MAX_APPTRAIN_COUNT));
		return 0;
	end
	
	local tblMemberList, nMemberCount = me.GetTeamMemberList()
	if (2 ~= nMemberCount) then
		Dialog:Say(Lang.gcscript.str110[Lang.Idx]);
		return 0;
	end
	
	local cTeamMate	= tblMemberList[1];
	for i = 1, #tblMemberList do
		if (tblMemberList[i].szName ~= me.szName) then
			cTeamMate = tblMemberList[i];
		end
	end
	
	-- 在拜师前，如果不是好友关系，先自动加为好友
	if (me.IsFriendRelation(cTeamMate.szName) ~= 1) then
		Dialog:Say(Lang.gcscript.str111[Lang.Idx]);
		return 0;
	end
	
	if (cTeamMate.nLevel < Relation.TEACHER_NIMIEVEL) then
		Dialog:Say(string.format(Lang.gcscript.str112[Lang.Idx], Relation.TEACHER_NIMIEVEL));
		return 0;
	end
	
	if (cTeamMate.nLevel - me.nLevel < Relation.GAPMINILEVEL) then
		Dialog:Say(string.format(Lang.gcscript.str113[Lang.Idx],
			Relation.GAPMINILEVEL));
		return 0;
	end
	
	local tbStudentList	= me.GetTrainingStudentList();
	if (tbStudentList and Lib:CountTB(tbStudentList) > Relation.MAX_STUDENCOUNT) then
		Dialog:Say(string.format(Lang.gcscript.str114[Lang.Idx], Relation.MAX_STUDENCOUNT));
		return 0;
	end
	
	if (cTeamMate.GetTrainingTeacher()) then
		Dialog:Say(Lang.gcscript.str115[Lang.Idx]);
		return 0;
	end
	
	return 1;
end

-- 申请拜师
function tbNpc:AppTrain()
	local bCanAppTrain = self:CheckAppTrainCond();
	if (1 == bCanAppTrain) then
		local tblMemberList, nMemberCount = me.GetTeamMemberList()
		local cTeamMate	= tblMemberList[1];
		for i = 1, #tblMemberList do
			if (tblMemberList[i].szName ~= me.szName) then
				cTeamMate = tblMemberList[i];
				break;
			end
		end
		local szTeacherName = cTeamMate.szName;
		-- me.CallClientScript({"Relation:CmdApplyTeacher", cTeamMate.szName});
		cTeamMate.CallClientScript({"Relation:ApplyTeacher_S2C", me.szName});
	end
end

-- 密友：建立指定密友对话
function tbNpc:BuddyDialog()
	local tblMemberList, nMemberCount = me.GetTeamMemberList()
	-- 玩家必须处于组队状态，且队伍中只有两个人
	if (nMemberCount ~= 2) then
		Dialog:Say(Lang.gcscript.str115[Lang.Idx])
		return
	end
	Dialog:Say(Lang.gcscript.str116[Lang.Idx],
		{
			{Lang.gcscript.str117[Lang.Idx], tbNpc.MakeBuddy, self},
			{Lang.gcscript.str118[Lang.Idx]}
		});
end

-- 密友：建立指定密友
function tbNpc:MakeBuddy()
	local tblMemberList, nMemberCount = me.GetTeamMemberList()
	-- 玩家必须处于组队状态，且队伍中只有两个人
	if (nMemberCount ~= 2) then
		Dialog:Say(Lang.gcscript.str115[Lang.Idx])
		return
	end
	for i = 1, #tblMemberList do
		local cTeamMate = tblMemberList[i]
		if (cTeamMate.szName ~= me.szName) then
			-- 检查级别
			if (me.nLevel < 61 or cTeamMate.nLevel < 61) then
				Dialog:Say(Lang.gcscript.str119[Lang.Idx])
				return
			end
			-- 两人必须互相是好友，且亲密度不低于 等级6级。即秦密度3600
			local nFavor = me.GetFriendFavor(cTeamMate.szName)
		
			if (nFavor <= 2500) then
				Dialog:Say(Lang.gcscript.str120[Lang.Idx])
				return
			end
			-- 检查是否已经是指定密友
			if (KPlayer.CheckRelation(me.szName, cTeamMate.szName, RELATIONTYPE_BUDDY) ~= 0) then
				Dialog:Say(Lang.gcscript.str121[Lang.Idx])
				return
			end
			-- 两人之间必须没有师徒关系
			if (KPlayer.CheckRelation(me.szName, cTeamMate.szName, RELATIONTYPE_TRAINING, 1) ~= 0 or
				KPlayer.CheckRelation(me.szName, cTeamMate.szName, RELATIONTYPE_TRAINED, 1) ~= 0) then
				Dialog:Say(Lang.gcscript.str122[Lang.Idx])
				return
			end
			-- 检查指定密友数量 暂时设定为4个
			if (me.GetRelationCount(RELATIONTYPE_BUDDY) >= 4 or
				cTeamMate.GetRelationCount(RELATIONTYPE_BUDDY) >= 4) then
				Dialog:Say(Lang.gcscript.str123[Lang.Idx])
				return
			end
			-- 两人之间必须没有介绍人关系
			if (KPlayer.CheckRelation(me.szName, cTeamMate.szName, RELATIONTYPE_INTRODUCE, 1) ~= 0) then
				Relation:DelRelation_GS(me.szName, cTeamMate.szName, RELATIONTYPE_INTRODUCE);
			end
			-- 建立指定密友关系
			Relation:AddRelation_GS(me.szName, cTeamMate.szName, RELATIONTYPE_BUDDY);
		end
	end
end

-- 确认介绍人对话
function tbNpc:IntroduceDialog()
	-- 检查级别
	if (me.nLevel > 11) then
		Dialog:Say(Lang.gcscript.str124[Lang.Idx])
		return
	end
	-- 玩家必须处于组队状态，且队伍中只有两个人
	local tblMemberList, nMemberCount = me.GetTeamMemberList()
	local pszTeamHint = Lang.gcscript.str125[Lang.Idx] 
	if (nMemberCount ~= 2) then
		Dialog:Say(pszTeamHint)
		return
	end
	for i = 1, #tblMemberList do
		local cTeamMate = tblMemberList[i]
		if (cTeamMate.szName ~= me.szName) then
			-- 检查级别
			if (cTeamMate.nLevel - me.nLevel < 30) then
				Dialog:Say(pszTeamHint)
				return
			end
			-- 检查是否已有介绍人
			if (me.GetRelationCount(RELATIONTYPE_INTRODUCE) ~= 0) then 
				Dialog:Say(Lang.gcscript.str126[Lang.Idx])
				return
			end
			-- 加介绍人之前需要已经是好友关系
			if (me.IsFriendRelation(cTeamMate.szName) ~= 1) then
				Dialog:Say(Lang.gcscript.str127[Lang.Idx]);
				return;
			end
			-- 建立介绍人关系
			Relation:AddRelation_GS(me.szName, cTeamMate.szName, RELATIONTYPE_INTRODUCE, 0);
			Dialog:Say(string.format(Lang.gcscript.str128[Lang.Idx],cTeamMate.szName))
			--Dialog:Say("您已经成功和" .. cTeamMate.szName .. "建立了密友关系，以后他也能享受您的成长，如果有什么不明白的，多问问他吧。")
		end
	end
end

-- 删除密友对话
function tbNpc:DelBuddyDialog()
	Dialog:Say(Lang.gcscript.str129[Lang.Idx],
		{
			{Lang.gcscript.str130[Lang.Idx], tbNpc.DeleteBuddy, self},
			{Lang.gcscript.str131[Lang.Idx]}
		})
end

-- 删除密友
function tbNpc:DeleteBuddy()
	local tblRelation = me.GetRelationList(RELATIONTYPE_BUDDY)
	if (#tblRelation == 0) then
		Dialog:Say(Lang.gcscript.str132[Lang.Idx])
		return
	end
	local tblOptions = {}
	for i = 1, #tblRelation do
		tblOptions[i] = {tblRelation[i], tbNpc.DeleteTheBuddyDialog, self, tblRelation[i]}
	end
	tblOptions[#tblRelation + 1] = {Lang.gcscript.str131[Lang.Idx]}
	Dialog:Say(Lang.gcscript.str133[Lang.Idx], tblOptions)
end

-- 删除某个指定密友对话
function tbNpc:DeleteTheBuddyDialog(pszBuddy)
	Dialog:Say(string.format(Lang.gcscript.str134[Lang.Idx],pszBuddy),
	--Dialog:Say("你确定要和<color=yellow>" .. pszBuddy .. "<color>解除密友关系吗？",
		{
			{string.format(Lang.gcscript.str135[Lang.Idx],pszBuddy), tbNpc.DeleteTheBuddy, self, pszBuddy},
			--{"是的，我要和" .. pszBuddy .. "解除密友关系", tbNpc.DeleteTheBuddy, self, pszBuddy},
			{Lang.gcscript.str131[Lang.Idx]}
		})
end

-- 删除某个指定密友
function tbNpc:DeleteTheBuddy(pszBuddyName)
	-- 扣除20W银两
	if (me.CostMoney(200000, Player.emKPAY_DEL_BUDDY) ~= 1) then
		Dialog:Say(Lang.gcscript.str136[Lang.Idx])
		return
	end
	Relation:DelRelation_GS(me.szName, pszBuddyName, RELATIONTYPE_BUDDY);
	me.Msg(string.format(Lang.gcscript.str137[Lang.Idx],pszBuddyName));
	--me.Msg("您花费了20万银两，与 "..pszBuddyName.." 解除了密友关系。");
	KPlayer.SendMail(pszBuddyName,
		Lang.gcscript.str138[Lang.Idx],
		string.format(Lang.gcscript.str139[Lang.Idx],me.szName))
end

-- 和师父解除关系对话
function tbNpc:DelTrainingTeacherDialog()
	local pszTeacher = me.GetTrainingTeacher()
	if (pszTeacher == nil) then
		Dialog:Say(Lang.gcscript.str140[Lang.Idx])
		return
	end
	Dialog:Say(string.format(Lang.gcscript.str141[Lang.Idx],pszTeacher),
	--Dialog:Say("你确定想和<color=yellow>" .. pszTeacher .. "<color>解除师徒关系吗？如果你们的关系解除，以后你们相关的各种师徒奖励都将再不能享受，你可得考虑清楚。另外解除师徒关系还需要<color=red>10000<color>两银子。",
		{
			{Lang.gcscript.str142[Lang.Idx], tbNpc.DelTrainingTeacher, self, pszTeacher},
			{Lang.gcscript.str143[Lang.Idx]}
		})
end

-- 和师父解除关系
function tbNpc:DelTrainingTeacher(pszTeacher)
	if (me.CostMoney(COST_DELTEACHER, Player.emKPAY_DEL_TEACHER) ~= 1) then
		Dialog:Say(Lang.gcscript.str144[Lang.Idx])
		return
	end
	Relation:DelRelation_GS(me.szName, pszTeacher, RELATIONTYPE_TRAINING, 0);
	
	-- 去掉师徒称号
	local szStudentTitle = pszTeacher .. Lang.gcscript.str43[Lang.Idx];
	me.RemoveSpeTitle(szStudentTitle);
	EventManager:WriteLog(Lang.gcscript.str145[Lang.Idx]..szStudentTitle, me);
	
	
	KPlayer.SendMail(pszTeacher, Lang.gcscript.str146[Lang.Idx],
		string.format(Lang.gcscript.str147[Lang.Idx],me.szName));
	--Dialog:Say("你和<color=yellow>" .. pszTeacher .. "<color>的师徒关系已经成功解除了，以后你们就天各一方，互不相干了。")
	Dialog:Say(string.format(Lang.gcscript.str148[Lang.Idx],pszTeacher))
end

-- 和弟子解除关系对话
function tbNpc:DelTrainingStudentDialog()
	local tbStudent = me.GetTrainingStudentList()
	if (tbStudent == nil) then
		Dialog:Say(Lang.gcscript.str149[Lang.Idx])
		return
	end
	local tbOption = {}
	for i = 1, #tbStudent do
		tbOption[i] = {tbStudent[i], tbNpc.DelTrainingStudent1, self, tbStudent[i]}
	end
	tbOption[#tbStudent + 1] = {Lang.gcscript.str150[Lang.Idx]}
	Dialog:Say(Lang.gcscript.str151[Lang.Idx], tbOption)
end

-- 和弟子解除关系
function tbNpc:DelTrainingStudent1(pszStudent)
	Dialog:Say(string.format(Lang.gcscript.str152[Lang.Idx],pszStudent),
	--Dialog:Say("你确定想和<color=yellow>" .. pszStudent .. "<color>解除师徒关系吗？如果你们的关系解除，以后你们相关的各种师徒奖励都将再不能享受，你可得考虑清楚。",
		{
			{Lang.gcscript.str153[Lang.Idx], tbNpc.DelTrainingStudent2, self, pszStudent},
			{Lang.gcscript.str143[Lang.Idx]}
		})
end

-- 和弟子解除关系
function tbNpc:DelTrainingStudent2(pszStudent)
	Relation:DelRelation_GS(me.szName, pszStudent, RELATIONTYPE_TRAINING, 1);
	
	local szTeacherTitle = pszStudent .. Lang.gcscript.str44[Lang.Idx];
	me.RemoveSpeTitle(szTeacherTitle);
	EventManager:WriteLog(Lang.gcscript.str145[Lang.Idx]..szTeacherTitle, me);
	
	KPlayer.SendMail(pszStudent, Lang.gcscript.str146[Lang.Idx],
		--"您好，您的师傅" .. me.szName .. "已经单方面和你解除了师徒关系。节哀啊节哀。");
		string.format(Lang.gcscript.str154[Lang.Idx],me.szName));
	--Dialog:Say("你和" .. pszStudent .. "的师徒关系已经成功解除了，以后你们就天各一方，互不相干了。")
	Dialog:Say(string.format(Lang.gcscript.str148[Lang.Idx],pszStudent))
end

--开启师徒副本
function tbNpc:PreStartMentor()

	local tbMiss = self:GetTeamMission();
	
	--如果副本已经开启了，显示进入副本
	if tbMiss then		
			
		--将玩家再次传入到FB中的起始点
		tbMiss:ReEnterMission(me.nId);
	else	--否则开启副本	
		if Esport.Mentor:CheckEnterCondition(me.nId) ~= 1 then
			return;
		end
			
		if Esport.Mentor:PreStartMission() == 0 then
			Dialog:Say(Lang.gcscript.str155[Lang.Idx])
		end
	end

end

--根据当前队伍来取得MISSION，即始终根据队伍中的徒弟来取MISSION
function tbNpc:GetTeamMission()
	--必须是师徒二人组成的队伍才能进，这时候已经不需要做其它判定了，因为该玩家只会被送到他之前开启了的副本里面
	local anPlayerId, nPlayerNum = KTeam.GetTeamMemberList(me.nTeamId);
	if not anPlayerId or not nPlayerNum or nPlayerNum ~= 2 then 
		Dialog:Say(Lang.gcscript.str156[Lang.Idx]);
		return;
	end
			
	--如果是徒弟要进，直接进入自己的副本就好了；
	--如果是师傅要进，进入到队伍中的徒弟的副本。
	local tbMiss;		
	if Esport.Mentor:CheckApprentice(me.nId) == 1 then
		tbMiss = Esport.Mentor:GetMission(me);
	else
		local pStudent = Esport.Mentor:GetApprentice(me.nId); 
		tbMiss = Esport.Mentor:GetMission(pStudent);		--如果当前队伍不是由满足关系的师徒二人组成的，得到的MISSION为NIL
	end
	
	return tbMiss;
end

-- 修复弟子已完成的固定成就
function tbNpc:RetrieveAchievement()
	-- 只有当前有师傅的时候才可以修复成就
	if (not me.GetTrainingTeacher()) then
		Dialog:Say(Lang.gcscript.str157[Lang.Idx]);
		return;
	end
	
	local szMsg = Lang.gcscript.str158[Lang.Idx];
	Dialog:Say(szMsg,
		{Lang.gcscript.str159[Lang.Idx], Achievement_ST.CheckPreviousAchievement, Achievement_ST, 1},
		{Lang.gcscript.str160[Lang.Idx]});
end

function tbNpc:GetShiTuChuanSongFu(bAutoGet)

	local tbChuanSongFu = { Item.SCRIPTITEM, 1, 65, 1 };
	local tbBaseProp = KItem.GetItemBaseProp(unpack(tbChuanSongFu));
	if not tbBaseProp then
		return;
	end

	local nCount = me.GetItemCountInBags(unpack(tbChuanSongFu));
	if (nCount >= 1 and (not bAutoGet)) then
		me.Msg(Lang.gcscript.str161[Lang.Idx]);
		return;
	elseif (nCount >= 1 and bAutoGet and bAutoGet == 1) then
		return;
	end

	-- 现在领取师徒传送符的条件只要达到拜师条件即可
	local nLevel = me.nLevel;
	if (nLevel < 20 and (not bAutoGet)) then
		me.Msg(Lang.gcscript.str162[Lang.Idx]);
		return 0;
	elseif (nLevel < 20 and bAutoGet and bAutoGet == 1) then
		return 0;
	end
		
	local tbItem =
	{
		nGenre		= tbChuanSongFu[1],
		nDetail		= tbChuanSongFu[2],
		nParticular	= tbChuanSongFu[3],
		nLevel		= tbChuanSongFu[4],
		nSeries		= (tbBaseProp.nSeries > 0) and tbBaseProp.nSeries or 0,
		bBind		= KItem.IsItemBindByBindType(tbBaseProp.nBindType),
		nCount		= 1,
	};

	if (me.CanAddItemIntoBag(tbItem) == 0 and (not bAutoGet)) then
		me.Msg(Lang.gcscript.str163[Lang.Idx]);
		return;
	elseif (me.CanAddItemIntoBag(tbItem) == 0 and bAutoGet and bAutoGet == 1) then
		return;
	end

	tbChuanSongFu[5] = tbItem.nSeries;
	me.AddItem(unpack(tbChuanSongFu));
end


function tbNpc:GetIbBindCoin()
	me.ApplyGainIbCoin();
end

-- 出师仪式
function tbNpc.Graduation()
	if (tbNpc:CanDoRelationOpt(me.szName) == 0) then
		return;
	end
	local tblMemberList, nMemberCount = me.GetTeamMemberList()
	if (nMemberCount ~= 2) then
		Dialog:Say(Lang.gcscript.str164[Lang.Idx])
		return
	end
	local cTeamMate	= tblMemberList[1];
	for i = 1, #tblMemberList do
		if (tblMemberList[i].szName ~= me.szName) then
			cTeamMate	= tblMemberList[i];		
		end	
	end
	local TeacherList	= me.GetTrainingTeacher();
	local StudentList	= me.GetTrainingStudentList();
	
	if (StudentList == nil) then
		if (TeacherList ~= nil) then
			Dialog:Say(Lang.gcscript.str165[Lang.Idx],
				{
					{Lang.gcscript.str166[Lang.Idx]};
				});
				return;
		end

		Dialog:Say(Lang.gcscript.str167[Lang.Idx]);
		return;
		
	end
	local bFind	= 0;
	for _,szStudentName in ipairs(StudentList) do
		if (szStudentName == cTeamMate.szName) then
			bFind	= 1;
			break;
		end
	end
	if (0 == bFind)then
			Dialog:Say(Lang.gcscript.str168[Lang.Idx]);
			return;
	end
	
	if (cTeamMate.nFaction == 0) then
		Dialog:Say(Lang.gcscript.str168[Lang.Idx]);
		return;
	end
	
	-- 获取所有固定成就
	local tbGudingAchievement = Achievement_ST:GetSpeTypeAchievementInfo(cTeamMate.nId, Lang.gcscript.str170[Lang.Idx]);
	local bAchieve = 1;
	for _, tbInfo in pairs(tbGudingAchievement) do
		if (tbInfo.bAchieve == 0) then
			bAchieve = 0;
			break;
		end
	end
	if (0 == bAchieve) then
		Dialog:Say(Lang.gcscript.str171[Lang.Idx]);
		return;
	end
	
	Dialog:Say(Lang.gcscript.str172[Lang.Idx],
		{
			{Lang.gcscript.str173[Lang.Idx], tbNpc.DoGraduation, self, cTeamMate},
			{Lang.gcscript.str174[Lang.Idx]}
		});

end

function tbNpc:DoGraduation(cTeamMate)
	local szStudent	= "";
	-- 检查级别
	if (cTeamMate.nLevel < 90) then
		Dialog:Say(Lang.gcscript.str175[Lang.Idx]);
		return
	end
	szStudent	= cTeamMate.szName;

	local pPlayer	= KPlayer.GetPlayerByName(szStudent);
	if (pPlayer ~= nil ) then
	
		me.TrainedStudent(szStudent);
		Dialog:Say(string.format(Lang.gcscript.str176[Lang.Idx],szStudent,szStudent));
		--Dialog:Say("您的弟子" .. szStudent.."已成功出师，从此以后，" .. szStudent.."将成为您的密友，他在奇珍阁消费额度的5%将作为您对他培养的奖励返还给您。<color=yellow>师徒关系将维持一年，到期后将自动解除。<color>");	
		
		pPlayer.Msg(Lang.gcscript.str177[Lang.Idx]);
	end
	
	-- 去取自定义称号，只把师傅的称号去掉，弟子的保留
	local szTeacherTitle = cTeamMate.szName .. Lang.gcscript.str44[Lang.Idx];
	me.RemoveSpeTitle(szTeacherTitle);
	EventManager:WriteLog(Lang.gcscript.str145[Lang.Idx]..szTeacherTitle, me);
end

function tbNpc:CanDoRelationOpt(szAppName)
	local pAppPlayer = KPlayer.GetPlayerByName(szAppName);
	if (not pAppPlayer) then
		return 0;
	end
	local bCanOpt, szErrMsg = Relation:CanDoRelationOpt(szAppName);
	if (bCanOpt == 0) then
		if ("" ~= szErrMsg) then
			pAppPlayer.Msg(szErrMsg);
		end
		return 0;
	end
	return 1;
end

function tbNpc:AchievementDlg()
	if (not Achievement.FLAG_OPEN or Achievement.FLAG_OPEN == 0) then
		Dialog:Say(Lang.gcscript.str178[Lang.Idx]);
		return;
	end
	
	local szMsg = Lang.gcscript.str179[Lang.Idx];
	local tbOpt = {
		{Lang.gcscript.str180[Lang.Idx], self.RepairAchievementDlg, self},
		{Lang.gcscript.str181[Lang.Idx], self.OpenAchievementShop, self},
		{Lang.gcscript.str174[Lang.Idx]},
		};
	Dialog:Say(szMsg, tbOpt);
end

function tbNpc:OpenAchievementShop()
	me.OpenShop(181, 10);
end

function tbNpc:RepairAchievementDlg()
	local szMsg = Lang.gcscript.str182[Lang.Idx];
	local tbOpt = {
		{Lang.gcscript.str183[Lang.Idx], self.RepairAchievement, self},
		{Lang.gcscript.str184[Lang.Idx]}
		};
	Dialog:Say(szMsg, tbOpt);
end

function tbNpc:RepairAchievement()
	Achievement:RepairAchievement();
end

function Relation:SyncIbBinCoinInfo(tbInfoList, bFirst)
	if (not tbInfoList or #tbInfoList == 0)then
		local szMsg	= "";

		if (bFirst == 1) then
			szMsg = string.format(Lang.gcscript.str185[Lang.Idx], IVER_g_szCoinName);
		else
			szMsg = Lang.gcscript.str186[Lang.Idx];
		end
		Dialog:Say(szMsg, 
		{
			{Lang.gcscript.str187[Lang.Idx]}
		});
		return;
	end
	if (bFirst ~= 1) then
		Dialog:Say(string.format(Lang.gcscript.str188[Lang.Idx], IVER_g_szCoinName),
		{
			{Lang.gcscript.str189[Lang.Idx], Relation.ShowGetIbCoin, self, tbInfoList},
			{Lang.gcscript.str190[Lang.Idx], Relation.CancelGainCoin, self},
		});
	else
		self:ShowGetIbCoin(tbInfoList);
	end
end

function Relation:ShowGetIbCoin(tbInfoList)
	for nIndex, tbInfo in ipairs(tbInfoList) do
		Dialog:Say(string.format(Lang.gcscript.str192[Lang.Idx],tbInfo.szName,IVER_g_szCoinName,tbInfo.nBindCoin),
		--Dialog:Say("您的密友 "..tbInfo.szName..string.format(" 在奇珍阁进行了消费，您获得返还绑定%s<color=red>", IVER_g_szCoinName)..tbInfo.nBindCoin.."<color>个",
		{
			{Lang.gcscript.str189[Lang.Idx], Relation.GainIbCoin, self, tbInfo.szName, tbInfo.nBindCoin},
			{Lang.gcscript.str190[Lang.Idx], Relation.CancelGainCoin, self},
		});
	end	
end

function Relation:GainIbCoin(szTarget, nBindCoin)
	if (me.nBindCoin + nBindCoin >= 2000000000) then
		me.Msg(string.format(Lang.gcscript.str193[Lang.Idx],IVER_g_szCoinName));
		return;
	end
	me.GainIbCoin(szTarget);
	
	-- 成就，获得密友返还金币
	Achievement:FinishAchievement(me, 14);
end

function Relation:CancelGainCoin()
	me.CancelGainIbCoin();
end

-- 找到当前师徒称号以及对应玩家的名字
function Relation:FindTitleAndName(szSuffix, pPlayer)
	local szPlayerName = "";
	
	local tbAllTitle = pPlayer.GetAllTitle();
	local szCurShituTitle = "";
	local bFind = 0;
	for _, tbTitleInfo in pairs(tbAllTitle) do
		-- 自定义称号大类的id是250
		local nTitleLen = string.len(tbTitleInfo.szTitleName);
		local nSuffixLen = string.len(szSuffix);
		local nStart, nEnd = string.find(tbTitleInfo.szTitleName, szSuffix);
		if (tbTitleInfo.byTitleGenre == 250 and nTitleLen > nSuffixLen and
			nStart ~= nEnd and nEnd == nTitleLen) then
			szPlayerName = string.sub(tbTitleInfo.szTitleName, 1, nTitleLen - nSuffixLen);
			return tbTitleInfo.szTitleName, szPlayerName;
		end
	end
end

function Relation:CheckTeacherTitle()
	-- 玩家的身份是师傅的话，检查无效的师徒称号并删除
	local szCurTitle, szPlayerName = self:FindTitleAndName(Lang.gcscript.str44[Lang.Idx], me);
	if (szCurTitle and szPlayerName and me.IsTeacherRelation(szPlayerName, 1) ~= 1) then
		me.RemoveSpeTitle(szCurTitle);
	end
end

function Relation:CheckStudentTitle()
	-- 玩家的身份是弟子的话，检查无效的师徒称号并删除
	local szCurTitle, szPlayerName = self:FindTitleAndName(Lang.gcscript.str43[Lang.Idx], me);
	if (szCurTitle and szPlayerName and me.IsTeacherRelation(szPlayerName, 0) ~= 1) then
		me.RemoveSpeTitle(szCurTitle);
	end
end

-- 在玩家上线的时候，检查师徒称号是否有效
function Relation:CheckShituTitle()
	self:CheckTeacherTitle();
	self:CheckStudentTitle();
end

-- 注册通用上线事件
PlayerEvent:RegisterGlobal("OnLogin", Relation.CheckShituTitle, Relation);
