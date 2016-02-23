function TaskCond:IsFinished(nTaskId, nTaskIdx)
	if (Task:GetFinishedIdx(nTaskId) >= nTaskIdx) then
		return 1
	end
	local szFailDesc = "";
	szFailDesc = string.format(Lang.task.str8[Lang.Idx],self.tbReferDatas[nTaskIdx].szName);
	return nil, szFailDesc;
end

function TaskCond:IsLevelAE(nLevel)
	if (me.GetLevel() >= nLevel) then
		return 1
	end
	local szFailDesc = "";
	szFailDesc = string.format(Lang.task.str9[Lang.Idx],nLevel);
	return nil, szFailDesc;
end

function TaskCond:IsLevelMax(nLevel)
	if (me.nLevel <= nLevel) then
		return 1;
	end
	local szFailDesc = "";
	szFailDesc = string.format(Lang.task.str10[Lang.Idx],nLevel);
	return nil, szFailDesc;
end

-- 注意可以有门派无关，无门无派是0
function TaskCond:IsFaction(nFaction)
	if (me.nFaction == nFaction) then
		return 1
	end
	return nil, Lang.task.str11[Lang.Idx]
end

function TaskCond:NotThisFaction(nFaction)
	if (me.nFaction ~= nFaction) then
		return 1;
	end
	
	return nil, Lang.task.str12[Lang.Idx];
end

function TaskCond:IsFactionMember(szFailDesc)
	if (me.nFaction > 0) then
		return 1;
	end
	return nil, szFailDesc;
end

function TaskCond:HaveMoney(nValue)
	if (me.nCashMoney >= nValue) then
		return 1
	end
	local szFailDesc = "";
	szFailDesc = string.format(Lang.task.str13[Lang.Idx],nValue);
	return nil, szFailDesc;
end

function TaskCond:HaveSkill(nSkillId)
	if (me.GetSkillLevel(nSkillId) >= 0) then
		return 1
	end
	return nil, Lang.task.str14[Lang.Idx]
end

function TaskCond:IsPkAE(nValue)
	if (me.nPKValue >= nValue) then
		return 1;
	end
	local szFailDesc = "";
	szFailDesc = string.format(Lang.task.str15[Lang.Idx],nValue);
	return nil, szFailDesc;
end

function TaskCond:IsAtPos(nMapId, nPosX, nPosY, nPosZ, nR)
	local tbpos = me.GetPosition();
	if (tbpos == nil) then
		print("TODO: Task Logout未清理！")
		return;
	end
	local nMyMapId 	= me.dwSceneTemplateId;
	local nMyPosX	= tbpos.nX;
	local nMyPosY	= tbpos.nY;
	if (nMapId == nMyMapId or nMapId == 0) then
		if (nPosX == 0) then
			return 1;
		else
			local nMyR	= (nPosX - nMyPosX)^2 + (nPosY - nMyPosY)^2;
			if (nMyR < nR^2) then
				return 1;
			end;
		end;
	end;
	return nil, Lang.task.str16[Lang.Idx];
end;

function TaskCond:IsNpcAtPos(nNpcId, nMapId, nPosX, nPosY, nPosZ, nR)
	if (nNpcId and nNpcId > 0) then
		local pNpc = KGameBase.GetNpcById(nNpcId);
		local tbPos = pNpc.GetPosition();
		
		if (pNpc.dwSceneTemplateId == nMapId) then
			if (nPosX == 0) then
				return 1;
			else
				local nMyR	= (nPosX - tbPos.nX)^2 + (nPosY - tbPos.nY)^2;
				if (nMyR < nR^2) then
					return 1;
				end;
			end;
		end
	else
		return nil, Lang.task.str17[Lang.Idx];
	end
	
	return nil, Lang.task.str18[Lang.Idx];
end;

function TaskCond:IsMeAtPos(nNpcId, nMapId, nPosX, nPosY, nPosZ, nR)
	if (nNpcId and nNpcId > 0) then
		--local pNpc = KGameBase.GetNpcById(nNpcId);
		local tbPos = me.GetPosition();
		
		if (me.dwSceneTemplateId == nMapId) then
			if (nPosX == 0) then
				return 1;
			else
				local nMyR	= (nPosX - tbPos.nX)^2 + (nPosY - tbPos.nY)^2;
				if (nMyR < nR^2) then
					return 1;
				end;
			end;
		end
	else
		return nil, Lang.task.str19[Lang.Idx];
	end
	
	return nil, Lang.task.str18[Lang.Idx];
end;

function TaskCond:IsReputeAE(nCamp, nClass, nLevel, nValue)
	local nMyLevel	= me.GetReputeLevel(nCamp, nClass);
	local nMyValue	= me.GetReputeValue(nCamp, nClass);
	if (nMyLevel and nMyValue) then
		if (nMyLevel > nLevel) then
			return 1;
		elseif (nMyLevel == nLevel and nMyValue >= nValue) then
			return 1;
		end;
	end;
	return nil, Lang.task.str20[Lang.Idx];
end;

function TaskCond:HaveTitleAE(byTitleGenre, byTitleDetailType, byTitleLevel)
	local tbTitles	= me.GetAllTitle(nCamp, nClass);
	for _, tbTitle in ipairs(tbTitles) do
		if (tbTitle.byTitleGenre == byTitleGenre and
			tbTitle.byTitleDetailType == byTitleDetailType and
			tbTitle.byTitleLevel >= byTitleLevel) then
			return 1;
		end;
	end;
	return nil, Lang.task.str21[Lang.Idx];
end;


function TaskCond:HaveTitle(byTitleGenre, byTitleDetailType, byTitleLevel, dwTitleParam)
	local tbTitles	= me.GetAllTitle(nCamp, nClass);
	for _, tbTitle in ipairs(tbTitles) do
		if (tbTitle.byTitleGenre == byTitleGenre and
			tbTitle.byTitleDetailType == byTitleDetailType and
			tbTitle.byTitleLevel == byTitleLevel and
			tbTitle.dwTitleParam == dwTitleParam) then
			return 1;
		end;
	end;
	return nil, Lang.task.str22[Lang.Idx];
end;


function TaskCond:HaveBagSpace(nNeedSpace)
	local nFreeCell = me.pItem.CountFreeBagCell();
	if (not nNeedSpace) then
		nNeedSpace = 1;
	end
	if (nFreeCell >= nNeedSpace) then
		return 1;
	end
	
	return nil, Lang.task.str23[Lang.Idx];
end;


function TaskCond:IsRefFinished(nRefSubId)
	local tbNeedReferData = Task.tbReferDatas[nRefSubId];	-- 需要判断的引用子任务数据
	local nNeedRefIdx = tbNeedReferData.nReferIdx; 			-- 需要完成的引用子任务索引号
	local nTaskId	  	= tbNeedReferData.nTaskId;				-- 此子任务所属的任务
	
	
	local nCurReferId = Task:GetFinishedRefer(nTaskId)
	if (nCurReferId > 0) then
		local nCurRefIdx = Task.tbReferDatas[nCurReferId].nReferIdx;	
		if (nCurRefIdx >= nNeedRefIdx) then
			return 1;
		end
	end
	
	return nil, Lang.task.str24[Lang.Idx];
end;

function TaskCond:NeedSex(nNeedMale)
	if (me.GetSex() == nNeedMale) then
		return 1;
	end

	return nil, Lang.task.str25[Lang.Idx];
end;

function TaskCond:HasBlueEquip()
	for i = 0, Item.EQUIPPOS_NUM do
		pItem = me.GetEquip(i);
		if (pItem) then
			if (pItem.nGenre == Item.EQUIP_GENERAL) and (pItem.IsWhite() ~= 1) then
				return 1;
			end
		end
	end
	
	return nil, Lang.task.str26[Lang.Idx];
end

function TaskCond:HaveItem(tbItem)
	if (Task:GetItemCount(me, tbItem) >= 1) then
		return 1;
	end
	
	return nil, Lang.task.str27[Lang.Idx];
end

function TaskCond:HaveBitItem(tbItem, nCount)	
	assert(nCount >= 1);
	if (Task:GetItemCount(me, tbItem) >= nCount) then
		return 1;
	end
	local szItemName = KItem.GetNameById(tbItem[1], tbItem[2], tbItem[3], tbItem[4]);
	
	return nil, string.format( Lang.task.str28[Lang.Idx],nCount,szItemName);
end

function TaskCond:RequireTaskValue(nGroupId, nTaskId, nValue, szDesc)
	assert(nGroupId > 0 and nTaskId > 0);

  print("TaskCond:RequireTaskValue", nGroupId, nTaskId, nValue, szDesc, me.pTask.GetTask(nGroupId, nTaskId))
	if (me.pTask.GetTask(nGroupId, nTaskId) == nValue) then
		return 1;
	end	
	
	return nil, szDesc;
end

function TaskCond:HaveRoute(szDesc)
	if (me.nRouteId > 0) then
		return 1;
	end
	
	return nil, szDesc;
end

function TaskCond:CanAddCountItemIntoBag(tbItem, nCount)
	if (nCount <= 0) then
		return 1;
	end
	
	local tbItems = {};	
	for i = 1, nCount do
		tbItems[#tbItems + 1] = tbItem;
	end
	
	return self:CanAddItemsIntoBag(tbItems);
end


function TaskCond:CanAddItemsIntoBag(tbItems)

	local tbDesItems = {};

	for _, tbItem in ipairs(tbItems) do
		local tbBaseProp = KItem.GetItemBaseProp(tbItem[1], tbItem[2], tbItem[3], tbItem[4]);
		if tbBaseProp then
			local tbDes =
			{
				nGenre		= tbItem[1],
				nDetail		= tbItem[2],
				nParticular	= tbItem[3],
				nLevel		= tbItem[4],
				--nSeries		= (tbBaseProp.nSeries > 0) and tbBaseProp.nSeries or tbItem[5],
				bBind		= KItem.IsItemBindByBindType(tbBaseProp.nBindType),
				nCount 		= 1;
			};
			table.insert(tbDesItems, tbDes);
		end
	end

	if (me.pItem.CanAddItemIntoBag(unpack(tbDesItems)) == 1) then
			return 1;
	end
		
	return nil,  Lang.task.str29[Lang.Idx];

end

function TaskCond:IsKinReputeAE(nRepute)
	if MODULE_GAMECLIENT then
		return 1;
	end
	local szFailDesc = "";
	if (me.nPrestige >= nRepute) then
		return 1
	end
	
	szFailDesc = string.format( Lang.task.str30[Lang.Idx],nRepute);
	return nil, szFailDesc;
end

function TaskCond:TaskValueLessThen(nGroupId, nTaskId, nTaskValue, szErrorDesc)
	if (me.pTask.GetTask(nGroupId, nTaskId, nTaskValue) < nTaskValue) then
		return 1;
	end
	
	return nil, szErrorDesc;
end


function TaskCond:RequireTaskValueBit(nGroupId, nTaskId, nBitNum, bBit, szErrorDesc)
	local nValue = me.pTask.GetTask(nGroupId, nTaskId);
	assert(nBitNum <= 16 and nBitNum >= 1);
	
	local nBit = KLib.GetBit(nValue, nBitNum);
	if ((nBit == 1 and bBit) or (nBit == 0) and not bBit) then
		return 1;
	end
	
	return nil, szErrorDesc;
end

function TaskCond:RequireTime(nStartTime, nEndTime, szErrorDesc)
	local nDate = tonumber(os.date("%Y%m%d", GetTime()));
	if (nStartTime <= nDate and nDate <= nEndTime) then
		return 1;
	end;
	return nil, szErrorDesc;
end;

function TaskCond:RequirRepute(nValue, szErrorDesc)
	if (me.nPrestige < nValue) then
		return nil, szErrorDesc;
	end;
	return 1;
end;

function TaskCond:RequirScript(varFunc, nTure, szErrorDesc)
	local fnFunc, tbSelf	= KLib.GetValByStr(varFunc);
	if not fnFunc then
		return 1;
	end
	if fnFunc(tbSelf) == tonumber(nTure) then
		return 1
	end
end;

function TaskCond:IsKinRegularMember()
	local nKinId = me.dwKinId;
	if (nKinId and nKinId > 0) then
		local cKin = KKin.GetKin(nKinId);
		if (cKin) then
			local _, nMemberId = self.GetKinMember();
			local cMember = cKin.GetMember(nMemberId);
			if (cMember) then
				local nFigure = cMember.GetFigure();
				if (nFigure == Kin.FIGURE_CAPTAIN or
					nFigure == Kin.FIGURE_ASSISTANT or
					nFigure == Kin.FIGURE_REGULAR ) then
					return 1;
				end
			end
		end
	end
	return nil,  Lang.task.str31[Lang.Idx]
end

-- 是否和另一个异性组队
function TaskCond:IsInTeamWithOtherSex()
	if (MODULE_GAMESERVER) then
		local tblMemberList, nMemberCount = KTeam.GetTeamMemberList(me.GetTeamId())
		if (2 ~= nMemberCount) then
			return nil,  Lang.task.str32[Lang.Idx];
		end
		if (tblMemberList[1].GetSex() == tblMemberList[2].GetSex()) then
			return nil,  Lang.task.str33[Lang.Idx];
		end
		return 1;
	elseif (MODULE_GAMECLIENT) then
		local tbInfo = me.GetTeamMemberInfo();
		if (1 ~= #tbInfo) then
			return nil,  Lang.task.str34[Lang.Idx];
		end
		if (tbInfo[1].GetSex() == me.GetSex()) then
			return nil,  Lang.task.str35[Lang.Idx];
		end
		return 1;
	end
	return nil, "";
end
