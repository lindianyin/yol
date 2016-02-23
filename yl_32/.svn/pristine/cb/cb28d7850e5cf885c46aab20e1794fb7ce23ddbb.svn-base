
-- TODO 待删除


function ExpScene:LoadCfg()
	self.tbCfg = {}
	local tbFileData	= Lib:LoadTabFile("/setting/expscene/expscene.txt")

	for _, tbRow in pairs(tbFileData) do
		local szFilePath = tbRow.PosFile
		
		local tbNpcList
		local tbDoodadList
		local tbCallNpcList
		local tbCallDoodadList
		local tbFlagList
		
		if (MODULE_GAMESERVER) then
			local szNpcListFilePath = string.format("%s/npc.txt",szFilePath)
			local szDoodadFilePath  = string.format("%s/doodad.txt",szFilePath)
			local szCallNpcListPath = string.format("%s/callnpc.txt",szFilePath)
			local szCallDoodadListPath = string.format("%s/calldoodad.txt",szFilePath)
			tbNpcList = self:LoadSceneObjFile(szNpcListFilePath)
			if not tbNpcList then
				local szMsg = string.format("LoadCfg: 读表错误！读取 %s 失败！",szNpcListFilePath)
				print(szMsg)
				Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
			end
			
			tbDoodadList = self:LoadSceneObjFile(szDoodadFilePath)
			if not tbDoodadList then
				local szMsg = string.format("LoadCfg: 读表错误！读取 %s 失败！",szDoodadFilePath)
				print(szMsg)
				Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
			end
			tbCallNpcList = self:_LoadCallNpcOrDoodadFile(szCallNpcListPath)
			tbCallDoodadList = self:_LoadCallNpcOrDoodadFile(szCallDoodadListPath)
		else
			local szFlagListFilePath = string.format("%s/flag.txt",szFilePath)
			tbNpcList = {}
			tbDoodadList = {}
			tbFlagList = self:_LoadFlag(szFlagListFilePath)
		end
		
		
		if tbNpcList and tbDoodadList then
			self.tbCfg[tonumber(tbRow.ExpSceneId)] =
			{
				nTemplateSceneId = tonumber(tbRow.TemplateSceneId),
				szPosFile = tbRow.PosFile,
				--Path = tbRow.Path,
				--NpcFile = tbRow.NpcFile,
				--DoodadFile = tbRow.DoodadFile,
				nType = tonumber(tbRow.Type) or 0,
				nTargetId = tonumber(tbRow.TargetId),
				nProgressId = tonumber(tbRow.ProgressId),
				nX = tonumber(tbRow.PosX) or 1,
				nY = tonumber(tbRow.PosY) or 1,
				nZ = tonumber(tbRow.PosZ) or 1,
				
				tbNpcList = tbNpcList,
				tbDoodadList = tbDoodadList,
				tbCallNpcList = tbCallNpcList,
				tbCallDoodadList = tbCallDoodadList,
				tbFlagList = tbFlagList,
				
				-- 星标数
				nTotalStar = tonumber(tbRow.TotalStar) or 0,
				tbStarTips = Lib:Str2Val(tbRow.StarTips),
				nAwardType = tonumber(tbRow.AwardType) or 0,
			}
		end
	end

end

function ExpScene:LoadOneItem(tbRow)
	local tbOneItem = {}
	local tbPosList = nil
	if tbRow.PosFile and tbRow.PosFile ~= "" then
		tbPosList = self:LoadRandomPosList(tbRow.PosFile)
	end
	
	tbOneItem.szNickName = tbRow.NickName ~= "" and tbRow.NickName or nil 
	tbOneItem.nTemplateId = tonumber(tbRow.TemplateId)
	tbOneItem.nX = tonumber(tbRow.X)
	tbOneItem.nY = tonumber(tbRow.Y)
	tbOneItem.nZ = tonumber(tbRow.Z)
	tbOneItem.nFaceDirection = tonumber(tbRow.FaceDirection)
	tbOneItem.tbPosList = tbPosList
	tbOneItem.nCount = tonumber(tbRow.Count)

	tbOneItem.nRespawnInterval = tbRow.RespawnInterval and tonumber(tbRow.RespawnInterval) or nil --calldoodad.txt
	tbOneItem.bRemoveAfterDialog = tbRow.RemoveAfterDialog and tonumber(tbRow.RemoveAfterDialog) or nil --calldoodad.txt
	tbOneItem.nBattleId = tbRow.Battle and tonumber(tbRow.Battle) or nil --callnpc.txt
	tbOneItem.nRelation = tbRow.Relation and tonumber(tbRow.Relation) or nil --callnpc.txt
	tbOneItem.nNormalAI = tbRow.NormalAI and tonumber(tbRow.NormalAI) or nil --callnpc.txt
	tbOneItem.nRegenerationTime = tbRow.RegenerationTime and tonumber(tbRow.RegenerationTime) or 0
	tbOneItem.nHuman = tonumber(tbRow.bHuman) or 0                      -- 是否是人型
	tbOneItem.nQuintessness = tonumber(tbRow.bQuintessness) or 0        -- 是否是精英
    tbOneItem.nProbability = tonumber(tbRow.Probability)
	--tbOneItem.nProgress = tbRow.Progress and tonumber(tbRow.Progress) or nil --calldoodad.txt
	if (tbRow.Progress and string.len(tbRow.Progress) > 0)	then
		local tb = Lib:SplitStr(tbRow.Progress, ",");
		if tb then
			local tbProgress = {};
			for _, strP in pairs(tb) do
				table.insert(tbProgress, tonumber(strP));
			end;
			tbOneItem.tbProgress = tbProgress;
		end;
	end;		
	return tbOneItem;
end;

function ExpScene:LoadSceneObjFile(szPath)
	assert(szPath)
	local tbCfg = {}
	local tbFileData	= Lib:LoadTabFile(szPath)
	if not tbFileData then
		local szMsg = string.format("LoadTabFile[%s] Failed", szPath)
		print(szMsg)
		Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
		return
	end
	for k, tbRow in pairs(tbFileData) do
		tbCfg[k] = self:LoadOneItem(tbRow);
	end
	return tbCfg
end

function ExpScene:LoadRandomPosList(szPath)
	assert(szPath)
	local tbPos = {}
	local tbFileData	= Lib:LoadTabFile(szPath)
	if not tbFileData then
		local szMsg = string.format("LoadTabFile[%s] Failed", szPath)
		print(szMsg)
		Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
		return;
	end;
	
	local tbCount = {};
	for k, tbRow in ipairs(tbFileData) do
		if(tbRow.PointClass and tbRow.PointClass ~= "") then
			local ClassName = tbRow.PointClass;
			if tbPos[ClassName] == nil then
				tbPos[ClassName] = {}
				tbCount[ClassName] = 1;
			end
			local i = tbCount[ClassName];
			tbPos[ClassName][i] = 
			{
				nX = tonumber(tbRow.X),
				nY = tonumber(tbRow.Y),
				nZ = tonumber(tbRow.Z),
				nFaceDirection =  tonumber(tbRow.FaceDirection),
			}
			tbCount[ClassName] = tbCount[ClassName] + 1;
		else
			tbPos[k] =
			{
				nX = tonumber(tbRow.X),
				nY = tonumber(tbRow.Y),
				nZ = tonumber(tbRow.Z),
				nFaceDirection =  tonumber(tbRow.FaceDirection),
			}
		end
	end
	return tbPos
end

function ExpScene:_LoadCallNpcOrDoodadFile(szPath)
	local tbCallNpcOrDoodad = {}

	local tbData = Lib:LoadTabFile(szPath) or {}
	for i, tbRow in ipairs(tbData) do
		if tbRow.NickName then
			tbCallNpcOrDoodad[tbRow.NickName] = self:LoadOneItem(tbRow);
		end
	end

	return tbCallNpcOrDoodad 
end

function ExpScene:_LoadFlag(szFileName)
	local tbFlagList = {}
	local tbData = Lib:LoadTabFile(szFileName) or {}
	for i, tbRow in ipairs(tbData) do
		local szName = tbRow.Name
		local tbFlag = {}
		tbFlag.nX = tonumber(tbRow.X)
		tbFlag.nY = tonumber(tbRow.Y)
		tbFlag.nZ = tonumber(tbRow.Z)
		tbFlagList[szName] = tbFlag	
	end
	
	return tbFlagList
end

function ExpScene:GetExpSceneInfo(nExpSceneId)
	return self.tbCfg[nExpSceneId]
end

function ExpScene:GetSceneTemplateId()
	--TODO
	local pPlayer = me
	local nExpSceneId = ExpScene:GetCurExpSceneId()
	if pPlayer.IsInDynamicScene() == 1 then
		assert(nExpSceneId)
		local tbExoConfig = ExpScene:GetExpSceneInfo(nExpSceneId)
		if tbExoConfig then
			return tbExoConfig.nTemplateSceneId
		else
			local szMsg = string.format(Lang.expscene.str1[Lang.Idx], nExpSceneId)
			Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
			return
		end
	end
	return pPlayer.dwSceneTemplateId
end

function ExpScene:GetTemplateSceneIdByParam(nType, nTargetId, nProgressId)
	for i, v in pairs(self.tbCfg) do
		if (v.nType == nType and v.nTargetId == nTargetId and v.nProgressId == nProgressId) then
			return v.nTemplateSceneId
		end
	end
end
