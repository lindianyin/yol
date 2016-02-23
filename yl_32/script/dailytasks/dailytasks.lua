print("..DailyTasks");
function DailyTasks:c2s_GetAward(pPlayer)
	if pPlayer == nil then 
		return;
	end
	
	local num = Player.tbDegree:GetDegree(pPlayer, "DailyTasks") or 0;
	if num == 1 then
		KAward.AddPlayerAward(pPlayer, "DailyTasks", 1)
		Player.tbDegree:ReduceDegree(pPlayer, "DailyTasks", 1);
	end
end

--任务的组id列表
DailyTasks.tbGroupDailyTask = {6001,6002,6003,6004,6005};
--任务全局数据配置
DailyTasks.GLOBAL_GROUP = 6006;
--任务全局配置子id
DailyTasks.GLOBAL_GROUP_TOTAL_STAR = 1;
DailyTasks.GLOBAL_GROUP_REWARD1    = 2;
DailyTasks.GLOBAL_GROUP_REWARD2    = 3;
DailyTasks.GLOBAL_GROUP_REWARD3    = 4;
DailyTasks.GLOBAL_GROUP_REWARD4    = 5;
DailyTasks.GLOBAL_GROUP_OVER_TIMES = 6;
DailyTasks.GLOBAL_GROUP_FLUSH_DAY  = 7;
--任务全局数据配置里面奖励的状态
DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE = 0;
DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD = 1;
DailyTasks.GLOBAL_GROUP_REWARD_STAT_REWARDED = 2;



--任务子id
DailyTasks.TASK_ID    = 1;
DailyTasks.TASK_TYPE  = 2;
DailyTasks.TASK_STAT  = 3;
DailyTasks.TASK_VALUE = 4;
DailyTasks.TASK_SPECIAL = 5; --1:special 0: not 


--任务的状态
DailyTasks.STAT_NONE   = 1;   --init stat
DailyTasks.STAT_ACCEPT = 2;   --player accept task
DailyTasks.STAT_FINISH = 4;   --finish daily task and can get reward
DailyTasks.STAT_OVER   = 8;   --already get daily task reward




--每日任务类型
DailyTasks.TYPE_KILL_NPC = 1;         --	1	击杀指定怪物目标和数量  
DailyTasks.TYPE_JOIN_ACTIVITY = 2;    --	2	参与指定活动一定次数
DailyTasks.TYPE_BUY_ITEM = 3;         --	3	购买指定道具(数量)
DailyTasks.TYPE_HAND_IN_ITEM = 4;     --	4	上交（扣除）指定物品(数量)
DailyTasks.TYPE_ENHANCE_ITEM = 5;     --	5	强化装备指定次数
DailyTasks.TYPE_UPGRADE_SKILL = 6;     --	6	升级技能指定次数
DailyTasks.TYPE_REFINE_ITEM = 7;      --	7	洗练装备指定次数
DailyTasks.TYPE_TAKE_OUT_PET = 8;     --	8	抽取神仙指定次数
DailyTasks.TYPE_UPGREAD_PET = 9;      --	9	进阶神仙指定次数
DailyTasks.TYPE_EIGHTTRIGRAM = 10;    --	10	练卦指定次数
DailyTasks.TYPE_KILL_PLAYER = 11;     --	11  在蛮荒之地击杀指定个数玩家
DailyTasks.TYPE_RECHARGE = 12;        --	12  进行指定次数充值


DailyTasks.tbFileData = Lib:LoadTabFile("/setting/task/dailytask.txt");
DailyTasks.tbStarReward = Lib:LoadTabFile("/setting/task/dailytask_reward.txt");


function DailyTasks:initDailyTask(pPlayer,tbGroupTask,bAuto)
	print("..DailyTasks:initDailyTask");
	if pPlayer == nil then 
		return;
	end
	local nLevel = pPlayer.GetLevel();
	local nIdx = 1;
	local tbFileData = DailyTasks.tbFileData;
	for _,v in pairs(tbGroupTask) do 
		local tbIds = {};
		local tbDailyTask = {};
		for i=2,#tbFileData do 
			local tbRow = tbFileData[i];
			local nId = tonumber(tbRow.id);
			local nTaskType = tonumber(tbRow.type);
			local tbLevelRange = Lib:SplitStr(tbRow.level_range, "#");
			if(nLevel >= tonumber(tbLevelRange[1]) and nLevel <= tonumber(tbLevelRange[2]) and v == tonumber(tbRow.group_id)) then 
				tbDailyTask[nId] = tbRow;
				tbIds[#tbIds+1] = nId; 
			end
		end

		local nTaskStat = pPlayer.pTask.GetTask(v, DailyTasks.TASK_STAT);
		print("..nTaskStat",nTaskStat);
		if(DailyTasks.STAT_NONE == nTaskStat or DailyTasks.STAT_OVER == nTaskStat or nTaskStat == 0) then
			--Lib:ShowTB(tbIds);
			local tbSelectedIds = Lib:RandArray(tbIds, 1);
			--Lib:ShowTB(tbSelectedIds);
			pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_ID, tbSelectedIds[1]);
			--pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_TYPE,nTaskType);
			pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_STAT,DailyTasks.STAT_NONE);
			pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_VALUE, 0);
			pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_SPECIAL, 0);
			
			local tbRand = {A=25,B=75};
			local szKey = Lib:RandProb(tbRand);
			if(szKey == "A" and not bAuto) then 
				pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_SPECIAL, 1);
			end
		end
		
	end
	--pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_TOTAL_STAR, 0);
end


function DailyTasks:c2s_accept(pPlayer,nTaskGroupId)
	print("..DailyTasks:c2s_accept pPlayer.dwId,nTaskGroupId",pPlayer.dwId,nTaskGroupId);
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_NONE ~= nTaskStat) then
		print("..DailyTasks.STAT_NONE ~= nTaskStat");
		return;
	end
	--判断处在accept状态的任务为0
	local nCount = 0;
	for _,v in pairs(DailyTasks.tbGroupDailyTask) do
		local _nTaskStat = pPlayer.pTask.GetTask(v, DailyTasks.TASK_STAT);
		if(DailyTasks.STAT_ACCEPT == _nTaskStat) then
			nCount = nCount + 1;
		end
	end
	if(nCount >  0) then 
		print("..DailyTasks:c2s_accept","nCount > 0");
		pPlayer.SysMsg("..Only one task can be accepted");
		return;
	end
	pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_STAT,DailyTasks.STAT_ACCEPT);
end

function DailyTasks:c2s_getreward(pPlayer,nTaskGroupId) 
	print("..DailyTasks:c2s_getreward pPlayer.dwId,nTaskGroupId",pPlayer.dwId,nTaskGroupId);
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_FINISH ~= nTaskStat) then
		print("..DailyTasks.STAT_FINISH ~= nTaskStat");
		return
	end

	local nOverTimes = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_OVER_TIMES);
	pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_OVER_TIMES,nOverTimes+1);

	local tbFileData = DailyTasks.tbFileData; --Lib:LoadTabFile("/setting/task/dailytask.txt");
	local nTaskId = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_ID);
	local nSpecial = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_SPECIAL);

	
	for i=2,#tbFileData do
		local tbRow = tbFileData[i];
		local nId = tonumber(tbRow.id);
		local nlevel = tonumber(tbRow.level);
		local tbRewardMoney = {};
		local tbRewardItem = {};
		local nReward_exp = tonumber(tbRow.reward_exp);

		if(nSpecial == 1 and nReward_exp) then 
			nReward_exp = nReward_exp * 2;
		end
		
		for i=1,4 do 
			local szKey = "reward_money" .. tostring(i);
			local szVal = tbRow[szKey];
			if(szVal ~= "") then 
				tbRewardMoney[#tbRewardMoney+1] = szVal;
				if(nSpecial == 1) then 
					tbRewardMoney[#tbRewardMoney+1] = szVal;
				end
			end
		end

		for i=1,4 do 
			local szKey = "reward_item" .. tostring(i);
			local szVal = tbRow[szKey];
			if(szVal ~= "") then 
				tbRewardItem[#tbRewardItem+1] = szVal;
				if(nSpecial == 1) then 
					tbRewardItem[#tbRewardItem+1] = szVal;
				end
			end
		end
		
		if(nId == nTaskId) then
			
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
			
			local nTotalStar = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_TOTAL_STAR);
			local nTotalStar = nTotalStar + nlevel;
			if(nSpecial == 1) then 
				nTotalStar = nTotalStar + 1;
			end
			pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_TOTAL_STAR,nTotalStar);

			
			if(nTotalStar >= 15 and nTotalStar < 26) then 
				local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_REWARD1);
				if(DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE == nRewardStat) then 
					pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_REWARD1,DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD);
				end				
			elseif(nTotalStar >= 26 and nTotalStar <37) then
				local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_REWARD2);
				if(DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE == nRewardStat) then 
					pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_REWARD2,DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD);
				end	
			elseif(nTotalStar >= 37 and nTotalStar < 48) then
				local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_REWARD3);
				if(DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE == nRewardStat) then 
					pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_REWARD3,DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD);
				end	
			elseif(nTotalStar >= 48) then
				local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_REWARD4);
				if(DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE == nRewardStat) then 
					pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_REWARD4,DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD);
				end	
			end	
			
			break;
		end
	end
	
	pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_STAT,DailyTasks.STAT_OVER);
	DailyTasks:initDailyTask(pPlayer,{nTaskGroupId});
	
end

function DailyTasks:c2s_giveup(pPlayer,nTaskGroupId) 
	print("..DailyTasks:c2s_giveup pPlayer.dwId,nTaskGroupId",pPlayer.dwId,nTaskGroupId);
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_ACCEPT ~= nTaskStat) then
		print("..DailyTasks.STAT_ACCEPT ~= nTaskStat");
		return
	end
	pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_VALUE,0);
	pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_STAT,DailyTasks.STAT_NONE);
end


function DailyTasks:commitTaskValue(pPlayer,szParam1,nTaskType,nTaskValue) 
	print("..DailyTasks:commitTaskValue pPlayer.dwId,szParam1,nTaskType,nTaskValue",pPlayer.dwId,szParam1,nTaskType,nTaskValue);
	local tbFileData = DailyTasks.tbFileData;--Lib:LoadTabFile("/setting/task/dailytask.txt");
	for _,v in pairs(DailyTasks.tbGroupDailyTask) do
		local nTaskGroupId = v;
		local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
		--print("..DailyTasks.STAT_ACCEPT == nTaskStat",DailyTasks.STAT_ACCEPT == nTaskStat);
		if(DailyTasks.STAT_ACCEPT == nTaskStat) then 
			local nTaskId = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_ID);
			local tbRow = nil;
			for i=2,#tbFileData do 
				local _tbRow = tbFileData[i];
				local nId = tonumber(_tbRow.id);
				if(nId == nTaskId) then
					tbRow = _tbRow;
					break;
				end
			end
			
			local param1 = tbRow.param1;
			local param2 = tonumber(tbRow.param2);
			local nlevel  = tonumber(tbRow.level);
			local nType = tonumber(tbRow.type);
			if(szParam1 == param1 and  nTaskType == nType) then 
				local _nTaskValue = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_VALUE);
				local _nTmp = _nTaskValue + nTaskValue;
				pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_VALUE,_nTmp);

				--check can finish task
				if(_nTmp >= param2) then 
					pPlayer.pTask.DirectSetTask(v, DailyTasks.TASK_STAT,DailyTasks.STAT_FINISH);

					
				end
			end
			break;
		end
	end
end

--一键完成
function DailyTasks:c2s_directFinish(pPlayer,nTaskGroupId)
	print("..DailyTasks:c2s_directFinish pPlayer.dwId,nTaskGroupId",pPlayer.dwId,nTaskGroupId);
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_ACCEPT ~= nTaskStat) then
		print("..DailyTasks.STAT_ACCEPT ~= nTaskStat");
		pPlayer.SysMsg("..please accept task first");
		return;
	end

	--扣元宝
	
	local tbFileData = DailyTasks.tbFileData;--Lib:LoadTabFile("/setting/task/dailytask.txt");
	local nTaskId = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_ID);
	
	for i=2,#tbFileData do
		local tbRow = tbFileData[i];
		local nCost = tonumber(tbRow.cost);
		if(tonumber(tbRow.id) == nTaskId) then
			Dialog:_AskForSure(pPlayer.dwId, string.format(Lang.mission.str1297[Lang.Idx], nCost), ({"DailyTasks:directFinish", pPlayer, nTaskGroupId,nCost}), "DailyTasks:directFinish", 1);			
			break;
		end
	end
	--pPlayer.pTask.DirectSetTask(nTaskGroupId, DailyTasks.TASK_STAT,DailyTasks.STAT_FINISH);
	
end

function DailyTasks:directFinish(pPlayer,nTaskGroupId,nCost)
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_ACCEPT ~= nTaskStat) then
		print("..DailyTasks.STAT_ACCEPT ~= nTaskStat");
		pPlayer.SysMsg("..please accept task first");
		return;
	end
	local tbFileData = DailyTasks.tbFileData; --Lib:LoadTabFile("/setting/task/dailytask.txt");
	local nTaskId = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_ID);
		-- 扣元宝
	local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	
	if (nCurCoin < nCost) then
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", nCost});
		return 0;
	end
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nCost);
	
	for i=2,#tbFileData do
		local tbRow = tbFileData[i];
		if(tonumber(tbRow.id) == nTaskId) then
			DailyTasks:commitTaskValue(pPlayer,tbRow.param1,tonumber(tbRow.type),tonumber(tbRow.param2));
			break;
		end
	end

end






--领取累计星级奖励
function DailyTasks:c2s_getStarAward(pPlayer,nIdx)
	print("..DailyTasks:c2s_getStarAward pPlayer.dwId,nIdx",pPlayer.dwId,nIdx);

	local szKey = "GLOBAL_GROUP_REWARD" .. tostring(nIdx);
	local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks[szKey]);
	if(DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD ~= nRewardStat) then 
		print("..DailyTasks.GLOBAL_GROUP_REWARD_STAT_CAN_REWARD ~= nRewardStat");
		return;
	end
	


	local tbFileData = DailyTasks.tbStarReward;--Lib:LoadTabFile("/setting/task/dailytask_reward.txt");
	
	for i=2,#tbFileData do
		local tbRow = tbFileData[i];
		local nId = tonumber(tbRow.id);
		print("nId = ",nId);
		local tbRewardMoney = {};
		local tbRewardItem = {};
		local nReward_exp = tonumber(tbRow.reward_exp);

		if(nId == nIdx) then
			print("..nId == nIdx",nId == nIdx);
			for i=1,4 do 
				local szKey = "reward_money" .. tostring(i);
				local szVal = tbRow[szKey];
				if(szVal ~= "") then 
					tbRewardMoney[#tbRewardMoney+1] = szVal;
				end
			end

			for i=1,4 do 
				local szKey = "reward_item" .. tostring(i);
				local szVal = tbRow[szKey];
				if(szVal ~= "") then 
					tbRewardItem[#tbRewardItem+1] = szVal;
				end
			end
		
			local nBagFreeCell = pPlayer.pItem.CountFreeBagCell();
			if nBagFreeCell < #tbRewardItem then
				print("..nBagFreeCell < #tbRewardItem",nBagFreeCell < #tbRewardItem);
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
				Lib:ShowTB(_tbNum);
				KItem.AddPlayerItem(pPlayer,unpack(_tbNum));
			end

			pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks[szKey],DailyTasks.GLOBAL_GROUP_REWARD_STAT_REWARDED);
			break;

		end
		
	end
end


--全部刷新功能
function DailyTasks:c2s_reflashAllTask(pPlayer)
	print("..DailyTasks:c2s_reflashAllTask pPlayer.dwId",pPlayer.dwId);
	--扣刷新符 或者扣元宝
	local tbCostGDPL = { 18, 1, 687 }; --刷新符
	local tbFind =  pPlayer.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nCount = Item:GetItemAmount(tbFind);
	local nCost = 50;
	
	if nCount == 0 then --询问是否使用元宝
		Dialog:_AskForSure(pPlayer.dwId, string.format(Lang.mission.str1299[Lang.Idx], nCost), ({"DailyTasks:reflashAllTask", pPlayer,nCost}), "DailyTasks:reflashAllTask", 1);
		return;
	end
	
	if nCount > 0 then
		local nCostReflashcardNum = 1;
		Dialog:_AskForSure(pPlayer.dwId, string.format(Lang.mission.str1298[Lang.Idx]), ({"DailyTasks:consumeReflashcard", pPlayer,nCostReflashcardNum}), "DailyTasks:consumeReflashcard", 1);
	end
end

function DailyTasks:reflashAllTask(pPlayer,nCost)
	local nCurCoin = pPlayer.pPlayerPurse.GetMoney(Purse.EM_MONEY_COIN);
	
	if (nCurCoin < nCost) then
		pPlayer.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowCoinNotEnoughPanel", "", nCost});
		return;
	end
	pPlayer.pPlayerPurse.AddMoney(Item.EM_MONEY_COIN, -1 * nCost);
	DailyTasks:initDailyTask(pPlayer,DailyTasks.tbGroupDailyTask,true);
	return;
end

function DailyTasks:consumeReflashcard (pPlayer,nCostReflashcardNum)
	local tbCostGDPL = { 18, 1, 687 }; --刷新符
	local tbFind = pPlayer.pItem.FindItemInBags(unpack(tbCostGDPL));
	local nCount = Item:GetItemAmount(tbFind);
	if nCount > 0 then
		pPlayer.pItem.ConsumeItemInBags(nCostReflashcardNum, unpack(tbCostGDPL));
		DailyTasks:initDailyTask(pPlayer,DailyTasks.tbGroupDailyTask);
	end
	return;
end

--上交道具
function DailyTasks:c2s_handin_item(pPlayer,nTaskGroupId)
	print("..DailyTasks:c2s_handin_item pPlayer.dwId",pPlayer.dwId);
	local nTaskStat = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_STAT);
	if(DailyTasks.STAT_ACCEPT ~= nTaskStat) then
		print("..DailyTasks.STAT_ACCEPT ~= nTaskStat");
	end
	local tbFileData = DailyTasks.tbFileData;--Lib:LoadTabFile("/setting/task/dailytask.txt");
	local nTaskId = pPlayer.pTask.GetTask(nTaskGroupId, DailyTasks.TASK_ID);
	for i=2,#tbFileData do
		local tbRow = tbFileData[i];
		local nId = tonumber(tbRow.id);
		if(nId == nTaskId) then 
			local szParam1 = tbRow.param1;
			local szParam2 = tbRow.param2;
			local tbParam1 = Lib:SplitStr(szParam1, "#");
			local tbParam1Num = {};
			for _,v in pairs(tbParam1) do 
				tbParam1Num[#tbParam1Num + 1] = tonumber(v);
			end
			local nCount = Item:GetItemCount(pPlayer,unpack(tbParam1Num));
			local nNeedCount = tonumber(szParam2);
			if(nCount >= nNeedCount) then 
				pPlayer.pItem.ConsumeItemInBags(nNeedCount, unpack(tbParam1Num));
				DailyTasks:commitTaskValue(pPlayer,szParam1,DailyTasks.TYPE_HAND_IN_ITEM,nNeedCount);
			else
				pPlayer.SysMsg("..item is not enough");
			end
			break;
		end
	end
end


function DailyTasks:resetStarReward(pPlayer)
	print("..DailyTasks:resetStarReward pPlayer.dwId",pPlayer.dwId);
	for i=1,4 do 
		local szKey = "GLOBAL_GROUP_REWARD" .. tostring(i);
		local nRewardStat = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks[szKey]);
		pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks[szKey],DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE);	
		
	end
	pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_TOTAL_STAR,DailyTasks.GLOBAL_GROUP_REWARD_STAT_NONE);
	pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_OVER_TIMES,0);
	
end


function DailyTasks:resetDailyTaskWhenChangeDay(pPlayer)
	print("DailyTasks:resetDailyTaskWhenChangeDay pPlayer.dwId",pPlayer.dwId);
	local nNowDay = Lib:GetLocalDay();
	local nFlushDay = pPlayer.pTask.GetTask(DailyTasks.GLOBAL_GROUP,DailyTasks.GLOBAL_GROUP_FLUSH_DAY);
	if(nFlushDay == 0 or (nNowDay - nFlushDay) >= 1) then 
		DailyTasks:resetStarReward(pPlayer);
		DailyTasks:initDailyTask(pPlayer,DailyTasks.tbGroupDailyTask,true);
		pPlayer.pTask.DirectSetTask(DailyTasks.GLOBAL_GROUP, DailyTasks.GLOBAL_GROUP_FLUSH_DAY,nNowDay);
	end
end




