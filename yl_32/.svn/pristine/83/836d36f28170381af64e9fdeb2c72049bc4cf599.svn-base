-- 伐魔塔常量定义

Tower.DO_SCRIPT	  	= 1;		-- 执行脚本
Tower.ADD_NPC      	= 2;
Tower.BLACK_MSG		= 3;	-- 黑条字模 	
Tower.TARGET_INFO	= 4;	-- 目标信息 
Tower.DEL_NPC 		= 5;
Tower.NPC_BUBBLE 	= 6;
Tower.TIME_INFO 	= 7;

Tower.SPEED_UP_COST = 10;
Tower.FINISH_NOW_COST = 10;
Tower.BUY_MOP_COST = 50;
Tower.BUY_CHALLENGE_COST = 10;

Tower.EXP_AWARD_ID = 2;

Tower.MAX_TOWER_LAYER = 60;

-- 伐魔塔配置

Tower.LayerCfgs = {};
Tower.nTotalExpPercent = 0;  --现在改成不是百分比了直接是经验

function Tower:LoadLayerCfgs()
	local cfgs = Tower.LayerCfgs;
	local tbFileData = Lib:LoadTabFile("/setting/tower/layer_info.txt")
	for _, tbRow in pairs(tbFileData) do
		local nLayer = tonumber(tbRow.LayerId);
		assert(nLayer);
		if not cfgs[nLayer] then
			cfgs[nLayer] = {
			nExpPercent = tonumber(tbRow.ExpAwardPercent);
			nMoney = tonumber(tbRow.Money);
			nMopTime = tonumber(tbRow.MopTime);
			};
		end
		Tower.nTotalExpPercent = Tower.nTotalExpPercent + cfgs[nLayer].nExpPercent;
	end
end

if #Tower.LayerCfgs <= 0 then
	Tower:LoadLayerCfgs();
end