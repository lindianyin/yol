if not (MODULE_GAMECENTER) then
	return
end

function Activity:UpdateManHuangZhiDi_GC(nScenId, tbManHuangZhiDi)

	local tbBoss = Lib:CopyTBN(tbManHuangZhiDi);
	
	GlobalExecute({"Activity:UpdateManHuangZhiDi_GS", nScenId, tbBoss});
end
