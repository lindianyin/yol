-- 关卡常量定义

local pre_env = _G;
setfenv(1, Stage);

DO_SCRIPT	  	= 1;		-- 执行脚本
ADD_NPC      	= 2;
BLACK_MSG		= 3;	-- 黑条字模 	
TARGET_INFO		= 4;	-- 目标信息 
DEL_NPC 		= 5;
NPC_BUBBLE 		= 6;
ME_BUBBLE 		= 7;
TIME_INFO 		= 8;
REVIVE_INFO 	= 9;
CLEAR_BUFF		= 10;
SET_TRAP_INVALID = 11;
SHOW_NOTICE_MSG = 12;

MAX_STAGE_LEVEL = 3;		-- 每个难度最大层数

FINISH_NOW_COST = 10;
BUY_MOP_COST = 10;
BUY_RUSH_COST = 10;

AWARD_MODULE_NAME = "Stage";

pre_env.setfenv(1, pre_env);

-- 关卡扫荡奖励配置

Stage.LayerCfgs = {};

function Stage:LoadLayerCfgs()
	local cfgs = Stage.LayerCfgs;
	local tbFileData = Lib:LoadTabFile("/setting/stage/layer_info.txt")
	for _, tbRow in pairs(tbFileData) do
		local nLayer = tonumber(tbRow.LayerId);
		assert(nLayer);
		if not cfgs[nLayer] then
			cfgs[nLayer] = {
			nBook = tonumber(tbRow.Book);
			nPrestige = tonumber(tbRow.Prestige);
			nMopTime = tonumber(tbRow.MopTime);
			};
		end
	end
end

if #Stage.LayerCfgs <= 0 then
	Stage:LoadLayerCfgs();
end
