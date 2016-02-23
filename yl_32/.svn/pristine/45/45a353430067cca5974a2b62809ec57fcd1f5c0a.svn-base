if not (MODULE_GAMECENTER) then
	return
end


function Activity:SetActivityOpenTime(nActivityIndex, tbStartTime, tbEndTime)
	GlobalExecute({"Activity:SetActivityOpenTime", nActivityIndex, tbStartTime, tbEndTime});
end

function Activity:OnSetKinActivityOpenTime_GC(nKinId)
	local tbMembers = GetTongMembers(nKinId);
	
	for _, nPlayerId in ipairs(tbMembers) do
		GlobalExecute({"Activity:OnSetKinActivityOpenTime_GS", nPlayerId});
	end
end