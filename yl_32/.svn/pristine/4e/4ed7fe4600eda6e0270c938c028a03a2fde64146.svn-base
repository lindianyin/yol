-- File       : expscene.lua
-- Creator    : simon
-- Date       : 2010-04-01 16:31:43
-- Description: 探索场景管理

if (not MODULE_GAMESERVER) then
	return;
end

Include("/script/expscene/expscene.lua")

ExpScene.nTotalExpScene = 0
ExpScene.nTotalEndExpScene = 0
ExpScene.nCurrentExpScene = 0

--队伍ID和该队伍创建的相关副本ID的数据结构
--nTeamId = {nExpSceneId, dwDySceneId, nTemplateSceneId, nType, szDesc};
function ExpScene:Init()
	self:LoadCfg()

	self.tbDynamicScene = {};		-- 动态副本的信息(探索地图Id，队伍Id，副本水平，副本等级)
end

--self.tbCfg[tonumber(tbRow.ExpSceneId)] =
--{
--	nTemplateSceneId = tonumber(tbRow.TemplateSceneId),
--	nType = tonumber(tbRow.Type) or 0;
--	nTargetId = tonumber(tbRow.nTargetId);
--	tbNpcList = tbNpcList,
--	tbDoodadList = tbDoodadList,
--}
--根据任务ID(或者剧本ID或者活动ID，这个由类型决定)获取对应的ExpSceneId
--todo  这个地方的效率可能会存在风险，可以将cfg表改造为几级结构 以类型，key分为两级，直接用[]来取
-- 同时要考虑探索地图本身的需求，可能可以再构造一张索引表


-- 创建副本
function ExpScene:CreateInstance(nExpSceneId, nTeamLevel, nMaxLevel, tbUserCallBack)
	assert(self.tbCfg[nExpSceneId])
	local tbCfg = self.tbCfg[nExpSceneId]
	local tbCallback = 
	{
		self.OnInstanceCreated, self,
		nExpSceneId,
		nTeamLevel,
		nMaxLevel,
		tbUserCallBack
	}
	Scene:ApplyDynamicScene(1, tbCfg.nTemplateSceneId, tbCallback, 0)
end

function ExpScene:OnInstanceCreated(nExpSceneId, nTeamLevel, nMaxLevel, tbUserCallback, dwSceneId)
	if not self.tbDynamicScene[dwSceneId] then
		self.tbDynamicScene[dwSceneId] = {}
	end
	local tbDynamicScene = self.tbDynamicScene[dwSceneId]
	tbDynamicScene.nExpSceneId = nExpSceneId
	tbDynamicScene.tbNpcList = {}
	tbDynamicScene.tbDoodadList = {}
	
	if tbUserCallback.dwPlayerId then
		local pPlayer = KGameBase.GetPlayerById(tbUserCallback.dwPlayerId);
		--为了新手转职故事做的特殊处理
		if nExpSceneId == 25 then
			tbDynamicScene.dwRecordPlayerId = tbUserCallback.dwPlayerId
			tbDynamicScene.nPlayerFactionId = pPlayer.GetPrimaryFaction()
		end
	end
	local tbCfg = self.tbCfg[nExpSceneId];

	self:CallNpcByList(tbCfg.tbNpcList, dwSceneId, nTeamLevel, nMaxLevel, tbCfg.nTemplateSceneId, nExpSceneId)
	self:CallDoodadByList(tbCfg.tbDoodadList, dwSceneId, nTeamLevel, nMaxLevel, nExpSceneId)
	
	table.insert(tbUserCallback, dwSceneId);
	Lib:CallBack(tbUserCallback);
	
	self.nTotalExpScene = self.nTotalExpScene + 1
	self.nCurrentExpScene = self.nCurrentExpScene + 1
end

-- 返回需要的NPC的templateId,nHuman(人形), nQuintessness(精英)
function ExpScene:GetNpcTemplateId(nNpcTemplateId, nTemplateSceneId, nHuman, nQuintessness, nLevel)
	if nNpcTemplateId > 0 then 
		return nNpcTemplateId, 0
	end
	
	local nNewNpcTemplateId = 0 

	local nNewNpcType = nHuman * 2 + nQuintessness + 1
	local nTagId = nNewNpcType
	local szNpcType = "tbNpcTag"..nNewNpcType
	
	local tbAllowNpc = {}
	local tbAllowNpcTag = {}

	if #tbAllowNpcTag <= 0 then
		return nNewNpcTemplateId, 0
	end
	
	if not Npc.tbTags then
		return nNewNpcTemplateId, 0
	end
	
	local nRand = KUnify.MathRandom(#tbAllowNpcTag)
	local tbOldTagNpc = Npc.tbTags[tbAllowNpcTag[nRand]]
	if not tbOldTagNpc or #tbOldTagNpc <= 0 then
		return nNewNpcTemplateId, 0
	end
	local tbNewTagNpc = {}
	for _, nId in pairs(tbOldTagNpc) do
		local nTemplateId = tonumber(nId)
		if not Npc.tbNpcTemplate or not Npc.tbNpcTemplate[nTemplateId] or    -- 等级条件判断
				(Npc.tbNpcTemplate[nTemplateId].nMinLevel <= nLevel and 
				Npc.tbNpcTemplate[nTemplateId].nMaxLevel >= nLevel) then  
				
				table.insert(tbNewTagNpc, nTemplateId)
		end
	end
	if #tbNewTagNpc <= 0 then
		return nNewNpcTemplateId, 0
	end
	
	nRand = KUnify.MathRandom(#tbNewTagNpc)
	return tonumber(tbNewTagNpc[nRand]), nTagId
end

function ExpScene:CallRandomBoss(dwSceneId, pPlayer, nBossCount)
	local nRandBossCount = 0
	local nRandPosCount = 0
	local tbPoses = nil

	local nExpSceneId = self:GetExpSceneIdByDySceneId(dwSceneId)
	if not nExpSceneId then
		return
	end

	local tbExpSceneInfo = self:GetExpSceneInfo(nExpSceneId)
	if not tbExpSceneInfo then
		return
	end

	local nTemplateSceneId = tbExpSceneInfo.nTemplateSceneId

	tbPoses = Scene.tbRandomPos[nTemplateSceneId]
	if not tbPoses then
		return
	end

	nRandPosCount = #tbPoses
	nRandBossCount = #Scene.tbRandomBoss
	if (nRandPosCount <= 0) or (nRandBossCount <= 0) then
		return
	end
	if nBossCount > nRandBossCount then
		nBossCount = nRandBossCount
	end
	if nBossCount > nRandPosCount then
		nBossCount = nRandPosCount
	end

	local tbBossIdx = self:RandomNumbers(nRandBossCount, nBossCount)
	local tbPosIdx = self:RandomNumbers(nRandPosCount, nBossCount)

	for i = 1, nBossCount do
		local tbNpc = {}
		local tbPos = tbPoses[tbPosIdx[i]]
		local tbBoss = Scene.tbRandomBoss[tbBossIdx[i]]

		tbNpc.nTemplateId = tbBoss.nTemplateId
		tbNpc.nNormalAI = tbBoss.nNormalAI

		tbNpc.nRegenerationTime = 0
		tbNpc.nRelation = 0
		tbNpc.nX = tbPos.nX
		tbNpc.nY = tbPos.nY
		tbNpc.nZ = tbPos.nZ
		tbNpc.nFaceDirection = tbPos.nFaceDirection
		local nTeamId = pPlayer.GetTeamId()
--		local nTeamLevel, nMaxLevel = Team:GetTeamLevel(nTeamId) -- 作废
		local tbCallNpcTable = self:CallOneNpc(tbNpc, dwSceneId, nTeamLevel, nMaxLevel, nil, nExpSceneId)
		--Notify Client
		
		local tbParam = {}
		for _, pNpc in ipairs(tbCallNpcTable) do
			local tbPos = pNpc.GetPosition()
			tbParam[#tbParam + 1] = {
				szName = pNpc.szName,
				nX = tbPos.nX,
				nY = tbPos.nY,
				nZ = tbPos.nZ,
			}
		end
		
		local tbTeamMember = KTeam.GetTeamMemberList(nTeamId) or {}
		if (tbCallNpcTable) then
			for _, nPlayerId in pairs(tbTeamMember) do
				local pTeamMember = KGameBase.GetPlayerById(nPlayerId)
				if pTeamMember then
					pTeamMember.CallClientScript({"Ui.tbLogic.tbMiniMapMgr:OnExpSceneCallBoss", tbParam})
				end
			end
		end
	end
end

--根据npc.txt和callnpc.txt中的一条Npc信息tbNpc来call这个npc
function ExpScene:CallOneNpc(tbNpc, dwSceneId, nLevel, nMaxLevel, nTemplateSceneId, nExpSceneId, tbFixPos)
	local pNpc = nil;
	local tbRetNpcTable = {}
	local nNormalAI = 0;
	local nRelation = 0;

	if tbNpc.tbPosList and not tbFixPos then
		-- 随机摆点的npc集合
		assert(tbNpc.nCount > 0)
		local nCount = tbNpc.nCount
		if nExpSceneId then
			local tbExpSceneInfo = ExpScene:GetExpSceneInfo(nExpSceneId)
			if not tbExpSceneInfo then
				return
			end
		end
		
		if tbNpc.nCount > #tbNpc.tbPosList then
			-- TODO: 是否需要先全刷新一次
		end
		local tbIdx = ExpScene:RandomNumbers(#tbNpc.tbPosList, tbNpc.nCount);
		for i = 1, nCount do
			local nIdx = tbIdx[i];
			local tbPos = tbNpc.tbPosList[nIdx]
			local nNpcTemplateId, nTagId = self:GetNpcTemplateId(tbNpc.nTemplateId, nTemplateSceneId or 0, tbNpc.nHuman, tbNpc.nQuintessness, nLevel)
		
			pNpc = ExpScene:AddNpc(dwSceneId, nNpcTemplateId, tbPos.nX, tbPos.nY, tbPos.nZ, tbPos.nFaceDirection)
			if (pNpc) then
				tbRetNpcTable[#tbRetNpcTable + 1] = pNpc
			end
			if (pNpc and tbNpc.nNormalAI and (tbNpc.nNormalAI > 0)) then
				pNpc.pAI.SetAIType(tbNpc.nNormalAI)
				nNormalAI = tbNpc.nNormalAI
			end
			
			if (pNpc and pNpc.GetRelation() ~= tbNpc.nRelation) then
				pNpc.SetRelation(tbNpc.nRelation);
				--self:NpcRelationChangedNotify(pPlayer, pNpc);
				nRelation = tbNpc.nRelation
			end
			
			--设置重生信息
			if pNpc and tbNpc.nRegenerationTime > 0 then
				KGameBase.SetNpcRegenerationInfo(pNpc.dwId, tbPos.nX, tbPos.nY, tbPos.nZ, tbPos.nFaceDirection, tbNpc.nRegenerationTime, nNormalAI, nRelation)
			end
			--为多个同样的NPC设置不同的NickName(NickName+Number)
			if tbNpc.szNickName and tbNpc.szNickName ~= "" then
				local szNickName = tbNpc.szNickName..tostring(i)
				pNpc.szNickName = szNickName
			end
		end
	else
		local nNpcTemplateId, nTagId = self:GetNpcTemplateId(tbNpc.nTemplateId, nTemplateSceneId or 0, tbNpc.nHuman, tbNpc.nQuintessness, nLevel)
		local nX, nY, nZ, nFaceDirection
		if tbFixPos then
			nX = tbFixPos.nX
			nY = tbFixPos.nY
			nZ = tbFixPos.nZ
			nFaceDirection = tbFixPos.nFaceDirection
		else
			nX = tbNpc.nX
			nY = tbNpc.nY
			nZ = tbNpc.nZ
			nFaceDirection = tbNpc.nFaceDirection
		end
		pNpc = ExpScene:AddNpc(dwSceneId, nNpcTemplateId, nX, nY, nZ, nFaceDirection)

		if (pNpc) then
			tbRetNpcTable[#tbRetNpcTable + 1] = pNpc
		end
		if not pNpc then
			local szMsg = string.format("CallOneNpc:Call Npc[%d] Failed", tbNpc.nTemplateId)
			print(szMsg)
			Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
		end
		
		if (tbNpc.nNormalAI and (tbNpc.nNormalAI > 0)) then
			pNpc.pAI.SetAIType(tbNpc.nNormalAI)
			nNormalAI = tbNpc.nNormalAI
		end
		
		if (pNpc.GetRelation() ~= tbNpc.nRelation) then
			pNpc.SetRelation(tbNpc.nRelation);
			--self:NpcRelationChangedNotify(pPlayer, pNpc);
			nRelation = tbNpc.nRelation
		end
		
		--设置重生信息
		if pNpc and tbNpc.nRegenerationTime > 0 then
			KGameBase.SetNpcRegenerationInfo(pNpc.dwId, nX, nY, nZ, nFaceDirection, tbNpc.nRegenerationTime, nNormalAI, nRelation)
		end
		--设置NickName
		if tbNpc.szNickName and tbNpc.szNickName ~= "" and not tbFixPos then
			pNpc.szNickName = tbNpc.szNickName
		elseif tbFixPos then
			pNpc.szNickName = tbFixPos.szNickName
		end

	end
	return tbRetNpcTable
end

--根据NickName和指定的刷怪点来Call一个Npc
function ExpScene:CallNpcByNickName(dwSceneId, szNickName, nIdx)
	local dwExpSceneId = ExpScene:GetExpSceneIdByDySceneId(dwSceneId)
	local tbNpc = ExpScene.tbCfg[dwExpSceneId].tbCallNpcList[szNickName]
	local pNpc = nil
	local tbPos = {}
	if tbNpc.tbPosList then
		local nIndex = nIdx
		if not nIndex or nIdx == 0 then
			nIndex = KUnify.MathRandom(1, #tbNpc.tbPosList)
		end
		tbPos = tbNpc.tbPosList[nIndex]
	else
		tbPos.nX = tbNpc.nX
		tbPos.nY = tbNpc.nY
		tbPos.nZ = tbNpc.nZ
		tbPos.nFaceDirection = tbNpc.nFaceDirection
	end
	pNpc = ExpScene:AddNpc(dwSceneId, tbNpc.nTemplateId, tbPos.nX, tbPos.nY, tbPos.nZ, tbPos.nFaceDirection)
	if not pNpc then
		Dbg:WriteLogEx(Dbg.LOG_ERROR, "ExpScene", string.format("CallNpcByNickName:Call Npc[%d] Failed", tbNpc.nTemplateId))
	end
	local dwNpcId = pNpc.dwId
	if (tbNpc.nNormalAI and (tbNpc.nNormalAI > 0)) then
		pNpc.pAI.SetAIType(tbNpc.nNormalAI)
	end
	
	if (pNpc.GetRelation() ~= tbNpc.nRelation) then
		pNpc.SetRelation(tbNpc.nRelation);
	end
	
	--[[
	--设置NickName
	if tbNpc.szNickName and tbNpc.szNickName ~= "" then
		pNpc.szNickName = tbNpc.szNickName
	end
	--]]
	
	return dwNpcId, pNpc, tbPos
end

--通过详细的参数来Call一个NPC(dwSceneId, nTemplateId, 坐标, AI, NickName, Relation)
function ExpScene:CallNpcByParam(tbParam)
	local pNpc = ExpScene:AddNpc(tbParam.dwSceneId, tbParam.nTemplateId, tbParam.nX, tbParam.nY, tbParam.nZ, tbParam.nFaceDirection)
	if not pNpc then
		local szMsg = string.format("CallOneNpc:Call Npc[%d] Failed", tbParam.nTemplateId)
		print(szMsg)
		return
	end
	local dwNpcId = pNpc.dwId
	if (tbParam.nNormalAI and (tbParam.nNormalAI > 0)) then
		pNpc.pAI.SetAIType(tbParam.nNormalAI)
	end
	
	if (tbParam.nRelation and pNpc.GetRelation() ~= tbParam.nRelation) then
		pNpc.SetRelation(tbParam.nRelation);
	end
	
	--设置NickName
	if tbParam.szNickName and tbParam.szNickName ~= "" then
		pNpc.szNickName = tbParam.szNickName
	end	

	return pNpc
end

function ExpScene:RegenerateNpc(dwSceneId, szNickName, nX, nY, nZ, nFaceDirection)
	local tbDynamicScene = self.tbDynamicScene[dwSceneId]
	
	local dwExpSceneId = ExpScene:GetExpSceneIdByDySceneId(dwSceneId)
	local tbCfg = self.tbCfg[dwExpSceneId]
	local nTemplateSceneId = tbCfg.nTemplateSceneId
	local tbFixPos = {["nX"] = nX, ["nY"] = nY, ["nZ"] = nZ, ["nFaceDirection"] = nFaceDirection, ["szNickName"] = szNickName}
	local nTeamLevel, nMaxLevel
	nTeamLevel = self:GetDynamicSceneLevel(dwSceneId) or 1
	nMaxLevel = nTeamLevel

	for i, tbNpc in pairs(tbCfg.tbNpcList) do
		if string.find(szNickName, tbNpc.szNickName) == 1 then
			local pNpc = ExpScene:CallOneNpc(tbNpc, dwSceneId, nTeamLevel, nMaxLevel, nTemplateSceneId, dwExpSceneId, tbFixPos)
			return pNpc
		end
	end	
	for i, tbNpc in pairs(tbCfg.tbCallNpcList) do
		if string.find(szNickName, tbNpc.szNickName) == 1 then
			local pNpc = ExpScene:CallOneNpc(tbNpc, dwSceneId, nTeamLevel, nMaxLevel, nTemplateSceneId, dwExpSceneId, tbFixPos)
			return pNpc
		end
	end
end

--同上
 function ExpScene:CallOneDoodad(tbDoodad, dwSceneId, nLevel, nMaxLevel, nExpSceneId)
	local pDoodad = nil;

    --有概率
    if tbDoodad.nProbability then
        local nRamdom = KUnify.MathRandom(100)
        if tbDoodad.nProbability < nRamdom then
            return
        end
    end

	if tbDoodad.tbPosList then
		-- 随机摆点集合
		assert(tbDoodad.nCount > 0)
		local nCount = tbDoodad.nCount
		
		if nExpSceneId and nLevel and nMaxLevel then
			local tbExpSceneInfo = ExpScene:GetExpSceneInfo(nExpSceneId)
			if not tbExpSceneInfo then
				return
			end
		end
		if tbDoodad.nCount > #tbDoodad.tbPosList then
			-- TODO: 是否需要先全刷新一次
		end
		
		local tbIdx = ExpScene:RandomNumbers(#tbDoodad.tbPosList, tbDoodad.nCount);
		for i = 1, nCount do
			local nIdx = tbIdx[i];
			local tbPos = tbDoodad.tbPosList[nIdx]
			pDoodad = ExpScene:AddDoodad(dwSceneId, tbDoodad.nTemplateId, tbPos.nX, tbPos.nY, tbPos.nZ)
			if not pDoodad then
				local szMsg = string.format("CallOneDoodad:Call Random Doodad[%d] Failed", tbDoodad.nTemplateId)
				print(szMsg)
				Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
				return ;
			end

			--设置Doodad的NickName
			if tbDoodad.szNickName and tbDoodad.szNickName ~= "" then
				local szNickName = tbDoodad.szNickName..tostring(i)
				pDoodad.szNickName = szNickName
			end
			if tbDoodad.tbProgress then
				--pDoodad.SetTaskValue(Doodad.PRO_EXPSCENE_PROCESS, tbDoodad.nProgress);
				Doodad:SetBindProgress(pDoodad, tbDoodad.tbProgress);
			end;
		end
	else
		pDoodad = ExpScene:AddDoodad(dwSceneId, tbDoodad.nTemplateId, tbDoodad.nX, tbDoodad.nY, tbDoodad.nZ)
		if not pDoodad then
			local szMsg = string.format("CallOneDoodad: Call Fix Doodad[%d] Failed", tbDoodad.nTemplateId)
			print(szMsg)
			Dbg:WriteLogEx(Dbg.LOG_ERROR,"ExpScene", szMsg)
			return ;
		end

		--设置Doodad的NickName
		if tbDoodad.szNickName and tbDoodad.szNickName ~= "" then
			pDoodad.szNickName = tbDoodad.szNickName
		end
		if tbDoodad.tbProgress then
			--pDoodad.SetTaskValue(Doodad.PRO_EXPSCENE_PROCESS, tbDoodad.nProgress);
			Doodad:SetBindProgress(pDoodad, tbDoodad.tbProgress);
		end;
	end
end

--[[
function ExpScene:CallDoodad(nTemplateId, dwSceneId, nX, nY, nZ, nFaceDirection, nExpSceneId)
	
	if not nExpSceneId then
		nExpSceneId = self:GetExpSceneIdByDySceneId(dwSceneId);
	end;
	
	local pDoodad = KGameBase.CallDoodad(nTemplateId, dwSceneId, nX, nY, nZ, nFaceDirection)
	if pDoodad and nExpSceneId then
		local tbDoodadList = self.m_tbSceneDoodadList[nExpSceneId];
		if not tbDoodadList then
			tbDoodadList = {};
		end;
		table.insert(tbDoodadList, pDoodad.dwId);
		self.m_tbSceneDoodadList[nExpSceneId] = tbDoodadList;
	end;
	return pDoodad;
end;
]]--

function ExpScene:CallNpcByList(tbNpcList, dwSceneId, nLevel, nMaxLevel,  nTemplateSceneId, nExpSceneId)
	for k, tbNpc in pairs(tbNpcList) do
		self:CallOneNpc(tbNpc, dwSceneId, nLevel, nMaxLevel, nTemplateSceneId, nExpSceneId)
	end
end

function ExpScene:CallDoodadByList(tbDoodadList, dwSceneId, nLevel, nMaxLevel, nExpSceneId)
	for k, tbDoodad in pairs(tbDoodadList) do
		self:CallOneDoodad(tbDoodad, dwSceneId, nLevel, nMaxLevel, nExpSceneId)
	end
end

function ExpScene:NpcRelationChangedNotify(pPlayer, pNpc)
	pPlayer.CallClientScript({"Npc.NpcMgr:OnServerNotify", unpack({Npc.PNC_NOTIFY.RELATIONCHANGED,pNpc.dwId})});
end

--随机出多少不重复的数字, 暂时放在这
function ExpScene:RandomNumbers(nMaxRandom, nNumCount)
	local tbRandoms = {};
	--判断一下如果nMaxRandom < nNumCount
	if (nMaxRandom <= nNumCount) then
		for i = 1, nMaxRandom do
			table.insert(tbRandoms, i);
		end;
		return tbRandoms;
	end;
	
	assert(nMaxRandom > nNumCount);
		
	local nRandom = KUnify.MathRandom(nMaxRandom);
	table.insert(tbRandoms, nRandom);

	while 1 do
		if (#tbRandoms >= nNumCount) then
			break;
		end;

		local nRand = KUnify.MathRandom(nMaxRandom);
		local bSameRand = 0;
		for _, nRandom in ipairs(tbRandoms) do
			if (nRandom == nRand) then
				bSameRand = 1;
				break;
			end;
		end;

		if (bSameRand == 0) then
			table.insert(tbRandoms, nRand);
		end;
	end;
	return tbRandoms;
end

