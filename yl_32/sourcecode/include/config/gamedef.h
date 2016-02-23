
#pragma once

// 创建INT64
#define MAKE_INT64(__A__, __B__)	(((UINT64)(__A__) << 32) + (UINT64)__B__)

#define GAME_FPS	16
// -------------------------------------------------------------------------
#define MAX_NAME_LEN 32

#define KD_MAX_GET_NEAR_BY_TEAMS 30
#define MAX_SENTENCE_LENGTH 256

#define BANISH_PLAYER_WAIT_SECONDS      8
#define DELETE_SCENE_WAIT_SECONDS       (BANISH_PLAYER_WAIT_SECONDS + 5) 

//玩家与场景对象对话的最小距离
//超过这个距离点击场景对象会自动走近再出对话框
//该值设定不应该小于10
#define KD_MIN_SCENCE_OBJ_TALK_DISTANCE	(300)
#define DOODAD_TALK_DISTANCE (50)

#define MAX_GS_COUNT		    16

#define LOGOUT_WAITING_TIME		(GAME_FPS * 20)	    // 玩家下线等待20秒
#define LOGIN_TIMEOUT			(GAME_FPS * 60)	    // 登陆超时时间

#define KD_GC_SAVE_INTERNAL (GAME_FPS * 60)		// CR 1分钟存盘一次

//////////////////////////////////////////////////////////////////////////
// 关于Id与模板Id：
//     Id是运行时的，模板Id是配置的。一个模板可以有多个实例，每个实例Id不同。
// 如果有这种应用请明确区分m_dwId和m_dwTemplateId。

// 关于场景物件Id分配策略：
// 1. Id用无符号整数表示。
// 2. Player、Npc、Doodad属于场景物件，由IdObjPool创建。创建时会自动指定一个Id。
// 3. 根据Id的高位可以判断物件类型。
// 4. Npc、Doodad的Id在本GS的运行期不重复（运行期 即从启动到关闭）。
// 5. Player登录时丢弃了IdObjPool给的Id，改由GC指定。PlayerId在GC运行期全服唯一。
//     因为Player的Id高位为0，所以GC统一分配PlayerId仍然和分配策略兼容。
#define KD_BAD_ID		0
#define ERROR_ID		0

#define KD_ID_MASK 0xC0000000	// ID掩码 (取最高两位区分玩家、NPC、Doodad)
#define KD_NPC_ID_MASK 0x80000000	// NPC ID掩码
#define KD_PLAYER_ID_MASK	0x00000000	// 玩家 ID掩码
#define KD_DOODAD_ID_MASK	0xC0000000	// 小物件 ID掩码
#define BZONE_PLAYER_ID_MASK	0x40000000	// 战区跨服玩家Id掩码

#define IS_NPC(dwId)		((dwId & KD_ID_MASK) == KD_NPC_ID_MASK)
#define IS_PLAYER(dwId)		((dwId & KD_ID_MASK) == KD_PLAYER_ID_MASK || (dwId & KD_ID_MASK) == BZONE_PLAYER_ID_MASK)
#define IS_BZONE_PLAYER(dwId)		((dwId & KD_ID_MASK) == BZONE_PLAYER_ID_MASK) // 是否漫游玩家（战区内跨服状态）
#define IS_DOODAD(dwId)		((dwId & KD_ID_MASK) == KD_DOODAD_ID_MASK)
//////////////////////////////////////////////////////////////////////////
#define GET_MAP_ID(CombinedMapId) ((CombinedMapId) & 0xFFFF)
#define GET_MAP_COPY_INDEX(CombinedMapId) (((CombinedMapId) & 0xFFFF0000) >> 16)

#define AI_TARGET_GROUP_MAX_COUNT   32

#define PLAYER_THREAT_TIME	(GAME_FPS * 6) //玩家的仇恨消退时间

// NPC AI拥有的技能数量
#define MAX_NPC_AI_SKILL		5

// NPC忽略debuff的数量
#define MAX_NPC_IGNORE_DEBUFF 5

#define KILO_NUM                1024            // 伤害计算，属性计算的比例基数，即用1/1024取代1/100

// character/player扩展变量组别的起始值
#define KD_MIN_EXTERN_VARGROUP		10
// 定义程序内置变量组别
enum KE_BASE_VAR_GROUP
{
	emKVARGROUP_CHARACTER_BYTE,
	emKVARGROUP_CHARACTER_WORD,
	emKVARGROUP_CHARACTER_DWORD,
	emKVARGROUP_CHARACTER_MAP,
	emKVARGROUP_PLAYER,
	emKVARGROUP_NPC,
};

enum ECOORDINATOR_MODE
{
	emRUNMODE_MASTER,
	emRUNMODE_SLAVE,
	emRUNMODE_STAND_ALONE,
};

#ifdef GAME_SERVER
	#define	MAX_PLAYER		1200
#else
	#define	MAX_PLAYER		2	// Index 0 reserved
#endif

enum KE_PLAYER_REPRESENT_TYPE
{
	emKPLAYER_REPRESENT_TYPE_BODY, // 体型
	emKPLAYER_REPRESENT_TYPE_HAIR, // 发型
	//emKPLAYER_REPRESENT_TYPE_FACE, // 脸型
	//emKPLAYER_REPRESENT_TYPE_SHIRT, // 上装
	//emKPLAYER_REPRESENT_TYPE_SHOE, // 下装

	emKPLAYER_REPRESENT_TYPE_COUNT
};


#define TALK_ERROR_S_OK                     0
#define TALK_ERROR_E_ERROR                  1
#define TALK_ERROR_E_OUT_OF_BUFFER          2
#define TALK_ERROR_E_NOT_IN_PARTY           3
#define TALK_ERROR_E_NOT_IN_SENCE           4
#define TALK_ERROR_E_PLAYER_NOT_FOUND       5
#define TALK_ERROR_E_PLAYER_OFFLINE         6
#define TALK_ERROR_E_YOU_BLACKLIST_TARGET   7
#define TALK_ERROR_E_TARGET_BLACKLIST_YOU   8
#define TALK_ERROR_E_BAN                    9
#define TALK_ERROR_E_CD                     10
#define TALK_ERROR_E_NOT_IN_TONG            11
#define TALK_ERROR_E_TONG_CAN_NOT_SPEAK     12
#define TALK_ERROR_E_DAILY_LIMIT            13
#define TALK_ERROR_E_NOT_IN_FORCE           14
#define TALK_ERROR_E_CHARGE_LIMIT           15

// 新聊天频道
enum CHAT_CHANNEL_TYPE
{
	trInvalid = 0,

	// Player to Player chat.
	trNearby,           // 近聊
	trTeam,             // 小队聊天
	trRaid,             // 团队聊天
	trBattleField,      // 战场团队
	trSence,            // 同场景聊天（城聊）
	trWhisper,          // 密聊
	trFace,             // 表情频道

	// System message
	trLocalSys,         // 本地系统消息
	trGlobalSys,        // 全局系统消息
	trGmMessage,        // GM 

	// NPC to Player chat.
	trNpcNearby,
	trNpcTeam,
	trNpcSence,
	trNpcWhisper,
	trNpcSayTo,         // NPC 和指定的玩家说话；功能上类似密聊，但是聊天的内容出现在客户端的NPC近聊频道。
	trNpcYellTo,        // NPC 向指定的玩家大喊；功能上类似密聊，但是聊天的内容出现在客户端的NPC地图频道。
	trNpcFace,
	trNpcSayToTargetUseSentenceID,
	trNpcSayToCamp,

	// Ognization
	trTong,
	trWorld,
	trForce,
	trCamp,
	trMentor,
	trFriends,
	trFaction,

	// Player custom channel.
	trCustomCh1 = 100,  // 玩家自定义频道预留
	trCustomCh2,
	trCustomCh3,
	trCustomCh4,
	trCustomCh5,
	trCustomCh6,
	trCustomCh7,
	trCustomCh8,

	// Debug message
	trDebugThreat,

	trTotal
};

enum KMAP_TYPE
{
	emtInvalid = -1,

	emtNormalMap,       // 正常场景
	emtDungeon,         // 任务副本
	emtBattleField,     // 战场
	emtBirthMap,        // 新手村

	emtTotal
};

enum BANISH_PLAYER_CODE
{
	bpcInvalid = 0,

	bpcMapRefresh,
	bpcMapUnload,
	bpcNotInMapOwnerTeam,
	bpcCancelBanish,

	bpcTotal
};

#define TEAM_MARK_COUNT        16
#define MAX_PLAYER_PER_GROUP    3
#define MIN_TEAM_GROUP_NUM      1
#define MAX_TEAM_GROUP_NUM      1

#define MAX_TEAM_SIZE      (MAX_PLAYER_PER_GROUP * MAX_TEAM_GROUP_NUM)

enum TEAM_AUTHORITY_TYPE
{
	tatInvalid,

	tatLeader,
	tatDistribute,
	tatMark,

	tatTotal
};

enum ITEM_LOOT_MODE
{
	ilmInvalid = 0,

	ilmFreeForAll,		// 队伍内自由拾取
	ilmDistribute,		// 高于品质线的物品分配者分配，其它物品轮流
	ilmGroupLoot,		// 高于品质线的物品roll决定归属，其它物品轮流

	ilmTotal           
};

// 角色类型（性别体型）
enum ROLE_TYPE
{
	emROLETYPE_MALE = 0, // 标准男

	emROLETYPE_FEMALE, // 标准女

	emROLETYPE_TOTAL
};

enum CHARACTER_NORMAL_BODY
{
	emBODY_MAN = 2,
	emBODY_WOMAN = 23,
	emHEAD_MAN = 1,
	emHEAD_WOMEN = 35,
};

#define PLAYER_NPC_TEMPLATE_ID 99999

enum TEAM_NOTIFY_CODE
{
	epncInvalid = 0,
	epncPlayerInviteNotExist,
	epncPlayerApplyNotExist,
	epncPlayerAlreadInYourTeam,
	epncPlayerAlreadInOtherTeam,
	epncYouAlreadInTeamState,
	epncPlayerIsBusy,
	epncInvitationDenied,   
	epncApplicationDenied,
	epncPlayerNotOnline,
	epncDestTeamIsFull,
	epncYourTeamIsFull,
	epncInvitationOutOfDate,  // 邀请已经过期
	epncApplicationOutOfDate, // 申请已经过期
	epncTeamCreated,
	epncTeamJoined,
	epncTeamLeaved,
	epncInvitationDone,       // 你向XXX发出邀请
	epncApplicationDone,      // 你向XXX发出申请
	epncInvitationReject,     // 你拒绝了XXX的邀请
	epncApplicationReject,    // 你拒绝了XXX的申请
	epncCampError,
	epncTotal
};

// 客户端角色人物头顶冒文字类型
enum CHARACTER_HEAD_TEXT_TYPE   // lua中定在在buffmagiceffect : medefine
{
    emHeadTextBlood = 0, // 减血
    emHeadTextAddBlood = 1, // 加血  绿色
    emHeadTextCriticalHit = 2, // 暴击
    emHeadTextDodge = 3, // 闪避
    emHeadTextImmune = 4, // 免疫
    emHeadTextExp = 5,  // 经验 
    emHeadTextYlEnergy = 6, // 游龙真气
    emHeadTextMoney = 7,  // 银两
    emHeadTextFightScore = 8, // 战斗力
    emHeadTextPvpScore = 9, // 精魄
    emHeadTextEnemyBlood = 10,  // npc扣血，黄色
    emHeadTextEnemyCriticalHit = 11,  // npc扣暴击，黄色
    emHeadTextGold = 12,   // 元宝
	emHeadTextTime = 13,
	emHeadTextIgnoreControl = 14,

    emHeadTextPureNumber = 99, // 纯数字
};

enum CHARACTER_RELATION_TYPE
{
	sortInvalid		= 0,

	sortNone		= 1,
	sortSelf		= 2,
	sortAlly		= 4,
	sortEnemy		= 8,
	sortNeutrality	= 16,
	sortParty		= 32,
	
	sortTeammate    = 64, // 队友

	sortAll	= sortNone | sortAlly | sortEnemy | sortSelf | sortNeutrality | sortParty,  // TODO 未有teammate

	sortTotal,
};

enum THREAT_TYPE
{
	thtInvalid = -1,

	thtMainThreat = 0,
	thtBaseThreat,
	thtDamageThreat,
	thtTherapyThreat,
	thtSpecialThreat,

	thtTotal
};

enum KAI_TARGET_TYPE
{
	aittInvalid = -3,

	aittSelectTarget = -2,
	aittSkillTarget = -1,

	aittLastSelectTarget = 0,
	aittLastSkillTarget,
	aittAttackTarget,
	aittCustomTarget1,
	aittCustomTarget2,
	aittCustomTarget3,
	aittCustomTarget4,

	aittTotal
};

enum KAI_RECORD_SKILL_TYPE
{
	airstInvalid = -1,

	airstSelectSkill,
	airstCastSkill,
	airstHitSkill,
	airstBehitSkill,

	airstTotal
};

enum TARGET_TYPE
{
	ttInvalid,

	ttNoTarget,
	ttCoordination,
	ttNpc,
	ttPlayer,
	ttDoodad,
	ttItem,
	ttItemPos,

	ttTotal
};

enum KOT_ACTION_TYPE
{
	otActionIdle,
	otActionSkillPrepare,
	otActionSkillChannel,
	otActionRecipePrepare,
	otActionPickPrepare,
	otActionPicking,
	otActionCustomPrepare,
	otActionCustomChannel,
	otActionUseItemPrepare
};

enum AI_SKILL_TYPE
{
	aistInvalid = 0,

	aistPeriodTarget,
	aistPeriodSelf,
	aistPeriodFriend,
	aistPassiveRespond,
	aistChannelingBreak,
	aistTargetCount,
	aistSelfHP,
	aistPeriodTargetRange,

	aistMainThreatNThreat,
	aistBaseThreatNThreat,
	aistDamageThreatNThreat,
	aistTherapyThreatNThreat,
	aistSpecialThreatNThreat,

	aistBaseRandomTarget,
	aistBaseRandomNotSelectTarget,
	aistInRangeRandomTarget,
	aistInRangeRandomNotSelectTarget,
	aistOutRangeRandomTarget,
	aistOutRangeRandomNotSelectTarget,
	aistNearestTarget,
	aistNearestNotSelectTarget,

	aistPeriodTargetNotClear,
	aistTargetCountNotClear,
	aistPeriodTargetRangeNotClear,

	aistTotal
};

// GM指令前缀
const static CHAR GM_CMD_PREFIX_CLIENT[] = "?cl ";
const static CHAR GM_CMD_PREFIX_GAMESERVER[] = "?gs ";
const static CHAR GM_CMD_PREFIX_GAMECENTER[] = "?gc ";
// -------------------------------------------------------------------------

// 错误返回值分类
enum ERROR_CODE_TYPE
{
	ectInvalid = 0,

	ectSkillErrorCode,
	ectLoginErrorCode,
	ectQuestErrorCode,
	ectFellowshipErrorCode,
	ectFellowshipAttractionErrorCode,
	ectItemErrorCode,
	ectTradingErrorCode,
	ectTalkErrorCode,
	ectCraftErrorCode,
	ectLootItemErrorCode,
	ectMailErrorCode,
	ectTeamNotifyCode,
	ectAddItemErrorCode,
	ectPeekOtherPlayerErrorCode,
	ectShopEventNotifyCode,
	ectPKErrorCode,
	ectFellowshipOnline,
	ectFoeOnline,
	ectBanishNotifyCode,
	ectRoadTrackErrorCode,
	ectSwitchMapErrorCode,
	ectUseItemErrorCode,
	ectOpenVenationRetCode,
	ectShareQuestErrorCode,
	ectSetQuestAssistDailyCount,
	ectFinishAssistQuest,
	ectAddSkillRecipe,
	ectFellowshipAttractionFallOff,
	ectCampResultCode,
	ectChargeLimitCode,

	ectTotal
};

// 添加道具的错误提示
enum ADD_ITEM_RESULT_CODE
{
	aircInvalid = 0,

	aircSuccess,
	aircFailed,

	aircNotEnoughFreeRoom,	// 没有足够的背包空间			
	aircItemAmountLimited,	// 你无法获得更多的此类物品
	aircCannotPutThatPlace, // 不能放在该位置

	aircTotal
};

// 技能相关的错误返回值
enum SKILL_RESULT_CODE
{
	srcInvalid = 0,

	srcSuccess,             // 成功
	srcFailed,              // 失败，未定义原因

	srcInvalidCastMode,     // 释放方式不对
	srcNotEnoughLife,		// 血量不足
	srcNotEnoughMana,		// 内力不足
	srcNotEnoughRage,       // 怒气不足
	srcNotEnoughStamina,	// 体力不足
	srcNotEnoughItem,		// 道具不足
	srcNotEnoughAmmo,       // 弹药不足
	srcSkillNotReady,		// 技能CD时间未到
	srcInvalidSkill,        // 无效的技能
	srcInvalidTarget,       // 无效的目标
	srcNoTarget,            // 没有目标
	srcNeedTarget,          // 需要指定一个目标
	srcTooCloseTarget,      // 目标太近
	srcTooFarTarget,		// 目标太远
	srcOutOfAngle,          // 目标角度不对
	srcTargetInvisible,     // 目标不在视线范围内
	srcWeaponError,         // 武器不匹配
	srcWeaponDestroy,       // 武器已损坏(耐久度为0)
	srcAmmoError,           // 弹药与武器不匹配
	srcNotEquitAmmo,        // 没有装备弹药
	srcMountError,          // 没有装备对应的武功
	srcInOTAction,          // 正处于其他OTAction行为
	srcOnSilence,           // 正处于禁言状态
	srcNotFormationLeader,  // 不是阵眼，不能释放阵法
	srcNotEnoughMember,     // 组成阵法的成员不足
	srcNotStartAccumulate,  // 没有开始蓄气
	srcBuffError,           // 没有所需要的Buff
	srcNotInFight,          // 不能在战斗中
	srcMoveStateError,      // 移动状态错误
	srcErrorByHorse,        // 上下马的状态出错
	srcBuffInvalid,         // 你作用的BUFF不存在
	srcForceEffect,         // 更强大的效果在作用中
	srcBuffImmunity,        // 你的BUFF被免疫了
	srcTargetLifeError,     // 目标血量不满足
	srcSelfLifeError,     // 自身血量不满足
	srcDstMoveStateError,
	srcNotTeam,            // 没有组队
	srcMapBan,

	srcTotal
};

enum LOOT_ITEM_RESULT_CODE
{
	lircInvalid = 0,

	lircSuccess,            // 成功
	lircFailed,             // 失败，未定义原因

	lircInventoryIsFull,	// 背包已满
	lircNotExistLootItem,	// 要拾取的物品不存在
	lircAddLootItemFailed,	// 拾取的物品无法放入背包
	lircNoLootTarget,		// 无拾取目标Doodad
	lircTooFarToLoot,		// 太远而不能拾取
	lircOverItemLimit,		// 超过携带上限

	lircTotal
};

enum QUEST_RESULT_CODE
{
	qrcInvalid = 0,

	qrcSuccess,
	qrcFailed,

	qrcErrorQuestIndex,					//错误的任务索引
	qrcErrorQuestID,					//错误的任务ID
	qrcQuestListFull,					//任务列表已满
	qrcErrorQuestState,					//错误的任务状态
	qrcAlreadyAcceptQuest,				//已经接受了任务
	qrcAlreadyFinishedQuest,			//已经完成了任务
	qrcCannotFindQuest,					//任务配置表中找不到指定的任务
	qrcTooLowLevel,						//角色等级太低
	qrcTooHighLevel,					//角色等级太高
	qrcErrorGender,						//角色性别不符合要求
	qrcErrorRoleType,					//角色体型不对
	qrcErrorForceID,					//角色门派不对
	qrcCooldown,						//Cooldown计时器未到时间
	qrcPrequestUnfinished,				//前置任务没完成
	qrcErrorItemCount,					//错误的任务物品数量
	qrcErrorKillNpcCount,				//错误的杀怪数量
	qrcTooLongTime,						//做任务时间超过限制
	qrcErrorTaskValue,					//错误的任务变量数值
	qrcAssistQuestListFull,				//协助任务列表错误
	qrcErrorAssistQuestIndex,			//错误的协助任务序号
	qrcErrorStartNpcTarget,				//目标Npc身上不能接该任务
	qrcErrorStartDoodadTarget,			//目标Doodad身上不能接该任务
	qrcErrorEndNpcTarget,				//目标Npc身上不能交该任务
	qrcErrorEndDoodadTarget,			//目标Doodad身上不能交该任务
	qrcNoStartQuestItem,				//没有任务起始道具
	qrcNoEndQuestItem,					//没有结束起始道具
	qrcTooEarlyTime,					//接任务时间太早
	qrcTooLateTime,						//接任务时间太晚
	qrcTooLowSkillLevel,				//武功技能等级太低
	qrcTooLowLifeLevel,					//生活技能等级太低
	qrcErrorGenerateItem,				//生成任务奖励物品失败
	qrcAddItemFailed,					//添加任务奖励物品失败
	qrcAddMoneyFailed,					//添加金钱失败
	qrcAddReputationFailed,				//添加声望失败
	qrcNotEnoughStartItem,				//接任务道具不足
	qrcNotEnoughFreeRoom,				//背包空间不足
	qrcErrorMap,						//错误的地图
	qrcUnfinishedQuest,					//未完成任务，不能协助
	qrcCannotFindQuestInDestPlayer,		//无法在目标玩家身上找到需要协助的任务
	qrcNeedAccept,						//必须先接任务才能交
	qrcNoNeedAccept,					//不用接就能交的任务
	qrcQuestFailed,						//此任务已失败
	qrcAddSkillFailed,					//添加技能失败
	qrcNotEnoughMoney,					//金钱不足
	qrcMoneyLimit,                      //钱已经达到上限
	qrcErrorRepute,                     //声望不满足
	qrcAddPresentItemFailed,            //无法奖励任务完成道具
	qrcDailyQuestFull,                  //每日任务已满
	qrcEscortQuestIsRunning,            //其他人正在做这个护送任务
	qrcErrorCamp,                       //阵营不对
	qrcQuestEndBookNotMemorized,        //必须阅读整套书
	qrcChargeLimit,
	qrcRequireApprentice,               //需求是徒弟
	qrcTooManyQuestOfferItem,           //任务发放道具超过拥有限制

	qrcTotal
};

enum ITEM_RESULT_CODE
{
	ircInvalid = 0,

	ircSuccess,
	ircFailed,

	ircItemNotExist,					// 物品不存在
	ircNotEnoughDurability,				// 耐久度不足或叠加数量不足
	ircErrorEquipPlace,					// 装备物品栏位置错误
	ircForceError,                      // 门派不对
	ircTooLowAgility,					// 敏捷太低,不能装备
	ircTooLowStrength,					// 力量太低,不能装备
	ircTooLowSpirit,					// 根骨太低,不能装备
	ircTooLowVitality,					// 体质太低,不能装备
	ircTooLowLevel,						// 等级太低,不能装备
	ircTooLowDurability,				// 耐久度为零,不能装备
	ircCannotEquip,						// 物品不可被装备
	ircCannotPutToThatPlace,			// 物品无法放置到那个位置	
	ircCannotDisablePackage,			// 背包内有东西无法卸下
	ircPackageNotEnoughSize,			// 背包大小不足
	ircPackageErrorType,				// 背包类型不符
	ircPlayerIsDead,					// 死亡状态不能进行这个操作
	ircBankPackageDisabled,				// 银行背包没有激活,不能往上放背包
	ircEquipedPackageCannotDestroy,     // 已经装备上的背包不能摧毁
	ircBinded,                          // 物品已经绑定
	ircCannotDestroy,                   // 该物品不能被摧毁
	ircGenderError,                     // 性别不符合
	ircPlayerInFight,                   // 战斗中不能切换
	ircOnHorse,                         // 骑马的时候不能拿下马
	ircCampCannotEquip,                 // 当前所在阵营不能装备该物品
	ircRequireProfession,               // 需要学习某生活技能
	ircRequireProfessionBranch,         // 需要学习某生活技能分支
	ircProfessionLevelTooLow,           // 生活技能等级太低

	ircTotal
};

enum USE_ITEM_RESULT_CODE
{
	uircInvalid = 0,

	uircSuccess,
	uircFailed,

	uircItemNotExist,                    // 物品不存在   
	uircCannotUseItem,                   // 物品不能使用
	uircNotEnoughDurability,             // 耐久度不够
	uircNotReady,                        // 物品还没准备好
	uircUseInPackage,					 // 在背包中才能使用
	uircUseWhenEquipped,				 // 装备上才能使用该物品
	uircTooLowLevel,                     // 等级太低
	uircRequireProfession,               // 需要学习某生活技能
	uircRequireProfessionBranch,         // 需要学习某生活技能分支
	uircProfessionLevelTooLow,           // 生活技能等级太低
	uircOnHorse,                         // 正在骑马
	uircInFight,                         // 正在战斗
	uircRequireMale,                     // 男人才能使用
	uircRequireFeMale,                   // 女人才能使用
	uircCampCannotUse,                   // 角色所在阵营不能使用
	uircCannotCastSkill,                 // 不能释放物品上的技能
	uircCastSkillFailed,                 // 释放失败
	uircCostItemDurabilityFailed,        // 消耗耐久度失败
	uircForceError,                      // 门派不对
	uircAleardHaveDesignation,           // 已经获得该称号

	uircBoxKeyMismatch,                  // 宝箱与钥匙不匹配
	uircBoxProduceRoomNotEnough,         // 背包位不够存放宝箱生成道具
	uircTotal
};

enum SHOP_SYSTEM_RESPOND_CODE
{
	ssrcInvalid = 0,
	ssrcSellSuccess,                    // 出售成功
	ssrcBuySuccess,                     // 购买成功
	ssrcRepairSuccess,                  // 修理成功
	ssrcSellFailed,                     // 出售失败
	ssrcBuyFailed,                      // 购买失败
	ssrcRepairFailed,                   // 修理成功

	ssrcTooFarAway,                     // 距离太远了
	ssrcNotEnoughMoney,                 // 金钱不足
	ssrcNotEnoughPrestige,              // 威望点数不足
	ssrcNotEnoughContribution,          // 贡献点数不足
	ssrcAchievementRecordError,         // 游戏完成度不够
	ssrcNotEnoughAchievementPoint,      // 成就点数不足
	ssrcNotEnoughRepate,                // 声望不足
	ssrcNotEnoughMentorValue,           // 师徒值不足
	ssrcNotEnoughRank,                  // 称号不达要求
	ssrcItemSoldOut,                    // 商店该物品卖完了
	ssrcBagFull,                        // 背包已满
	ssrcItemExistLimit,                 // 物品拥有限制
	ssrcItemNotNeedRepair,              // 物品不需修理
	ssrcNoneItemNeedRepair,             // 没有物品可以修理
	ssrcCanNotSell,                     // 不能出售改该物品
	ssrcItemHadLimit,                   // 物品拥有达到上限
	ssrcItemModifiedWhileBuy,           // 在你发出购买请求的这段时间内，该栏物品发生了变化
	ssrcYouDeath,                       // 你挂了
	ssrcItemBroken,                     // 该物品耐久度太低，无法出售。
	ssrcItemNotInPackage,               // 物品不在背包中。
	ssrcNotEnoughItem,                  // 需要的道具不足
	ssrcItemCD,                         // 限量物品CD中
	ssrcOnlyOne,                        // 一次只能买一个
	ssrcHaveTooMuchMoney,               // 拥有的金钱太多了，再加就溢出了

	ssrcTotal
};

enum TRADING_RESULT_CODE
{
	trcInvalid,
	trcSuccess,
	trcFailed,                  

	trcInvalidTarget,
	trcInvalidInvite,           // 交易邀请已过期
	trcRefuseInvite,            // 目标拒绝邀请
	trcTargetNotInGame,         // 邀请的目标不在游戏中
	trcTargetBusy,              // 目标正在忙
	trcYouBusy,                 // 你正在忙别的事情
	trcTooFar,                  // 距离太远了
	trcTradingCancel,           // 交易取消
	trcTradingCanceled,         // 交易被取消
	trcItemBeBound,             // 物品已绑定
	trcNotEnoughMoney,          // 没有足够的金钱
	trcNotEnoughFreeRoom,       // 没有足够的空间    
	trcItemExistAmountLimit,    // 物品拥有超过上限
	trcYouDead,                 // 玩家已经死亡
	trcItemNotInPackage,        // 只有背包内的物品可以交易
	trcTargetDead,              // 目标已经死亡

	trcTotal
};

enum CRAFT_RESULT_CODE
{
	crcInvalid = 0,

	crcSuccess,                 // 成功
	crcFailed,                  // 失败，未定义原因

	crcNotEnoughStamina,	    // 精力不足
	crcNotEnoughThew,	        // 体力不足
	crcNotEnoughItem,		    // 道具不足
	crcSkillNotReady,		    // 技能CD时间未到
	crcWeaponError,             // 武器不匹配
	crcTooLowProfessionLevel,   // 生活技能等级太低
	crcTooLowExtProfessionLevel,// 扩展生活技能等级太低
	crcAddItemFailed,			// 添加道具失败
	crcInventoryIsFull,		    // 背包已满
	crcProfessionNotLearned,    // 技能未学习
	crcExtProfessionNotLearned, // 扩展技能未学习
	crcErrorTool,			    // 没有相关工具
	crcBookIsAlreadyMemorized,	// 书籍已经被记忆了
	crcRequireDoodad,			// 需求周围有工作用的Doodad
	crcItemTypeError,           // 目标物品类型不对
	crcBookCannotBeCopy,        // 该书不能被抄录
	crcDoingOTAction,           // 正在做其他的动作
	crcMoveStateError,          // 当前状态无法做这个操作
	crcTooLowLevel,             // 玩家等级太低

	crcTotal
};

enum PLAYER_FELLOWSHIP_RESPOND_CODE
{
	rrcInvalid = 0,

	rrcSuccess,                 // 成功。

	rrcSuccessAdd,              // 添加关系成功
	rrcSuccessAddFoe, 
	rrcSuccessAddBlackList, 

	rrcFailed,                  // 失败，未定义。

	rrcInvalidName,             // 无效的玩家名字。
	rrcAddSelf,                 // 添加自己为好友
	rrcFellowshipListFull,      // 好友列表已满。
	rrcFoeListFull,             // 仇人列表已满。
	rrcBlackListFull,           // 黑名单列表已满。
	rrcFellowshipExists,        // 指定的好友已经存在。
	rrcFoeExists, 
	rrcBlackListExists, 
	rrcFellowshipNotFound,      // 指定的好友不存在。
	rrcFoeNotFound, 
	rrcBlackListNotFound,

	rrcInvalidGroupName,        // 无效的分组名字。
	rrcInvalidRemark,           // 无效的备注。
	rrcFellowshipGroupFull,     // 好友分组已满。
	rrcFellowshipGroupNotFound, // 指定的组不存在。

	rrcInFight,                 // 战斗状态不能操作仇人。

	rrcBeAddFriend,             // 被加入好友
	rrcBeAddFoe,                // 被加入仇人
	rrcBeAddBlackList,          // 被加入黑名单

	rrcTotal
};

enum PEEK_OTHER_PLAYER_RESPOND_CODE
{
	prcInvalid = 0,

	prcSuccess,                 // 成功
	prcFailed,                  // 失败，未定义错误
	prcCanNotFindPlayer,        // 失败，找不到指定玩家
	prcTooFar,                  // 距离太远

	prcTotal
};

enum PK_RESULT_CODE
{
	pkrcInvalid = 0,

	pkrcSuccess,				// 成功
	pkrcFailed,					// 未知错误

	pkrcApplyDuelFailed,
	pkrcTargetIsDueling,
	pkrcInSameTeam,            // 在同一个队伍里面不能PK
	pkrcOutofRange,             // 不在距离范围内
	pkrcApplySlayFailed,
	pkrcLevelTooLow,            // 级别太低
	pkrcPKNotReady,             // CD中

	pkrcTotal
};

enum ROAD_TRACK_RESULT_CODE
{
	rtrcInvalid = 0,

	rtrcSuccess,
	rtrcFailed,

	rtrcNotOpen,
	rtrcCostError,
	rtrcNotEnoughMoney,

	rtrcTotal
};

enum SWITCH_MAP_RESULT_CODE
{
	smrcInvalid = 0,

	smrcSuccess,
	smrcFailed,

	smrcMapCreating,
	smrcEnterNewCopyTooManyTimes,
	smrcGameMaintenance,
	smrcGameOverLoaded,
	smrcCreateMapFinished,
	smrcMapCopyCountLimit,

	smrcNotAllowed,

	smrcTotal
};

enum SHARE_QUEST_RESULT_CODE
{
	sqrcInvalid = 0,

	sqrcSuccess,
	sqrcFailed,

	sqrcTooFar,
	sqrcQuestCannotShare,
	sqrcNotInTeam,
	sqrcAlreadyAcceptQuest,
	sqrcAlreadyFinishedQuest,
	sqrcQuestListFull,
	sqrcAcceptQuest,
	sqrcRefuseQuest,

	sqrcTotal
};

enum BATTLE_FIELD_RESULT_CODE
{
	bfrcInvalid = 0,

	bfrcSuccess,
	bfrcFailed,

	bfrcInBlackList,
	bfrcLevelError,
	bfrcForceError,
	bfrcTeamMemberError,
	bfrcTeamSizeError,
	bfrcTooManyJoin,
	bfrcNotInSameMap,
	bfrcCampError,
	bfrcTimeError,

	bfrcTotal
};

enum TONG_EVENT_NOTIFY_CODE
{
	tenInvalid = 0,

	tenInviteSuccess,
	tenInviteNameNotFoundError,
	tenInviteTargetAlreadyJoinTongError,
	tenInviteTargetBusyError,
	tenInviteTargetOfflineError,
	tenInviteNoPermissionError, 

	tenInviteInvaildError,
	tenInviteCampLimit, 
	tenInviteLevelLimit,
	tenInviteTimeOutError,
	tenInviteTargetRefuse,
	tenInviteTargetChargeLimit,

	tenInviteMemberNumberLimitError,

	tenKickOutSuccess,
	tenKickOutTargetNotFoundError,
	tenKickOutTargetNoPermissionError,
	tenTargetInActivity,

	tenModifyAnnouncementSuccess,
	tenModifyAnnouncementNoPermissionError,
	tenModifyAnnouncementBadWordsError,

	tenModifyOnlineMessageSuccess,
	tenModifyOnlineMessageNoPermissionError,
	tenModifyOnlineMessageBadWordsError,

	tenModifyIntroductionSuccess,
	tenModifyIntroductionNoPermissionError,
	tenModifyIntroductionBadWordsError,

	tenModifyRulesSuccess,
	tenModifyRulesNoPermissionError,
	tenModifyRulesBadWordsError,

	tenModifyGroupNameSuccess,
	tenModifyGroupNameNoPermissionError,
	tenModifyGroupNameBadWordsError,

	tenModifyBaseOperationMaskSuccess,
	tenModifyBaseOperationMaskNoPermissionError,

	tenModifyAdvanceOperationMaskSuccess,
	tenModifyAdvanceOperationMaskNoPermissionError,

	tenModifyGroupWageSuccess,
	tenModifyGroupWageNoPermissionError,

	tenModifyMemorabiliaSuccess,
	tenModifyMemorabiliaError,

	tenQuitNotInTongError,
	tenQuitIsMasterError,

	tenChangeMemberGroupSuccess,
	tenChangeMemberGroupError,

	tenChangeMasterSuccess,
	tenChangeMasterError,

	tenChangeMemberRemarkSuccess,
	tenChangeMemberRemarkError,

	tenInviteTargetInRivalCampError,
	tenTargetTongIsRivalCampError,

	tenChangeCampPermissionError,
	tenChangeCampLimitError,

	tenCreateTongNameIllegalError,
	tenCreateTongAlreadyInTongError,
	tenCreateTongNameConflictError,

	tenRepertoryGridFilledError,
	tenRepertoryPageFullError,
	tenPutItemInRepertorySuccess,
	tenItemNotInRepertoryError,
	tenTakeItemFromRepertorySuccess,
	tenExchangeRepertoryItemSuccess,

	tenTakeRepertoryItemPermissionDenyError,
	tenPutItemToRepertoryPermissionDenyError,
	tenStackItemToRepertoryFailError,
	tenRepertoryTargetItemChangeError,
	tenRemoveRepertoryItemSuccess,
	tenRemoveRepertoryItemPermissionError,
	tenGetBackRepertoryItemSuccess,
	tenGetBackItemAleadyRentError,
	tenSetItemRemoveOnExpireSuccess,
	tenTakeItemAlreadyRentError,

	tenSaveMoneySuccess,
	tenSaveMoneyTooMushError,
	tenPaySalarySuccess,
	tenPaySalaryFailedError,

	tenRenameSuccess,
	tenRenameNoRightError,
	tenRenameUnnecessaryError,
	tenRenameConflictError,
	tenRenameIllegalError,

	tenJoinListFullError, // 申请队列已满
	tenAlreadyApplyJoin,  // 已经发送过申请
	tenApplyBeAllowed,    // 申请入族获批
	tenApplyerJoinedOtherKin, // 申请者已加入其它家族

	tenTotal
};

enum CAMP_RESULT_CODE
{
	crtInvalid,

	crtFaild,
	crtSuccess,

	crtTongConflict,
	crtInTeam,

	crtTotal
};

enum CHARGE_LIMIT_CODE
{
	cltInvalid,

	cltTalkWorld,
	cltTalkCamp,
	cltYouTrading,
	cltTargetTrading,
	cltMail,
	cltAuctionBid,
	cltAuctionSell,
	cltYouJoinTong,
	cltTargetJoinTong,
	cltLoginMessage,

	cltTotal
};

enum ENCHANT_RESULT_CODE
{
	ercInvalid,

	ercSuccess,
	ercPackageIsFull,
	ercCannotUnMount,
	ercFailed,

	ercTotal
};

// 扫荡状态
enum emMopState
{
	emMopStateNone = 0,		// 还未开始扫荡

	emMopStateDoing,		// 正在扫荡
	emMopStateDone,			// 扫荡完成
};