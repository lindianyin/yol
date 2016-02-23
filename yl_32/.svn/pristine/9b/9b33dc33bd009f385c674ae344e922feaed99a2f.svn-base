
local tbNpc = Npc:GetClass("battlefield_zhanshentang")

-- added here
local tbActivityList = { -- 活动索引，活动步骤索引
--npcTemplatId = {{Activity1Id,Step1Id,NeedDialog},{Activity2Id,Step2Id,NeedDialog},...},
		{20, 21, 22, 23},  --时间1
		--{{26, 27, 28, 29, 30, 31}},  --时间2
		--{{32, 33, 34, 35, 36, 37}},  --时间3
		--{{38, 39, 40, 41, 42, 43}},  --时间4
		--{{44, 45, 46, 47, 48, 49}},  --时间5
		--{{50, 51, 52, 53, 54, 55}},  --时间6
};

local ZHANSHENTANGSTEP = 2;  --加入战神堂对应的活动步骤
local WUHUNJUSTEP = 3;  --加入武魂居对应的活动步骤
--local tbBattleScene = {840,841,842,843,844,845};
--local tbBattlePos ={7300,623,1048576};

local tbActivitySetting = Activity.ActivitySetting or {};

function tbNpc:OnDialog()
	local nActivityGroup,nAlreadyStart = tbNpc:GetActivity();
	local tbOpt = {
		{Lang.npc.str12[Lang.Idx],self.GoToActivity,self, nActivityGroup, tonumber(1), nAlreadyStart},
		{Lang.npc.str13[Lang.Idx],self.GoToActivity,self, nActivityGroup, tonumber(2), nAlreadyStart},
		{Lang.npc.str14[Lang.Idx],self.GoToActivity,self, nActivityGroup, tonumber(3), nAlreadyStart},
		{Lang.npc.str15[Lang.Idx],self.GoToActivity,self, nActivityGroup, tonumber(4), nAlreadyStart},
		--{"进入五号战场",self.GoToActivity,self,nActivityGroup,5,nAlreadyStart},
		--{"进入六号战场",self.GoToActivity,self,nActivityGroup,6,nAlreadyStart},
		{Lang.npc.str16[Lang.Idx]},
	}
	--print("活动参数：",nActivity);
	if (nActivityGroup and nActivityGroup == 0) then
		Dialog:Say(Lang.npc.str17[Lang.Idx]);
	elseif (nActivityGroup and tbActivityList[nActivityGroup]) then
		Dialog:Say(Lang.npc.str18[Lang.Idx],tbOpt);
	end
end

function tbNpc:GoToActivity(nActivityGroup,nIndex,nAlreadyStart)
	local nPlayerNum1 = Activity:GetActivityStepPlayerNum(tbActivityList[nActivityGroup][nIndex], ZHANSHENTANGSTEP) or 0;
	local nPlayerNum2 = Activity:GetActivityStepPlayerNum(tbActivityList[nActivityGroup][nIndex], WUHUNJUSTEP) or 0;
	local tbOpt = {
		{Lang.npc.str19[Lang.Idx],self.GoToActivity2,self,nActivityGroup,nIndex,nAlreadyStart,nPlayerNum1},
		{Lang.npc.str20[Lang.Idx]}
	}
	local szMsg1 = Lang.npc.str21[Lang.Idx];
	local szMsg2 = Lang.npc.str22[Lang.Idx];
	local szMsg = string.format(Lang.npc.str23[Lang.Idx], nPlayerNum1, nPlayerNum2);
	if nAlreadyStart == 0 then
		szMsg = szMsg1..szMsg;
	else
		szMsg = szMsg2..szMsg;
	end
	Dialog:Say(szMsg,tbOpt);
end

function tbNpc:GoToActivity2(nActivityGroup,nIndex,nAlreadyStart,nPlayerNum)
	--print("tbNpc:OnDialog", nTemplateId, tbActivityList[nTemplateId][nActivity], #tbActivityList[nTemplateId][nActivity])
	local nActivityTarget = tbActivityList[nActivityGroup][nIndex];
	if nAlreadyStart == 0 then
		Activity:JoinActivity(me.dwId, nActivityTarget, ZHANSHENTANGSTEP);
	else
		if nPlayerNum >= 20 then
			Dialog:Say(Lang.npc.str26[Lang.Idx]);
		else
			Dialog:Say(Lang.npc.str25[Lang.Idx]);
		end
	end
end

function tbNpc:GetActivity()
	local nStartTime = 0;
	local nStepTime = 0;
	local nTime = KGameBase.GetTime();
	local nCurrentSec = Lib:GetLocalDayTime(nTime);
	--print("当前秒数：",nCurrentSec);
	
	--local nCurHour = math.floor(nCurrentSec / 3600);
	--local nCurMin = math.floor(nCurrentSec % 3600 / 60);
	--local nCurSec = math.floor(nCurrentSec % 60);
	
	--local nCurT = nCurSec + nCurMin * 100 + nCurHour * 10000;
	--print("hour:",nCurHour,"min",nCurMin,"sec",nCurSec);
	--print("当前时间：",nCurT);
	local nStepId = ZHANSHENTANGSTEP;
	for i = 1, #tbActivityList do
		local nActivityId = tbActivityList[i][1];
		--local nStepId = tbActivityList[nTemplateId][i][2];
		
		if tbActivitySetting[nActivityId].StartTime then
			nStartTime = tbActivitySetting[nActivityId].StartTime;
		end
		local nStartSec = math.floor(nStartTime % 100);
		local nStartMin = math.floor(nStartTime / 100 % 100);
		local nStartHour = math.floor(nStartTime / 10000);		
		local nGetStartSecend = nStartHour * 3600 + nStartMin * 60 + nStartSec;
		
		--print("hour:",nStartHour,"min",nStartMin,"sec",nStartSec);
		--print("开始秒数：",nGetStartSecend);
		if tbActivitySetting[nActivityId].STEP[1].Time then
			nStepTime = nStepTime + tbActivitySetting[nActivityId].STEP[1].Time;
		end
		
		local nStepStartTime = nGetStartSecend + nStepTime;
		local nStepEndTime = nStepStartTime + tbActivitySetting[nActivityId].STEP[nStepId].Time;
		local nActivityEndTime = nStepEndTime + tbActivitySetting[nActivityId].STEP[4].Time;
		--print("阶段开始秒数：",nStepStartTime);
		--print("阶段结束秒数：",nStepEndTime);
		
		--tbNpc:GoToActivity(nTemplateId);
		if nCurrentSec >= nStepStartTime and nCurrentSec < nStepEndTime then
			return i, 0;
		elseif	nCurrentSec >= nStepEndTime and nCurrentSec <= nActivityEndTime then
			return i, 1;
		end
	end
	return 0, 0;
end

