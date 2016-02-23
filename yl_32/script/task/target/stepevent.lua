
local tb	= Task:GetTarget("StepEvent");
tb.szTargetName	= "StepEvent";


function tb:Init(szMsg)
	self.szMsg = szMsg;
end;


function tb:Start()
	Dialog:SendInfoBoardMsg(self.me, "<color=Yellow>"..self.szMsg.."<color>");
end;

function tb:Save(nGroupId, nStartTaskId)
	return 0;
end;


function tb:Load(nGroupId, nStartTaskId)
	return 0;
end;


function tb:IsDone()
	return 1;
end;


function tb:GetDesc()
	return "";
end;


function tb:GetStaticDesc()
	return "";
end;

function tb:Close(szReason)
end;

