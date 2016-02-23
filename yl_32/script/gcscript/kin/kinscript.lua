if not (MODULE_GAMECENTER) then
	return
end

-- 发起弹劾的玩家ID
Kin.tbImpeachIds = {};

-- 弹劾间隔时间，10分钟，16是fps
Kin.ImpeachInterval = 10 * 60 * 16;

-- 间隔时间内超过规定人数弹劾则弹劾成功
Kin.ImpeachMinMember = 1;

-- 族长不在线时间，以秒为单位
-- 族长不在线并须连续不在线时间超过7天以上才能弹劾
Kin.MasetOffLineTime = 7 * 24 * 3600;

-- 弹劾族长
function Kin:Impeach_gc(dwPlayerId)
	local nServerCounter = KTimer.GetFrame();
	
	-- 清过期的请求
	for pId, tbVar in pairs(self.tbImpeachIds) do
		if (nServerCounter - tbVar["ServerCounter"] >= self.ImpeachInterval) then
			self.tbImpeachIds[pId] = nil;
		end
	end
	
	if (self.tbImpeachIds[dwPlayerId] == nil) then
		self.tbImpeachIds[dwPlayerId] = {};
		self.tbImpeachIds[dwPlayerId]["ServerCounter"] = nServerCounter;
		-- 通知
		local tbPlayerIds = {};
		for pId, _ in pairs(self.tbImpeachIds) do
			table.insert(tbPlayerIds, pId);
		end
		--GlobalExecute({"Kin:NotifyImpeach_gs", tbPlayerIds, "您进行了弹劾族长操作"});
	else
		return; -- 已经请求过一次
	end
	
	local nCount = 0;
	for pId, tbVar in pairs(self.tbImpeachIds) do
		nCount = nCount + 1;
	end
	
	if (nCount >= self.ImpeachMinMember) then
		local bSuccess = ImpeachKinMaster(dwPlayerId, self.MasetOffLineTime);
		
		local tbPlayerIds = {};
		for pId, _ in pairs(self.tbImpeachIds) do
			table.insert(tbPlayerIds, pId);
		end
		self.tbImpeachIds = {};
		if (bSuccess == 1) then
			GlobalExecute({"Kin:NotifyImpeach_gs", dwPlayerId, 1});
		else
			GlobalExecute({"Kin:NotifyImpeach_gs", dwPlayerId, 0});
		end
	end
end

function Kin:PrintRoleInfo(nPlayerId)
    local tbRole = GetRoleInfoById(nPlayerId)
    print("角色信息", tbRole.szAccount, tbRole.szName, tbRole.nLevel)
end

function Kin:CheckTongAward(dwPlayerId)
	local tbMember = GetTongMember(dwPlayerId);
  local offlineDay = Lib:GetLocalDay(tbMember.nLastOfflineTime);
  local nowDay = Lib:GetLocalDay(GetCurrentTime());

  if nowDay > offlineDay then
     local nConnectIndex = GetPlayerConnectIndex(dwPlayerId);
     local nKinLevel = GetKinLevelByPlayerId(dwPlayerId)
     GlobalExecute({"Kin:GiveAward", dwPlayerId, nKinLevel, tbMember.nDailyContribution}, nConnectIndex);     
     ResetTongMemberDailyContribution(dwPlayerId) -- 把每日贡献度清0
  end
end

function Kin:DoKinUpgrade(nPlayerId, dwKinId)
    local nSuccess = UpgradeKin(nPlayerId);
    if nSuccess == 1 then
	    local nConnectIndex = GetPlayerConnectIndex(nPlayerId);
	    local nKinLevel = GetKinLevelByPlayerId(nPlayerId)
	    GlobalExecute({"Kin:NotifyUpgradeSuccess", nPlayerId, nKinLevel}, nConnectIndex);
	    GlobalExecute({"Kin:UpdatePlayerKinLevelInfos", dwKinId, nKinLevel}); -- 更新各个服的家族等级信息
	 end
end

function Kin:OnPlayerLogin(dwPlayerId)
   self:CheckTongAward(dwPlayerId);
end

function Kin:OnPlayerJoinTong(nPlayerId, nTongId)
    local nConnectIndex = GetPlayerConnectIndex(nPlayerId);
    GlobalExecute({"Kin:OnPlayerJoinTong", nPlayerId, nTongId}, nConnectIndex);
end

function Kin:CheckAllKinPlayersAward()
   local tbPlayerIds = GetOnlineAndHaveKinPlayerIds();
   for _, dwPlayerId in pairs(tbPlayerIds) do
       self:CheckTongAward(dwPlayerId);
   end
end

function Kin:SendJoinKinQueueFullMsg(nPlayerId)
   local nConnectIndex = GetPlayerConnectIndex(nPlayerId);
   GlobalExecute({"Kin:SendKinErrorMsg", nPlayerId, Lang.gcscript.str2[Lang.Idx]}, nConnectIndex);
end

function Kin:CreateKinActivitiesInGS(nKinId, nGSIndex)
   if nGSIndex ~= -1 then
       GlobalExecute({"Activity:CreateKinActivities", nKinId}, nGSIndex);
   else
       GlobalExecute({"Activity:CreateKinActivities", nKinId});
   end
end

function Kin:OnKinDisband(nKinId)
   GlobalExecute({"Activity:DestroyKinActivities", nKinId});
end

function Kin:SetKinActivityOpenTime(nKinId, nKinActivityIndex, nOpenTime)
   GlobalExecute({"Activity:SetKinActivityOpenTime", nKinId, nKinActivityIndex, nOpenTime});
end

function Kin:OnKinActivitiesCreated(nKinId)
   local tbKinOpenTimes = GetKinActivityOpenTimes(nKinId);
   for nActivityIndex, tbOpenTime in pairs(tbKinOpenTimes) do
       self:SetKinActivityOpenTime(nKinId, nActivityIndex, tbOpenTime.dwActivityOpenTime);
   end
end

function Kin:SignUpKinActivity(nKinId, nActivityIndex)
   
end