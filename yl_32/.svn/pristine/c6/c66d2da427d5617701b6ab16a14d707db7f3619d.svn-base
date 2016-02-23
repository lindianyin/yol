
if not (MODULE_GAMECLIENT or MODULE_GAMESERVER or MODULE_GC_SERVER) then
	return
end

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

-- 常量定义
local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

Chat.emKCHATCHANNEL_NONE		= IdAccumulator(0);
Chat.emKCHATCHANNEL_NEARBY		= IdAccumulator();
Chat.emKCHATCHANNEL_WORLD		= IdAccumulator();
Chat.emKCHATCHANNEL_GLOBAL		= IdAccumulator();
Chat.emKCHATCHANNEL_TEAM		= IdAccumulator();
Chat.emKCHATCHANNEL_KIN			= IdAccumulator();
Chat.emKCHATCHANNEL_GROUP		= IdAccumulator();
Chat.emKCHATCHANNEL_PRIVATE		= IdAccumulator();
Chat.emKCHATCHANNEL_GM			= IdAccumulator();
Chat.emKCHATCHANNEL_NUM			= IdAccumulator();


-- 系统消息类型
Chat.emKSYSTEMMSG_NORMAL = IdAccumulator(0);       -- 普通，聊天信息区
Chat.emKSYSTEMMSG_IMPORTANT = IdAccumulator();     -- 重要，中央提示
Chat.emKSYSTEMMSG_NOTICE = IdAccumulator();        -- 公告，中央滚动提示
Chat.emKSYSTEMMSG_NUM = IdAccumulator();