
local tbNpc = Npc:GetClass("canjiahuodong")

-- added here
local tbActivityTargets = { -- 活动索引，活动步骤索引
--npcTemplatId = {{Activity1Id,Step1Id,NeedDialog},{Activity2Id,Step2Id,NeedDialog},...},
	[2520] = {{2, 3, 0}}, --战宝堂一层入口
	[2521] = {{2, 4, 0}}, --战宝堂一层入口
	[2522] = {{2, 5, 0}}, --战宝堂一层入口
	[2523] = {{2, 6, 0}}, --战宝堂一层入口
	[2524] = {{2, 7, 0}}, --战宝堂二层入口
	[2525] = {{2, 7, 0}}, --战宝堂二层入口
	[2526] = {{2, 8, 0}}, --战宝堂二层入口
	[2527] = {{2, 8, 0}}, --战宝堂二层入口
	[2528] = {{2, 9, 0}}, --战宝堂三层入口
	[2529] = {{2, 9, 0}}, --战宝堂三层入口
	[6102] = {{10, 1, 1}},  -- 战神无双


	-- 戰場， 活動id 10， step 1， 可存放多个活动
};

local tbActivitySetting = Activity.ActivitySetting or {};

function tbNpc:OnDialog(pDialogNpc)
	local nTemplateId = pDialogNpc.dwTemplateId;
	--print("nTemplateId",nTemplateId);
	local nActivity = tbNpc:GetActivity(nTemplateId);-- 对应targets中的序号(非活动ID)
	--print("nActivity",nActivity);
	local tbOpt = {
		{Lang.npc.str34[Lang.Idx],self.GoToActivity,self,nTemplateId,nActivity},
		{Lang.npc.str67[Lang.Idx]},
	}
	--print("活动参数：",nActivity);
	if (nActivity and nActivity == 0) then
		if #tbActivityTargets[nTemplateId][1] and tbActivityTargets[nTemplateId][1][3] ~= 0 then
			Dialog:Say(Lang.npc.str36[Lang.Idx]);
		else
			--print("活动步骤不对");
			Dialog:SendBlackBoardMsg(me, Lang.npc.str37[Lang.Idx]);
			--me.CallClientScript({"YoulongApp.UI.Controller.QUIClientScript:ShowNoticeMessage", "时间不正确，无法加入该活动"});
			return;
		end
	elseif (nActivity and tbActivityTargets[nTemplateId][nActivity] and type(tbActivityTargets[nTemplateId][nActivity]) == "table") then  -- 通过序号，获取activityTarget Table
		if #tbActivityTargets[nTemplateId][nActivity] == 3 and tbActivityTargets[nTemplateId][nActivity][3] ~= 0 then
			Dialog:Say(Lang.npc.str37[Lang.Idx],tbOpt);
		else
			tbNpc:GoToActivity(nTemplateId,nActivity);
		end
	end
end

function tbNpc:CheckCanEnterActivity(pPlayer, nTemplateId, nActivity)
	local tbActivityTarget = tbActivityTargets[nTemplateId][nActivity];
	local nActivityIndex = tbActivityTarget[1];
	
	local nMapId = pPlayer.dwSceneTemplateId;
	local szSceneType = KScene.GetSceneTypeName(nMapId);
	
	if nActivityIndex == Activity.ACTIVITY_INDEX_ZBT and szSceneType == "activity_zb" then
		--local dwLastEnterTime = KActivity.GetLastEnterZBTTime(pPlayer);
		local dwNow = KGameBase.GetTime();
		local dwStartTime = Activity:GetStepStartTime(nActivityIndex, Activity.ActivitySetting[nActivityIndex].StartStep);
		local nPrepareTime = Activity.ActivitySetting[nActivityIndex].nPrepareTime;
		if Lib:GetLocalDay(dwStartTime) == Lib:GetLocalDay(dwNow) and dwNow > dwStartTime + nPrepareTime then
			pPlayer.SendBlackMsg(Lang.npc.str38[Lang.Idx]);
			return 0;
		end
	end
	
	return 1;
end

-- 执行活动, 传入npc模板id， 活动Target序号
function tbNpc:GoToActivity(nTemplateId,nActivity)
	print("tbNpc:OnDialog", nTemplateId, tbActivityTargets[nTemplateId][nActivity], #tbActivityTargets[nTemplateId][nActivity])
	
	--if self:CheckCanEnterActivity(me, nTemplateId, nActivity) ~= 1 then
	--	return;
	--end
	
	if (tbActivityTargets[nTemplateId][nActivity] and type(tbActivityTargets[nTemplateId][nActivity]) == "table" and #tbActivityTargets[nTemplateId][nActivity] >= 2) then
		local tbActivityTarget = tbActivityTargets[nTemplateId][nActivity];
			
		Activity:JoinActivity(me.dwId, tbActivityTarget[1], tbActivityTarget[2]);
	end
end;

-- 根据npcid 获取活动 序号
function tbNpc:GetActivity(nTemplateId)
	local nStartTime = 0;
	local nStepTime = 0;
	local tNow = KGameBase.GetTime();
	
	for i = 1, #tbActivityTargets[nTemplateId] do
		--print("start to get step list!")
		local tbStepList = tbNpc:GetStepRelation(nTemplateId,i);
		local nActivityId = tbActivityTargets[nTemplateId][i][1];
		local nStepId = tbActivityTargets[nTemplateId][i][2];
		
		local nGetStartSecend = nil;
		
		for _, tbOpenTime in pairs(tbActivitySetting[nActivityId].tbOpenTimes) do
			if Activity:IsOpenTime(tbOpenTime) == 1 then
				nGetStartSecend = Lib:GetSecByTimeTable(tbOpenTime[1]);
				break;
			end
		end
		
		--print("hour:",nStartHour,"min",nStartMin,"sec",nStartSec);
		--print("开始秒数：",nGetStartSecend);
		if tbStepList and #tbStepList ~= 0 then
			for j = 1, #tbStepList do
				if tbActivitySetting[nActivityId].STEP[tbStepList[j]].Time then
					nStepTime = nStepTime + tbActivitySetting[nActivityId].STEP[tbStepList[j]].Time;
				end
			end
		end

		local nStepStartTime = nGetStartSecend + nStepTime;
		local nStepEndTime = nStepStartTime + tbActivitySetting[nActivityId].STEP[nStepId].Time;
		--print("阶段开始秒数：",nStepStartTime);
		--print("阶段结束秒数：",nStepEndTime);

		--tbNpc:GoToActivity(nTemplateId);
		if tNow >= nStepStartTime and tNow <= nStepEndTime then
			return i;
		end
	end
end

function tbNpc:GetStepRelation(nTemplateId,nIndex)
	local nActivityId = tbActivityTargets[nTemplateId][nIndex][1];
	local nStepId = tbActivityTargets[nTemplateId][nIndex][2];
	local tbNextStep = {};
	local tbPreStep = {};
	local tbStepList = {};
	local StepInfo = Activity.ActivitySetting[nActivityId].STEP;
	for i = 1, #StepInfo do  --获取所有锁的后续锁
		tbNextStep[#tbNextStep + 1] = StepInfo[i].NextSteps or {};
	end

	tbPreStep[1] = 0;
	for i = 1, #tbNextStep do  --获取所有锁的前驱锁
		if #tbNextStep[i] ~= 0 then
			for j = 1, #tbNextStep[i] do
				for k = 2, #tbNextStep do
					if tbNextStep[i][j] == k then
						if not tbPreStep[k] then
							tbPreStep[k] = i;
						end
					end
				end
			end
		end
	end

	--print("tbPreStep");
	--Lib:ShowTB(tbPreStep);
	local nGetStepId = 0;
	local nFlag = nStepId;
	if nFlag ~= 1 then
		repeat --获取目标锁的所有前驱链
			nGetStepId = tbPreStep[nFlag];
			print("nGetStepId",nGetStepId);
			tbStepList[#tbStepList + 1] = nGetStepId;
			nFlag = nGetStepId;
		until  tbPreStep[nFlag] == 0;
	else
		return tbStepList;
	end
	--print("nStepId",nStepId);
	--print("tbStepList");
	--Lib:ShowTB(tbStepList);
	return tbStepList;
end


