-- NPC行为
if not (MODULE_GAMECLIENT or MODULE_GAMESERVER) then
	return
end

Npc.ACT_FIGHTSTAND		= 0;		-- 战斗状态站立
Npc.ACT_STAND1			= 1;		-- 非战斗状态站立一
Npc.ACT_STAND2			= 2;		-- 非战斗状态站立二
Npc.ACT_FIGHTWALK		= 3;		-- 战斗状态行走
Npc.ACT_WALK			= 4;		-- 非战斗状态行走
Npc.ACT_FIGHTRUN		= 5;		-- 战斗状态奔跑
Npc.ACT_RUN				= 6;		-- 非战斗状态奔跑
Npc.ACT_HURT			= 7;		-- 受伤
Npc.ACT_DEATH			= 8;		-- 死亡
Npc.ACT_ATTACK1			= 9;		-- 攻击一
Npc.ACT_ATTACK2			= 10;		-- 攻击二
Npc.ACT_MAGIC			= 11;		-- 技能攻击
Npc.ACT_SIT				= 12;		-- 打坐
Npc.ACT_JUMP			= 13;		-- 跳跃
Npc.ACT_NONE			= 14;		-- 无

-- NPC状态
Npc.STATE_BASE			= 1000;		-- 状态基点
Npc.STATE_HURT			= 0;		-- 受伤动作状态
Npc.STATE_WEAK			= 1;		-- 虚弱状态
Npc.STATE_SLOWALL		= 2;		-- 迟缓状态
Npc.STATE_BURN			= 3;		-- 灼伤状态
Npc.STATE_STUN			= 4;		-- 眩晕状态
Npc.STATE_FIXED			= 5;		-- 定身状态
Npc.STATE_PALSY			= 6;		-- 麻痹状态
Npc.STATE_SLOWRUN		= 7;		-- 减跑速状态
Npc.STATE_FREEZE		= 8;		-- 冻结状态
Npc.STATE_CONFUSE		= 9;		-- 混乱状态
Npc.STATE_KNOCK			= 10;		-- 击退状态
Npc.STATE_POISON		= 11;		-- 中毒状态
Npc.STATE_HIDE			= 12;		-- 隐身状态
Npc.STATE_SHIELD		= 13;		-- 护盾状态
Npc.STATE_SUDDENDEATH	= 14;		-- 猝死状态
Npc.STATE_IGNORETRAP	= 15;		-- 不触发陷阱状态

Npc.STATE_SLEEP			= 335;		-- 睡眠
Npc.STATE_ZHONGDU		= 336;		-- 中毒
Npc.STATE_SHOUSHANG		= 337;		-- 受伤
Npc.STATE_DEATH			= 338;		-- 死亡
Npc.STATE_FUHUO			= 339;		-- 复活状态，对应于死亡

-- NPC资源部位
Npc.NPCRES_PART_HELM	= 0;		-- 头部资源
Npc.NPCRES_PART_ARMOR	= 1;		-- 身体资源
Npc.NPCRES_PART_WEAPON	= 2;		-- 武器资源
Npc.NPCRES_PART_HORSE	= 3;		-- 马匹资源

Npc.NPCRELATION_FRIEND  = 0;        -- 关系友好
Npc.NPCRELATION_ENEMY   = 1;		-- 关系敌对

Npc.PNC_NOTIFY = 
{
	RELATIONCHANGED  = 1,
	BORN			 = 2,
}

-- Npc聊天泡泡组的属性，Weight是权重据此来随机到一个组(最终再根据具体泡泡权重随机到具体的一项),Index为Npc组的下标，不能有重复
Npc.BubbleProperty = 
{
	Silence		= { Weight = 200 },								-- 沉默
	Task 		= { Weight = 50, nIndex = "Task" },				-- 普通泡泡
	Normal		= { Weight = 50, nIndex = "Normal" },			-- 任务泡泡
};

-- NPC虚拟关系枚举 (请与程序 knpcdef.h 中 KE_NPC_VIRTUALRELATIONTYPE 对应)
Npc.emNPCVRELATIONTYPE_NONE 	= 0;		-- 无关系
Npc.emNPCVRELATIONTYPE_TONE 	= 1;		-- 帮会关系

-- Npc常规对话类型
Npc.GENERAL_DIALOG_TYPE_NORMAL = 1
Npc.GENERAL_DIALOG_TYPE_STORY = 2
Npc.GENERAL_DIALOG_TYPE_EXPSCENE = 3

-- 补足npc 
Npc.ComplementalNpc = {}

-- 战场，剧情character个数
Npc.CHARACTERCOUNT = 
{
	emHEALTH_COUNT = 1;      -- 治疗
	emDAMAGE_COUNT = 3;      -- DPS
}

-- NPC 分组
Npc.emNPCGROUPTYPE_GROUPS = 0;
Npc.emNPCGROUPTYPE_ALLYGROUPS = 1;
Npc.emNPCGROUPTYPE_ADDGROUPS = 2;

Npc.emModelType = 
{
	emHUMEN_QUINTESSNECE = 3,                 -- 人型精英
	emNOTHUMEN_NOTQUINTESSNECE = 0,           -- 非人型非精英
	emNOTHUMEN_QUINTESSNECE = 1,              -- 非人型精英
	emHUMEN_NOTQUINTESSNECE = 2,              -- 人型非精英
}

Npc.nMinLevel = 0                             -- Npc 的最小等级0
Npc.nMaxLevel = 100                           -- Npc 的最大等级

Npc.EXP_TYPE_AVE = 1							--所有人均分
Npc.EXP_TYPE_ALL = 2							--所有人得到Exp的经验
Npc.EXP_TYPE_BY_DMG = 3							--按伤害量均分
Npc.EXP_TYPE_FIRSTBLOOD = 4						--第一击获得全部
Npc.EXP_TYPE_LASTBLOOD = 5						--最后一击获得全部
Npc.EXP_TYPE_DMGMAX = 6							--伤害最多获得全部