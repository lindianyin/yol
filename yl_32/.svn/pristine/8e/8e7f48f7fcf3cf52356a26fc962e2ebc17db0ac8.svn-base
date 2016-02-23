--=================================================
-- 文件名　：training.lua
-- 创建者　：
-- 创建时间：
-- 功能描述：师徒关系（未出世的师徒关系）
--=================================================

Require("\\script\\gcscript\\relation\\relation_logic.lua");
local tbTraining = {};
tbTraining.RELATION_TYPE = Player.emKPLAYERRELATION_TYPE_TRAINING;

-- if (not MODULE_GC_SERVER and not MODULE_GAMESERVER) then
-- 	return;
-- end

--=================================================
--============= MODULE_GC_SERVER ==================
--=================================================

if (MODULE_GAMECENTER) then


tbTraining.MAX_STUDENTCOUNT = 3;			-- 当前弟子数量最多3个
tbTraining.MAX_STUDENTCOUNT_PERMONTH = 3;	-- 每个月最多教3个弟子
tbTraining.MIN_TEACHERLEVEL = 80;			-- 师傅的最顶等级80级

tbTraining.MIN_STUDENTLEVEL = 20;			-- 弟子拜师的最低等级
tbTraining.MAX_STUDENTLEVEL = 60;			-- 弟子拜师的最高等级

-- 创建人际关系的条件判断
function tbTraining:CanCreateRelation(nRole, nAppId, nDstId)
	local nRet = 0;
	if (not nRole or not nAppId or not nDstId) then
		return 0;
	end
	
	if (0 == nRole) then
		nAppId, nDstId = nDstId, nAppId;
	end
	
	nRet = self:__CheckTeacher(nAppId);
	if (not nRet or nRet == 0) then
		return 0;
	end
	nRet = self:__CheckStudent(nDstId);
	if (not nRet or nRet == 0) then
		return 0;
	else
		return 1;
	end
end

function tbTraining:__CheckTeacher(nTeacherId)
	local nCurMonth = tonumber(os.date("%Y%m", GetTime()));
	local nMonthCode = KGCPlayer.OptGetTask(nTeacherId, KGCPlayer.TRAINING_MONTH);
	local nMonthCount = KGCPlayer.OptGetTask(nTeacherId, KGCPlayer.TRAINING_COUNT);
	if (nCurMonth == nMonthCode and nMonthCount >= self.MAX_STUDENTCOUNT_PERMONTH) then
		Relation:SetInfoMsg(string.format(Lang.gcscript.str37[Lang.Idx], self.MAX_STUDENTCOUNT_PERMONTH));
		return 0;
	end
	
	local nLevle = KGCPlayer.OptGetTask(nTeacherId, KGCPlayer.LEVEL);
	if (nLevle < self.MIN_TEACHERLEVEL) then
		Relation:SetInfoMsg(string.format(Lang.gcscript.str38[Lang.Idx], self.MIN_TEACHERLEVEL));
		return 0;
	end
	
	local nCurStudentCount = KRelation.GetOneRelationCount(nTeacherId, Player.emKPLAYERRELATION_TYPE_TRAINING, 1);
	if (nCurStudentCount >= self.MAX_STUDENTCOUNT) then
		Relation:SetInfoMsg(string.format(Lang.gcscript.str39[Lang.Idx], self.MAX_STUDENTCOUNT));
		return 0;
	end
	
	if (KRelation.GetOneRelationCount(nTeacherId, Player.emKPLAYERRELATION_TYPE_TRAINING, 0) ~= 0) then
		Relation:SetInfoMsg(Lang.gcscript.str40[Lang.Idx]);
		return 0;
	end
	
	return 1;
end

function tbTraining:__CheckStudent(nStudentId)
	
	local nLevle = KGCPlayer.OptGetTask(nStudentId, KGCPlayer.LEVEL);
	if (nLevle < self.MIN_STUDENTLEVEL or nLevle >= self.MAX_STUDENTLEVEL) then
		Relation:SetInfoMsg(string.format(Lang.gcscript.str41[Lang.Idx], self.MIN_STUDENTLEVEL, self.MAX_STUDENTLEVEL - 1));
		return 0;
	end
	
	if (KRelation.GetOneRelationCount(nStudentId, Player.emKPLAYERRELATION_TYPE_TRAINING, 0) ~= 0) then
		Relation:SetInfoMsg(Lang.gcscript.str42[Lang.Idx]);
		return 0;
	end
	
	return 1;
end

-- 增加师徒关系之后的回调
function tbTraining:ProcessAfterAddRelation_GC(nRole, nAppId, nDstId)
	self:ProcessAfterAddShitu(nDstId, nAppId);
end

-- 添加师徒关系的后续处理
function tbTraining:ProcessAfterAddShitu(nStudentId, nTeacherId)
	if (not nStudentId or not nTeacherId or nStudentId <= 0 or nTeacherId <= 0) then
		return;
	end
	self:AddShituTitle_GC(nStudentId, nTeacherId);
	self:SendChuansongfu(nStudentId, nTeacherId);
	self:__ProcessAftterAddShitu_AccMonthCount(nTeacherId);
end

function tbTraining:__ProcessAftterAddShitu_AccMonthCount(nTeacherId)
	if (not nTeacherId or nTeacherId <= 0) then
		return;
	end
	
	local nCurMonth = tonumber(os.date("%Y%m", GetTime()));
	local nMonthCode = KGCPlayer.OptGetTask(nTeacherId, KGCPlayer.TRAINING_MONTH);
	local nMonthCount = KGCPlayer.OptGetTask(nTeacherId, KGCPlayer.TRAINING_COUNT);
	if (nCurMonth == nMonthCode and (nMonthCount + 1) <= self.MAX_STUDENTCOUNT_PERMONTH) then
		KGCPlayer.OptSetTask(nTeacherId, KGCPlayer.TRAINING_COUNT, nMonthCount + 1);
		return 1;
	end
	
	if (nCurMonth ~= nMonthCode) then
		KGCPlayer.OptSetTask(nTeacherId, KGCPlayer.TRAINING_COUNT, 1);
		KGCPlayer.OptSetTask(nTeacherId, KGCPlayer.TRAINING_MONTH, nCurMonth);
		return 1;
	end
end

-- 为师父双方发放师徒传送符
function tbTraining:SendChuansongfu(nStudentId, nTeacherId)
	if (not nStudentId or not nTeacherId or nStudentId <= 0 or nTeacherId <= 0) then
		return;
	end
	Relation:CallServerScript_Relation(self.RELATION_TYPE, "SendChuansongfu_GS", {nStudentId, nTeacherId});
end

-- 增加师徒称号
function tbTraining:AddShituTitle_GC(nStudentId, nTeacherId)
	if (not nStudentId or not nTeacherId or nStudentId <= 0 or nTeacherId <= 0) then
		return;
	end
	
	local szStudentName = KGCPlayer.GetPlayerName(nStudentId);
	local szTeacherName = KGCPlayer.GetPlayerName(nTeacherId);
	Relation:CallServerScript_Relation(self.RELATION_TYPE, "AddShituTitle_GS", {szStudentName, szTeacherName});
end




end	-- if (MODULE_GC_SERVER) then








--=================================================
--============= MODULE_GAMESERVER =================
--=================================================

if (MODULE_GAMESERVER) then




-- 为师徒双发发放师徒传送符
function tbTraining:SendChuansongfu_GS(tbParam)
	local nStudentId = tbParam[1] or 0;
	local nTeacherId = tbParam[2] or 0;
	local pStudent = KGameBase.GetPlayerById(nStudentId);
	local pTeacher = KGameBase.GetPlayerById(nTeacherId);
	
	if (pStudent) then
		Setting:SetGlobalObj(pStudent);
		local tbNpc	= Npc:GetClass("renji");
		tbNpc:GetShiTuChuanSongFu(1);
		Setting:RestoreGlobalObj();
	end
	
	if (pTeacher) then
		Setting:SetGlobalObj(pTeacher);
		local tbNpc	= Npc:GetClass("renji");
		tbNpc:GetShiTuChuanSongFu(1);
		Setting:RestoreGlobalObj();
	end
end

-- 添加师徒称号
function tbTraining:AddShituTitle_GS(tbParam)
	if (not tbParam) then
		return;
	end
	local szStudentName = tbParam[1] or "";
	local szTeacherName = tbParam[2] or "";
	local pStudent = KPlayer.GetPlayerByName(szStudentName);
	local pTeacher = KPlayer.GetPlayerByName(szTeacherName);
	local nTitleEndTime = GetTime() + 3600 * 24 * 365 * 10;	-- 称号的有效期上限暂定为10年
	
	-- 为徒弟增加自定义称号
	if (pStudent) then
		local szStudentTitle = szTeacherName .. Lang.gcscript.str43[Lang.Idx];
		pStudent.AddSpeTitle(szStudentTitle, nTitleEndTime, "gold");
		EventManager:WriteLog("获得自定义称号"..szStudentTitle, pStudent);
	end
	
	-- 为师傅增加自定义称号，如果原来已经有其他的师徒称号，替换掉
	if (pTeacher) then
		local szCurTitle, szPlayerName = Relation:FindTitleAndName(Lang.gcscript.str44[Lang.Idx], pTeacher);
		if (szCurTitle and szPlayerName) then
			pTeacher.RemoveSpeTitle(szCurTitle);
		end
		local szTeacherTitle = szStudentName .. Lang.gcscript.str44[Lang.Idx];
		pTeacher.AddSpeTitle(szTeacherTitle, nTitleEndTime, "gold");
		EventManager:WriteLog("获得自定义称号"..szTeacherTitle, pTeacher);
	end
end

function tbTraining:TrainingResponse(nTOrSFlag, szPlayerName, bAnswer)
	if (not nTOrSFlag or not szPlayerName or not bAnswer) then
		return;
	end
	if (0 == bAnswer) then
		self:__TrainingResponse_Refuse(nTOrSFlag, szPlayerName);
	elseif (1 == bAnswer) then
		self:__TrainingResponse_Accept(nTOrSFlag, szPlayerName);
	end
end

function tbTraining:__TrainingResponse_Refuse(nTOrSFlag, szPlayerName)
	local szMsg = "";
	local pPlayer = KPlayer.GetPlayerByName(szPlayerName);
	if (not pPlayer) then
		return;
	end
	
	if (1 == nTOrSFlag) then
		szMsg = string.format(Lang.gcscript.str45[Lang.Idx], me.szName);
	elseif (0 == nTOrSFlag) then
		szMsg = string.format(Lang.gcscript.str46[Lang.Idx], me.szName);
	end
	
	pPlayer.Msg(szMsg);
end

function tbTraining:__TrainingResponse_Accept(nTOrSFlag, szPlayerName)
	if (not nTOrSFlag or not szPlayerName or (nTOrSFlag ~= 0 and nTOrSFlag ~= 1)) then
		return;
	end
	local szTeacherName = me.szName;
	local szStudentName = szPlayerName;
	if (0 == nTOrSFlag) then
		szTeacherName, szStudentName = szStudentName, szTeacherName;
	end
	Relation:AddRelation_GS(szTeacherName, szStudentName, Player.emKPLAYERRELATION_TYPE_TRAINING, 1)
end







end	-- if (MODULE_GAMESERVER) then

Relation:Register(Relation.emKPLAYERRELATION_TYPE_TRAINING, tbTraining)
