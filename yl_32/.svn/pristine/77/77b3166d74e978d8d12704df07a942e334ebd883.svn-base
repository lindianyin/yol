function Opense:DoOpense(pPlayer, nDay, nId)
	assert(pPlayer);
	
	if not nId then
		nId = 0;
	end
	
	pPlayer.pOpense.DoOpense(nDay, nId);
end

function Opense:GetEquipNumByLevel(pPlayer, nLevel)
	local nEquipNum = Item:GetEquipNumByLevel(pPlayer, nLevel);
	return nEquipNum;
end

function Opense:GetKillPlayerCount(pPlayer, param)
	local nKillCount = pPlayer.pTask.GetTask(Player.KILL_COUNT_GID, Player.KILL_PLAYER_COUNT_ID) or 0;
	return nKillCount;
end

function Opense:GetArtifactNumByLevel(pPlayer, nLevel)
	local nCount = pPlayer.pArtifact.GetArtifactNumByLevel(nLevel) or 0;
	return nCount;
end

function Opense:GetStoneNumByLevel(pPlayer, nLevel)
	local nStoneCount = 0;
	for i = Item.EQUIP_SWORD, Item.EQUIP_FOOT do
		local pEquip = pPlayer.pItem.GetEquipByDetail(i);
		if pEquip then
			for i = 1, 3 do
				if pEquip.HasStone(i) >= nLevel then
					nStoneCount = nStoneCount + 1;
				end
			end
		end
	end
	
	return nStoneCount;
end

function Opense:GetPetNumByLevel(pPlayer,nLevel)
	local nCount = 0;
	local tbPets = KPet.GetPlayerPetList(pPlayer);
	
	for _, nPetId in pairs(tbPets) do
		local pPet = KPet.GetPetById(nPetId);
		if pPet.nLevel >= nLevel then
			nCount = nCount + 1;
		end
	end
	
	return nCount;
end

function Opense:GetPetNumByQuality(pPlayer, nQuality)
	local nCount = 0;
	local tbPets = KPet.GetPlayerPetList(pPlayer);
	
	for _, nPetId in pairs(tbPets) do
		local pPet = KPet.GetPetById(nPetId);
		if pPet.nQuality >= nQuality then
			nCount = nCount + 1;
		end
	end
	
	return nCount;
end

function Opense:GetPetNumByRankLevel(pPlayer, nLevel)
	local nCount = 0;
	local tbPets = KPet.GetPlayerPetList(pPlayer);
	
	for _, nPetId in pairs(tbPets) do
		local pPet = KPet.GetPetById(nPetId);
		if pPet.nRankLevel >= nLevel then
			nCount = nCount + 1;
		end
	end
	
	return nCount;
end

function Opense:GetRideRankLevel(pPlayer, nLevel)
	local pRide = KPet.GetRide(pPlayer);
	if not pRide then
		return 0;
	end

	if pRide.nLevel < nLevel then
		return 0;
	end
	
	return 1;
end

function Opense:GetMultiMapPassed(pPlayer, nMapId)
	local nMapPassed = KInstance.IsMultiMapPassed(pPlayer, nMapId) or 0;
	return nMapPassed;
end

function Opense:GetPetMaxPower(pPlayer)
	local tbPets = KPet.GetPlayerPetMaxPower(pPlayer);
	return tbPets;
end
function Opense:ProcessOpense()

	local curDegree = 0;
	local tbAllPlayer = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbAllPlayer) do
		local tbOpense = {
			[1]={
				--   功能函数                                 参数类型  参数                目标数量   条件类型(0 : == targetNum, 1: >= targetNum, -1: <= targetNum)
				[1]={pPlayer.GetLevel, 							0, 		"", 				42, 		1}, 
				[2]={pPlayer.GetLevel, 							0, 		"", 				45, 		1}, 
				},
			[2]={
				[1]={Opense.GetRideRankLevel, 					2, 		3, 					1, 			1}, 
				[2]={Opense.GetRideRankLevel, 					2, 		4, 					1, 			1}, 
				},
			[3]={
				[1]={pPlayer.GetArenaRank, 						0, 		"", 				200, 		-1},
				[2]={pPlayer.GetArenaRank, 						0, 		"", 				100, 		-1},
				},
			[4]={
				[1]={Opense.GetPetMaxPower, 					2, 		"", 				5000,		1},
				[2]={Opense.GetPetMaxPower, 					2, 		"", 				8000,		1},
			},
			[5]={
				[1]={pPlayer.GetEightTrigramLevelTaiChi,		0, 		"", 				5, 			0}, 
				[2]={pPlayer.GetEightTrigramLevelTaiChi, 		0, 		"", 				8, 			0}, 
			},
			[6]={
				[1]={pPlayer.GetTowerLayer, 					0, 		"", 				20,			0},
				[2]={pPlayer.GetTowerLayer, 					0, 		"", 				30,			0},
			},
			[7]={
				[1]={pPlayer.GetFightScore, 					0, 		"", 				20000, 		-1}, 
				[2]={pPlayer.GetFightScore, 					0, 		"", 				30000, 		-1}, 
			},
		};
		
		for i=1,#tbOpense,1 do
			for j=1,#tbOpense[i],1 do
				curDegree = 0;
				if tbOpense[i][j][2] == 0 then
					curDegree = tbOpense[i][j][1]();
				elseif tbOpense[i][j][2] == 1 then
					curDegree = tbOpense[i][j][1]( Player.tbDegree, pPlayer, tbOpense[i][j][3]);
				elseif tbOpense[i][j][2] == 2 then
					curDegree = tbOpense[i][j][1](Opense, pPlayer, tbOpense[i][j][3]);
				end
				
				if (tbOpense[i][j][5] == 0 and curDegree == tbOpense[i][j][4]) or
				   (tbOpense[i][j][5] == 1 and curDegree >= tbOpense[i][j][4]) or
				   (tbOpense[i][j][5] == -1 and curDegree <= tbOpense[i][j][4]) then
					Opense:DoOpense(pPlayer, i, j);
				end
			end
		end
	end
end

Timer:Register(Env.GAME_FPS * 5, Opense.ProcessOpense, Opense);