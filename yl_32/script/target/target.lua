
function Target:DoTarget(pPlayer, nDay, nId)
	assert(pPlayer);
	
	if not nId then
		nId = 0;
	end
	
	pPlayer.pTarget.DoTarget(nDay, nId);
end

function Target:GetEquipNumByLevel(pPlayer, nLevel)
	local nEquipNum = Item:GetEquipNumByLevel(pPlayer, nLevel);
	return nEquipNum;
end

function Target:GetKillPlayerCount(pPlayer, param)
	local nKillCount = pPlayer.pTask.GetTask(Player.KILL_COUNT_GID, Player.KILL_PLAYER_COUNT_ID) or 0;
	return nKillCount;
end

function Target:GetArtifactNumByLevel(pPlayer, nLevel)
	local nCount = pPlayer.pArtifact.GetArtifactNumByLevel(nLevel) or 0;
	return nCount;
end

function Target:GetStoneNumByLevel(pPlayer, nLevel)
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

function Target:GetPetNumByLevel(pPlayer,nLevel)
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

function Target:GetPetNumByQuality(pPlayer, nQuality)
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

function Target:GetPetNumByRankLevel(pPlayer, nLevel)
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

function Target:GetRideRankLevel(pPlayer, nLevel)
	local pRide = KPet.GetRide(pPlayer);
	if not pRide then
		return 0;
	end

	if pRide.nLevel < nLevel then
		return 0;
	end
	
	return 1;
end

function Target:GetMultiMapPassed(pPlayer, nMapId)
	local nMapPassed = KInstance.IsMultiMapPassed(pPlayer, nMapId) or 0;
	return nMapPassed;
end

function Target:ProcessTarget()

	local curDegree = 0;
	local tbAllPlayer = KGameBase.GetAllPlayers();
	for _, pPlayer in pairs(tbAllPlayer) do
		local tbTarget = {
			[1]={
				--   功能函数                                 参数类型  参数                目标数量   条件类型(0 : == targetNum, 1: >= targetNum, -1: <= targetNum)
				[1]={pPlayer.GetLevel, 							0, 		"", 				40, 		0}, 
				[2]={Player.tbDegree.GetReduceDegree, 			1, 		"ArenaChallenge", 	5, 			1},
				[3]={Player.tbDegree.GetReduceDegree, 			1, 		"EscortAttack", 	5, 			1},
				[4]={Target.GetEquipNumByLevel, 				2, 		2, 					6, 			1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				20, 		0},
				[6]={Target.GetKillPlayerCount, 				2, 		"", 				1, 			1},
				},
			[2]={
				[1]={Target.GetArtifactNumByLevel, 				2, 		1, 					1, 			1},
				[2]={Target.GetMultiMapPassed, 					2, 		521, 				1, 			0},
				[3]={Target.GetMultiMapPassed, 					2, 		318, 				1, 			0},
				[4]={Target.GetEquipNumByLevel, 				2, 		2, 					10, 		1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				30, 		0},
				[6]={pPlayer.GetFightScore, 					0, 		"", 				20000, 		1},
				},
			[3]={
				[1]={Target.GetRideRankLevel, 					2, 		3, 					1, 			1},
				[2]={Target.GetPetNumByQuality, 				2, 		3, 					1, 			1},
				[3]={pPlayer.GetArenaRank, 						0, 		"", 				300, 		-1},
				[4]={Target.GetEquipNumByLevel, 				2, 		3, 					2, 			1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				35, 		0},
				[6]={pPlayer.GetFightScore, 					0, 		"", 				25000, 		1},
				},
			[4]={
				[1]={Target.GetArtifactNumByLevel, 				2, 		1, 					2, 			1},
				[2]={Target.GetMultiMapPassed, 					2, 		520, 				1, 			0},
				[3]={Target.GetMultiMapPassed, 					2, 		316, 				1, 			0},
				[4]={Target.GetEquipNumByLevel, 				2, 		3, 					5, 			1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				40, 		0},
				[6]={pPlayer.GetFightScore, 					0, 		"", 				30000, 		1},
			},
			[5]={
				[1]={Target.GetStoneNumByLevel, 				2, 		1, 					4, 			1},
				[2]={Target.GetPetNumByLevel, 					2, 		40, 				1, 			1},
				[3]={pPlayer.GetArenaRank, 						0, 		"", 				200, 		-1},
				[4]={Target.GetEquipNumByLevel, 				2, 		3, 					7, 			1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				42, 		0},
				[6]={pPlayer.GetFightScore, 					0, 		"", 				35000, 		1},
			},
			[6]={
				[1]={Target.GetPetNumByQuality, 				2, 		3, 					3, 			1},
				[2]={Target.GetArtifactNumByLevel, 				2, 		2, 					1, 			1},
				[3]={Target.GetPetNumByRankLevel, 				2, 		3, 					1, 			1},
				[4]={Target.GetEquipNumByLevel, 				2, 		3, 					10, 		1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				45, 		0},
				[6]={pPlayer.GetFightScore, 					0,		"", 				40000, 		1},
			},
			[7]={
				[1]={Target.GetPetNumByQuality, 				2, 		4, 					1, 			1},
				[2]={Target.GetRideRankLevel, 					2, 		4, 					1, 			1},
				[3]={Target.GetStoneNumByLevel, 				2, 		5, 					1, 			1},
				[4]={Target.GetKillPlayerCount, 				2, 		"", 				15, 		1},
				[5]={pPlayer.GetTowerLayer, 					0, 		"", 				50, 		0},
				[6]={pPlayer.GetFightScore, 					0, 		"", 				45000, 		1},
			},
		};
		
		for i=1,#tbTarget,1 do
			for j=1,#tbTarget[i],1 do
				curDegree = 0;
				if tbTarget[i][j][2] == 0 then
					curDegree = tbTarget[i][j][1]();
				elseif tbTarget[i][j][2] == 1 then
					curDegree = tbTarget[i][j][1]( Player.tbDegree, pPlayer, tbTarget[i][j][3]);
				elseif tbTarget[i][j][2] == 2 then
					curDegree = tbTarget[i][j][1](Target, pPlayer, tbTarget[i][j][3]);
				end
				
				if (tbTarget[i][j][5] == 0 and curDegree == tbTarget[i][j][4]) or
				   (tbTarget[i][j][5] == 1 and curDegree >= tbTarget[i][j][4]) or
				   (tbTarget[i][j][5] == -1 and curDegree <= tbTarget[i][j][4]) then
					Target:DoTarget(pPlayer, i, j);
				end
			end
		end
	end
end

Timer:Register(Env.GAME_FPS * 5, Target.ProcessTarget, Target);