 
#pragma once

#include "ProtocolBasic.h"

#pragma	pack(push, 1)

////////////////////////////////////////////////////////////////////////////////
const unsigned KG_LOG_SERVER_MAX_PACKET_SIZE = 4096;

enum KG_LOG_SERVER_C2S_PROTOCAL_ID
{
    LOGC2S_PING/* = PROTOCOL_NORMAL_PACKAGE_BEGIN + 1*/,
    LOGC2S_LOG_LOGIN,              // must be first command, ref: LOGSERVER_LOGIN
    LOGC2S_LOG_ITEM_OPERATION,     // ref: KGItemLog
    LOGC2S_LOG_PLAYER_ACTION,       // ref: KGPlayerActionLog
	LOGC2S_LOG_DATA_ANALYSIS,       // ref: KGDataAnalysisLog    
};

enum KG_LOG_SERVER_S2C_PROTOCAL_ID
{
    LOGS2C_PING_RESULT/* = PROTOCOL_NORMAL_PACKAGE_BEGIN + 1*/,
    LOGC2S_LOG_LOGIN_RESULT,            // ref: TProcessData 
                                        // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success
    LOGS2C_LOG_ITEM_OPERATION_RESULT,   // ref: TProcessData
                                        // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success
    LOGS2C_LOG_PLAYER_ACTION_RESULT,     // ref: TProcessData
                                        // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success
	LOGS2C_LOG_DATA_ANALYSIS_RESULT     // ref: TProcessData
										// TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success
};

////////////////////////////////////////////////////////////////////////////////
struct LOGSERVER_LOGIN : tagProtocolHeader2
{
    char szGatewayName[_NAME_LEN];
};

//////////////////////////////////////////////////////////////////////////
const unsigned LOG_ITEM_RESTORE_DATA_MAX_SIZE   = 1024;
const unsigned LOG_COMMENT_MAX_SIZE             = 2048;

enum LOG_RETURN_CODE
{
    LOG_RETURN_FAILED  = 0,
    LOG_RETURN_SUCCESS = 1,
    LOG_RETURN_FILTER  = 2
};

enum ITEM_OPERATION_TYPE
{
    ITEM_OPERATION_TYPE_IN  = 0, // 获得物品
    ITEM_OPERATION_TYPE_OUT = 1, // 失去物品


    ITEM_OPERATION_TYPE_COUNT
};

struct KGItemLog : tagProtocolHeader2
{
    ITEM_OPERATION_TYPE eOperationType;
    char                szAccountName[_NAME_LEN];
    char                szRoleName[_NAME_LEN];
    unsigned long long  ullItemID;               // 物品ID，用来唯一标识一个特定的物品
    char                szItemName[_NAME_LEN];
    unsigned            uItemValue;              // 物品价值量
    unsigned            uItemRestoreDataSize;    // include string's terminal '\0', max size is LOG_ITEM_RESTORE_DATA_MAX_SIZE
    unsigned            uCommentSize;            // include string's terminal '\0', max size is LOG_COMMENT_MAX_SIZE, set to 0 for none comment
    unsigned char       pbyData[1];       // following data size is :uItemRestoreDataSize + uCommentSize
};

//////////////////////////////////////////////////////////////////////////
//统计点
enum PLAYER_ACTION_TYPE
{
    PLAYER_ACTION_PLAYER_LEVELUP        = 0,    // 玩家升级
    PLAYER_ACTION_PLAYER_LEVELDOWN      = 1,    // 玩家降级

    PLAYER_ACTION_JOIN_FACTION          = 2,    // 加入门派
    PLAYER_ACTION_LEAVE_FACTION         = 3,    // 退出门派

    PLAYER_ACTION_JOIN_TONG             = 4,    // 加入家族
    PLAYER_ACTION_LEAVE_TONG            = 5,    // 推出家族
    PLAYER_ACTION_KICK_TONG             = 6,    // 踢出家族
    PLAYER_ACTION_FIGURECHANGE_TONG     = 7,    // 职位变更

    PLAYER_ACTION_ACCEPT_TASK           = 8,    // 接受任务
    PLAYER_ACTION_FINISH_TASK           = 9,    // 完成任务
    PLAYER_ACTION_CANCEL_TASK           = 10,   // 取消任务

    PLAYER_ACTION_ACCEPT_AWARD          = 11,   // 领取奖励
    PLAYER_ACTION_ACCEPT_REPAIR         = 12,   // 获取补偿

    PLAYER_ACTION_LOGIN                 = 13,   // 登录
    PLAYER_ACTION_LOGOUT                = 14,   // 正常离线
    PLAYER_ACTION_BEGIN_OFFLIVE         = 15,   // 离线托管开始
    PLAYER_ACTION_END_OFFLIVE           = 16,   // 离线托管结束
    PLAYER_ACTION_KICK_OFFLIVE          = 17,   // 离线托管踢号

    PLAYER_ACTION_CHOOSE_SKILLROUTE     = 18,   // 选择技能路线
    PLAYER_ACTION_SKILL_LEVELUP         = 19,   // 技能升级
    PLAYER_ACTION_RESET_SKILL           = 20,   // 洗点

    PLAYER_ACTION_TRADE                 = 21,   // 交易

    PLAYER_ACTION_MARRY                 = 22,   // 结婚
    PLAYER_ACTION_UNMARRY               = 23,   // 离婚

    PLAYER_ACTION_JOIN_ACTIVITY         = 24,   // 参加活动
    PLAYER_ACTION_LEAVE_ACTIVITY        = 25,   // 结束活动

    PLAYER_ACTION_DELETE_ROLE           = 26,   // 删除角色
    PLAYER_ACTION_RESTORE_ROLE          = 27,   // 恢复角色
    PLAYER_ACTION_FREEZE_ROLE           = 28,   // 冻结角色
    PLAYER_ACTION_UNFREEZE_ROLE         = 29,   // 解除角色
    PLAYER_ACTION_RENAME_ROLE           = 30,   // 角色改名

    PLAYER_ACTION_SKILLPOINT_CHANGE     = 31,   // 技能点变化
    PLAYER_ACTION_PROPPOINT_CHANGE      = 32,   // 潜能点变化
    PLAYER_ACTION_GATHERPOINT_CHANGE    = 33,   // 活力值变化
    PLAYER_ACTION_MAKEPOINT_CHANGE      = 34,   // 精力值变化

	PLAYER_ACTION_APPRAISAL             = 35,   // 评价
	PLAYER_ACTION_SHOP					= 36,   //商店购买


	
	
	PLAYER_ACTION_CREATE_ROLE           = 37,   //创建角色
	//消耗元宝不包括绑定元宝
	PLAYER_ACTION_RESIZEEXTBAG          = 40,   //背包格解锁消耗
	PLAYER_EQUIP_LEVEL_UP_NEED			= 41,	//装备升级材料不足
	PLAYER_EQUIP_REFINE					= 42,	//装备洗练
	PLAYER_FEED_RIDE_HHG				= 43,	//喂养坐骑幻化果
	PLAYER_FEED_RIDE_HHD				= 44,	//喂养坐骑幻化丹
	PLAYER_PET_EXP_BOOK_USE				= 45,	//宠物经验书使用不足购买
	PLAYER_PET_SKILL_BOOK_USE			= 46,	//宠物技能书使用不足
	PLAYER_PET_UNLOCK					= 47,	//宠物栏位解锁
	PLAYER_PET_ACCESS_ITEM				= 48,	//宠物进阶丹
	PLAYER_CHUAN_CHENG					= 49,	//宠物传承

	PLAYER_MARKET_BUY_ITEM				= 50,	//商城购买物品

	

	PLAYER_ADD_ARENA_DEGREE				=100,		//增加竞技场次数
	PLAYER_CLEAR_CD_TIME				=101,		//清空竞技场cd
	PLAYER_PET_CARD						=102,		//神仙抽卡
	PLAYER_PET_WRITHE					=103,		//神仙翻牌
	PLAYER_ADD_MONEY_LJ					=104,		//炼金消耗元宝
	PLAYER_TOWER_ADD_DEGREE				=105,		//伏魔塔增加次数
	PLAYER_TOWER_REDUCE_TIME			=106,		//伏魔塔时间减少
	PLAYER_TOWER_FINIST					=107,		//伏魔塔直接完成
	PLAYER_ESCORT_ADD_DEGREE			=108,		//修仙增加次数
	PLAYER_ESCORT_ADD_REFRESH_DEGREE	=109,		//修仙增加刷新次数
	PLAYER_ESCORT_JIEJIA_ADD_DEGREE		=110,		//增加劫架次数
	PLAYER_ESCORT_JIUJIA_ADD_DEGREE		=111,		//增加救驾次数
	PLAYER_ESCORT_REDUCE_TIME			=112,		//修仙时间减半
	PLAYER_ESCORT_FINISH				=113,		//修仙直接完成
	PLAYER_SIGN_ADD						=114,		//每日签到补签
	PLAYER_QUESTION_ADD_TWICE_DEGREE	=115,		//日常答题增加双倍次数
	PLAYER_QUESTION_XUEBA				=116,		//日常答题学霸次数
	PLAYER_SWEEP_ADD_DEGREE				=117,		//扫荡增加次数
	PLAYER_SWEEP_REDUCE_TIME			=118,		//扫荡时间减半
	PLAYER_SWEEP_FINISH					=119,		//扫荡直接完成
	PLAYER_INVESTED_ZG					=120,		//大神宝号尊贵投保
	PLAYER_INVESTED_QS					=121,		//大神宝号轻松投保
	PLAYER_BUY_STONE					=122,		//宝石不足购买宝石
	PLAYER_EQUIP_DAKONG					=123,		//装备打孔
	PLAYER_INSTANCE_ADD_DEGREE			=124,		//增加单人副本次数




	// LBH_Modify:剑世分支修改，直接把枚举上限改大，枚举类型游戏世界内自定义
    PLAYER_ACTION_TYPE_COUNT = 999 // must be the last one
};

struct KGPlayerActionLog : public tagProtocolHeader2
{
    unsigned            uActionLevel;   // 行为重要程度
    PLAYER_ACTION_TYPE  eActionType;
    char                szActionTarget[_NAME_LEN];  // 行为目标，如某个任务名，活动名等
    char                szAccountName[_NAME_LEN];
    char                szRoleName[_NAME_LEN]; 
    unsigned            uCommentSize;    // include string's terminal '\0', max size is LOG_COMMENT_MAX_SIZE, set to 0 for none comment
    unsigned char       pbyData[1]; // following data size is uCommentSize
};


/////////////////////////////////////////////////////////////////////////////////
enum KE_DATA_ANALYSIS_LOG_TYPE
{
	emDATA_ANALYSIS_LOG_NOMAL                        = 0,        // 普通，不可以修改
	emDATA_ANALYSIS_LOG_PLAYER_LEVELUP               = 1,        // 角色升级
	emDATA_ANALYSIS_LOG_LOGIN_OR_LOGOUT              = 2,        // 上下线时
	emDATA_ANALYSIS_LOG_ENTER_OR_OUT_SCENE           = 3,        // 场景进出
	emDATA_ANALYSIS_LOG_PROFIT_CHANGED               = 4,        // 活力变化
	emDATA_ANALYSIS_LOG_RECEIVER_ENTRUST             = 5,        // 角色接委托
	emDATA_ANALYSIS_LOG_EXPSCENE_DESTROY             = 6,        // 副本销毁时
	emDATA_ANALYSIS_LOG_PAY_ENTRUST                  = 7,        // 角色叫委托
	emDATA_ANALYSIS_LOG_GIVEUP_ENTRUST               = 8,        // 放弃委托
	emDATA_ANALYSIS_LOG_MAIN_FACTION_CHANGED         = 9,        // 主职业转职
	emDATA_ANALYSIS_LOG_SECOND_FACTION_CHANGED       = 10,       // 副职业转职
	emDATA_ANALYSIS_LOG_MAIN_FACTION_UP_LEVEL        = 11,	     // 主职业升级
	emDATA_ANALYSIS_LOG_GET_PET                      = 12,       // 获得宠物
	emDATA_ANALYSIS_LOG_DELETE_PET                   = 13,       // 删除宠物
	emDATA_ANALYSIS_LOG_PLAYER_PET_UP_LEVEL          = 14,       // 角色的宠物升级
	emDATA_ANALYSIS_LOG_DISPATCH_PET                 = 15,       // 角色派遣宠物
	emDATA_ANALYSIS_LOG_PLAYER_MAIN_PET              = 16,       // 角色的主宠物
	emDATA_ANALYSIS_LOG_RESET_POINT                  = 17,       // 角色的宠物洗点
	emDATA_ANALYSIS_LOG_EQUIPMENT_COMPOSE            = 18,       // 角色装备合成
	emDATA_ANALYSIS_LOG_EQUIPMENT_REFINERY           = 19,       // 角色装备炼化
	emDATA_ANALYSIS_LOG_EQUIPMENT_ADD_START          = 20,       // 角色装备化冥
	emDATA_ANALYSIS_LOG_PLAYER_BUY_ITEM              = 21,       // 角色商店买
	emDATA_ANALYSIS_LOG_PLAYER_SELL_ITEM             = 22,       // 角色商店卖
	emDATA_ANALYSIS_LOG_PLAYER_MONEY_CHANGED         = 23,       // 角色金钱变化
	emDATA_ANALYSIS_LOG_PVE_BATTLE                   = 24,       // 角色PVE战斗
	emDATA_ANALYSIS_LOG_CREATE_LEAGUE_CLAN           = 25,       // 角色创建联赛战队
	emDATA_ANALYSIS_LOG_DISSOLVE_LEAGUE_CLAN         = 26,       // 角色解散联赛战队
	emDATA_ANALYSIS_LOG_JOIN_LEAGUE_CLAN             = 27,       // 联赛战队成员加入
	emDATA_ANALYSIS_LOG_QUIT_LEAGUE_CLAN             = 28,       // 联赛战队成员离开
	emDATA_ANALYSIS_LOG_TEAM_LEAGUE_MATCH_WT         = 29,       // 队伍联赛等待log
	emDATA_ANALYSIS_LOG_TEAM_LEAGUE_FIGHT            = 30,       // 队伍参加联赛
	emDATA_ANALYSIS_LOG_TEAM_CHANGE_GS_4_LEAGUE      = 31,       // 队伍参加联赛跨线是否成功
	emDATA_ANALYSIS_LOG_PLAYER_JOIN_LEAGUE           = 32,       // 角色参加联赛
	emDATA_ANALYSIS_LOG_CUP_SIGNUP                   = 33,       // 杯赛报名情况
	emDATA_ANALYSIS_LOG_CUP_SIGNIN                   = 34,       // 杯赛报名成功后的参与
	emDATA_ANALYSIS_LOG_PLAYER_JOIN_PK_LAB           = 35,       // 角色参加无差别战斗
	emDATA_ANALYSIS_LOG_CHALLENGE                    = 36,       // 角色仙人挑战
	emDATA_ANALYSIS_LOG_PIG_ABDUCT                   = 37,       // 角色抢亲记录
	emDATA_ANALYSIS_LOG_THREEXIAODAO                 = 38,       // 角色三仙岛记录
	emDATA_ANALYSIS_LOG_CATCH_SHEEP                  = 39,       // 角色参加抓羊
	emDATA_ANALYSIS_LOG_TREASURE                     = 40,       // 角色进行挖宝
	emDATA_ANALYSIS_LOG_CREATE_KIN                   = 41,       // 创建术士团记录   
	emDATA_ANALYSIS_LOG_KIN_TRY_SUCCESS              = 42,       // 角色创建的术士团考验成功
	emDATA_ANALYSIS_LOG_KIN_TRY_FAIL                 = 43,       // 角色创建的术士团考验失败
	emDATA_ANALYSIS_LOG_PLAYER_JOIN_KIN              = 44,       // 角色加入术士团
	emDATA_ANALYSIS_LOG_PLAYER_QUIT_KIN              = 45,       // 角色离开术士团
	emDATA_ANALYSIS_LOG_VOTE_EVENT_BEGIN             = 46,       // 投票事件开始
	emDATA_ANALYSIS_LOG_VOTE_EVENT_END               = 47,       // 投票事件结果
	emDATA_ANALYSIS_LOG_KIN_UP_LEVEL                 = 48,       // 术士团升级
	emDATA_ANALYSIS_LOG_KIN_WEEK_RATING              = 49,       // 术士团每周评级
	emDATA_ANALYSIS_LOG_PLAYER_KIN_LEVEL_CHANGED     = 50,       // 角色在术士团中的等级变化
	emDATA_ANALYSIS_LOG_PLAYER_KIN_CONTRIBUTION_CHANGED = 51,    // 每周角色在术士团中的贡献度等级变化
	emDATA_ANALYSIS_LOG_ADD_FRIEND                   = 52,       // 增加好友
	emDATA_ANALYSIS_LOG_DELETE_FRIEND                = 53,       // 删除好友
	emDATA_ANALYSIS_LOG_ITEM                         = 54,       // 道具相关（世界规则，现在都记）
	
	emDATA_ANALYSIS_LOG_TYPE_COUNT           = 999       // must be the last one      
};

struct KGDataAnalysisLog : public tagProtocolHeader2
{
	unsigned                  uLogLevel;   // Log重要程度
	KE_DATA_ANALYSIS_LOG_TYPE    eLogType;
	unsigned uGameServerId;
	DWORD dwPlayerId;
	DWORD dwTeamId;
	unsigned uPlayerLevel;
	unsigned uPlayerRank;
	unsigned                  uCommentSize;    // include string's terminal '\0', max size is LOG_COMMENT_MAX_SIZE, set to 0 for none comment
	unsigned char             pbyData[1]; // following data size is uCommentSize
};

#pragma	pack(pop)

