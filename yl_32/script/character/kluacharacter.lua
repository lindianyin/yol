-- 这个是属于World层的
if (not MODULE_GAMESERVER and not MODULE_GAMECLIENT) then
	return;
end

local self;


function _KLuaCharacter.GetTempTable(szModelName)
	if (not szModelName) then
		print("获得玩家临时变量必须传入模块名", self, type(self), szModelName);
		print(debug.traceback());
		assert(false);
	end
	
	if (not Env.tbModelSet[szModelName]) then
		print("没有此模块名，查看scripttable.txt", szModelName)
		assert(false);
	end
	
	local tbTemp = self.GetCharacterTempTable();
	if (not tbTemp[szModelName]) then
		tbTemp[szModelName] = {};
	end
	
	return tbTemp[szModelName];
end
