-- 关卡常量定义

local pre_env = _G;
setfenv(1, MultiInstance);

DO_SCRIPT	  		= 1;		-- 执行脚本
ADD_NPC      		= 2;
BLACK_MSG			= 3;	-- 黑条字模 	
TARGET_INFO			= 4;	-- 目标信息 
DEL_NPC 			= 5;
NPC_BUBBLE 			= 6;
TIME_INFO 			= 7;
REVIVE_INFO 		= 8;
SHOW_NOTICE_MSG 	= 9;
CLEAR_BUFF 			= 10;
SET_TRAP_INVALID 	= 11;
CLEAR_NPC_BUFF		= 12;
SHOW_NOTICE_STATE = 13;
REMOVE_NOTICE_STATE = 14;
DEL_ALL_NPC = 15;

BUY_RUSH_COST = 10;

AWARD_MODULE_NAME = "MultiInstance";

pre_env.setfenv(1, pre_env);

-- 多人副本派生

MultiInstance.GAME_TYPE = {}
MultiInstance.GAME_TYPE.BASE = 1           -- 基础
MultiInstance.GAME_TYPE.LV_20_EASY = 2   -- 塔防

MultiInstance.GAME_FACTORY = {};		-- 派生工厂

function MultiInstance.GAME_FACTORY:Create(nGameType)
	return Lib:NewClass(self[nGameType]);
end

MultiInstance.VIP_MAP_IDS = 
{
	[600] = 1,
	[601] = 1,
	[602] = 1,
	[603] = 1,
}

-- 读取副本等级配置

function MultiInstance:GetMultiMinLevel(nMapId)
	if not self.tbMultiCfg[nMapId] then
		return 0;
	end
	
	return self.tbMultiCfg[nMapId].nMinLevel;
end

function MultiInstance:GetMultiDegreeName(nMapId)
	if not self.tbMultiCfg[nMapId] then
		return 0;
	end
	
	return self.tbMultiCfg[nMapId].szDegree;
end

function MultiInstance:GetMultiName(nMapId)
	if not self.tbMultiCfg[nMapId] then
		return 0;
	end
	
	return self.tbMultiCfg[nMapId].szMultiName;
end

function MultiInstance:GetEnterNeedItem(nMapId)
	if not self.tbMultiCfg[nMapId] then
		return 0;
	end
	
	return self.tbMultiCfg[nMapId].tbNeedItem;
end 

function MultiInstance:LoadMultiInstanceCfg()
	if not self.tbMultiCfg then
		self.tbMultiCfg = {};
		
		local tbData = Lib:LoadTabFile("/setting/instance/multi_cfg.txt");
		for _, row in pairs(tbData) do
			row.nMapId = tonumber(row.MapId);
			row.nMinLevel = tonumber(row.MinLevel);
			row.szDegree = row.Degree;
			row.szMultiName = row.Name;
			
			if row.EnterNeedItem ~= "" then
				local tbItem = Lib:SplitStr(row.EnterNeedItem, ",");
				row.tbNeedItem = {};
				row.tbNeedItem.nGenre = tonumber(tbItem[1]);
				row.tbNeedItem.nDetail = tonumber(tbItem[2]);
				local tbP_Amount = Lib:SplitStr(tbItem[3], ":");
				row.tbNeedItem.nParticular = tonumber(tbP_Amount[1]);
				row.tbNeedItem.nAmount = tonumber(tbP_Amount[2]);
			end
			
			self.tbMultiCfg[row.nMapId] = row;
		end
	end
end

MultiInstance:LoadMultiInstanceCfg();
