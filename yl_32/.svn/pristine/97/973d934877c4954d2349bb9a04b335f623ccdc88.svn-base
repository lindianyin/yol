#ifndef _KTONGDEF_H_
#define _KTONGDEF_H_

#include "onlinegamemodule/item2/itemdef.h" // added by liyun for KIN_REPERTORY_GRID_SYNC_DATA
#include <stdint.h>
const int MASTER_GROUP_INDEX = 0;
const int ELITE_GROUP_INDEX = 1;
const int DEFAULT_GROUP_INDEX = 2;

#define USE_TONG_GROUP                          3                   // 已经使用的成员组个数
#define MAX_TONG_GROUP                          16                  // 帮会成员组上限
#define MAX_TONG_MEMORABILIA_SIZE               10                  // 帮会大事记条目上限
#define TONG_INIT_MAX_MEMBER_COUNT              20                  // 帮会初始人数上限
#define TONG_MAX_APPLY_JOIN_PLAYER_NUM          30                  // 最多能容纳的申请入帮者数量
#define TONG_BASE_OPERATION_MASK_LEN            32
#define TONG_ADVANCE_OPERATION_MASK_LEN         8
#define MAX_TONG_TECHTREE_NODE_NUM              64                  // 科技树结点数上限
#define MAX_TONG_MEMBER_COUNT                   255
#define MIN_TONG_MEMBER_COUNT                   5                   
#define TONG_TRIAL_TIME                         (5 * 24 * 60 * 60)  // 帮会筹备期限      5 days
#define TONG_DISBAND_TIME                       (5 * 24 * 60 * 60)  // 帮会淘汰期限      5 days
#define TONG_INVITE_TIME                        (2  * 60)           // 入会邀请有效时间  2 min
#define INVALID_TONG_OPERATION                  -1
#define MAX_TONG_DEVELOPMENT_POINT              100000              //  帮会发展点数上限
#define MAX_TONG_MEMORABILIA_COUNT              16                  //  帮会大事记最大数量
#define TONG_MEMORABILIA_RECORD_LEN             64                  //  帮会大事记字符串长度
#define TONG_MAX_LEVEL                          8                   //  帮会最高等级
#define TONG_MEMBER_REMARK_LEN                  32
#define TONG_ANNOUNCEMENT_LEN                   256
#define TONG_ONLINE_MESSAGE_LEN                 128
#define TONG_INTRODUCTION_LEN                   128
#define TONG_RULES_LEN                          1024
#define TONG_MAX_WAGE_RATE_LIMIT                25                  // 帮会工资发放比例硬上限25%

#define TONG_OPERATION_NUM						31 // TODO by guess
#define MAX_ITEM_DATA_SIZE						200/*sizeof(TDBItemData)*/ // 临时


#define TONG_REPERTORY_PAGE_NUM                 10                  //  帮会仓库页数
#define TONG_REPERTORY_PAGE_CAPACITY            98                  //  帮会仓库单页道具容量
// #define MAX_TONG_REPERTORY_ITEM_DATA_SIZE       30
// #define TONG_REPERTORY_TOTAL_CAPACITY        (TONG_REPERTORY_PAGE_NUM * TONG_REPERTORY_PAGE_CAPACITY)

#define MAX_SYNC_TONG_REPERTORY_PAGE_PAK_SIZE   (1024 * 16)          // 帮会仓库道具同步包极限

#define MAX_TONG_FUND_BY_SILVER                 INT_MAX             // 帮会资金上限(以银为单位)
#define MONEY_CONVERSION_SILVER_RATE            100                 // 金钱换算成硬币的进率
#define TONG_EVENT_NOTIFY_FUNCTION              "OnSendTongEvent"   // 帮会消息通知(Server端远程调用函数名)

// 获取家族列表相关，最大页数，和每页最大个数
#define MAX_TONG_LIST_ITEM_NUM					50

// 帮会操作历史记录
#define MAX_MEMBER_MANAGE_RECORD_CONUT      120
#define MAX_FUND_RECORD_COUNT               100
#define MAX_TECH_TREE_RECORD_COUNT          80
#define MAX_MISSION_RECORD_COUNT            80
#define MAX_REPERTORY_RECORD_COUNT          120
#define TOTAL_RECORD_COUNT \
(MAX_MEMBER_MANAGE_RECORD_CONUT + MAX_FUND_RECORD_COUNT + MAX_TECH_TREE_RECORD_COUNT + MAX_MISSION_RECORD_COUNT + MAX_REPERTORY_RECORD_COUNT)

#define TONG_OPERATION_HISTORY_PAGE_SIZE    10

#define KTONG_REPERTORY_INVALID_PAGE_POS    0xff
#pragma pack(1)
// 帮会组织结构描述数据,用于帮会编辑器存盘格式以及帮会数据库存盘格式
struct KTongGroupV3
{
    char szName[_NAME_LEN];
    bool bEnable;
    int  nWage;                                                 // 工资额度(银币)

    DWORD   dwBaseOperationMask;                                // 基本操作权限
    BYTE    byAdvanceOperationMask[MAX_TONG_GROUP];             // 高级操作,即与被操作对象所属Group相关的操作权限

    DWORD   dwBaseGrantMask[MAX_TONG_GROUP];                    // 基本授权
    BYTE    byAdvanceGrantMask[MAX_TONG_GROUP][MAX_TONG_GROUP]; // 高级授权,即与被操作对象所属Group相关的操作授权
};

struct KTongGroup // v2
{
	char szName[_NAME_LEN];
	bool bEnable;
	int  nWage;                                                 // 工资额度(银币)

	BYTE    byBaseOperationMask;                                // 基本操作权限
	BYTE    byAdvanceOperationMask[MAX_TONG_GROUP];             // 高级操作,即与被操作对象所属Group相关的操作权限

	BYTE    byBaseGrantMask[MAX_TONG_GROUP];                    // 基本授权
	BYTE    byAdvanceGrantMask[MAX_TONG_GROUP][MAX_TONG_GROUP]; // 高级授权,即与被操作对象所属Group相关的操作授权
};


struct KTongSchemaData
{
    int        nMasterGroup;
    int        nDefaultGroup;
	KTongGroup Data[USE_TONG_GROUP];
    // KTongGroup Data[MAX_TONG_GROUP];
};

struct KinActivityInfo
{
	DWORD dwOpenTime;   // 开启时间
	BYTE  bySignUp;     // 是否已报名
};

struct QTongApplyerInfo
{
	INT   nPlayerId;
	char  szName[_NAME_LEN];
	INT   nLevel;
	INT   nFightScore;
	DWORD dwLastLogoutTime;
};

//--------------------------
// 版本兼容

struct KTONG_GROUP_V2
{
    char szName[_NAME_LEN];
    bool bEnable;
    int  nWage;                                                 // 工资额度(银币)

    BYTE    byBaseOperationMask;                                // 基本操作权限
    BYTE    byAdvanceOperationMask[MAX_TONG_GROUP];             // 高级操作,即与被操作对象所属Group相关的操作权限

    BYTE    byBaseGrantMask[MAX_TONG_GROUP];                    // 基本授权
    BYTE    byAdvanceGrantMask[MAX_TONG_GROUP][MAX_TONG_GROUP]; // 高级授权,即与被操作对象所属Group相关的操作授权
};

struct KTong_Schema_Data_V2
{
    int             nMasterGroup;
    int             nDefaultGroup;
    KTONG_GROUP_V2  Data[MAX_TONG_GROUP];
};
//--------------------------

// 帮会成员信息网络传输结构, 客户端表示
struct KTongMemberInfo
{
    DWORD   dwID;
    int     nGroupID;                           // 所属成员组
	int32_t  nJoinTime;                          // 加入时间
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // 备注
	int32_t  nLastOfflineTime;                   // 最后在线时间
    char    szName[_NAME_LEN];
    BYTE    byLevel;
    BYTE    byForceID;
    DWORD   dwMapID;
    bool    bIsOnline;
	int     nSalary;                            // 贡献度
	int     nDailyContribution;                 // 每日贡献度
};

// 帮会大事记
struct KTongIncidentRecordData
{
    int32_t      nTime;
    char        szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct SyncKinActivityInfo
{
	INT   nKinActivityIndex;
	DWORD dwActivityOpenTime;    // 当前开启时间
	DWORD dwLastChangeTime;      // 最后修改时间
	BYTE  bySignUp;              // 是否已报名
};
#pragma pack()

// 服务端帮会成员信息
struct KTongMember
{
    DWORD   dwID;
    int     nGroupID;                           // 所属成员组
	int32_t  nJoinTime;                          // 加入时间
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // 备注
    int32_t  nLastOfflineTime;                   // 最后在线时间
    int     nSalary;                            // 贡献度
	int     nDailyContribution;                 // 每日贡献度
//---------- 以下变量周期性查询更新 ----------
    int     nLevel;
    DWORD   dwForceID;
    DWORD   dwMapID;
	DWORD	dwFightScore;
    BOOL    bIsOnline;
//--------------------------------------------
	int		nLastUpdateFrame;
    int     nLastUpdateCount;
};

struct KTongIncidentRecord
{
    DWORD   dwID;
    int32_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct KinActivity
{
	INT nKinActivityIndex;
	INT nKinActivityPersist;
	std::vector<DWORD> vecKinOpenTimes;
};


struct QTongUpgradeNeed
{
	INT nPealNeed;
	INT nJadeiteNeed;
	INT nAgateNeed;
	INT nDiamondNeed;
};

enum QTONG_ITEM_GDPL
{
	totPealGenre = 18,
	totPealDetail = 1,
	totPealParticular = 262,
	totJadeiteGenre = 18,
	totJadeiteDetail = 1,
	totJadeiteParticular = 263,
	totAgateGenre = 18,
	totAgateDetail = 1,
	totAgateParticular = 264,
	totDiamondGenre = 18,
	totDiamondDetail = 1,
	totDiamondParticular = 265,
	totImpeachGenre = 18,
	totImpeachDetail = 1,
	totImpeachParticular = 266
};

enum KTONG_OPERATION_TYPE
{
    totInvalid = 0,

    //----------------------基础权限----------------------------
    //totBaseOperationBegin,

    totSpeak,                   // 	在帮会频道发言
    totJoinManagerChannel,      //	加入官员频道，并可以查看频道发言
    totSpeakInManagerChannel,   //	在官员频道发言
    totCheckOperationRecord,    //	查看帮会操作记录
    totModifyAnnouncement,      //	修改帮会公告
    totModifyOnlineMessage,     //	修改帮会上线提示
    totModifyIntroduction,      //	修改帮会介绍
    totModifyRules,             //	修改帮会帮规
    totModifyMemorabilia,       //	修改帮会大事记
    totDevelopTechnology,       // 	投资科技树
    totProductItem,	            //  生产道具
	totAllowOrDenyApplyer,      //  同意或拒绝他人入族

    //totPutRepositoryPage1,      //  往仓库内第一页存放物品
    totPutRepositoryPage2,      // 	往仓库内第二页存放物品
    totPutRepositoryPage3,	    //  往仓库内第三页存放物品
    totPutRepositoryPage4,	    //  往仓库内第四页存放物品
    totPutRepositoryPage5,	    //  往仓库内第五页存放物品
    totPutRepositoryPage6,	    //  往仓库内第六页存放物品
    totPutRepositoryPage7,	    //  往仓库内第七页存放物品
    totPutRepositoryPage8,	    //  往仓库内第八页存放物品
    totPutRepositoryPage9,	    //  往仓库内第九页存放物品
    totPutRepositoryPage10,     //  往仓库内第十页存放物品

    //totGetFromRepositoryPage1,  //	从仓库内第一页取出物品
    totGetFromRepositoryPage2,  //	从仓库内第二页取出物品
    totGetFromRepositoryPage3,  //	从仓库内第三页取出物品
    totGetFromRepositoryPage4,  //	从仓库内第四页取出物品
    totGetFromRepositoryPage5,  //	从仓库内第五页取出物品
    totGetFromRepositoryPage6,  //	从仓库内第六页取出物品
    totGetFromRepositoryPage7,  //	从仓库内第七页取出物品
    totGetFromRepositoryPage8,  //	从仓库内第八页取出物品
    totGetFromRepositoryPage9,  //	从仓库内第九页取出物品
    totGetFromRepositoryPage10, //	从仓库内第十页取出物品
    
    totModifyCalendar,          //	增加/编辑/删除帮会相关的日历项
    totManageTask,              //	发起/取消帮会任务
    totManageVote,              //	发起/取消投票

    //totBaseOperationEnd,

    //----------------------高级权限----------------------------

    //totAdvanceOperationBegin,

    totAddToGroup,              //	加人入某一用户组
    totMoveFromGroup,           //	将人从某一用户组中移除
    totModifyWage,              //	编辑他人工资
    totPaySalary,               //	发工资
    totModifyMemberRemark,      //	编辑他人的备注
    totModifyGroupName,         //  修改组名
    //totAdvanceOperationEnd,

    totTotal
};

// 帮会状态
enum KTONG_STATE
{
    tsInvalid,
    tsTrialTong,            // 尚在筹备期
    tsNormalTong,           // 正常状态
    tsDisbandTong,          // 濒临淘汰
    tsTotal
};

enum QTONG_CAMP
{
    campXian = 1,     // 仙阵营
	campFo   = 2,     // 佛阵营
};

// -----------------------    帮会基础信息同步    -------------------------
enum KTONG_SYNC_DATA_TYPE
{
    tdtBaseInfo,
    tdtDevelopmentInfo,
    tdtAnnouncement,
    tdtOnlineMessage,
    tdtIntroduction,
    tdtRules,
    tdtSchemaInfo,
    tdtMemorabilia,
	tdtApplyersInfo,
    tdtTotal
};

enum KTONG_MODIFY_REQUEST_TYPE
{
    tmrBegin,
    tmrAnnouncement,
    tmrOnlineMessage,
    tmrIntroduction,
    tmrRules,
    tmrMemorabilia,
    tmrEnd
};

enum KTONG_MODIFY_SCHEMA_TYPE
{
    tmsBegin,
    tmsGroupName,
    tmsBaseOperation,
    tmsAdvanceOperation,
    tmsGroupWage,
	tmsBaseGrant,
	tmsAdvanceGrant,
    tmsEnd
};

enum KTONG_UPDATE_FLAG
{
    tufInfo,
    tufRoster
};

enum KCHANGE_TONG_REASON
{
    ctrCreate,
    ctrDisband,
    ctrJoin,
    ctrQuit,
    ctrFired
};

#pragma pack(1)

struct KTONG_BASE_INFO
{
    DWORD       dwMasterID;
    BYTE        byLevel;
    int         nMaxMemberCount;
    BYTE        byState;
    int32_t      nStateTimer;
    char        szTongName[_NAME_LEN];
    BYTE        byCamp;
	BYTE		byFreeJoin;
	DWORD       dwPearlNum;          // 珍珠数量
	DWORD		dwJadeiteNum;        // 翡翠数量
	DWORD		dwAgateNum;          // 玛瑙数量
	DWORD		dwDiamondNum;        // 钻石数量
};

struct KTONG_DEVELOPMENT_INFO
{
    int     nFound;
    int     nMaxWageRate;
    int     nDevelopmentPoint;
    int     nUsedDevelopmentPoint;
    BYTE    m_byTechTree[MAX_TONG_TECHTREE_NODE_NUM];
};

struct KTONG_TEXT_INFO
{
    uint32_t uDataLen;
    BYTE   byContent[0];
};

struct KTONG_SCHEMA_DATA_INFO
{
    KTongSchemaData SchemaData;
};

struct KTONG_MEMORABILIA_INFO
{
    BYTE                byCount;
    KTongIncidentRecord Memorabilia[0];
};

struct KTONG_MODIFY_GROUP_NAME
{
    char szGroupName[_NAME_LEN];
};

struct KTONG_MODIFY_BASE_OPERATION_MASK
{
    BYTE byOffset;
    bool bValue;
};

struct KTONG_MODIFY_ADVANCE_OPERATION_MASK
{
    BYTE byIndex;
    BYTE byOffset;
    bool bValue;
};

struct KTONG_MODIFY_BASE_GRANT_MASK
{
	BYTE byIndex;
	BYTE byOffset;
	bool bValue;
};

struct KTONG_MODIFY_ADVANCE_GRANT_MASK
{
	BYTE byIndex1;
	BYTE byIndex2;
	BYTE byOffset;
	bool bValue;
};

enum KTONG_MODIFY_MEMORABILIA_TYPE
{
    mmtAdd,
    mmtModify,
    mmtDelete
};
// 修改帮会大事记
struct KTONG_MODIFY_MEMORABILIA_INFO
{
    BYTE byOperationType;
};

struct KTONG_ADD_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    int32_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct KTONG_MODIFY_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    DWORD   dwID;
    int32_t  nTime;
    char    szDescription[TONG_MEMORABILIA_RECORD_LEN];
};

struct KTONG_DELETE_MEMORABILIA : KTONG_MODIFY_MEMORABILIA_INFO
{
    DWORD dwID;
};

struct KTONG_ITEM_SYNC_DATA
{
    BYTE    byPos;
    BYTE    byRent;
	BYTE    byRemoveOnExpire;
	int32_t  nExpireTime;
	CHAR    szRenterName[_NAME_LEN];
	CHAR    szPutterName[_NAME_LEN];
    BYTE    byDataLen;
    BYTE    byData[0];
};

struct KIN_REPERTORY_GRID_SYNC_DATA : KVIEWITEMINFO
{
	BYTE    byPos;
	BYTE    bRent;
	BYTE    bRemoveOnExpire;
	int32_t  nExpireTime;
	CHAR    szRenterName[_NAME_LEN];
	CHAR    szPutterName[_NAME_LEN];
	GUID	sGuid;
};

#pragma pack()

struct Kin_Repertory_Rent_Config
{
	BOOL    s_bKinRepertoryEnable;
	BOOL    s_bKinRepertoryRentEnable;
	int32_t  s_tRentPeriod;
};

// -----------------------    帮会广播    -------------------------

enum TONG_BROADCAST_MESSAGE_TYPE
{
    tbmInvalid,
    tbmTongStateChange,
    tbmGroupRightChange,
    tbmGroupNameChange,
    tbmGroupWageChange,
    tbmMemberJoin,
    tbmMemberQuit,
    tbmMemberFired,
    tbmMemberChangeGroup,
    tbmMasterChange,
    tbmCampChange,
    tbmMemberOnline,
    tbmMemberOffline,
    tbmMaxMemberCountChange,
    tbmGroupEnabled,
	tbmTongFreeJoinStateChange,
	tbmSetTongActivityTime,
    tbmTotal
};

#pragma pack(1)

struct QTONG_ACTIVITY_TIME_SET_BROADCAST
{
	DWORD dwTongActivityIndex;
	DWORD dwOpenTime;
};

struct KTONG_CHANGE_GROUP_RIGHT_BROADCAST
{
    char szGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_GROUP_NAME_BROADCAST
{
    char szOldGroupName[_NAME_LEN];
    char szNewGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_GROUP_WAGE_BROADCAST
{
    char szGroupName[_NAME_LEN];
    int  nWage;
};

struct KTONG_MEMBER_JOIN_OR_QUIT_BROADCAST
{
    char szMemberName[_NAME_LEN];
};

struct KTONG_MEMBER_CHANGE_GROUP_BROADCAST
{
    char szMemberName[_NAME_LEN];
    char szOldGroupName[_NAME_LEN];
    char szNewGroupName[_NAME_LEN];
};

struct KTONG_CHANGE_MASTER_BROADCAST
{
    char szOldMasterName[_NAME_LEN];
    char szNewMasterName[_NAME_LEN];
};

struct KTONG_MEMBER_ONLINE_STATE_CHANGE_BROADCAST
{
    char szMemberName[_NAME_LEN];
};

struct KTONG_GROUP_ENABLED_BROADCAST
{
    char szGroupName[_NAME_LEN];
};

enum INVITE_TONG_RESULT_CODE
{
    itrInvalid,

    itrSuccess,
    itrRefuse,
    itrChargeLimit,
	itrCampLimit,
	itrLevelLimit,

    itrTotal
};

#pragma pack()

// ---------------------    帮会历史记录    ---------------------
enum KTONG_OPERATION_HISTORY_TYPE
{
    ohtMemberManage = 0,
    ohtFund,
    ohtTechTree,
    ohtActivity,
    ohtRepertory,
    ohtTotal
};

enum KTONG_HISTORY_RECORD_TYPE
{
    hrtInvalied,

    hrtMemberManageBegin,

    hrtChangeGroupBaseRight,
    hrtChangeGroupAdvanceRight,
    hrtChangeMemberGroup,
    hrtJoin,
    hrtQuit,
    hrtKickOut,

    hrtMemberManageEnd,
// ------------------------
    hrtFundBegin,

    hrtChangeGroupWage,
    hrtPaySalary,
    hrtSaveFund,

    hrtFundEnd,
// ------------------------
    hrtTechTreeBegin,

    hrtActivateTechtree,
    hrtProductItem,

    hrtTechTreeEnd,
// ------------------------
    hrtActivityBegin,

    hrtMissionStart,
    hrtMissionComplete,
    hrtMissionFailed,
    hrtWarStart,
    hrtWarWin,
    hrtWarLose,
    hrtAchievement,

    hrtActivityEnd,
// ------------------------
    hrtRepertoryBegin,

    hrtRepertoryTakeItem,
    hrtRepertoryPutItem,

    hrtRepertoryEnd,

    hrtTotal
};

#pragma pack(1)
struct KTONG_SYNC_HISTORY
{
    BYTE    byRecordType;
    int32_t  nTime;
};

// ------------ 人事变动 ----------------

struct KTONG_SYNC_HISTORY_CHANGE_GROUP_BASE_RIGHT : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTargetGroup;
    BYTE    byRightOffset;
    BYTE    byIsOpen;
};

struct KTONG_SYNC_HISTORY_CHANGE_GROUP_ADVANCE_RIGHT : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    bySrcGroup;
    BYTE    byDstGroup;
    BYTE    byRightOffset;
    BYTE    bIsOpen;
};

struct KTONG_SYNC_HISTORY_CHANGE_MEMBER_GROUP : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    char    szTargetMember[_NAME_LEN];
    BYTE    bySrcGroup;
    BYTE    byDstGroup;
};

struct KTONG_SYNC_HISTORY_JOIN_OR_QUIT : KTONG_SYNC_HISTORY
{
    char szMember[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_KICK_OUT_MEMBER : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    char  szTargetMember[_NAME_LEN];
};

// ------------ 资金变动 ----------------
struct KTONG_SYNC_HISTORY_CHANGE_GROUP_WAGE : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTargetGroup;
    DWORD   dwOldWage;
    DWORD   dwNewWage;
};

struct KTONG_SYNC_HISTORY_PAY_SALARY : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwTotalSalary;
};

struct KTONG_SYNC_HISTORY_SAVE_FUND : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwSavePealNum;
	DWORD dwSaveJadeiteNum;
	DWORD dwSaveAgateNum;
	DWORD dwSaveDiamondNum;
};

// ------------ 科技树和道具出产 ------------

struct KTONG_SYNC_HISTORY_ACTIVATE_TECHTREE : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byNode;
    BYTE    byLevel;
    DWORD   dwConsumeDevelopmentPoint;
    DWORD   dwConsumeFund;
};

struct KTONG_SYNC_HISTORY_PRODUCT_ITEM : KTONG_SYNC_HISTORY
{
    char  szOperator[_NAME_LEN];
    DWORD dwRecipeID;
    DWORD dwCount;
    DWORD dwConsumeDevelopmentPoint;
    DWORD dwConsumeFund;
};

// ------------ 帮会活动 ----------------

struct KTONG_SYNC_HISTORY_MISSION_START : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    DWORD   dwMission;
    int32_t  nMissionStartTime;
};

struct KTONG_SYNC_HISTORY_MISSION_END : KTONG_SYNC_HISTORY
{
    DWORD   dwMission;
};

struct KTONG_SYNC_HISTORY_WAR_START : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    char    szEnemyTong[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_WAR_END : KTONG_SYNC_HISTORY
{
    char   szEnemyTong[_NAME_LEN];
};

struct KTONG_SYNC_HISTORY_ACHIEVEMENT : KTONG_SYNC_HISTORY
{
    DWORD   dwAchievement;
};

// ------------ 帮会仓库 ----------------

struct KTONG_SYNC_HISTORY_REPERTORY_TAKE_OR_PUT_ITEM : KTONG_SYNC_HISTORY
{
    char    szOperator[_NAME_LEN];
    BYTE    byTabType;
    WORD    wTabIndex;
    WORD    wStackNum;
};

// ------------帮会列表------
struct KTONG_LIST_ITEM
{
	DWORD dwTongID;
	char szTongName[_NAME_LEN];
	char szMasterName[_NAME_LEN];
	BYTE byLevel;
	BYTE byCamp;
	BYTE byMemberNum;
	BYTE byMaxMemberNum;
};

#pragma pack()

#endif
