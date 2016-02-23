if not (MODULE_GAMECENTER) then
	return
end

--[[
Include("/script/gcscript/gcconfig.lua");
Include("/script/chat/chatchanneldef.lua")

local szChatChannelFile = "/setting/chat/channelcfg_new.ini";
Chat.tbChatChannelCfg = {};
local tbConfig = Lib:LoadIniFile(szChatChannelFile);
for k, v in pairs(tbConfig) do
	Chat.tbChatChannelCfg[tonumber(k)] = v;
	for szKey, value in pairs(v) do
		if (string.sub(szKey,0,1) == "n") then
			v[szKey] = tonumber(value);
		end
	end
	
end

function Chat:GetDispathInterfaceId(nChannelId)
	local szModuleName = self.tbChatChannelCfg[nChannelId].szDispatchInterface;

	if (szModuleName) then
		return KGCMODULE_DEF[szModuleName];
	end
	
	return 0;
end
--]]

function Chat:ApplyDisableChat_GC(nSenderId, szName)
	GlobalExecute({"Chat:DoDisableChat_GS", nSenderId, szName});
end


