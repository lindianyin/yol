
-- 逍遥谷GC逻辑
function XoyoGame:InitGC()
	self.tbData = {}
	self.tbXoyoRank = {};
end

function XoyoGame:LoadRankData()
	self.tbXoyoRank = GetGblIntBuf(GBLINTBUF_XOYO_RANK, 0, 1) or {};
end

function XoyoGame:SaveRankData()
	SetGblIntBuf(GBLINTBUF_XOYO_RANK, 0, 1, self.tbXoyoRank);

end

function XoyoGame:DeleteRankData(nDifficuty, nRank)
	if (XoyoGame.tbXoyoRank[nDifficuty] == nil or #XoyoGame.tbXoyoRank[nDifficuty] == 0) then
		return;
	end
	if (nRank < 1 or nRank > XoyoGame.RANK_RECORD) then
		return;
	end
	local tbRank = self.tbXoyoRank[nDifficuty];
	for i = nRank, #tbRank[nRank] do
		tbRank[i] = tbRank[i + 1];
	end
end

function XoyoGame:ApplySyncData()
	GSExcute(-1, {"XoyoGame:OnSyncRankData", self.tbXoyoRank});
end

function XoyoGame:RecordRankData(nNewTime, nDifficuty, tbMember)
	if MODULE_GAMESERVER then
		GCExcute({"XoyoGame:RecordRankData", nNewTime, nDifficuty, tbMember});
	elseif MODULE_GC_SERVER then
		XoyoGame.tbXoyoRank[nDifficuty] = XoyoGame.tbXoyoRank[nDifficuty] or {};
		local tbRank = self.tbXoyoRank[nDifficuty];
		local tbNewRecord = { nDate = GetTime(), nTime = nNewTime, tbMember = tbMember };
		table.insert(tbRank, tbNewRecord);
		table.sort(tbRank, function (tb1, tb2)
			return tb1.nTime < tb2.nTime;
		end);
		if (#tbRank > XoyoGame.RANK_RECORD) then
			tbRank[XoyoGame.RANK_RECORD + 1] = nil;
		end
		self:SaveRankData();
		XoyoGame:ApplySyncData();
	end
end

function XoyoGame:CreateManager_GC(nMapId)
	if not XoyoGame.MANAGER_GROUP[nMapId] then
		return 0;
	end
	local tbCurData = {}
	for i, nGameId in pairs(self.MANAGER_GROUP[nMapId]) do
		tbCurData[nGameId] = self.tbData[nGameId];
	end
	GlobalExcute {"XoyoGame:CreateManager_GS2", nMapId, tbCurData};
end

function XoyoGame:SyncGameData_GC(nCityMapId, nData)
	self.tbData[nCityMapId] = nData;
	GlobalExcute{"XoyoGame:SyncGameData_GS2", nCityMapId, nData};
end

function XoyoGame:ReduceTeam_GC(nGameId, nData)
	self.tbData[nGameId] = nData;
	GlobalExcute{"XoyoGame:ReduceTeam_GS2", nGameId};
end

-- 开始新的一轮闯关
function XoyoGame:StartNewRound()
	GlobalExcute{"XoyoGame:LockManager", 1};		-- 先锁所有的manager
	Timer:Register(self.LOCK_MANAGER_TIME * Env.GAME_FPS, self.StartGame_GC, self)	-- 一定时间后执行开始操作
end

function XoyoGame:StartGame_GC()
	GlobalExcute{"XoyoGame:StartGame_GS2"};
	return 0;
end

-- 每天更新逍遥排行榜数据
function XoyoGame:ChallengeDailyRankUpdate()
	local tbTime = os.date("*t", GetTime());
	if tbTime.day == 1 then
		PlayerHonor:UpdateXoyoLadder(1);
		SetXoyoAwardResult();
		ClearXoyoLadderData();
		KGblTask.SCSetDbTaskInt(DBTASK_XOYO_FINAL_LADDER_MONTH, tbTime.year*100+tbTime.month);
	else
		PlayerHonor:UpdateXoyoLadder(0);
	end
end

function XoyoGame:ProcessCoZoneAndSubZoneBuf()
	self.tbXoyoRank = {};
	self:SaveRankData();
end

XoyoGame:InitGC();


if GCEvent ~= nil and GCEvent.RegisterGCServerStartFunc ~= nil then
	GCEvent:RegisterGCServerStartFunc(XoyoGame.LoadRankData, XoyoGame);
end
