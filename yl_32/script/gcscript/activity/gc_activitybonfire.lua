if not (MODULE_GAMECENTER) then
	return
end

Activity.tbBonfireInfo = {};	--篝火状态

function Activity:UpdateBonfireInfo_GC(nKinId, tbBonfireInfo)

	self.tbBonfireInfo[nKinId] = Lib:CopyTBN(tbBonfireInfo);
	
	GlobalExecute({"Activity:UpdateBonfireInfo_GS", nKinId, self.tbBonfireInfo[nKinId]});
end
