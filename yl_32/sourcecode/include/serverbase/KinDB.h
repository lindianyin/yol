#ifndef _KTONGDB_H_
#define _KTONGDB_H_

// 帮会数据库存盘数据结构定义

#define TONG_DB_VERSION     1
#define MAX_TONG_DATA_LEN   (1024 * 1024)

enum KTongDataSection
{
    eTongDataBegin,
    eTongBaseInfo,
    eTongSchema,
    eTongRoster,
    eTongAnnouncement,
    eTongOnlineMessage,
    eTongIntroduction,
    eTongRules,
    eTongMemorabilia,
    eTongRepertory,
    eTongDevelopmentInfo,
    eTongHistory,
    eTongDataEnd
};

#pragma pack(1)
struct KTONG_DB_HEADER
{
    int nVersion;
};

struct KTONG_DB_SECTION
{
    int     nType;
    size_t  uDataLen;
    BYTE    byData[0];
};

struct KTONG_DB_BASEINFO
{
    char    szName[_NAME_LEN];
    DWORD   dwMaster;           // 帮主
    WORD    nLevel;
	WORD	bFreeJoin;			// 是否自由招募
    WORD    wMaxMemberCount;    // 成员上限
	DWORD   dwPearlNum;          // 珍珠数量
	DWORD   dwJadeiteNum;        // 翡翠数量
	DWORD   dwAgateNum;          // 玛瑙数量
	DWORD   dwDiamondNum;        // 钻石数量
    BYTE    byTongState; 
    time_t  nStateTimer;
    BYTE    byCamp;             // 阵营
    BYTE    byApplyerNum;       // 申请入族者数量
	BYTE    byActOpenTimeNum;   // 设置的活动开启时间数量
};

struct KTONG_DB_SCHEMA
{
    KTongSchemaData Schema;
};

//--------------------------
// 版本兼容

struct KTONG_DB_SCHEMA_V2
{
    KTong_Schema_Data_V2 Schema;
};

//--------------------------

struct KTONG_DB_ROSTER
{
    int     nMemberCount;
    BYTE    byData[0];
};

struct KTONG_DB_MEMORABILIA
{
    time_t  nTime;
    BYTE    byDataLen;
    BYTE    byData[0];
};

struct KTONG_DB_MEMBER_DATA
{
    DWORD   dwID;
    int     nGroupID;                           // 所属成员组
	time_t  nJoinTime;                          // 加入时间
    char    szRemark[TONG_MEMBER_REMARK_LEN];   // 备注
    time_t  nLastOfflineTime;                   // 最后在线时间
    int     nSalary;                            // 工资储蓄(银币)
	int     nDailyContribution;                 // 每日贡献度
};

struct KTONG_DB_DEVELOPMENT_DATA
{
    int     nFund;              // 资金(银币)
    int     nMaxWageRate;
    int     nDevelopmentPoint;  // 发展点
    int     nUsedDevelopmentPoint;
    BYTE    byTechTree[MAX_TONG_TECHTREE_NODE_NUM];
};

struct KTONG_REPERTORY_DB_PAGE
{
    BYTE    byPageIndex;
    BYTE    byEnable;
    size_t  uItemDalaLen;
    BYTE    byItemData[0];
};

struct KTONG_REPERTORY_DB_ITEM
{
    BYTE byPos;
    BYTE byDataLen;
    BYTE byData[0];
};

struct KTONG_DB_HISTORY_CATEGORY
{
    BYTE byType;
    WORD wRecordCount;
};
#pragma pack()

#endif

