
local tb	= Task:GetTarget("TipPopo");
tb.szTargetName	= "TipPopo";


function tb:Init(nGroupId)
	self.nGroupId = nGroupId;
end;


function tb:Start()
	self.me.CallClientScript({"PopoTip:ShowPopo", self.nGroupId});
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
	self.me.CallClientScript({"PopoTip:HidePopo", self.nGroupId});
end;

