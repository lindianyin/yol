Include("script/mission/activity/activitydef.lua")

local tbNpc = Npc:GetClass("enterworldboss")

-- added here
local tbActivityTargets = { -- 活动索引，活动步骤索引
--npcTemplatId = {{Activity1Id,Step1Id,NeedDialog},{Activity2Id,Step2Id,NeedDialog},...},

	[2500] = {{15, 6, 0}},  -- 世界BOSS
	[2501] = {{15, 4, 0}},  -- 世界BOSS
	[2502] = {{15, 3, 0}},  -- 世界BOSS
	[2503] = {{15, 5, 0}},  -- 世界BOSS
	[2504] = {{15, 2, 0}},  -- 世界BOSS
	
	-- 戰場， 活動id 10， step 1， 可存放多个活动
};


local tbActivitySetting = Activity.ActivitySetting or {};

function tbNpc:OnDialog(pDialogNpc)
	local nTemplateId = pDialogNpc.dwTemplateId;
	local nActivity = tbNpc:GetActivity(nTemplateId);-- 对应targets中的序号(非活动ID)
	local tbOpt = {
		{Lang.npc.str66[Lang.Idx],self.GoToActivity,self,nTemplateId,nActivity},
		{Lang.npc.str67[Lang.Idx]},
	}

	if (nActivity and nActivity == 0) then
		if #tbActivityTargets[nTemplateId][1] and tbActivityTargets[nTemplateId][1][3] ~= 0 then
			Dialog:Say(Lang.npc.str68[Lang.Idx]);
		else
			Dialog:SendBlackBoardMsg(me, Lang.npc.str69[Lang.Idx]);
			return;
		end
	elseif (nActivity and tbActivityTargets[nTemplateId][nActivity] and type(tbActivityTargets[nTemplateId][nActivity]) == "table") then  -- 通过序号，获取activityTarget Table
		if #tbActivityTargets[nTemplateId][nActivity] == 3 and tbActivityTargets[nTemplateId][nActivity][3] ~= 0 then
			Dialog:Say(Lang.npc.str70[Lang.Idx],tbOpt);
		else
			tbNpc:GoToActivity(nTemplateId,nActivity);
		end
	end
end

function tbNpc:RandomSteps()
    tbActivitySetting[15].tbWorldBossRandSteps = {};
	local tbSteps = {3,4,5,6};
	while #tbSteps > 0 do
		local nRandom = KUnify.MathRandom(1,#tbSteps);
		print(tbSteps[nRandom], tbActivitySetting[15].tbWorldBossRandSteps);
		table.insert(tbActivitySetting[15].tbWorldBossRandSteps, tbSteps[nRandom]);
		table.remove(tbSteps, nRandom);
	end
	
	local nTemplateId = 2500;
	for _, v in pairs(tbActivitySetting[15].tbWorldBossRandSteps) do
		local tbActivityTarget = tbActivityTargets[nTemplateId][1];
		tbActivityTarget[2] = v;
		nTemplateId = nTemplateId + 1;
		print("____", v); 
	end
end
-- 执行活动, 传入npc模板id， 活动Target序号
function tbNpc:GoToActivity(nTemplateId,nActivity)
	if #(tbActivitySetting[15].tbWorldBossRandSteps) <=0 then
		--打乱步骤
		tbNpc:RandomSteps();
	end
	print("tbNpc:OnDialog", nTemplateId, tbActivityTargets[nTemplateId][nActivity], #tbActivityTargets[nTemplateId][nActivity])
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
	return 0;
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

