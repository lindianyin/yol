if (not MODULE_GAMESERVER and not MODULE_GAMECLIENT) then
	return;
end

Include("/script/npc/npctaskdef.lua")

local self;
for nTypeGroupId, tbIdGroup in pairs(Npc.Var) do
	for _, tbId in pairs(tbIdGroup) do
		if (tbId[2] ~= "") then
			local funGet = 
				function ()
					return self.GetVar(nTypeGroupId, tbId[1])
				end
			local funSet = 
				function (nValue)
					return self.SetVar(nTypeGroupId, tbId[1], nValue)
				end
			rawset(_KLuaNpc, "Get"..tbId[2], funGet)
			rawset(_KLuaNpc, "Set"..tbId[2], funSet)	
		end
	end
end
