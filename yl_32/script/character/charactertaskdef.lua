-- 任务变量定义
if (not MODULE_GAMESERVER and not MODULE_GAMECLIENT) then
	return;
end

Character.KVARGROUP = 
{
	emKVARGROUP_CHARACTER_BYTE 	= 0;
	emKVARGROUP_CHARACTER_WORD	= 1;
	emKVARGROUP_CHARACTER_DWORD	= 2;
	emKVARGROUP_CHARACTER_MAP	= 3;
	
	emKVARGROUP_PLAYER		= 4; -- 玩家专有变量
	emKVARGROUP_NPC			= 5; -- Npc专有变量
};

local newCounter = function()
	local i = -1;
	return function ()
		i = i + 1;
		return i;
	end
end
local ById = newCounter();
local WId = newCounter();
local DwId = newCounter();
local MpId = newCounter();
local PlayerDwId = newCounter();
local NpcDwId = newCounter();

local tbVarGroup = Character.KVARGROUP;
--定义变量的逻辑映射，若该变量不作逻辑映射则指定一个空串
Character.Var = 
{
	[tbVarGroup.emKVARGROUP_CHARACTER_BYTE] = 
	{
		-- 0
		emKCHARACTER_TASK_SEX							= {ById(), "Sex"},							-- 性别

		emKCHARACTER_TASK_CURRENT_ROUTE					= {ById(), "CurrentRoute"},					-- 当前天赋路线
		emKCHARACTER_TASK_CURRENT_CAMP					= {ById(), "CurrentCamp"},					-- 当前阵营
		emKCHARACTER_TASK_KIN_CAMP					    = {ById(), "KinCamp"},					    -- 家族阵营(仙,佛)
		
		emKCHARACTER_TASK_OCCUPATION_COEFFICIENT		= {ById(),"OccupationCoefficient"},			-- 職業係數(仇恨用)
		emKCHARACTER_TASK_STUN							= {ById(),"IsStun"},						-- 眩晕
		emKCHARACTER_TASK_FIX							= {ById(),"IsFix"},							-- 定身
		emKCHARACTER_TASK_FEAR							= {ById(),"IsFear"},						-- 恐惧
		emKCHARACTER_TASK_SILENCE						= {ById(),"IsSilence"},						-- 沉默
		emKCHARACTER_TASK_CRAZY							= {ById(),"IsCrazy"},						-- 疯狂
		emKCHARACTER_TASK_HIDE_TO_NPC					= {ById(),"IsHidetoNPC"},					-- 对NPC隐藏
		emKCHARACTER_TASK_HIDE_TO_PLAYER				= {ById(),"IsHidetoPlayer"},				-- 对PLAYER隐藏
		emKCHARACTER_TASK_VISION_TO_NPC					= {ById(),"IsVisiontoNPC"},					-- 对NPC可见
		emKCHARACTER_TASK_VISION_TO_PLAYER				= {ById(),"IsVisiontoPlayer"},				-- 对PLAYER可见
		emKCHARACTER_TASK_IGNOR_DMG						= {ById(),"IgnorDmg"},						-- 免疫伤害
		emKCHARACTER_TASK_IGNOR_CRISTAL					= {ById(),"IgnorCristal"},					-- 免疫会心一击
		emKCHARACTER_TASK_IGNOR_DEBUFF					= {ById(),"IgnorDebuff"},					-- 免疫所有debuff
		emKCHARACTER_TASK_RIDE							= {ById(),"Ride"},							-- 人物当前的骑马状态				add by NN
		emKCHARACTER_TASK_IS_FIGHT_MAP					= {ById(),"IsFightMap"},					-- 人物进入的地图是否是战斗地图		add by NN
		emKCHARACTER_TASK_PET_CAPACITY					= {ById(),"PetCapacity"},					-- 宠物栏数量
		emKCHARACTER_TASK_VIP_LEVEL						= {ById(),"VipLevel"},						-- VIP等级
		emKCHARACTER_TASK_MISSION_STATE					= {ById(),"MissionState"},					-- 排队状态
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_1			= {ById(), "EightTrigramLevel1"},			-- 乾卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_2			= {ById(), "EightTrigramLevel2"},			-- 兑卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_3			= {ById(), "EightTrigramLevel3"},			-- 离卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_4			= {ById(), "EightTrigramLevel4"},			-- 震卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_5			= {ById(), "EightTrigramLevel5"},			-- 坤卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_6			= {ById(), "EightTrigramLevel6"},			-- 艮卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_7			= {ById(), "EightTrigramLevel7"},			-- 坎卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_8			= {ById(), "EightTrigramLevel8"},			-- 巽卦
		emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_TAICHI	= {ById(), "EightTrigramLevelTaiChi"},		-- 天地二仪
		emKCHARACTER_TASK_MEDITATION_STATE				= {ById(), "MeditationState"},				-- 打坐
		emKCHARACTER_TASK_IS_SET_FIGHT_MODE				= {ById(), "IsSetFightMode"},				-- 战斗模式，目前只在蛮荒之地用
		emKCHARACTER_TASK_MOP_STATE						= {ById(), "MopState"},						-- 扫荡状态
		emKCHARACTER_TASK_ANGER							= {ById(), "Anger"},						-- 怒气值
		emKCHARACTER_TASK_ESCORT_TYPE					= {ById(), "EscortType"},					-- 修仙坐骑类型
		emKCHARACTER_TASK_ESCORT_STATE					= {ById(), "EscortState"},					-- 修仙状态
	},
	[tbVarGroup.emKVARGROUP_CHARACTER_WORD] = 
	{
		-- 0
		emKCHARACTER_TASK_LEVEL							= {WId(), "Level"},							-- 等级（可能包含转生数，用WORD）	
		emKCHARACTER_TASK_PRIMARY_FACTION				= {WId(), "PrimaryFaction"},				-- 主职业
		emKCHARACTER_TASK_HAT_REPRESENTID				= {WId(), "HatRepresentId"},				-- 头部资源id
		emKCHARACTER_TASK_BODY_REPRESENTID				= {WId(), "BodyRepresentId"},				-- 身体资源id
		emKCHARACTER_TASK_LEFTWEAPON_REPRESENTID		= {WId(), "LeftWeaponRepresentId"},			-- 左手武器资源id
		emKCHARACTER_TASK_RIGHTWEAPON_REPRESENTID		= {WId(), "RightWeaponRepresentId"},		-- 右手武器资源id
		emKCHARACTER_TASK_BACKPENDANT_REPRESENTID		= {WId(), "BackPendantRepresentId"},		-- 背部挂件id
		emKCHARACTER_TASK_HEADPENDANT_REPRESENTID		= {WId(), "HeadPendantRepresentId"},		-- 头顶挂件id
		emKCHARACTER_TASK_TAILPENDANT_REPRESENTID		= {WId(), "TailPendantRepresentId"},		-- 尾部挂件id
		emKCHARACTER_TASK_LEFT_SHOULDER_REPRESENTID     = {WId(), "LeftShoulderRepresentId"},		-- 左肩
	    emKCHARACTER_TASK_RIGHT_SHOULDER_REPRESENTID    = {WId(), "RightShoulderRepresentId"},		-- 右肩
		emKCHARACTER_TASK_RIDE_REPRESENTID				= {WId(), "RideRepresentId"},				-- 人物的坐骑表现ID		add by NN
		emKCHARACTER_TASK_PET_REPRESENTID				= {WId(), "PetRepresentId"},				-- 人物的宠物表现ID		add by NN
		
		emKCHARACTER_TASK_ARENA_RANK_NUM				= {WId(), "ArenaRank"},						-- 竞技场排名信息
		emKCHARACTER_TASK_CONQUER_TOWER_LAYER			= {WId(), "TowerLayer"},					-- 伐魔塔讨伐层数
		
		emKCHARACTER_TASK_TITLE_CARRY_1					= {WId(), "TitleCarry1"},					-- 正在使用的称号1
		emKCHARACTER_TASK_TITLE_CARRY_2					= {WId(), "TitleCarry2"},					-- 正在使用的称号2
		emKCHARACTER_TASK_TITLE_CARRY_3					= {WId(), "TitleCarry3"},					-- 正在使用的称号3
	},
	[tbVarGroup.emKVARGROUP_CHARACTER_DWORD] = 
	{
		-- 0
		emKCHARACTER_COMMAND_FLAG						= {DwId(), ""},								-- 普通标志位（不做映射）
		
		emKCHARACTER_TASK_EXP							= {DwId(), "Exp"},							-- 经验
		emKCHARACTER_TASK_CURRENT_LIFE					= {DwId(), "CurrentLife"},					-- 当前生命值
		emKCHARACTER_TASK_CURRENT_MANA					= {DwId(), "CurrentMana"},					-- 当前内力值
		emKCHARACTER_TASK_CURRENT_POWER					= {DwId(), "CurrentPower"},       			-- 当前力量值(默认100点)
		
		emKCHARACTER_TASK_RANK							= {DwId(), "Rank"},							-- 玩家水平值
		emKCHARACTER_TASK_LAST_LOGOUT_TIME				= {DwId(), "LastLogoutTime"},				-- 上次Logout时间
		emKCHARACTER_TASK_LAST_LOGIN_TIME				= {DwId(), "LastLoginTime"},				-- 上次Logoin时间
		emCHARACTER_LAST_QUIT_TONG_TIME         = {DwId(), "LastQuitTongTime"},     -- 上次退出家族的时间点
		emKCHARACTER_TASK_BAN_CHAT_END_TIME				= {DwId(), "BanChatEndTime"},				-- 禁言至此时间(UTC)
		emKCHARACTER_TASK_EXT_BAG						= {DwId(), "ExtBag"},						-- 扩展背包							ADD BY NN
		emKCHARACTER_TASK_LAST_UPDATE_ESCORT_TIME		= {DwId(), "LastUpdateEscortTime"},			-- 上一次更新修仙任务的系统时间		ADD BY NN
		emKCHARACTER_TASK_CUR_ESCORT_TASK_ID			= {DwId(), "CurEscortTaskId"},				-- 当前刷新的修仙任务ID				ADD BY NN
		emKCHARACTER_TASK_RECEIVED_ESCORT_TASK_ID		= {DwId(), "ReceivedEscortTaskId"},			-- 已接修仙任务ID					ADD BY NN
		emKCHARACTER_TASK_LAST_UPDATE_PURSE_FREE_TIME	= {DwId(), "LastUpdateBowlFreeTime"},		-- 聚宝盆上一次免费获取的时间		ADD BY NN
		emKCHARACTER_TASK_BOWL_FREE_CUMULATIVE_TIME		= {DwId(), "BowlFreeCumulativeTime"},		-- 聚宝盆免费领取累计时间			ADD BY NN
		emKCHARACTER_TASK_ESCORT_AWARD_TOTAL_EXP		= {DwId(), "EscortAwardTotalExp"},			-- 每日修仙获得的总经验值
		emKCHARACTER_TASK_ESCORT_AWARD_TOTAL_ENERGY	 	= {DwId(), "EscortAwardTotalEnergy"},		-- 每日修仙获得的总真气值
		emKCHARACTER_TASK_LAST_TEAM_MSG_TIME			= {DwId(), "LastTeamMsgTime"},				-- 上次创建队伍群发的时间
		emKCHARACTER_TASK_CHANGE_NAME_COLOR				= {DwId(), "ChangeNameColor"},
		emKCHARACTER_TASK_DWORD_SAVED_END				= {DwId(), ""},								-- 不作映射
                        		                                             
		-- UnSaved, 7                               
		-- 10
		emKCHARACTER_TASK_MAX_LIFE						= {DwId(),"MaxLife"},						-- 最大生命值
	 	emKCHARACTER_TASK_MAX_MANA				 		= {DwId(),"MaxMana"},						-- 最大内力值
		emKCHARACTER_TASK_MAX_POWER				 		= {DwId(),"MaxPower"},						-- 最大力量值
		
		emKCHARACTER_TASK_ATTACK_ABILITY				= {DwId(),"Attack"},						-- 攻击
		emKCHARACTER_TASK_HITCOEFFICIENT				= {DwId(),"HitCoefficient"},				-- 命中系数
		emKCHARACTER_TASK_DODGE_ABILITY					= {DwId(),"Dodge"},							-- 闪避
		emKCHARACTER_TASK_DEF_ABILITY				   	= {DwId(),"Def"},							-- 防御
		emKCHARACTER_TASK_SPEED_ABILITY				 	= {DwId(),"Speed"},							-- 移动速度
		emKCHARACTER_TASK_SPEED_DELAYSET				= {DwId(),"SpeedDelaySet"},			-- 将要设置的移动速度（下次移动）
		
		emKCHARACTER_TASK_CRITICAL_VALUE				= {DwId(),"CriticalValue"},					-- 会心值
		emKCHARACTER_TASK_CRITICAL_ABILITY				= {DwId(),"CriticalAttack"},				-- 会心伤害
		emKCHARACTER_TASK_RESIST						= {DwId(),"Resist"},						-- 抗暴
		emKCHARACTER_TASK_CRITICAL_DEABILITY			= {DwId(),"CriticalDeAttack"},				-- 抵御会心伤害
	 emKCHARACTER_TASK_IGNORE_DEF          = {DwId(),"IgnoreDef"},					-- 破防
		
		emKCHARACTER_TASK_CURING_LIFE_BASE				= {DwId(),"CuringLifeBase"},				-- 治疗生命基础值
		emKCHARACTER_TASK_CURING_LIFE					= {DwId(),"CuringLife"},					-- 治疗生命值
		emKCHARACTER_TASK_CURING_MANA_BASE				= {DwId(),"CuringManaBase"},				-- 治疗内力基础值
		emKCHARACTER_TASK_CURING_MANA					= {DwId(),"CuringMana"},					-- 治疗内力值
		
		emKCHARACTER_TASK_WITHSTAND_NORMAL_P			= {DwId(),"WithstandNormalP"},				-- 普通抵挡伤害百分比[0~100]
		emKCHARACTER_TASK_WITHSTAND_NORMAL_V			= {DwId(),"WithstandNormalV"},				-- 普通抵挡伤害值
		emKCHARACTER_TASK_REBOUND_P						= {DwId(),"ReboundP"},						-- 反弹伤害百分比[0~100]
		emKCHARACTER_TASK_REBOUND_V						= {DwId(),"ReboundV"},						-- 反弹伤害值
		emKCHARACTER_TASK_TRANSTOLIFE_P					= {DwId(),"TransToLifeP"},					-- 伤害转生命[0~100]
		emKCHARACTER_TASK_TRANSTOMANA_P					= {DwId(),"TransToManaP"},					-- 伤害转魔法[0~100]
		emKCHARACTER_TASK_ENLARGE_ATTACK_P				= {DwId(),"EnlargeAttackP"},				-- 放大伤害百分比[0~100]
		emKCHARACTER_TASK_REDUCE_ATTACK_P				= {DwId(),"ReduceAttackP"},					-- 减少伤害百分比[0~100]
		
		emKCHARACTER_TASK_TEAM_ID						= {DwId(),"TeamId"},		
		emCHARACTER_TASK_FOLLOW_TARGET					= {DwId(),"FollowTarget"},	
		emCHARACTER_TASK_FOLLOWED_BY					= {DwId(),"FollowBy"},
		
		emKCHARACTER_TASK_AI_TYPE                       = {DwId(), "AIType"},
		emKCHARACTER_TASK_RELATION                      = {DwId(), "Relation"},                --  Npc对应玩家的关系，暂时只有npc需要这个属性
		emKCHARACTER_TASK_CURRENT_DAMAGE                = {DwId(), "CurrentDamage"},           --  当前伤害值
		emKCHARACTER_TASK_MODIFYDMG_P					= {DwId(), "ModifyDmgP"},				-- 伤害加成
		emKCHARACTER_TASK_MEDICINE_EFFECT				= {DwId(), "MedicineEffect"},			-- 药效

		emKCHARACTER_TASK_ENABLE_ENYMY_CONDITION		= {DwId(), "EnableEnemyCondition"},			-- 活动中是否使用敌人判断条件
		emKCHARACTER_TASK_ENYMY_CONDITION1				= {DwId(), "EnemyCondition1"},
		emKCHARACTER_TASK_ENYMY_CONDITION2				= {DwId(), "EnemyCondition2"},
		emKCHARACTER_TASK_ENYMY_CONDITION3				= {DwId(), "EnemyCondition3"},
		emKCHARACTER_TASK_ENYMY_CONDITION4				= {DwId(), "EnemyCondition4"},
		emKCHARACTER_TASK_ENYMY_CONDITION5				= {DwId(), "EnemyCondition5"},
		
		emKCHARACTER_TASK_FIGHT_SCORE					= {DwId(), "FightScore"},		-- 战斗力评分
		emKCHARACTER_TASK_OVERALL_TEAM_IDX				= {DwId(), "OverallTeamIdx"},
		emKCHARACTER_TASK_ANGER_LEFT_TIME				= {DwId(), "AngerLeftTime"},
		emKCHARACTER_TASK_MULTI_MAP_ID					= {DwId(), "MultiMapId"},
		emKCHARACTER_TASK_LAST_TAKE_MAKEUP_TIME			= {DwId(), "LastTakeMakeupTime"},

		emKCHARACTER_TASK_DTASK1		                = {DwId(), "Dtask1"}, -- 日常任务1
		emKCHARACTER_TASK_DTASK2	                    = {DwId(), "Dtask2"}, -- 日常任务2
		emKCHARACTER_TASK_DTASK3	                    = {DwId(), "Dtask3"}, -- 日常任务3
		emKCHARACTER_TASK_DTASK4	                    = {DwId(), "Dtask4"}, --日常任务4
		emKCHARACTER_TASK_DTASK_REFRESH_TIME            = {DwId(), "DtaskRefreshTime"},-- 日常任务刷新时间
		emKCHARACTER_TASK_DTASK_TIMES_REFRESH_TIME      = {DwId(), "DtaskTimesRefreshTime"},-- 日常任务次数刷新时间
		emKCHARACTER_TASK_DTASK_LAST_TIMES              = {DwId(), "DtaskUsedTimes"},-- 日常任务使用次数

	},
	[tbVarGroup.emKVARGROUP_CHARACTER_MAP] =
	{
		-- 0
		emKCHARACTER_CHAT_WORLD_SPEAKER					= {MpId(), "WorldSpeakerCount"},			-- 线聊喇叭
		emKCHARACTER_CHAT_WORLD_FREESPEAKER				= {MpId(), "WorldFreeSpeakerCount"},		-- 赠送的线聊喇叭
		emKCHARACTER_CHAT_WORLD_FREESPEAKER_RESETTIME	= {MpId(), "WorldFreeSpeakerResetTime"},	-- 上一次线聊喇叭赠送时间
		emKCHARACTER_CHAT_GLOBAL_SPEAKER				= {MpId(), "GlobalSpeakerCount"},			-- 全区喇叭
		emCHARACTER_FACTION_BATTLE_RANK                 = {MpId(), "FactionBattleRank"},			-- 战神无双排名
		emCHARACTER_GLOBAL_BATTLE_SCORE                 = {MpId(), "GlobalBattleScore"},			-- 跨服战场积分
		emKCHARACTER_TASK_MAP_SAVED_END					= {MpId(), ""},
		emCHARACTER_LEVEL_DIFFICUTY                     = {MpId(), "LevelDifficuty"},               -- 关卡难度
	},
	
	
	-- ---------------------------玩家和Npc专有变量-----------------------------------
	[tbVarGroup.emKVARGROUP_PLAYER] = 
	{
		-- 0
		emKPLAYER_TASK_ONLINE_TIME						= {PlayerDwId(), "OnlineTime"},			-- 总在线时长
		emKPLAYER_LAST_UPDATE_MYSTERY_SHOP_TIME			= {PlayerDwId(), "LastUpdateMysteryShopTime"}, ----上次刷新神秘商店的时间
		emKPLAYER_TASK_SAVED_END						= {PlayerDwId(), ""},
	},
	[tbVarGroup.emKVARGROUP_NPC] = 
	{
	},
}

-- 获取任务变量名字
function Character.GetTaskName(nGroupId, nTaskId)
	if (nGroupId == nil or nTaskId == nil) then
		return "";
	end
	if (Character.Var and Character.Var[nGroupId]) then
		for i, v in pairs(Character.Var[nGroupId]) do
			if (v[1] == nTaskId) then
				return i;
			end
		end
	end
	return "";
end

