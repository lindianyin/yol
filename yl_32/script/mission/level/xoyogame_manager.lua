
XoyoGame.tbXoyoGames = {};
XoyoGame.tbPlayerGames = {};
XoyoGame.tbPlayerLeavePos = {};  --玩家离开位置

function XoyoGame:ClearGameInfo(nTeamId)
	if nTeamId ~= nil then
		print("Clear Team XoyoGame Info!");
		XoyoGame.tbXoyoGames[nTeamId] = nil;
	end
end

function XoyoGame:TestEnterRoom(pPlayer, nRoomId, nDifficulty)
	--需要先组队
	local pGame = nil;
	
	local nTeamId = pPlayer.GetTeamId();
	assert(nTeamId > 0);
	assert(nDifficulty >= 1 and nDifficulty <= 3);
	
	self.tbPlayerLeavePos[pPlayer.dwId] = pPlayer.GetPosition();
	pPlayer.SetLevelDifficuty(nDifficulty); -- 设置难度
	
	if self.tbXoyoGames[nTeamId] == nil then
		pGame = Lib:NewClass(self.BaseGame);
		pGame:InitGame(XoyoGame.TB_MAPID_LIST, nTeamId);
		self.tbXoyoGames[nTeamId] = pGame;
	end
		
	pGame = self.tbXoyoGames[nTeamId];
	pGame:JoinNextGame(pPlayer);
	
	pGame:TestEnterGame(nRoomId);
end

--?gs XoyoGame:TryEnterGame({me.dwId}, 1)
function XoyoGame:TryEnterGame(tbPlayerIds, nDifficulty)
	assert(type(tbPlayerIds) == "table" and #tbPlayerIds > 0);
	
	-- 如果没队伍先组个队
	if #tbPlayerIds == 1 then
		local pPlayer = KGameBase.GetPlayerById(tbPlayerIds[1]);
		
		if pPlayer then
			if Player.tbDegree:GetDegree(pPlayer, "XoyoGame") <= 0 then
				assert(false);
			end
			
			if pPlayer.GetTeamId() == 0 then
				KTeam.CreateTeam(pPlayer.dwId);
			end
		end
	end

	-- 先传送到准备场
	for _, nPlayerId in pairs(tbPlayerIds) do
		local pPlayer = KGameBase.GetPlayerById(nPlayerId);
		if pPlayer then
			-- 进关卡前下马
			Pet:GetDownRide(pPlayer);
			
			self.tbPlayerLeavePos[pPlayer.dwId] = pPlayer.GetPosition();
			pPlayer.SetLevelDifficuty(nDifficulty); -- 设置难度
			
			-- 消耗次数
			Player.tbDegree:ReduceDegree(pPlayer, "XoyoGame", 1);
	
			-- 传送到准备场
			local tbPos = XoyoGame.GAME_IN_POS;
			pPlayer.TeleportTo(tbPos.nMapId, tbPos.nX, tbPos.nY, tbPos.nZ);
		end
	end
end

function XoyoGame:c2s_TryEnterDTTGame(pPlayer)
	self:TryEnterDTTGame(pPlayer);
end

function XoyoGame:c2s_TryEndDTTGame(pPlayer)
	if (pPlayer ~= nil) then
		local nTeamId = pPlayer.GetTeamId();
		if (nTeamId == 0) then
			return;
		end
		
		local pGame = self.tbXoyoGames[nTeamId];
		if pGame ~= nil then
			pGame:EndRoomDTTTime(pGame.nCurDTTRoomId, 1);
		end
	end
end

-- 进入登天塔游戏
function XoyoGame:TryEnterDTTGame(pPlayer, nLevel)
	if (pPlayer) then
	
		if Player.tbDegree:GetDegree(pPlayer, "DTT") <= 0 then
			assert(false);
		end
		
		local nTeamId = pPlayer.GetTeamId();
		local tbMembers, nCount = KTeam.GetTeamMemberList(nTeamId);
		if nCount > 1 then
			pPlayer.SendBlackMsg(Lang.mission.str817[Lang.Idx]);
			return;
		end

		-- 战斗地图中不能进入登天塔
		local nMapId = pPlayer.dwSceneTemplateId;
		local nState = KScene.GetSceneFightState(nMapId);
		if (nState > 0) then
			return;
		end

		if (pPlayer.GetTeamId() == 0) then
			KTeam.CreateTeam(pPlayer.dwId);
		end

		-- 创建队伍是个异步操作，等待gc同步组队信息过来后再进入登天塔关卡
		Timer:Register(1 * Env.GAME_FPS, self.EnterDTTGame, self, pPlayer.dwId, nLevel);
	end
end

function XoyoGame:EnterDTTGame(nPlayerId, nLevel)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer ~= nil) then
		local nTeamId = pPlayer.GetTeamId();
		if (nTeamId == 0) then
			return;
		end
		
		local pGame = nil;
		if self.tbXoyoGames[nTeamId] == nil then
			pGame = Lib:NewClass(self.BaseGame);
			pGame:InitGame(XoyoGame.TB_MAPID_LIST, nTeamId);
			self.tbXoyoGames[nTeamId] = pGame;

			-- 进登天塔前下马
			Pet:GetDownRide(pPlayer);
			
			pGame:JoinNextGame(pPlayer);
			pGame:StartNewDTTGame(nLevel);
			self.tbPlayerLeavePos[pPlayer.dwId] = pPlayer.GetPosition();
			
			Player.tbDegree:ReduceDegree(pPlayer, "DTT", 1);

			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetDTTGameState", 1});
		end
	end

	return 0;
end

function XoyoGame:IsMissionMap(nMapId)
	for _, nId in ipairs(self.TB_MAPID_LIST) do
		if nMapId == nId then
			return 1;
		end
	end
	return 0;
end

-- 进准备场后关卡初始化并开始关卡
function XoyoGame:OnCharacterEnterScene(dwCharacterId)
	local pGame = nil;
	local pCharacter = KGameBase.GetCharacterById(dwCharacterId);
	if pCharacter.IsNpc() == 1 then
		return;
	end

	local nMapId = pCharacter.dwSceneTemplateId;
	local nTeamId = pCharacter.GetTeamId();
	
	if nMapId ~= XoyoGame.GAME_IN_POS.nMapId and self:IsMissionMap(nMapId) ~= 1 then
		return;
	end
	
	if nTeamId and nTeamId == 0 then
		self:QuitMap(pCharacter.dwId);
		return;
	end
	
	-- 进入准备场
	if nMapId == XoyoGame.GAME_IN_POS.nMapId then
		pGame = self:GetMyGame(pCharacter);
		if not pGame then
			assert(nTeamId > 0);
			if self.tbXoyoGames[nTeamId] == nil then
				pGame = Lib:NewClass(self.BaseGame);
				pGame:InitGame(XoyoGame.TB_MAPID_LIST, nTeamId);
				self.tbXoyoGames[nTeamId] = pGame;
			end
			
			pGame = self.tbXoyoGames[nTeamId];
			pGame:JoinNextGame(pCharacter);
			-- 建立player和game的映射
			self.tbPlayerGames[pCharacter.dwId] = pGame;
			
			if KTeam.GetTeamLeader(nTeamId) == pCharacter.dwId then
				Timer:Register(XoyoGame.PREPARE_TIME * Env.GAME_FPS, self.StartXoyoGame, self, nTeamId);
			end
		end
	elseif KScene.GetSceneType(nMapId) == 1 then		-- 进入关卡地图
		if self:IsMissionMap(nMapId) ~= 1 then
			return;
		end
		local pGame = self.tbXoyoGames[nTeamId];
		if (pGame) then
			pGame:OnCharacterEnterScene(pCharacter);
		end
	end
end

function XoyoGame:StartXoyoGame(nTeamId)
	local pGame = self.tbXoyoGames[nTeamId];
	if not pGame then
		return;
	end
	pGame:StartNewGame();
	return 0;
end

function XoyoGame:GetMyGame(pPlayer)
	local pGame = self.tbPlayerGames[pPlayer.dwId];
	return pGame;
end

function XoyoGame:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pGame = self:GetMyGame(pPlayer);
	if pGame then
		pGame:OnLogin(pPlayer);
	end
end

function XoyoGame:OnLogout(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	local pGame = self:GetMyGame(pPlayer);
	if pGame then
		pGame:OnLogout(pPlayer);
	end
end

function XoyoGame:QuitMap(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if pPlayer then
		local tbPos = XoyoGame.tbPlayerLeavePos[nPlayerId];
		if tbPos then
			pPlayer.TeleportTo(tbPos.dwSceneId, tbPos.nX, tbPos.nY, tbPos.nZ);
		else
			local tbLeavePos = XoyoGame.LEAVE_POS;
			pPlayer.TeleportTo(tbLeavePos[1], tbLeavePos[2], tbLeavePos[3], tbLeavePos[4]);
			--pPlayer.ReturnToLastCity();
		end
		XoyoGame.tbPlayerLeavePos[nPlayerId] = nil;
		XoyoGame.tbPlayerGames[nPlayerId] = nil;
	end
	return 0;
end

-- 注册登录事件
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, XoyoGame.OnLogin, XoyoGame);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, XoyoGame.OnLogout, XoyoGame);
