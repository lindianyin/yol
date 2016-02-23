-- 用于活动的阵营enum（起始数）
Activity.START_CAMP = 4;

-- 当一个活动步骤完结，剩下的玩家分配到下一步骤的方式
Activity.ASSIGN_TYPE = {}
Activity.ASSIGN_TYPE.DONOT_NEED = 1         -- 不需要自动分配进入下个步骤
Activity.ASSIGN_TYPE.RANDOMLY_ASSIGNED = 2  -- 随机分均分配，随机分配玩家到每一个接继步骤中,可填写每个步骤分到的玩家数，不填则是平均分配
Activity.ASSIGN_TYPE.GROUP_ORDER_ASSIGNED = 3 -- 以组为单位顺序分配，即按组的加入顺序把组顺序分配到每个接继步骤中
Activity.ASSIGN_TYPE.SPECIFY_ASSIGNED = 4  -- 指定分配，此种分配模式下只能有一个接继步骤


-- 分组类型，一个步骤对所参加玩家分组的方式
Activity.MAX_GROUP_NUM = 4
Activity.GROUP_TYPE = {}
Activity.GROUP_TYPE.DONNOT_GROUP = 1   -- 不分组
Activity.GROUP_TYPE.RANDOM_GROUP = 2   -- 完全随机的分组
Activity.GROUP_TYPE.FIGHT_SCORE_GROUP = 3	--战力分组

-- 步骤中锁的事件类型
Activity.EVENT_TYPE = {}
Activity.EVENT_TYPE.MOVIE_DIALOG = 1;
Activity.EVENT_TYPE.DO_SCRIPT	  = 2;		-- 执行脚本
Activity.EVENT_TYPE.ADD_NPC      = 3;
Activity.EVENT_TYPE.BLACK_MSG	= 4;	-- 黑条字模 	
Activity.EVENT_TYPE.TARGET_INFO	= 5;	-- 目标信息 
Activity.EVENT_TYPE.DEL_NPC 	= 6;
Activity.EVENT_TYPE.NPC_BUBBLE = 8;
Activity.EVENT_TYPE.ME_BUBBLE = 9;
Activity.EVENT_TYPE.CHANGE_TRAP = 10;
Activity.EVENT_TYPE.HOOK_TRAP = 11;
Activity.EVENT_TYPE.TIME_INFO = 12;
Activity.EVENT_TYPE.SHOW_REWARD_PANEL = 13;
Activity.EVENT_TYPE.TIMING_FUNCTION = 14;    -- 定时执行一个函数 (szFucName, nCurrentExecuteTime, nMaxExecuteTime, {arg...})
Activity.EVENT_TYPE.ADD_EXC_ALL_EVENT = 15;  -- 添加一个对所有人都会执行一次的事件，即在添加后有角色进入此活动步骤即执行一次(nEventType, {arg..})
Activity.EVENT_TYPE.REMOVE_EXC_ALL_EVENT = 16;  -- 删除一个对所有人都会执行一次的事件(nEventType, {arg..})
Activity.EVENT_TYPE.SET_RELATION = 17;  -- 设置战场关系(nEventType, {arg..})
Activity.EVENT_TYPE.ADD_PLAYER_IN_LOCK = 18;  -- 把特定组玩家加入当前锁，死亡触发解锁，类似npc死亡解锁的机制(nEventType, nPlayerGroup)
Activity.EVENT_TYPE.JOIN_STEP = 19;  -- 把特定组玩家加入某个步骤(nEventType, nActivityIndex, nStepIndex, IsRemoveWinner, nPlayerGroup) 参数IsRemoveWinner: 如是当前步骤胜利者，在加入指定步骤时是否清掉在当前步骤的胜利者记录
Activity.EVENT_TYPE.SHOW_NOTICE_MSG = 20; -- 显示公告(nEventType, szMsg)  可插入参数，{Killer}表示杀人者名字，{Dead}表示被杀者名字
Activity.EVENT_TYPE.REVIVE_INFO = 21;
Activity.EVENT_TYPE.REDUCE_ALL_DEGREE = 22;	--减次数，目前只有战宝堂用到
Activity.EVENT_TYPE.DEL_ALL_NPC = 23;	--删除地图所有NPC
Activity.EVENT_TYPE.SET_TRAP_EFFECT = 24;	--	设置trap点是否生效
Activity.EVENT_TYPE.ADD_NPC_ON_TIME = 25;	--	定时添加npc

--活动分组数值
Activity.Type_All = 1;
Activity.Type_Self = 2;
Activity.Type_Npc = 4;
Activity.Type_Team = 8;
Activity.Type_Kin = 16;
Activity.Type_Server = 32;

Activity.Type_All_Enable = 65536;
Activity.Type_Self_Enable = 131072;
Activity.Type_Npc_Enable = 262144;
Activity.Type_Team_Enable = 524288;
Activity.Type_Kin_Enable = 1048576;
Activity.Type_Server_Enable = 2097152;

Activity.Type_Not = 2147483648;--1073741824;--2147483648;

-- 活动配置index定义,有的没用枚举,修改相关变量的时候注意
Activity.ACTIVITY_INDEX_ZBT		= 2			-- 战宝堂
Activity.ACTIVITY_INDEX_BOSS	= 15		-- 世界boss
Activity.ACTIVITY_INDEX_MPJJ	= 10		-- 战神无双(门派竞技)
Activity.ACTIVITY_INDEX_KIN_PTSY = 1002		-- 蟠桃盛宴
--------------------------------------

-- 活动开始步骤定义
Activity.tbStartSteps = 
{
	[290] = {nActivityIndex = 15, nStepIndex = 2},		-- 世界boss
	[330] = {nActivityIndex = 2, nStepIndex = 2},		-- 战宝堂
	[338] = {nActivityIndex = 10, nStepIndex = 1},		-- 战神无双(门派竞技)
	[340] = {nActivityIndex = 20, nStepIndex = 1},		-- 战场1
	[345] = {nActivityIndex = 21, nStepIndex = 1},		-- 战场2
	[346] = {nActivityIndex = 22, nStepIndex = 1},		-- 战场3
	[347] = {nActivityIndex = 23, nStepIndex = 1},		-- 战场4
	[348] = {nActivityIndex = 24, nStepIndex = 1},		-- 战场5
	[353] = {nActivityIndex = 30, nStepIndex = 1},		--蛮荒之地1
	[354] = {nActivityIndex = 31, nStepIndex = 1},		--蛮荒之地2
	[355] = {nActivityIndex = 32, nStepIndex = 1},		--蛮荒之地3
	[356] = {nActivityIndex = 33, nStepIndex = 1},		--蛮荒之地4
	[341] = {nActivityIndex = 26, nStepIndex = 1},		-- 神族争霸
	[3] = {nActivityIndex = 1001, nStepIndex = 1},	-- 篝火活动
	[1002] = {nActivityIndex = 1002, nStepIndex = 1},	-- 蟠桃盛宴活动
}

Activity.tbBattleFieldMaps = 
{
	340, 345, 346, 347, 348,
}

-- 活动步骤定义
Activity.STEP_TYPE = {}
Activity.STEP_TYPE.BASE = 1           -- 基础步骤
Activity.STEP_TYPE.ZHUNBEICHANG = 2   -- 准备场
Activity.STEP_TYPE.MENPAIJINGJI_YUXIANSAI = 3   -- 战神无双(门派竞技)预选赛
Activity.STEP_TYPE.MENPAIJINGJI_SAICHANG = 4   -- 战神无双(门派竞技)赛场

Activity.STEP_TYPE.BATTLE_FIELD = 5   -- 战场
Activity.STEP_TYPE.BATTLE_FIELD_ZHUNBEICHANG_1 = 7  -- 战场、准备场, 准备场1， 
Activity.STEP_TYPE.BATTLE_FIELD_ZHUNBEICHANG_2 = 8  -- 战场、准备场, 准备场2， 

Activity.STEP_TYPE.KUAFUBOSS_ZHUNBEICHANG = 6   -- 跨服BOSS准备场
Activity.STEP_TYPE.KUAFUBOSS_SAICHANG 	  = 9	-- 跨服boss赛场

Activity.STEP_TYPE.ZHANBAOTANG_ZHUNBEICHANG = 10 --战宝堂准备场
Activity.STEP_TYPE.ZHANBAOTANG_SAICHANG 	= 11 --战宝堂赛场

Activity.STEP_TYPE.TONGFIGHT_BATTLEFILED 	= 12 -- 神族争霸

Activity.STEP_TYPE.KIN_PANTAOSHENGYAN		= 13 -- 蟠桃盛宴(家族活动)

Activity.STEP_TYPE.BONFIRE_SAICHANG		= 14 -- 篝火(家族活动)

Activity.STEP_TYPE.MANHUANGZHIDI		= 15 --蛮荒之地

Activity.ACTSTEP_FACTORY = {}
Activity.ActivitySetting = {}

function Activity.ACTSTEP_FACTORY:Create(stepType)
	return Lib:NewClass(self[stepType])
end

-- 配置相关

-- 活动提醒
Activity.REMIND_TIMES_SPAN = (5 * 60)		-- 5分钟时间间隔

Activity.tbReminds = {};

function Activity:LoadReminds()
	local cfgs = Activity.tbReminds;
	local tbFileData = Lib:LoadTabFile("/setting/activity/activity_remind.txt")
	for _, tbRow in pairs(tbFileData) do
		local nActivityIdx = tonumber(tbRow.ActivityIdx);
		assert(nActivityIdx);
		if not cfgs[nActivityIdx] then
			cfgs[nActivityIdx] = {
			nRemindId1 = tonumber(tbRow.Remind1Id);
			nRemindId2 = tonumber(tbRow.Remind2Id);
			nRemindId3 = tonumber(tbRow.Remind3Id);
			};
		end
	end
end

function Activity:GetRemindByIdx(nActivityIdx, nRemindIdx)
	if not self.tbReminds[nActivityIdx] then
		return;
	end
	
	assert(nRemindIdx >= 1 and nRemindIdx <= 3);
	
	local nRemindId = self.tbReminds[nActivityIdx]["nRemindId"..nRemindIdx];
	assert(nRemindId);
	
	return nRemindId;
end

if #Activity.tbReminds <= 0 then
	Activity:LoadReminds();
end