Instance.MOVIE_DIALOG = 1;
Instance.DO_SCRIPT	  = 2;		-- 执行脚本
Instance.ADD_NPC      = 3;
Instance.BLACK_MSG	= 4;	-- 黑条字模 	
Instance.TARGET_INFO	= 5;	-- 目标信息 
Instance.DEL_NPC 	= 6;
Instance.NPC_BUBBLE = 8;
Instance.ME_BUBBLE = 9;
Instance.CHANGE_TRAP = 10;
Instance.HOOK_TRAP = 11;
Instance.TIME_INFO = 12;
Instance.SHOW_REWARD_PANEL = 13;
Instance.SET_TASK = 14;
Instance.REVIVE_INFO = 15;
Instance.CLEAR_BUFF = 16;
Instance.NOVICE_GUIDE = 17;
Instance.SHOW_NOTICE_MSG = 18;
Instance.SHOW_NOTICE_STATE = 19;
Instance.REMOVE_NOTICE_STATE = 20;
Instance.UNLOCK_GOD = 21;

Instance.END_TIME = 1800;	-- 30分钟后自动关闭副本


Instance.tbBeginPos = {}

Instance.UNLOCK_GOD_GID = 5004;
Instance.UNLOCK_GOD_STEP = 1;
Instance.UNLOCK_GOD_START = 2;

Instance.FINISH_TASK_GID = 2070;
Instance.LAST_FINISH_DAY = 1;
Instance.FINISH_TASK_ID = 2;

Instance.TIMES_FULL_REWARD = 6;

Instance.REMAIN_INSTANCE_COUNT_GID = 2080; -- 玩家剩余副本次数
Instance.REMAIN_INSTANCE_COUNT_ID = 1; -- 玩家剩余副本次数ID
Instance.REMAIN_INSTANCE_COUNT_DAY_ID = 2; -- 剩余副本次数日期，根据文档，每天需要增加相应次数

Instance.CURRENT_SWEEP_TASK_GID = 2081; -- 记录当前正在扫荡的副本信息
Instance.CURRENT_SWEEP_INSTANCE_ID = 1; -- 扫荡副本ID
Instance.CURRENT_SWEEP_START_TIME = 2; -- 开始时间
Instance.CURRENT_SWEEP_COUNT = 3; -- 当前正在扫荡副本总次数
Instance.CURRENT_SWEEP_FINISH_COUNT = 4; -- 当前完成的扫荡副本次数
Instance.CURRENT_SWEEP_FINISH = 5; -- 当前扫荡是否已经完成
Instance.CURRENT_SWEEP_SPEED_COUNT = 6; -- 当前扫荡从第几个加速
Instance.CURRENT_SWEEP_IS_SPEED = 7; -- 当前扫荡是否加速
Instance.TOTAL_SWEEP_FINISH_COUNT = 8; -- 总共完成的扫荡次数
Instance.CURRENT_SWEEP_TIMER_ID = 9; -- 定时器id
Instance.CURRENT_INSTANCE_FINISH = 10; -- 进入副本是否完成

Instance.MAX_SWEEP_COUNT = 360; -- 单次扫荡副本的最大次数
Instance.MAX_AUTO_ADD_COUNT = 120; -- 每天自然增长的上限次数
Instance.DAY_AUTO_ADD_COUNT = 40; -- 每天自然增长的次数
Instance.SWEEP_ONCE_TIME = 2 * 60; -- 扫荡单个副本的时间，单位为秒，根据文档，固定为2分钟

Instance.BUY_NEED_COIN = 10;	--购买一次次数需要的元宝
Instance.UNLOCK_GOD_TOTAL_STEP = 4;	--解封大神所需全部步骤
-- 离开点
Instance.LEAVE_POS =
{
	--[1] = {1648, 3185},
	100, 3120, 7152, 1048576
}

------------------------------------------------------------------------------------------------------------------------
-- 通副本奖励

Instance.tbInstanceAward = {};
Instance.tbTaskInstanceAward = {};
local tbInstanceAward = Instance.tbInstanceAward;
local tbTaskInstanceAward = Instance.tbTaskInstanceAward;

function Instance:LoadInstanceAward()
	local tbFileData = Lib:LoadTabFile("/setting/award/instanceaward.txt")
	for _, tbRow in pairs(tbFileData) do
		local nMapId = tonumber(tbRow.MapId);
		assert(nMapId);
		if not tbInstanceAward[nMapId] then
			tbInstanceAward[nMapId] = {};
		end
		local tbInfo = tbInstanceAward[nMapId];
		tbInfo[tonumber(tbRow.Level)] = {
			ExpAward = tonumber(tbRow.ExpAward);
			CoinAward = tonumber(tbRow.CoinAward);
			ItemGerne = tonumber(tbRow.ItemGerne);
			ItemDetail = tonumber(tbRow.ItemDetail);
			ItemParticularType = tonumber(tbRow.ItemParticularType);
			ItemLevel = tonumber(tbRow.ItemLevel);
		};
	end
end

function Instance:LoadTaskInstanceAward()
	local tbFileData = Lib:LoadTabFile("setting/award/instancetaskaward.txt");
	for _, tbRow in pairs(tbFileData) do
		local nMapId = tonumber(tbRow.InstanceId);
		assert(nMapId);
		if not tbTaskInstanceAward[nMapId] then
			tbTaskInstanceAward[nMapId] = {};
		end
		local tbInfo = tbTaskInstanceAward[nMapId];
		tbInfo[tbRow.SlotNum] = {};
		local tbSlotAward = tbInfo[tbRow.SlotNum];
		tbSlotAward.Type = tonumber(tbRow.Type);
		tbSlotAward.Value = tbRow.Value;
	end
end

function Instance:Init()
	Instance:LoadInstanceAward();
	Instance.LoadTaskInstanceAward();
end

Instance:Init();

