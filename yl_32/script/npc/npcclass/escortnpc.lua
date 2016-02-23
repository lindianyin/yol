local tbNpc = Npc:GetClass("escortnpc")

function tbNpc:OnDialog()
	me.CallClientScript({"QClientScript.Escort:OpenEscortPanel"});
end;
