
BufferMagicEffect.DEBUG_MODE = 0;
BufferCondition.DEBUG_MODE = 0;

function BufferMagicEffect:DebugPrint(szMsg, ...)
	if self.DEBUG_MODE == 1 then
		print(szMsg, ...);
	end
end

function BufferCondition:DebugPrint(szMsg, ...)
	if self.DEBUG_MODE == 1 then
		print(szMsg, ...);
	end
end

local newCounter = function()
	local i = -1;
	return function ()
		i = i + 1;
		return i;
	end
end

local ById = newCounter();

-- Buff可修改数据类型
BufferMagicEffect.BuffDataType =
{
	DATATYPE_STUN							= ById(),	-- 眩晕
	DATATYPE_FIX							= ById(),	-- 定身
	DATATYPE_FEAR							= ById(),	-- 恐惧
	DATATYPE_SILENCE						= ById(),	-- 沉默
	DATATYPE_CRAZY							= ById(),	-- 疯狂
	DATATYPE_SPEED_PERCENT					= ById(),	-- 速度百分比
	DATATYPE_ARMOR							= ById(),	-- 护甲
	DATATYPE_PHYSICATTACK_MAX				= ById(),	-- 最大物理伤害
	DATATYPE_PHYSICATTACK_MIN				= ById(),	-- 最小物理伤害
	DATATYPE_MAGICATTACK_MAX				= ById(),	-- 最大法术伤害
	DATATYPE_MAGICATTACK_MIN				= ById(),	-- 最小法术伤害
	DATATYPE_MAXLIFE						= ById(),	-- 最大生命值
	DATATYPE_MAXMANA						= ById(),	-- 最大法术值
	DATATYPE_MAXPOWER						= ById(),	-- 最大集中值
	DATATYPE_ATTACK							= ById(),	-- 攻击
	DATATYPE_RESIST							= ById(),	-- 抗暴
	DATATYPE_HITCOEFFICIENT					= ById(),	-- 命中系数
	DATATYPE_CRITICALVALUE					= ById(),	-- 会心值
	DATATYPE_CRITICALATTACK					= ById(),	-- 会心伤害
	DATATYPE_DODGEPHYSIC					= ById(),	-- 物理闪避
	DATATYPE_DODGEMAGIC						= ById(),	-- 法术闪避
	DATATYPE_DODGEALL						= ById(),	-- 全部闪避
	DATATYPE_HIDETO_NPC						= ById(),	-- 对NPC隐藏
	DATATYPE_HIDETO_PLAYER					= ById(),	-- 对Player隐藏
	DATATYPE_REBOUND_P						= ById(),	-- 反弹百分比
	DATATYPE_REBOUND_V						= ById(),	-- 反弹值
	DATATYPE_TRANSTOLIFE_P					= ById(),	-- 伤害转生命
	DATATYPE_TRANSTOMANA_P					= ById(),	-- 伤害转魔法
	DATATYPE_VISIONTO_NPC					= ById(),	-- 对NPC可见
	DATATYPE_VISIONTO_PLAYER				= ById(),	-- 对PLAYER可见
	DATATYPE_FORCEATTACK_ID					= ById(),	-- 强制攻击目标ID(无目标为0)
	DATATYPE_IGNORE_DMG						= ById(),	-- 免疫伤害
	DATATYPE_IGNORE_CRISTAL					= ById(),	-- 免疫会心一击
	DATATYPE_IGNORE_DEBUFF					= ById(),	-- 免疫所有debuff
	DATATYPE_CURING_LIFE_BASE				= ById(),	-- 生命治疗
	DATATYPE_CURING_MANA_BASE				= ById(),	-- 真气治疗
	DATATYPE_SKILL_COST						= ById(),	-- 技能消耗
	DATATYPE_TRANSPARAM_TYPE				= ById(),	-- 转换的参数类型
	DATATYPE_TRANSPARAM_VALUE				= ById(),	-- 转化的参数值
	DATATYPE_SKILL_CHANNELTIME				= ById(),	-- 技能的引导时间
	DATATYPE_SKILL_CATETYPE					= ById(),	-- 技能的CateType
	DATATYPE_SKILL_CATEID					= ById(),	-- 技能的CateID
	DATATYPE_SKILL_CRITICAL					= ById(),	-- 技能的会心百分比
	DATATYPE_SKILL_BUFF						= ById(),	-- 技能+-Buff
	DATATYPE_IGNOREBUFF_ID					= ById(),	-- 忽略Buff id
	DATATYPE_IGNOREBUFF_GROUP				= ById(),	-- 忽略Buff group
	DATATYPE_IGNOREBUFF_CATETYPE			= ById(),	-- 忽略Buff catetype
	DATATYPE_IGNOREBUFF_CATEID				= ById(),	-- 忽略Buff cateid
	DATATYPE_WITHSTAND_BUFF_PLAYER			= ById(),	-- 抵挡伤害的角色
	DATATYPE_WITHSTAND_BUFF_PERCENT			= ById(),	-- 抵挡伤害比例
	DATATYPE_WITHSTAND_BUFF_LIFEPERCENT		= ById(),	-- 抵挡伤害的量
	DATATYPE_WITHSTAND_MAGIC				= ById(),	-- 法术抗性
	DATATYPE_ENLARGE_ATTACK_P				= ById(),	-- 伤害放大百分比
	DATATYPE_REDUCE_ATTACK_P				= ById(),	-- 伤害减小百分比
	DATATYPE_MODIFYDMG_P					= ById(),	-- 伤害增益百分比
	DATATYPE_BUFF_TOTAL_DMG					= ById(),	-- Buff总伤害量
	DATATYPE_CURING_LIFE_PERCENT			= ById(),	-- 生命治疗百分比
	DATATYPE_CURING_MANA_PERCENT			= ById(),	-- 真气治疗百分比
	DATATYPE_CHANGE_CHANNEL_CATETYPE		= ById(),	-- 设置技能释放类型catetype
	DATATYPE_CHANGE_CHANNEL_CATEID			= ById(),	-- 设置技能释放类型cateid
	DATATYPE_CALL_NPC						= ById(),	-- 召唤NPC
	DATATYPE_MODIFYTHREAT_P					= ById(),	-- 提高伤害产生仇恨百分比
	DATATYPE_SPEED							= ById(),	-- 速度
	DATATYPE_YONGZHEWUDI					= ById(),	-- 勇者无敌
	DATATYPE_SKILLCOST_CATEID 				= ById(),	-- 降低消耗的技能cateid
	DATATYPE_MAGICCRITICALVALUE				= ById(),	-- 魔法会心值
	DATATYPE_MEDICINE_EFFECT				= ById(),	-- 药效

	DATATYPE_MAX							= 256	--
};

BufferMagicEffect.KE_CHARACTER_DOING =
{
	emKDOING_STAND					= 1, -- 站立状态
	emKDOING_WALK					= 2, -- 走路状态
	emKDOING_RUN					= 3, -- 跑步状态
	emKDOING_JUMP					= 4, -- 跳跃状态
	emKDOING_FLY					= 5, -- 飞行中
	emKDOING_SIT					= 6, -- 坐下状态
};

BufferMagicEffect.TransParamIdx = 
{
	["MaxLife"] 			= 6,
	["MaxMana"] 			= 7,
	["HitCoefficient"] 		= 10,
	["PhysicDodge"] 			= 12,
	["MagicDodge"] 			= 13,
	["CriticalValue"] 		= 14,
	["CuringLife"] 			= 15,
}

-- 头顶冒字类型定义
BufferMagicEffect.CHARACTER_HEAD_TEXT_TYPE =
{
	emHeadTextBlood = 0, -- 减血
	emHeadTextAddBlood = 1, -- 加血
	emHeadTextCriticalHit = 2, -- 暴击
	emHeadTextDodge = 3, -- 闪避
	emHeadTextImmune = 4, -- 免伤
	emHeadTextExp = 5,  -- 经验 
	emHeadTextYlEnergy = 6, -- 游龙真气
	emHeadTextMoney = 7,  -- 银两
	emHeadTextFightScore = 8, -- 战斗力
	emHeadTextSpirit = 9, -- 战魂
	emHeadTextEnemyBlood = 10,  -- 对方扣血，黄色
	emHeadTextEnemyCriticalHit = 11,  -- npc扣暴击，黄色
	emHeadTextGold = 12,   -- 元宝
	emHeadTextTime = 13,   
	emHeadTextIgnoreControl = 14, -- 免控
	emHeadTextFightScoreTitle = 15, -- 战斗力文字
  emHeadTextEnemyBloodByGod = 16,
  emHeadTextBloodByGod =17,
  emHeadTextAddBloodByGod = 18,
  emHeadTextAddPrestige = 19,        -- 增加声望
  emHeadTextAddMaxLife = 20,         -- 生命值增加
  emHeadTextAddMaxAbility = 21,      -- 内力值增加
  emHeadTextAddAttack = 22,          -- 攻击值增加
  emHeadTextAddDefence = 23,         -- 防御值增加
  emHeadTextAddHit = 24,             -- 命中值增加
  emHeadTextAddDodge = 25,           -- 闪避值增加
  emHeadTextAddCriticalHit = 26,     -- 暴击值增加
  emHeadTextAddResist = 27,          -- 抗暴值增加
	
	emHeadtextPure = 99,  -- 纯数字
};
