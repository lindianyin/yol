Meditation.OfflineAwardType = {1,2,3,6};	-- 离线打坐领奖类型
Meditation.OfflineAwardCost = {0,5,10,20}; --离线打坐领取消费
Meditation.TotalTime = 12 * 60 * 60; -- 每日最多打坐时间秒数
Meditation.ExpPercent = 0.2;		 -- 打坐获每天得当前等级升级经验百分比(当配置表中不存在某一等级时)
Meditation.BaseTime = 30;			 -- 每30s发一次奖励
Meditation.RequireLevel = 22;		 -- 打坐开放等级
Meditation.SilverAward = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};		 -- 家族篝火每30秒获得的银两奖励
Meditation.BonfireAddPercent = {1, 1.1, 1.2, 1.3, 1.4 , 1.5, 1.6, 1.7, 1.8, 1.9, 2.0};	--篝火加成经验
Meditation.OfflineRemindIdx = 29;	--离线经验提醒
Meditation.tbAwardCfg = {};

function Meditation:Init()
	self.nTotalExp = {};	--累计经验统计
	self.nTotalSilver = {};	--累计银两统计
	self.tbPlayerOfflineExp = {}; -- 离线经验
	self:LoadMeditation();	--load配置
end

function Meditation:SetMeditationTotalTime(nTime)
	Meditation.TotalTime = nTime;
end

function Meditation:OpenOfflineMeditationPanel(pPlayer)
	local dwNow = KGameBase.GetTime();
	local dwTakeOfflineAwardTime = pPlayer.GetTakeMeditationOfflineAwardTime();

	local nLastMeditationOnlineTime = pPlayer.GetLastMeditationOnlineTime();
	if nLastMeditationOnlineTime >= Meditation.TotalTime then
		nLastMeditationOnlineTime = Meditation.TotalTime;
	end
	
	local nLevel = pPlayer.GetLevel();
	local dwExpPercent = Meditation.ExpPercent;
	if Meditation.tbAwardCfg[nLevel] ~= nil then
		dwExpPercent = Meditation.tbAwardCfg[nLevel].nExpPercent / 100.0;
	end
	
	if self.tbPlayerOfflineExp[pPlayer.dwId] == nil then
		if Lib:GetLocalDay(dwNow) ~= Lib:GetLocalDay(dwTakeOfflineAwardTime) then
			local totalExp = KGameBase.GetNextLevelExp(nLevel) * dwExpPercent;
			local nTimeSeconds = Meditation.TotalTime - nLastMeditationOnlineTime;
			local OfflineExp = math.floor( totalExp * 0.5 *  nTimeSeconds / Meditation.TotalTime);
			--print(Lang.meditation.str1[Lang.Idx], nTimeSeconds, OfflineExp);
			self.tbPlayerOfflineExp[pPlayer.dwId] = {OfflineExp, nTimeSeconds}
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOfflinePanelContent", nTimeSeconds, OfflineExp});
			Remind:AddRemindToPlayer(pPlayer, Meditation.OfflineRemindIdx, -1);
		end
	else
	   local offlineExp  = self.tbPlayerOfflineExp[pPlayer.dwId][1]
	   local offlineTime = self.tbPlayerOfflineExp[pPlayer.dwId][2]
	   --print(Lang.meditation.str2[Lang.Idx], offlineTime, offlineExp);
	   pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOfflinePanelContent", offlineTime, offlineExp});
		 Remind:AddRemindToPlayer(pPlayer, Meditation.OfflineRemindIdx, -1);
	end
end

function Meditation:ErrorMsg(pPlayer, strMsg)
	if pPlayer then
		pPlayer.SendBlackMsg(strMsg);
	end
end

function Meditation:OnLogin(nPlayerId)
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then
		return;
	end
	
	if pPlayer.GetLevel() < Meditation.RequireLevel then
		return;
	end
	
	local dwNow = KGameBase.GetTime();
	local dwLastLogoutTime = pPlayer.GetLastLogoutTime();
	
	if Lib:GetLocalDay(dwNow) ~= Lib:GetLocalDay(dwLastLogoutTime) then
		pPlayer.ResetMeditationData();
	end	
	Meditation:OpenOfflineMeditationPanel(pPlayer);
end

function Meditation:ResetMeditationData()
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		if pPlayer.GetLevel() >= Meditation.RequireLevel then
			pPlayer.ResetMeditationData();
			local nLastMeditationOnlineTime = pPlayer.GetLastMeditationOnlineTime();
			if nLastMeditationOnlineTime >= Meditation.TotalTime then
				nLastMeditationOnlineTime = Meditation.TotalTime;
			end
			
			local nLevel = pPlayer.GetLevel();
			local dwExpPercent = Meditation.ExpPercent;
			if Meditation.tbAwardCfg[nLevel] ~= nil then
				dwExpPercent = Meditation.tbAwardCfg[nLevel].nExpPercent / 100.0;
			end
	
			local totalExp = KGameBase.GetNextLevelExp(nLevel) * dwExpPercent;
			local nTimeSeconds = Meditation.TotalTime - nLastMeditationOnlineTime;
			local OfflineExp = math.floor( totalExp * 0.5 *  nTimeSeconds / Meditation.TotalTime);
			--print(Lang.meditation.str1[Lang.Idx], nTimeSeconds, OfflineExp);
			self.tbPlayerOfflineExp[pPlayer.dwId] = {OfflineExp, nTimeSeconds}
			pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOfflinePanelContent", nTimeSeconds, OfflineExp});
			Remind:AddRemindToPlayer(pPlayer, Meditation.OfflineRemindIdx, -1);
		end
	end
end

function Meditation:c2s_TakeOfflineAward(pPlayer, nType)
	if not Meditation.OfflineAwardType[nType] then
		return;
	end
	if pPlayer.GetLevel() < Meditation.RequireLevel then
		return;
	end
	
	local vipId = 0;
	if nType == 2 then
		vipId = Vip.ID_MEDITATION_OFFLINE_2;
	elseif nType == 3 then
		vipId = Vip.ID_MEDITATION_OFFLINE_3;
	elseif nType == 4 then
		vipId = Vip.ID_MEDITATION_OFFLINE_6;
	end
	
	if vipId~= 0 and Vip:IsOpen(pPlayer, vipId) ~= 1 then
		--pPlayer.SendBlackMsg("当前vip等级未开启此功能，请升级vip!");
		return;
	end
	
	if pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN) < Meditation.OfflineAwardCost[nType] then
		pPlayer.SendBlackMsg(string.format(Lang.meditation.str3[Lang.Idx], Meditation.OfflineAwardCost[nType]));
		return;
	end
	
	pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_COIN, -Meditation.OfflineAwardCost[nType]);
	local nLastMeditationOnlineTime = pPlayer.GetLastMeditationOnlineTime();
	if nLastMeditationOnlineTime >= Meditation.TotalTime then
		nLastMeditationOnlineTime = Meditation.TotalTime;
	end
	
	--[[local nLevel = pPlayer.GetLevel();
	local dwExpPercent = Meditation.ExpPercent;
	if Meditation.tbAwardCfg[nLevel] ~= nil then
		dwExpPercent = Meditation.tbAwardCfg[nLevel].nExpPercent / 100.0;
	end
	
	local totalExp = KGameBase.GetNextLevelExp(nLevel) * dwExpPercent;
	local OfflineExp = math.floor( totalExp * 0.5 * (Meditation.TotalTime - nLastMeditationOnlineTime) / Meditation.TotalTime) * Meditation.OfflineAwardType[nType];--]]
	 
	local offlineExp  = self.tbPlayerOfflineExp[pPlayer.dwId][1] * Meditation.OfflineAwardType[nType]
	--local offlineTime = self.tbPlayerOfflineExp[pPlayer.dwId][2]
	pPlayer.SetTakeMeditationOfflineAwardTime(KGameBase.GetTime());
	pPlayer.AddExp(offlineExp);

	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:HideMeditationOfflinePanel"});
	Remind:RemoveRemindToPlayer(pPlayer, Meditation.OfflineRemindIdx);
	self.tbPlayerOfflineExp[pPlayer.dwId] = nil
end

--C++中每30s发一次奖励
function Meditation:GiveOnlineAward(pPlayer)
	if pPlayer.GetLevel() < Meditation.RequireLevel then
		return;
	end
	--Dbg:WriteLog(Dbg.LOG_INFO,string.format("..Meditation:GiveOnlineAward(pPlayer=%d)",pPlayer.dwId));
	--判断是否在篝火活动中
	local bonfire = Activity:GetBonfireStatus(pPlayer);
	local bonfireAddExp = 0;
	local bonfireAddSilver = 0;
	local bonfireLeftTime = 0;
	
	local dwMeditationOnlineTime = pPlayer.GetMeditationOnlineTime();
	local tbPlayerKinInfo = GetPlayerKinInfo(pPlayer.dwId);
	--print("..dwMeditationOnlineTime",dwMeditationOnlineTime);
	if dwMeditationOnlineTime > Meditation.TotalTime then
		dwMeditationOnlineTime = Meditation.TotalTime;
	else
		local nNum = Meditation.TotalTime / Meditation.BaseTime;
		local nLevel = pPlayer.GetLevel();
		local dwExpPercent = Meditation.ExpPercent;
		if Meditation.tbAwardCfg[nLevel] ~= nil then
			dwExpPercent = Meditation.tbAwardCfg[nLevel].nExpPercent / 100.0;
		end
		local totalExp = KGameBase.GetNextLevelExp(nLevel) * dwExpPercent;
		local addExp = math.floor(totalExp / nNum);
		--print("..totalExp",totalExp,"addExp",addExp);
		
		if bonfire == 1 then
			if tbPlayerKinInfo ~= nil and tbPlayerKinInfo.nKinLevel > 0 then
				bonfireAddExp = addExp * Meditation.BonfireAddPercent[tbPlayerKinInfo.nKinLevel];
				--print("..bonfireAddExp",bonfireAddExp);
				bonfireAddSilver = Meditation.SilverAward[tbPlayerKinInfo.nKinLevel];
				bonfireLeftTime = Activity:GetKinActivityLeftTime(pPlayer.GetKinId(), 1001);
				pPlayer.pPlayerPurse.AddMoney(Purse.EM_MONEY_SILVER, bonfireAddSilver);
				--print("..bonfireAddSilver",bonfireAddSilver);
			end
		end
		
		if not self.nTotalExp[pPlayer.dwId] then
			self.nTotalExp[pPlayer.dwId] = 0;
		end
		
		self.nTotalExp[pPlayer.dwId] = self.nTotalExp[pPlayer.dwId] + addExp + bonfireAddExp;
		pPlayer.AddExp(addExp + bonfireAddExp);
		--print("..self.nTotalExp[pPlayer.dwId]",self.nTotalExp[pPlayer.dwId]);
		--print("..pPlayer.AddExp(addExp + bonfireAddExp);",addExp + bonfireAddExp);
		
	end
	local nTimeSeconds = Meditation.TotalTime - dwMeditationOnlineTime;

	local bonfireExp = 0;
	if tbPlayerKinInfo ~= nil and tbPlayerKinInfo.nKinLevel > 0 then
		bonfireExp = self.nTotalExp[pPlayer.dwId] / (1 + Meditation.BonfireAddPercent[tbPlayerKinInfo.nKinLevel]) * Meditation.BonfireAddPercent[tbPlayerKinInfo.nKinLevel];
	end
	
	if not self.nTotalSilver[pPlayer.dwId] then
		self.nTotalSilver[pPlayer.dwId] = 0;
	end
	self.nTotalSilver[pPlayer.dwId] = self.nTotalSilver[pPlayer.dwId] + bonfireAddSilver;
	Pet:GetDownRide(pPlayer);
	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOnlinePanelContent", nTimeSeconds, self.nTotalExp[pPlayer.dwId], bonfire, math.floor(bonfireExp), self.nTotalSilver[pPlayer.dwId], bonfireLeftTime});
	--print(unpack({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOnlinePanelContent", nTimeSeconds, self.nTotalExp[pPlayer.dwId], bonfire, math.floor(bonfireExp), self.nTotalSilver[pPlayer.dwId], bonfireLeftTime}))
end

function Meditation:ResetOnlineTotalExp(pPlayer)
	self.nTotalExp[pPlayer.dwId] = 0;
	self.nTotalSilver[pPlayer.dwId] = 0;
end

function Meditation:ShowMeditationOnlinePanel(pPlayer)
	if pPlayer.GetLevel() < Meditation.RequireLevel then
		return;
	end
	local dwMeditationOnlineTime = pPlayer.GetMeditationOnlineTime();
	if dwMeditationOnlineTime > Meditation.TotalTime then
		dwMeditationOnlineTime = Meditation.TotalTime;
	end
	if not self.nTotalExp[pPlayer.dwId] then
		self.nTotalExp[pPlayer.dwId] = 0;
	end
	local nTimeSeconds = Meditation.TotalTime - dwMeditationOnlineTime;
	
	local bonfire = Activity:GetBonfireStatus(pPlayer);
	local bonfireLeftTime = 0;
	if bonfire == 1 then
		bonfireLeftTime = Activity:GetKinActivityLeftTime(pPlayer.GetKinId(), 1001);
	end
	if not self.nTotalSilver[pPlayer.dwId] then
		self.nTotalSilver[pPlayer.dwId] = 0;
	end
	Pet:GetDownRide(pPlayer);
	pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:SetMeditationOnlinePanelContent",nTimeSeconds, self.nTotalExp[pPlayer.dwId], bonfire, 0, 0, bonfireLeftTime});
end

function Meditation:HideMeditationOnlinePanel(pPlayer)
	if pPlayer then
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:HideMeditationOnlinePanel"});
	end
end

function Meditation:OnEnterScene(dwCharacterId)
	local pPlayer = KGameBase.GetCharacterById(dwCharacterId);
	
	if pPlayer.IsNpc() == 1 then
		return;
	end
	
	if pPlayer.GetIsFightMap() > 0 then 
		--关闭在线打坐面板
		Meditation:HideMeditationOnlinePanel(pPlayer);
		pPlayer.SetMeditationState(0);
	end
end

function Meditation:LoadMeditation()
	local cfgs = Meditation.tbAwardCfg;
	local tbFileData = Lib:LoadTabFile("/setting/meditation/meditation_award.txt");
	for _, tbRow in pairs(tbFileData) do
		local nLevel = tonumber(tbRow.Level);
		assert(nLevel);
		if not cfgs[nLevel] then
			cfgs[nLevel] = {
			nExpPercent = tonumber(tbRow.ExpPercent);
			};
		end
	end
end

Meditation:Init();

CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, Meditation.OnLogin, Meditation);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_CHARACTER_ENTER_SCENE, Meditation.OnEnterScene, Meditation);