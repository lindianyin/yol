print("..onlinereward");

OnlineReward.tbFileData   = Lib:LoadTabFile("/setting/task/online_reward.txt");

OnlineReward.tbReward = {};
for _,v in pairs(OnlineReward.tbFileData) do 
	if(tonumber(v.id)) then 
		local nId = tonumber(v.id);
		OnlineReward.tbReward[nId] = v;
	end
end



OnlineReward.GROUP_ID = 6007;

--累计在线时间
OnlineReward.ONLINE_TIME = 1;
--登录时间
OnlineReward.LOGIN_TIME =  2;


OnlineReward.REWARD_N = 10;

--奖励状态
OnlineReward.STAT_NONE = 0;
OnlineReward.STAT_CAN_REWARD = 1;
OnlineReward.STAT_REWARDED = 2;


--初始化在线奖励
function OnlineReward:initOnlineReward(pPlayer)
	print("..OnlineReward:initOnlineReward");
	pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, OnlineReward.ONLINE_TIME, 0);
	for k,v in pairs(OnlineReward.tbReward) do 
		local nKey = OnlineReward.REWARD_N + k;
		pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, nKey, 0);
	end
end


--领取在线奖励
function OnlineReward:c2s_getOnlineReward(pPlayer,nId) 
	print("..OnlineReward:c2s_getOnlineReward(pPlayer,nId)",pPlayer.dwId,nId);
	--判断状态
	local nKey = OnlineReward.REWARD_N + nId;
	local nRewardStat = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, nKey);
	if(nRewardStat ~= OnlineReward.STAT_CAN_REWARD) then
		print("..if(nRewardStat ~= OnlineReward.STAT_CAN_REWARD) then");
		pPlayer.SysMsg("..can not take the online reward");
		return;
	end
	
	local tbRow = OnlineReward.tbReward[nId];
	local tbRewardMoney = {};
	local tbRewardItem = {};
	local nReward_exp = tonumber(tbRow.reward_exp);
	
	for i=1,4 do 
		local szKey = "reward_money" .. tostring(i);
		local szVal = tbRow[szKey];
		if(szVal ~= "") then 
			tbRewardMoney[#tbRewardMoney+1] = szVal;
		end
	end

	for i=1,6 do 
		local szKey = "reward_item" .. tostring(i);
		local szVal = tbRow[szKey];
		if(szVal ~= "") then 
			tbRewardItem[#tbRewardItem+1] = szVal;
		end
	end
	local nBagFreeCell = pPlayer.pItem.CountFreeBagCell();
	if nBagFreeCell < #tbRewardItem then
		pPlayer.SendBlackMsg(string.format(Lang.item.str8[Lang.Idx], #tbRewardItem));
		return;
	end
	
	if(nReward_exp) then
		pPlayer.AddExp(nReward_exp);
	end
	for _,v in pairs(tbRewardMoney) do 
		local _tb = Lib:SplitStr(v,"#");
		local nMoneyType = tonumber(_tb[1]);
		local nMoneyValue = tonumber(_tb[2]);
		pPlayer.pPlayerPurse.AddMoney(nMoneyType, nMoneyValue);
	end

	for _,v in pairs(tbRewardItem) do 
		local _tb = Lib:SplitStr(v,"#");
		local _tbNum = {};
		for _,v in pairs(_tb) do 
			_tbNum[#_tbNum + 1] = tonumber(v);
		end
		
		KItem.AddPlayerItem(pPlayer,unpack(_tbNum));
	end
	--设置奖励已经领取
	pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, nKey, OnlineReward.STAT_REWARDED);
end


--登录事件
function OnlineReward:OnLogin(nPlayerId)
	print("..OnlineReward:OnLogin(nPlayerId)",nPlayerId);
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then 
		return;
	end
	--判断是否当天登录
	local tNow = KGameBase.GetTime();
	local nTime = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.LOGIN_TIME);
	local nDay = Lib:GetLocalDay(nTime);
	local nDayNow = Lib:GetLocalDay(tNow);
	if((nDayNow - nDay) >= 1) then
		OnlineReward:initOnlineReward(pPlayer);
	end
	pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, OnlineReward.LOGIN_TIME, tNow);
	--pPlayer.SysMsg("onlinetime=" .. pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.ONLINE_TIME));
end


--登出事件
function OnlineReward:OnLogout(nPlayerId)
	print("..OnlineReward:OnLogout(nPlayerId)",nPlayerId);
	local pPlayer = KGameBase.GetPlayerById(nPlayerId);
	if not pPlayer then 
		return;
	end
	--累计在线时间
	local tNow = KGameBase.GetTime();
	local nTime = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.LOGIN_TIME);
	local nDelta = tNow - nTime;
	local nOnlineTime = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.ONLINE_TIME);
	if(nDelta > 0) then 
		pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, OnlineReward.ONLINE_TIME, nDelta + nOnlineTime);
	end
end


function OnlineReward:GetOnlineTime(pPlayer) 
	local tNow = KGameBase.GetTime();
	local nTime = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.LOGIN_TIME);
	local nDay = Lib:GetLocalDay(nTime);
	local nDayNow = Lib:GetLocalDay(tNow);
	if((nDayNow - nDay) >= 1) then 
		OnlineReward:OnLogin(pPlayer.dwId);
	end
	local nTotal = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, OnlineReward.ONLINE_TIME);
	return tNow -nTime + nTotal;
end

function OnlineReward:CheckRewardStat()
	local tbPlayers = KGameBase.GetAllPlayers()
	for _, pPlayer in pairs(tbPlayers) do
		local nOnlineTime = OnlineReward:GetOnlineTime(pPlayer);
		--print("...nOnlineTime",nOnlineTime);
		for k,v in pairs(OnlineReward.tbReward) do 
			local nKey = OnlineReward.REWARD_N + k;
			local nRange = tonumber(v.range);
			local nRewardStat = pPlayer.pTask.GetTask(OnlineReward.GROUP_ID, nKey);
			if(nRewardStat == OnlineReward.STAT_NONE) then 
				if(nOnlineTime >= nRange) then 
					pPlayer.pTask.DirectSetTask(OnlineReward.GROUP_ID, nKey, OnlineReward.STAT_CAN_REWARD);
				end
			end
		end
	end
end



CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGOUT, OnlineReward.OnLogout, OnlineReward);
CallCenter:RegisterGlobalEvent(KOBJEVENT.emKOBJEVENTTYPE_PLAYER_LOGIN, OnlineReward.OnLogin, OnlineReward);

Timer:Register(Env.GAME_FPS, OnlineReward.CheckRewardStat, OnlineReward);


