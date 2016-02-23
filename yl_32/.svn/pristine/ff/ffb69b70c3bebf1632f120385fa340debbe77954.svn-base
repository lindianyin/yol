-- File       : scene.lua
-- Creator    : simon
-- Date       : 2009-12-18 10:56:16
-- Description: 场景相关

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

Include("/script/scene/scenebase.lua")

Scene.CopySceneCallBack = {}
Scene.tbTrapInfo = {}
Scene.tbClass = {}
Scene.tbScenePosName = {}
Scene.tbTrapInstance = {}

--临时增加,记录场景中的Npc信息，用于重生 yulei
Scene.tbNpcCfg = {}

function Scene:Init()
	self:LoadTrap()
	self:LoadPosName()
	self:LoadTrapInstance()
end

-- 临时增加
function Scene:LoadNpc(dwSceneId, dwTemplateSceneId, szSceneDir)
	if  self.tbNpcCfg[dwTemplateSceneId] ~= nil then  -- 先判断是否已经加载过
		return;
	end

	local szNpcListFilePath = string.format("%s/npc.txt", szSceneDir)
	local tbNpcList = {}
	
	local tbFileData = Lib:LoadTabFile(szNpcListFilePath) or {}
	for k, tbRow in pairs(tbFileData) do
		--tbNpcList[k] = ExpScene:LoadOneItem(tbRow) --用NPC模版ID做索引，方便查找(zzx)
		tbNpcList[tonumber(tbRow.TemplateId)] = ExpScene:LoadOneItem(tbRow);
	end
	--self.tbNpcCfg[szNpcListFilePath] = tbNpcList --为毛不用场景模版ID做索引(zzx)
	self.tbNpcCfg[dwTemplateSceneId] = tbNpcList;
	
	for k, tbNpc in pairs(tbNpcList) do
		if tbNpc.tbPosList then
			for szPos, _ in pairs(tbNpc.tbPosList) do
				self:_CallOneNpc(tbNpc, dwSceneId, dwTemplateSceneId, nil, szPos);
			end
		else
			self:_CallOneNpc(tbNpc, dwSceneId, dwTemplateSceneId);
		end
		
	end
end

function Scene:RegenerateNpc(dwSceneId, dwTemplateSceneId, szSceneDir, dwTemplateId, nX, nY, nZ, nFaceDirection, dwExValue, szExInfo)
	--local szNpcListFilePath = string.format("%s/npc.txt", szSceneDir)
	local tbNpcList = self.tbNpcCfg[dwTemplateSceneId]
	local tbFixPos = {["nX"] = nX, ["nY"] = nY, ["nZ"] = nZ, ["nFaceDirection"] = nFaceDirection, ["szNickName"] = szNickName}
	
	if not tbNpcList then
		tbNpcList = {}
		local tbFileData = Lib:LoadTabFile(szNpcListFilePath) or {}
		
		for k, tbRow in pairs(tbFileData) do
			tbNpcList[tbRow.nTemplateId] = ExpScene:LoadOneItem(tbRow)
		end
		self.tbNpcCfg[dwTemplateSceneId] = tbNpcList
	end
--	print("Scene:RegenerateNpc",dwSceneId, dwTemplateSceneId, szSceneDir, dwTemplateId,nX, nY, nZ, nFaceDirection)
	for k, tbNpc in pairs(tbNpcList) do
		--TODO 临时解决办法 yulei
		if tbNpc.nTemplateId == dwTemplateId then
			local tbCopyNpc = Lib:CopyTBN(tbNpc)
			tbCopyNpc.nCount = 1
			local tbRetNpc = self:_CallOneNpc(tbCopyNpc, dwSceneId, dwTemplateSceneId, tbFixPos, nil, dwExValue, szExInfo);
			self:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szExInfo);
			break;
		end
	end
end

function Scene:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szExInfo)
	Stage:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szExInfo);
	
	Instance:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szExInfo);
	
	MultiInstance:OnRegenerateNpc(dwSceneId, tbRetNpc, dwExValue, szExInfo);
end

function Scene:LoadDoodad(dwSceneId, dwTemplateSceneId, szSceneDir)
	local szDoodadFilePath  = string.format("%s/doodad.txt", szSceneDir)
	local tbDoodadList = {}
	
	local tbFileData = Lib:LoadTabFile(szDoodadFilePath) or {}
	for k, tbRow in pairs(tbFileData) do
		tbDoodadList[k] = ExpScene:LoadOneItem(tbRow)
	end
	
	for k, tbDoodad in pairs(tbDoodadList) do
		self:_CallOneDoodad(tbDoodad, dwSceneId)
	end
end

function Scene:_CallOneNpc(tbNpc, dwSceneId, nTemplateSceneId, tbFixPos, szPointClass, dwExValue, szExInfo)
	local pNpc = nil
	local nNormalAI = 0;
	local nRelation = 0;
	local nBattleSceneId = 0;
	local nEcologyId = 0; 
	local tbRetNpcTable = {};
	if  nTemplateSceneId then 
		--nBattleSceneId = KScene.GetBattleSceneIdByTemplateId(nTemplateSceneId) or 0  -- 默认的战斗场景
		--nEcologyId = KScene.GetEcologyIdByTemplateId(nTemplateSceneId) or 0  -- 通过场景模板Id获得场景的生态环境
	end
	if tbNpc.tbPosList and not tbFixPos then
		-- 随机摆点的npc集合
		--assert(tbNpc.nCount > 0) --测试用，先允许表里填数量0(zzx)
		if tbNpc.nCount == nil or tbNpc.nCount <= 0 then
		    return;
		 end
		
		local tbIdx = 0;
		if szPointClass and szPointClass ~= ""  and tbNpc.tbPosList[szPointClass] ~= nil then
			tbIdx = ExpScene:RandomNumbers(#tbNpc.tbPosList[szPointClass], tbNpc.nCount);
		else
			-- 全部随机
			tbIdx = ExpScene:RandomNumbers(#tbNpc.tbPosList, tbNpc.nCount);
		end;
		
		for i = 1, tbNpc.nCount do
			local nIdx = tbIdx[i]
			local tbPos = 0;
			if szPointClass and szPointClass ~= "" then
			    tbPos = tbNpc.tbPosList[szPointClass][nIdx];
			else
			    tbPos = tbNpc.tbPosList[nIdx]
			end
			local nNpcTemplateId, nTagId = ExpScene:GetNpcTemplateId(tbNpc.nTemplateId, nTemplateSceneId or 0, tbNpc.nHuman, tbNpc.nQuintessness, 1)
		
			pNpc = KGameBase.CallNpc(nNpcTemplateId, dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ, tbPos.nFaceDirection)

			if (pNpc and tbNpc.nNormalAI and (tbNpc.nNormalAI > 0)) then
				pNpc.pAI.SetAIType(tbNpc.nNormalAI)
				nNormalAI = tbNpc.nNormalAI
			end
			
			if pNpc and tbNpc.nRelation then
				pNpc.RawSetRelation(tbNpc.nRelation);
				nRelation = tbNpc.nRelation
			end
			
			--设置重生信息
			if pNpc and tbNpc.nRegenerationTime > 0 then
				KGameBase.SetNpcRegenerationInfo(pNpc.dwId, tbPos.nX, tbPos.nY, tbPos.nZ, tbPos.nFaceDirection, tbNpc.nRegenerationTime, nNormalAI, nRelation, dwExValue, szExInfo);
			end
			--为多个同样的NPC设置不同的NickName(NickName+Number)
			if tbNpc.szNickName and tbNpc.szNickName ~= "" then
				local szNickName = tbNpc.szNickName..tostring(i)
				pNpc.szNickName = szNickName
			end

			if (pNpc) then
				tbRetNpcTable[#tbRetNpcTable + 1] = pNpc
			end
		end
	else
		local nNpcTemplateId, nTagId = ExpScene:GetNpcTemplateId(tbNpc.nTemplateId, nTemplateSceneId or 0, tbNpc.nHuman, tbNpc.nQuintessness, 1)
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
		
		pNpc = KGameBase.CallNpc(nNpcTemplateId, dwSceneId, nX, nY, nZ, nFaceDirection)
		
		if (tbNpc.nNormalAI and (tbNpc.nNormalAI > 0)) then
			pNpc.pAI.SetAIType(tbNpc.nNormalAI)
			nNormalAI = tbNpc.nNormalAI
		end
		
		if tbNpc.nRelation then
			pNpc.RawSetRelation(tbNpc.nRelation)
			nRelation = tbNpc.nRelation
		end
		
		--设置重生信息
		if pNpc and tbNpc.nRegenerationTime > 0 then
			KGameBase.SetNpcRegenerationInfo(pNpc.dwId, nX, nY, nZ, nFaceDirection, tbNpc.nRegenerationTime, nNormalAI, nRelation, dwExValue, szExInfo);
		end
		--设置NickName
		if tbNpc.szNickName and tbNpc.szNickName ~= "" and not tbFixPos then
			pNpc.szNickName = tbNpc.szNickName
		elseif tbFixPos and tbFixPos.szNickName then
			pNpc.szNickName = tbFixPos.szNickName
		end
		
		if (pNpc) then
		    tbRetNpcTable[#tbRetNpcTable + 1] = pNpc
		end
	end
	
	return tbRetNpcTable;
end

 function Scene:_CallOneDoodad(tbDoodad, dwSceneId)
	local pDoodad = nil

	if tbDoodad.tbPosList then
		-- 随机摆点集合
		assert(tbDoodad.nCount > 0)
		
		local tbIdx = ExpScene:RandomNumbers(#tbDoodad.tbPosList, tbDoodad.nCount);
		for i = 1, tbDoodad.nCount do
			local nIdx = tbIdx[i]
			local tbPos = tbDoodad.tbPosList[nIdx]
			pDoodad = KGameBase.CallDoodad(tbDoodad.nTemplateId, dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ)

			--设置Doodad的NickName
			if tbDoodad.szNickName and tbDoodad.szNickName ~= "" then
				local szNickName = tbDoodad.szNickName..tostring(i)
				pDoodad.szNickName = szNickName
			end
			if tbDoodad.tbProgress then
				Doodad:SetBindProgress(pDoodad, tbDoodad.tbProgress)
			end;
		end
	else
		pDoodad = KGameBase.CallDoodad(tbDoodad.nTemplateId, dwSceneId, tbDoodad.nX, tbDoodad.nY, tbDoodad.nZ)

		--设置Doodad的NickName
		if tbDoodad.szNickName and tbDoodad.szNickName ~= "" then
			pDoodad.szNickName = tbDoodad.szNickName
		end
		if tbDoodad.tbProgress then
			Doodad:SetBindProgress(pDoodad, tbDoodad.tbProgress)
		end
	end
end

-- 载入Trap表
function Scene:LoadTrap()
	local tbFileData	= Lib:LoadTabFile("/setting/scene/traptemplate.txt")
	for _, tbRow in pairs(tbFileData) do
		-- 生成地图Trap点
		self.tbTrapInfo[tbRow.TrapClass]	= {
			Type = tbRow.Type,
			nTrapTemplateId = tonumber(tbRow.TrapId),
			Param1 = tbRow.Param1,
			Param2 = tbRow.Param2,
			Param3 = tbRow.Param3,
			Param4 = tbRow.Param4,
		}
		
		self.tbTrapInstance[tbRow.TrapClass] = {
			nMapId = tbRow.MapIn,
			nPosX = tbRow.TrapPointX,
			nPosY = tbRow.TrapPointY,
			nPosZ = tbRow.TrapPointZ,
		}
	end

end

--载入大神宝典副本传送点表
function Scene:LoadTrapInstance()
	local tbFileData	= Lib:LoadTabFile("/setting/scene/trapinstance.txt")
	for _, tbRow in pairs(tbFileData) do
		self.tbTrapInstance[tbRow.MapId] = {
			nPosX = tbRow.PointX,
			nPosY = tbRow.PointY,
			nPosZ = tbRow.PointZ,
		}
	end

end

function Scene:LoadPosName()
	local tbFileData = Lib:LoadTabFile("/setting/scene/posname.txt")
	for _, tbRow in pairs(tbFileData) do
		local nSceneTemplateId = tonumber(tbRow.SceneTemplateId)
		local tbPosName = self.tbScenePosName[nSceneTemplateId] or {}
		self.tbScenePosName[nSceneTemplateId] = tbPosName
		
		tbPosName[tbRow.PosName] = {
			nX = tonumber(tbRow.X),
			nY = tonumber(tbRow.Y),
			nZ = tonumber(tbRow.Z),
			nFaceDirection = tonumber(tbRow.FaceDirection),
		}
	end
end

function Scene:GetPosByName(nSceneTemplateId, szPosName)
	local tbPosName = self.tbScenePosName[nSceneTemplateId]
	if tbPosName then
		return tbPosName[szPosName]
	end
end

function Scene:GetTrapClass(szTrapClass)
	return self.tbSceneBase:GetTrapClass(szTrapClass)
end

function Scene:GetClass(szSceneClass)
	local tbClass	= self.tbClass[szSceneClass]
	if not tbClass then
		-- 新模板从基础模板派生
		tbClass = Lib:NewClass(self.tbSceneBase)
		tbClass.szClass = szSceneClass
		
		tbClass.tbTrapInfo = {}
		tbClass.tbTrapInstance = {}
		
		-- 加入到模板库里面
		self.tbClass[szSceneClass] = tbClass
	end
	return tbClass
end

function Scene:OnTrap(szSceneClass, szScript)
	local tbScene	= self:GetClass(szSceneClass)
	print("OnTrap", me.szName, szSceneClass, szScript)
	--Lib:ShowTB(tbScene)
	tbScene:OnTrapPlayer(szScript)
end

function Scene:OnLeaveTrap(szSceneClass, szScript)
	print("OnLeaveTrap", me.szName, szSceneClass, szScript)
end

-- 申请载入动态场景（副本）
-- tbCallBack结构：
-- {
-- 	func,   -- 回调函数
--	self,    -- 回调表
--	....       -- 用户参数
--   nSceneId -- 回调时会在用户参数后多传一个参数，表示副本Id
-- }
function Scene:ApplyDynamicScene(nTemplateId, nMapOwnerId, tbCallBack)
	if not tbCallBack then
		print("Cannot apply new copy scenes without a callback")
		return 0;
	end
	self.CopySceneCallBack[nTemplateId] = self.CopySceneCallBack[nTemplateId] or {}

	local nParam = #self.CopySceneCallBack[nTemplateId] + 1
	self.CopySceneCallBack[nTemplateId][nParam] = tbCallBack -- register callback

	local szMsg = string.format("ApplyDynamicScene %d, %d", nTemplateId, nParam)
	print(szMsg)
	return KScene.LoadScene(nTemplateId, nMapOwnerId, nParam)
end

function Scene:SetPlayerFightState(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return 
	end
	local nMapId = pPlayer.dwSceneTemplateId;
	local nState = KScene.GetSceneFightState(nMapId);
	local nMode = KScene.GetSceneSetFightMode(nMapId);
	
	pPlayer.SetIsFightMap(nState);
	pPlayer.SetIsSetFightMode(nMode);
end

function Scene:AutoRecoverLife(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	
	if not pPlayer then
		return 
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	
	-- 回城自动满血
	if KScene.GetSceneType(nMapId) == Scene.MAP_TYPE_NORMAL or KScene.GetSceneType(nMapId) == Scene.MAP_TYPE_BIRTH then
		local nMaxLife = pPlayer.GetMaxLife();
		pPlayer.SetCurrentLife(nMaxLife);
		
		local nMaxMana = pPlayer.GetMaxMana();
		pPlayer.SetCurrentMana(nMaxMana);
	end
end

-- 不能变大神的场景
Scene.CANNOT_BLADE_SCENETYPE = {
	"arena",
	"escort",
	"city",
}

function Scene:c2s_CheckRemoveBladeState(pPlayer)
	local nMapId = pPlayer.dwSceneTemplateId;
	local szSceneType = KScene.GetSceneTypeName(nMapId);
	
	for _, szType in ipairs(self.CANNOT_BLADE_SCENETYPE) do
		if szType == szSceneType then
			KGameBase.CheckRemoveBladeState(pPlayer);
			break;
		end
	end
end

function Scene:ResetAnger(dwCharacterId)
	local pPlayer = KGameBase.GetPlayerById(dwCharacterId);
	if not pPlayer then
		return;
	end
	
	local nMapId = pPlayer.dwSceneTemplateId;
	local szSceneType = KScene.GetSceneTypeName(nMapId);
	
	if szSceneType == "city" then
		pPlayer.SetAnger(0);
	end
end

function Scene:OnEnterScene(dwCharacterId)

	self:ResetAnger(dwCharacterId);
	
	self:SetPlayerFightState(dwCharacterId);
	
	self:AutoRecoverLife(dwCharacterId);
	
	--Player:DelayCalcFightScore(dwCharacterId);
	
	CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, dwCharacterId, 0, 0);
end

function Scene:OnLeaveScene(dwCharacterId, bNpc)
	CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_LEAVE_SCENE, dwCharacterId, 0, 0);
end

function Scene:OnLoadScene(nTemplateId, nSceneId, szSceneDir)
	--print("OnLoadScene [MapId]: ", nTemplateId, "[SceneId]:", nSceneId)
	Lib:CallBack{self.LoadNpc, self, nSceneId, nTemplateId, szSceneDir}
	Lib:CallBack{self.LoadNpc, self, nSceneId, nTemplateId, szSceneDir}

	CallCenter:FireEvent(KOBJEVENT.emKOBJEVENTTYPE_SCENE_CREATE, nSceneId, nTemplateId, 0);
end

function Scene:OnUnLoadScene(nTemplateId, nSceneId)
	BufferMgr:OnUnloadScene(nSceneId);
end

function Scene:OnLoadDynamicScene(nTemplateId, nSceneId, nType, nParam)
	--print("OnLoadDynamicScene SceneId["..nSceneId.."] Template ["..nTemplateId.."]")
	
	if self.CopySceneCallBack[nTemplateId] and
	   self.CopySceneCallBack[nTemplateId][nParam] then
	   local tbCallBack = self.CopySceneCallBack[nTemplateId][nParam];
	   table.insert(tbCallBack, nSceneId);
	   Lib:CallBack(tbCallBack);
	   self.CopySceneCallBack[nTemplateId][nParam] = nil;
	end
end

-- 离开当前活动场景
function Scene:c2s_LeaveMission(pPlayer)
	local nMapId = pPlayer.dwSceneTemplateId;
	
	if KScene.GetSceneTypeName(nMapId) == "fb" then
		local szTips = Lang.scene.str1[Lang.Idx];
		if nMapId == PetIsland.nPetIslandMapId then
			szTips = Lang.scene.str2[Lang.Idx];
		end
		--Dialog:_AskForSure(pPlayer.dwId, szTips, ({"Instance:LeaveMyInstance", pPlayer}));
		Instance:LeaveMyInstance(pPlayer);
	elseif KScene.GetSceneTypeName(nMapId) == "escort" then
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ExitEscortConfirm"});
	elseif KScene.GetSceneTypeName(nMapId) == "tower" then
		--Dialog:_AskForSure(pPlayer.dwId, Lang.scene.str3[Lang.Idx], ({"Tower:LeaveGame", pPlayer}));
                Tower:LeaveGame(pPlayer);
	elseif KScene.GetSceneTypeName(nMapId) == "stage" then
		--Dialog:_AskForSure(pPlayer.dwId, Lang.scene.str4[Lang.Idx], ({"Stage:LeaveGame", pPlayer}));
		Stage:LeaveGame(pPlayer);
	elseif KScene.GetSceneTypeName(nMapId) == "multiinstance" then
		--Dialog:_AskForSure(pPlayer.dwId, Lang.scene.str5[Lang.Idx], ({"MultiInstance:LeaveGame", pPlayer}));
		MultiInstance:LeaveGame(pPlayer);
	elseif KScene.GetSceneTypeName(nMapId) == "activity" or KScene.GetSceneTypeName(nMapId) == "activity_zb" then
	        --Dialog:_AskForSure(pPlayer.dwId, Lang.scene.str6[Lang.Idx], ({"Activity:LeaveGame", pPlayer}));
	        Activity:LeaveGame(pPlayer);
	end
end

-- 副本申请 <示范>
function Scene:Test(nTemplateId)
	nTemplateId = nTemplateId or 125
	Scene:ApplyDynamicScene(nTemplateId, {self.TestCallback, self, nTemplateId})
	print("副本申请Test")
end
function Scene:TestCallback(nTemplateId, nSceneId)
	print("TestCallback SceneId["..nSceneId.."] Template ["..nTemplateId.."]")
end

function Scene:c2s_GoToInstance(pPlayer)
	local nMapId = pPlayer.dwSceneTemplateId;
	for key, tbTrap in pairs(self.tbTrapInstance) do
		if tonumber(key) == nMapId then
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:AutoGoTo", tonumber(key), tonumber(tbTrap.nPosX), tonumber(tbTrap.nPosY), tonumber(tbTrap.nPosZ)});
			break;
		end
	end
end

-- -------------------------------------------------------------
Scene:Init();

