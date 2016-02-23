if (not MODULE_GAMESERVER and not MODULE_GAMECLIENT) then
	return;
end

Include("/script/character/charactertaskdef.lua")

local self;
for nTypeGroupId, tbIdGroup in pairs(Character.Var) do
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
			local funRawSet = 
				function (nValue)
					return self.RawSetVar(nTypeGroupId, tbId[1], nValue)
				end
			
			rawset(_KLuaCharacter, "Get"..tbId[2], funGet)
			rawset(_KLuaCharacter, "Set"..tbId[2], funSet)
			rawset(_KLuaCharacter, "RawSet"..tbId[2], funRawSet)	
		end
	end
end
