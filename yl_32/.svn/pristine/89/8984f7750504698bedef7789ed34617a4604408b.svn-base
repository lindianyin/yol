
-- 仅仅作为描述的目标
local tb	= Task:GetTarget("UserTrackInfo");
tb.szTargetName	= "UserTrackInfo";

function tb:Init(szTrackInfo)
	self.szTrackInfo = szTrackInfo;
end

function tb:Start()
	
end

function tb:Load()
	return 0;
end

function tb:Save()
	return 0;
end;

function tb:IsDone()
	return 1;
end;

function tb:GetDesc()
	return self:GetStaticDesc();
end;

function tb:GetStaticDesc()
	local szMsg = self.szTrackInfo;
	szMsg = Lib:ParseExpression(szMsg);
	szMsg = Task:ParseTag(szMsg);
	return szMsg;
end;

function tb:Close()
	
end
