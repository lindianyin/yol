Player.tbDegree = Player.tbDegree or {};
local tbDegree = Player.tbDegree;

tbDegree.TASK_GROUP = 5;

tbDegree.tbSetting =
{
	--						次数变量	累加周期		累加次数	累加上限	描述
	--						Id			Day/Week/Month
	TestFuben 			= { 1,			"Day", 			2,			10,			Lang.player.str5[Lang.Idx]},
	XoyoGame 			= { 2,			"Day", 			5,			5,			Lang.player.str6[Lang.Idx]},
	Instance 			= { 3,			"Day", 			60,		    60,		    Lang.player.str7[Lang.Idx]},
	DTT 				= { 4,			"Day", 			1,			1,			Lang.player.str8[Lang.Idx]},
	ArenaChallenge 		= { 6,			"Day", 			20,			20,			Lang.player.str9[Lang.Idx]},
	AlFreeTimes 		= { 7,			"Day", 			10,		    10,	     	Lang.player.str10[Lang.Idx]},	--次数要大于最高获取次数
	AlCoinTimes 		= { 8,			"Day", 			0,			0,			Lang.player.str11[Lang.Idx]},	--次数要大于最高获取次数
	Escort  			= { 9,			"Day",			5,			5,			Lang.player.str12[Lang.Idx]},
	EscortRefresh  		= { 10,			"Day",			20,			20,			Lang.player.str13[Lang.Idx]},
	MopTower  			= { 11,			"Day",			1,	    	1,		    Lang.player.str14[Lang.Idx]},
	ChallengeTower  	= { 12,			"Day",			10,	    	10,		    Lang.player.str15[Lang.Idx]},
	QuizDoubleFreeTimes = { 13,			"Day",			3,			3,			Lang.player.str16[Lang.Idx]},
	QuizJoinFirst 		= { 14,			"Day",			1,			1,			Lang.player.str17[Lang.Idx]},
	QuizJoinSecond 		= { 15,			"Day",			1,			1,			Lang.player.str18[Lang.Idx]},
	RushStage 			= { 16,			"Day", 			3,	    	3,		    Lang.player.str19[Lang.Idx]},
	MopStage			= { 17,			"Day", 			100,		100,		Lang.player.str20[Lang.Idx]},
	MultiInstance 		= { 18,			"Day", 			2,		    2,		    Lang.player.str21[Lang.Idx]},
	ZhanBaoTang			= { 19,			"Day",			3,			3,			Lang.player.str22[Lang.Idx]},
	DailyTasks			= { 20,			"Day",			1,			1,			Lang.player.str23[Lang.Idx]},
	RefreshPetCardTimes = { 21,			"Day",			3,			3,			Lang.player.str24[Lang.Idx]},
	GetPetCardTimes 	= { 22,			"Day",			3,			3,			Lang.player.str25[Lang.Idx]},
	SignInTimes 		= { 23,			"Month",		0,			0,			Lang.player.str26[Lang.Idx]},
	YS_Multi 			= { 24,			"Day", 			2,		    2,		    Lang.player.str27[Lang.Idx]},
	LS_Multi 			= { 25,			"Day", 			2,		    2,		    Lang.player.str28[Lang.Idx]},
	ZZ_Multi 			= { 26,			"Day", 			10,	    	10,	     	Lang.player.str29[Lang.Idx]},
	EscortAttack  		= { 27,			"Day",			10,		    10,			Lang.player.str30[Lang.Idx]},
	EscortProtect  		= { 28,			"Day",			10,	    	10,			Lang.player.str31[Lang.Idx]},
	EscortHelp  		= { 29,			"Day",			10,		    10,			Lang.player.str32[Lang.Idx]},
	JoinZSWS  	    	= { 30,			"Day",			1,	        1,			Lang.player.str33[Lang.Idx]},
	JoinSMZC  	    	= { 31,			"Day",			1,	    	1,			Lang.player.str34[Lang.Idx]},
	JoinPZ  	    	= { 32,			"Day",			1,	    	1,			Lang.player.str35[Lang.Idx]},
}

function tbDegree:GetTask(pPlayer, nId)
	local nCurDegree = pPlayer.pTask.GetTask(self.TASK_GROUP, nId * 3 - 2);
	local nMaxDegree = pPlayer.pTask.GetTask(self.TASK_GROUP, nId * 3 - 1);
	local nLastTime = pPlayer.pTask.GetTask(self.TASK_GROUP, nId * 3);
	return nCurDegree, nMaxDegree, nLastTime;
end

function tbDegree:SetTask(pPlayer, nId, nCurDegree, nMaxDegree, nLastTime)
	pPlayer.pTask.SetTask(self.TASK_GROUP, nId * 3 - 2, nCurDegree);
	pPlayer.pTask.SetTask(self.TASK_GROUP, nId * 3 - 1, nMaxDegree);
	pPlayer.pTask.SetTask(self.TASK_GROUP, nId * 3, nLastTime);
end

-- 判断是否已经达到最大次数
function tbDegree:IsDegreeMax(pPlayer, szName)
	local tbInfo = self.tbSetting[szName];
	assert(tbInfo);

	local nMaxDegree = self:GetMaxDegree(pPlayer, szName);
	local nDegree = self:GetDegree(pPlayer, szName);

	if nDegree >= nMaxDegree then
		return 1;  -- 已经抵达最大
	end

	return 0;  -- 非最大
end

function tbDegree:GetDegree(pPlayer, szName)
	local tbInfo = self.tbSetting[szName];
	assert(tbInfo);

	local szFunction = "GetLocal"..tbInfo[2];
	local nCurTime = KGameBase.GetTime();
	local nCurDate = Lib[szFunction](Lib, nCurTime);
	local nCurDegree, nMaxDegree, nLastTime = self:GetTask(pPlayer, tbInfo[1]);
	local nLastDate = 0;

	if nLastTime == 0 then
		nLastDate = nCurDate;
		nCurDegree = tbInfo[3];
		nMaxDegree = tbInfo[4];
		self:SetTask(pPlayer, tbInfo[1], nCurDegree, nMaxDegree, KGameBase.GetTime());
	else
		nLastDate = Lib[szFunction](Lib, nLastTime);
	end

	if nLastDate == nCurDate then
		return nCurDegree;
	end

	local nNum = nCurDegree + (nCurDate - nLastDate) * tbInfo[3];
	nMaxDegree = math.max(tbInfo[4], nMaxDegree);
	nNum = math.min(nMaxDegree, math.max(nNum, 0));

	if nNum < 0 then
		nNum = 0;
	end

	self:SetDegree(pPlayer, szName, nNum, 0);

	return nNum;
end

function tbDegree:GetMaxDegree(pPlayer, szName)
	local tbInfo = self.tbSetting[szName];
	assert(tbInfo);

	local szFunction = "GetLocal"..tbInfo[2];
	local nCurTime = KGameBase.GetTime();
	local nCurDate = Lib[szFunction](Lib, nCurTime);
	local nCurDegree, nMaxDegree, nLastTime = self:GetTask(pPlayer, tbInfo[1]);
	local nLastDate = 0;

	if nLastTime == 0 then
		nLastDate = nCurDate;
		nCurDegree = tbInfo[3];
		nMaxDegree = tbInfo[4];
		self:SetTask(pPlayer, tbInfo[1], nCurDegree, nMaxDegree, KGameBase.GetTime());
	else
		nLastDate = Lib[szFunction](Lib, nLastTime);
	end

	if nLastDate == nCurDate then
		return nMaxDegree;
	end

	nMaxDegree = tbInfo[4];

	return nMaxDegree;
end

function tbDegree:GetReduceDegree(pPlayer, szName)
	local maxDegree = tbDegree:GetMaxDegree(pPlayer, szName);
	local curDegree = tbDegree:GetDegree(pPlayer, szName);
	
	return (maxDegree - curDegree);
end

function tbDegree:SetDegree(pPlayer, szName, nDegree, nSync)
	local tbInfo = self.tbSetting[szName];
	assert(tbInfo);

	local nMaxDegree = self:GetMaxDegree(pPlayer, szName);

	nDegree = math.min(nDegree, nMaxDegree);
	nDegree = math.max(nDegree, 0);

	self:SetTask(pPlayer, tbInfo[1], nDegree, nMaxDegree, KGameBase.GetTime());

	if not nSync or nSync == 1 then
		self:SyncDegree(pPlayer, szName);
	end
end

function tbDegree:SetMaxDegree(pPlayer, szName, nMaxDegree, nSync)
	local tbInfo = self.tbSetting[szName];
	assert(tbInfo);

	local nDegree = self:GetDegree(pPlayer, szName);

	nMaxDegree = math.max(nDegree, nMaxDegree);
	nMaxDegree = math.max(nMaxDegree, 0);

	self:SetTask(pPlayer, tbInfo[1], nDegree, nMaxDegree, KGameBase.GetTime());

	if not nSync or nSync == 1 then
		self:SyncDegree(pPlayer, szName);
	end
end

function tbDegree:ReduceDegree(pPlayer, szName, nDegree)
	nDegree = nDegree or 1;
	local nCurDegree = self:GetDegree(pPlayer, szName);
	if nCurDegree < nDegree then
		return false;
	end

	self:SetDegree(pPlayer, szName, nCurDegree - nDegree);

	return true;
end

function tbDegree:AddDegree(pPlayer, szName, nDegree)
	nDegree = nDegree or 1;
	local nCurDegree = self:GetDegree(pPlayer, szName);
	self:SetDegree(pPlayer, szName, nCurDegree + nDegree);
end

function tbDegree:AddMaxDegree(pPlayer, szName, nDegree)
	nDegree = nDegree or 1;
	local nCurMaxDegree = self:GetMaxDegree(pPlayer, szName);
	self:SetMaxDegree(pPlayer, szName, nCurMaxDegree + nDegree);
end


function tbDegree:SyncDegree(pPlayer, szName)
	local nCurDegree = self:GetDegree(pPlayer, szName);
	local nMaxDegree = self:GetMaxDegree(pPlayer, szName);

	pPlayer.SyncDegree(szName, nCurDegree, nMaxDegree);
end

function tbDegree:SyncAllDegree(pPlayer)
	for szName, _ in pairs(self.tbSetting) do
		self:SyncDegree(pPlayer, szName);
	end
end

function tbDegree:c2s_SyncDegree(pPlayer)
	self:SyncAllDegree(pPlayer);
end

-- 调试用，正式发布前要删除
function tbDegree:c2s_ReduceDegree(pPlayer, szName, nDegree)
   print("tbDegree:c2s_ReduceDegree", pPlayer, szName, nDegree)
   self:ReduceDegree(pPlayer, szName, nDegree)
end

function tbDegree:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if (pPlayer) then
		self:SyncAllDegree(pPlayer);
	else
		print("Degree Login Error");
	end

	return 0;
end

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, tbDegree.OnLogin, tbDegree);
