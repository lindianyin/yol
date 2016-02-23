-- File       : characterdef.lua
-- Creator    : simon
-- Date       : 2010-01-21 10:20:10
-- Description: Character基础定义

if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

local nId = 0;
local function IdAccumulator(nSegment)
	nId = nSegment or nId + 1;
	return nId;
end

Character.CHARACTER_PROPERTY = 
{
	emNONE                  = 0,      -- 无属性
	emWIND                  = 1,      -- 风
	emTHUNDER               = 2,      -- 雷
	emWATER                 = 3,      -- 水
	emFIRE                  = 4,     -- 火
}

Character.tbCharacter_Camp = 
{
	emKCHARACTER_CAMP_BLACK		= 0,
	emKCHARACTER_CAMP_RED		= 1,	--
	emKCHARACTER_CAMP_GREEN		= 2,	-- 
}

-- 门派路线定义，和factiondef.h常量对应
-- 门派门派路线
Character.FACTION_WUSHANG	= 1;	-- 无上
Character.FACTION_TIANYAO	= 2;	-- 天曜
Character.FACTION_YOUMING	= 3;	-- 幽冥
Character.FACTION_SENLUO	= 4;	-- 森罗

-- 无上门派路线
Character.ROUTE_WUSHANG_TONGYONG	= 0;	-- 通用
Character.ROUTE_WUSHANG_JIANSHEN	= 1;	-- 剑神
Character.ROUTE_WUSHANG_YUHE		= 2;	-- 愈合

-- 天曜门派路线
Character.ROUTE_TIANYAO_TONGYONG	= 0;	-- 通用
Character.ROUTE_TIANYAO_FANGHU		= 1;	-- 防护
Character.ROUTE_TIANYAO_SHENSHENG	= 2;	-- 神圣

-- 幽冥门派路线
Character.ROUTE_YOUMING_TONGYONG	= 0;	-- 通用
Character.ROUTE_YOUMING_WENGU		= 1;	-- 稳固
Character.ROUTE_YOUMING_CHONGJI		= 2;	-- 冲击

-- 森罗门派路线
Character.ROUTE_SENLUO_TONGYONG	= 0;	-- 通用
Character.ROUTE_SENLUO_SHOULIE	= 1;	-- 狩猎
Character.ROUTE_SENLUO_CISHA	= 2;	-- 刺杀

-- 受控状态
Character.MOVE_STATE_ON_STAND = 1;		-- 站立状态
Character.MOVE_STATE_ON_WALK = 2;		-- 走路状态
Character.MOVE_STATE_ON_RUN = 3;		-- 跑步状态
Character.MOVE_STATE_ON_JUMP = 4;		-- 跳跃状态
Character.MOVE_STATE_ON_SWIMJUMP = 5;		-- 水中跳跃
Character.MOVE_STATE_ON_SWIM = 6;		-- 水中游泳
Character.MOVE_STATE_ON_FLOAT = 7;		-- 水中悬浮
Character.MOVE_STATE_ON_SIT = 8;		-- 坐状态

-- 不受控状态
Character.MOVE_STATE_ON_KNOCKED_DOWN = 11;		-- 被击倒状态
Character.MOVE_STATE_ON_KNOCKED_BACK = 12;		-- 被击退状态
Character.MOVE_STATE_ON_KNOCKED_OFF = 13;		-- 被击飞状态
Character.MOVE_STATE_ON_HALT = 14;		-- 定身(眩晕、笑穴等等)
Character.MOVE_STATE_ON_FREEZE = 15;		-- 不能动(固定动作)
Character.MOVE_STATE_ON_ENTRAP = 16;		-- 不能移动
Character.MOVE_STATE_ON_AUTO_FLY = 17;		-- 自动移动,比如坐车等
Character.MOVE_STATE_ON_DEATH = 18;		-- 死亡状态
Character.MOVE_STATE_ON_DASH = 19;		-- 冲刺状态
Character.MOVE_STATE_ON_PULL = 20;		-- 被抓状态，被别人抓住自己拉到对方身边
Character.MOVE_STATE_ON_REPULSED = 21;		-- 滑步状态，一个会减速的状态
Character.MOVE_STATE_ON_RISE = 22;		-- 爬起状态
Character.MOVE_STATE_ON_SKID = 23;		-- 停滑状态

--初始移动速度
Character.KD_BASE_RUN_SPEED = 20;		--注意和C++中的数值保持一致