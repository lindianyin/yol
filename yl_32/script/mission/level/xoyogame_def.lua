--
-- 逍遥谷关卡 
-- 

--Include("script/preload.lua")
local preEnv = _G	--保存旧的环境
--Lang = preEnv.Lang
--assert(Lang)

--zzf
setfenv(1, XoyoGame)	--设置当前环境为Kin
local   Lang = preEnv.Lang
Lang.mission.str812= {"普通","","","","",""}
Lang.mission.str813= {"难度","","","","",""}
Lang.mission.str814= {"困难","","","","",""}
Lang.mission.str815= {"传说","","","","",""}
Lang.Idx = 1
-- 事件ID

ADD_NPC 		= 1		-- 添加NPC
DEL_NPC			= 2		-- 删除NPC
CHANGE_TRAP		= 3		-- 更改Trip点
DO_SCRIPT		= 4		-- 执行脚本
TARGET_INFO		= 5		-- 目标信息
TIME_INFO		= 6		-- 时间信息
CLOSE_INFO		= 7		-- 关闭界面
CHANGE_FIGHT	= 8		-- 更换战斗状态
MOVIE_DIALOG	= 9		-- 电影模式
BLACK_MSG		= 10	-- 黑底字幕
CHANGE_NPC_AI	= 11	-- 更换NPC的AI
ADD_GOUHUO		= 12	-- 增加篝火
SEND_CHAT		= 13	-- 发送NPC近聊
ADD_TITLE		= 14	-- 加称号
TRANSFORM_CHILD = 15    -- 变小孩
SHOW_NAME_AND_LIFE = 16 -- 显示姓名和血条
NPC_CAN_TALK	= 17	-- Dialog Npc 禁止对话
CHANGE_CAMP		= 18	-- 改变阵形
SET_SKILL		= 19	-- 设置左右键技能
DISABLE_SWITCH_SKILL = 20 -- 禁止切换技能
TRANSFORM_CHILD_2	= 21 -- 变牧童
FINISH_ACHIEVE		= 22 -- 成就
NPC_BLOOD_PERCENT	= 23 -- NPC血量注册回调
NEW_WORLD_PLAYER = 24 	 -- 传送玩家
NPC_CAST_SKILL = 25 	-- NPC释放技能
NPC_SET_LIFE = 26		-- 按百分比设置npc血量
PLAYER_ADD_EFFECT = 27 	-- 房间内所有玩家加一个状态
PLAYER_REMOVE_EFFECT = 28  	-- 房间内所有玩家移除一个状态
NPC_ADD_BUFF_TO_PLAYER = 29 -- npc向范围玩家加buff
SET_NPC_NICKNAME = 30 -- 设置npc nickname
NPC_BUBBLE = 31 --npc喊话
CLEAR_BUFF = 32	-- 删除玩家身上buff
SYS_MSG = 33	-- 聊天框里显示文字
REVIVE_INFO = 34

-- AI模式
AI_MOVE			= 1
AI_RECYLE_MOVE	= 2
AI_ATTACK		= 3

MAX_TIMES			= 14 	-- 最多累计次数
MIN_TEAM_PLAYERS	= 4		-- 队伍至少人数
MIN_LEVEL			= 50	-- 最低等级
MAX_TEAM			= 8		-- 闯关最大队伍数
PLAY_ROOM_COUNT		= 5		-- 闯关关数
ROOM_MAX_LEVEL		= 7		-- 房间最大等级
GUESS_QUESTIONS 	= 30	-- 猜迷最大题目数	
MIN_CORRECT			= 20	-- 最少要答对多少才能晋级
LOCK_MANAGER_TIME	= 20	-- 锁定报名的时间
PK_REFIGHT_TIME		= 20	-- PK重投战斗时间
MAX_REPUTE_TIMES	= 60	-- 最大兑换次数
START_TIME1			= 0800	-- 开启时间1
END_TIME1			= 2300	-- 关闭时间1
START_TIME2			= 0000	-- 开启时间2
END_TIME2			= 0200	-- 关闭时间2

TASK_GROUP			= 2050
TIMES_ID			= 1		-- 参加次数任务变量
CUR_DATE			= 2		-- 已经废弃
REPUTE_TIMES		= 3		-- 兑换次数任务变量
CUR_REPUTE_DATE		= 4		-- 最近兑换日期
ADDTIMES_TIME		= 5		-- 增加次数的时间
TASK_DIFFICUTY		= 54	-- 记录难度

DELAY_ENDTIME		= 15	-- 延迟关闭时间，15秒

REPUTE_CAMP			= 5
REPUTE_CLASS		= 3
REPUTE_VALUE		= 10	-- 兑换声望值

START_GAME_TIME 	= 60	-- 准备场准备时间

ITEM_BAOXIANG		= {18,1,190,1};

LOG_ATTEND_OPEN     = 1;      --逍遥谷参与LOG开关

RANK_RECORD = 5;

-- 房间等级对应的时间总数（秒）
ROOM_TIME = 
{
	[1] = 210,	-- 总 3分
	[2] = 240,	-- 总 3分30
	[3] = 270,	-- 总 4分
	[4]	= 300,	-- 总 4分30
	[5] = 330,	-- 总 5分
	[6] = 360,	-- 总 5分30
	[7] = 390,	-- 总 6分
}

ROOM_DTT_TIME = 2000 -- 最长超时时间

-- 地图组 每组地图必须在同一个服务器~否则晋级之后跨服无法获得原来活动数据
MAP_GROUP = 
{         -- 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10
	[1] =  {119,501,502,505,504,507,511,510,509,508,
			506,512,513,516,514,518,517,520,519,521,
			515,600,601,602,603,604,605,606,607,608,
			609,610,611,612,613,614,615,616,617,618,
			619,522,523,524,525,526,527,528,529,530,
			531},
}

TB_MAPID_LIST = {	119,501,502,505,504,507,511,510,509,508,
			506,512,513,516,514,518,517,520,519,521,
			515,600,601,602,603,604,605,606,607,608,
			609,610,611,612,613,614,615,616,617,618,
			619,522,523,524,525,526,527,528,529,530,
			531};	

-- 管理组
MANAGER_GROUP = 
{
	[341] = {23, 24, 25},
	[342] = {26, 27, 28, 29},
}

-- 开启开关
START_SWITCH = 
{
	[1] = 1,
}

BAOMING_IN_POS = {1625,3180};

GAME_IN_POS	= {
	nMapId = 97, 
	nX = 2339, 
	nY = 2670, 
	nZ = 1053184
};	-- 准备场传送坐标

PREPARE_TIME = 60;	-- 在准备场准备时间为1分钟

TARGET_INFO_IDX = 1;


-- 离开点
LEAVE_POS =
{
	--[1] = {1648, 3185},
	100, 3120, 7152, 1048576
}

-- 4、5、6人队长增加的的领袖荣誉值
HONOR = {{[4] = 6,  [5] = 8,  [6] = 10},	-- 第一关
		 {[4] = 3,  [5] = 4, [6] = 5},	-- 第二关
		 {[4] = 3, [5] = 4, [6] = 5},	-- 第三关
		 {[4] = 3, [5] = 4, [6] = 5},	-- 第四关
		 {[4] = 3, [5] = 4, [6] = 5},	-- 第五关
		};

--print("sssssssssssssssssss")
-- { 开关，描述，星级描述，难度系数 }
LevelDesp =
{
	[1] = { 1, Lang.mission.str812[Lang.Idx],	"★"};
	[2] = { -1, Lang.mission.str813[Lang.Idx] .. "2",	"★★"};
	[3] = { 0, Lang.mission.str814[Lang.Idx],	"★★★"};
	[4] = { -1, Lang.mission.str813[Lang.Idx] .. "4",	"★★★★"};
	[5] = { 0, Lang.mission.str815[Lang.Idx],	"★★★★★"};
};
NPC_LEVEL_FILE = "\\setting\\level\\npc_level.txt";

preEnv.setfenv(1, preEnv)

XoyoGame.TIMES_PER_DAY		= 100 --EventManager.IVER_nXoyoGameCount		-- 每天能参加逍遥谷的次数

-- 每天只能做6次，6次以后 没有道具奖励，金钱，经验 只有80%
XoyoGame.TIMES_FULL_REWARD = 6;

-- 用来统计每天完成次数的task groupid
XoyoGame.FINISH_TASK_GID = 2060;
XoyoGame.LAST_FINISH_DAY = 1;
XoyoGame.FINISH_TASK_ID = 2;


-----------------------------------------
-- RoomSetting初始化
XoyoGame.RoomSetting = {};
local BaseRoom = XoyoGame.BaseRoom;
local RoomSetting = XoyoGame.RoomSetting;

RoomSetting.tbRoom = {};
