
/*
ROLE_DATA Memory Layout

-----------ROLE_DATA-----------
|                    CRC                       |	<--- CRC of Block Header & Content
|                  Version                    |	<--- ROLE_DATA Version
|                BlockCount                |	<--- Number of Blocks
|                  DataLen                   |	<--- Size of the Remaining (Including Block Header & Content)
|---------BLOCK_HEADER-------|	[Block Header 0]
|                BlockType                  |	<--- One of the KE_DB_ROLE_BLOCK_TYPE
|                    Flag                       |	<--- Not Used Now
|               ContentSize                |	<--- Size of the Content
|---------BLOCK_HEADER-------|	[Block Header 1]
|                BlockType                  |
|                    Flag                       |
|               ContentSize                |
                     ......
                     ......
|---------BLOCK_HEADER-------|	[Block Header n]
|                BlockType                  |
|                    Flag                       |
|               ContentSize                |
|--------BLOCK_CONTENT------|	[Block Content 0]
|                   [Data]                     |	<--- RawData (Stored By PlayerDbMgr), Size Was
|                                                 |	<--- Specified In Block Header 0 Section.
|--------BLOCK_CONTENT------|	[Block Content 1]
|                   [Data]                     | 
|                                                 |
                     ......
                     ......
|--------BLOCK_CONTENT------|	[Block Content n]
|                   [Data]                     | 
|                                                 |
-----------ROLE_DATA-----------

*/

#pragma once

#include "config/gamedef.h"
#include "Runtime/qstring.h"
#include <stdint.h>

#define MAX_ROLE_DATA_SIZE		64 * 1024		// 单个角色数据最大容量
#define MAX_DELAY_ROLE_EXTEND_DATA_NUM  512     // 离线玩家扩展数据块大小

#pragma pack(push, 1)
// -------------------------------------------------------------------------

enum KROLE_DATA_VERSION
{
	emKROLE_DATA_VERSION_PREVIOUS = 6,
	emKROLE_DATA_VERSION_CURRENT
};

// 注意：增加存盘模块最好在后面加，可以不增加DATA_VERSION
// 插在中间会导致数据不兼容，要增加DATA_VERSION
// 不能超过三万个存盘模块，每个存盘块容量不超过64KB
enum KE_DB_ROLE_BLOCK_TYPE
{
	emKDRBT_NONE = 0,
	emKDRBT_BASE_INFO,	// 基础存盘模块(在Base中实现)

	// 扩展存盘模块开始(在PlayerCtrl中实现)，存储结构要在自己的模块中定义，这里只定义基本存盘模块数据结构
	emKDRBT_EXTENSION_START,
	emKDRBT_ITEM = emKDRBT_EXTENSION_START,
	emKDRBT_TASK,
	emKDRBT_PURSE,  //玩家钱袋

	emKDRBT_MISC, // MISC
	emKDRBT_MAIL,	// 邮件
	emDRBT_FIGHT_SKILL,	// 游龙skill数据
	emDRBT_BUFFER,		// 游龙Buffer数据(需要在skill后载入数据，以便对skill操作)

	emDRBT_PET,			// 游龙宠物数据
	emDRBT_FACTION,
	emDRBT_RIDE,
	
	emDRBT_AWARD,
	emDRBT_QUIZ, // 问答题
	emDRBT_ARENA, // 竞技场PVE
	emDRBT_TOWER,	// 伐魔塔
	emDRBT_STAGE,	// 关卡
	emDRBT_MEDITATION,	//打坐
	emDRBT_TITLE,		//称号
	emDRBT_INSTANCE,	// 副本
	emDRBT_ACTIVITY,	// 活动
	emDRBT_RANK,		// 排行榜
	emDRBT_TARGET,		//目标
	emDRBT_VIP,
	emDRBT_CHAT,
	emDRBT_AWARD_LOBBY,
	emDRBT_GEM,
	emDRBT_ARTIFACT,
	emDRBT_BANK,
	emDRBT_SHOP,
	emDRBT_OPENSE,
	emKDRBT_TOTAL
};

struct KDB_ROLE_BLOCK_HEADER
{
	BYTE	byBlockType; // 数据块类型（KE_ROLE_DATA_BLOCK_TYPE）
	WORD	wDataLen; // 本数据块长度(不得超过64k)
	BYTE	byFlag; // 暂时未使用
};

// 创建角色参数 KC2GW_CREATE_ROLE::abyParam
struct KCREATE_ROLE_PARAM
{
	char	szRoleName[_NAME_LEN];
	char    cRoleType;
	BYTE	byFaction;
	UINT	dwMapID;
	int     nMapCopyIndex;
	WORD    wRepresentId[emKPLAYER_REPRESENT_TYPE_COUNT];
};

struct KDB_ROLE_GENERATOR_PARAM
{
	char    szAccountName[_NAME_LEN];
	int     nX;
	int     nY;
	int     nZ;
	BYTE    byDirection;
	KCREATE_ROLE_PARAM UserParam;
};

// 场景位置
struct KDB_ROLE_SCENE
{
	DWORD	dwMapID;
	int	    nMapCopyIndex;
	INT		nX;
	INT		nY;
	INT		nZ;
	BYTE		byFaceDirection;
};

//#pragma warning(disable: 4200) // 弹性成员数组是符合C99标准的
struct KROLE_DATA
{
	DWORD				dwDataLen;
	DWORD				dwCRC;
	WORD				wVersion;
	WORD				wBlockCount : 15;
	WORD				wFirstTimeLogin : 1;
	KDB_ROLE_BLOCK_HEADER		BlockList[];
};

struct KRoleBaseInfo
{
	char                cRoleType; // 性别体型
	KDB_ROLE_SCENE   CurrentPos;
	KDB_ROLE_SCENE   LastEntry;
	BYTE	            byLevel;
	BYTE                byCamp;
	BYTE                byFaction;
	WORD                wRepresentId[emKPLAYER_REPRESENT_TYPE_COUNT];
	int32_t	            nLastSaveTime;
	int32_t              nLastLoginTime;
	int32_t              nTotalGameTime;
	int32_t              nCreateTime;

	BYTE                byReserved[8];
};

#define MAX_DELAY_PET_COUNT 7
#define MAX_DELAY_EQUIP_COUNT 13

// 竞技场战斗记录
struct KArenaRecord
{
	DWORD dwTimestamp;  // 时间
	BOOL bAttack;	// 挑战还是被挑战
	BOOL bResult;	// 结果
	CHAR szTargetName[_NAME_LEN];	// 对方姓名
	INT	nTargetIdx;					// 排名变化(目标排名)
};


// 家族战记录（历任神皇）
struct KKinBattleRecord
{
	DWORD dwRoleId;
	DWORD dwTimestamp;  // 时间
	INT nBattleNum;		// 第几届
	CHAR szKinMasterName[_NAME_LEN];
	CHAR szKinName[_NAME_LEN];
};

//钱庄投资记录
struct KBankInvestRecord
{
	INT nCoins;						//投资数量
	DWORD dwTimestamp;				//时间
	CHAR szRoldName[_NAME_LEN];
};

struct KBuyAllPurchaseRecord
{
	INT nShopId;
	INT nCategoryId;
	INT nGoodsId;		
	INT nCount;			
	DWORD dwRoleId;
	DWORD dwLastTime;
	INT nTotal;//总数
	INT nMaxCount;
};

// 基本信息 TODO refact
struct DB_PLAYER_BASE_DATA
{
	CHAR	szAccount[MAX_NAME_LEN];
	CHAR	szRole[MAX_NAME_LEN];

	DWORD	dwLastLoginIp;
	DWORD	dwCoin; // 下线时BillSys金币剩余数，仅供数据分析用
	union
	{
		BYTE abyReserved[16];
	};

	uint32_t	uTaskVarSize;
	BYTE	abyTaskVar[];
};

struct KROLE_LIST_INFO
{
	DWORD	            dwPlayerID;
	char	            szAccount[_NAME_LEN];
	char	            szRoleName[_NAME_LEN];
	KRoleBaseInfo       BaseInfo;
	BYTE                byCanRename;
	uint32_t              nFreezeTime;
	uint32_t              nDeleteTime;
};

//#pragma warning(default: 4200)

// -------------------------------------------------------------------------
#pragma pack(pop)


// 离线数据

#pragma pack(1)

struct KDelayPetData
{
	WORD wType;
	BYTE byLevel;
	CHAR szPetName[MAX_NAME_LEN];

	BYTE byCharacter;
	BYTE byRankLevel;

	INT nFightScore;

	BYTE byState;

	INT nMaxLife;
	INT nAttack;			// 攻击
	INT nHit;				// 命中
	INT nDuck;				// 闪避
	INT nDef;				// 防御
	INT nCritical;			// 暴击
	INT nResist;		// 抗暴

	INT arySkillIds[3];
	WORD arwSkillLevels[3];  // 技能等级
	INT arySkillValues[12];

	INT aryEquips[7];
	BYTE aryEquipLocks[7];
	INT nQuality;
};

struct QDelaySkillData
{
	INT arrSkillIdAndLevel[36];     // 技能id和等级
	INT nFactionRoute;              // 技能路线
};

struct KDelayAttr
{
	BYTE byAttr;
	INT nValue;
};

struct KDelayRideData
{
	BYTE byLevel;
	INT nMagicRate;
	INT nFightScore;
};

struct KDelayEquipData
{
	INT nDetail;
	INT nParticular;
	BYTE byLevel;

	BYTE byEnhTimes;//

	INT nFightScore;

	KDelayAttr sBaseAttr;

	KDelayAttr aryAppendAttrs[5];
	BYTE aryAppendQuality[5];

	BYTE aryStones[6];
};

// 玩家基础信息
struct KDelayBaseData
{
	DWORD dwRoleId;
	CHAR m_szRoleName[MAX_NAME_LEN];
	BYTE bySex;
	BYTE byLevel;
	DWORD dwExp;
	DWORD dwFightScore;

	DWORD dwTongId;
	CHAR m_szTongName[MAX_NAME_LEN];

	// 基础属性
	INT nMaxLife;
	INT nMaxMana;
	INT nAttack;	 // 攻击
	INT nHit;	 // 命中
	INT nDodge;	 // 闪避
	INT nDef;	 // 防御
	INT nCritical;	 // 暴击
	INT nResist;	 // 抗暴
	BYTE bySpeed;

	// 身体外观
	WORD wBodyRepresentId;
	WORD wHatRepresentId;
	WORD wLeftWeaponRepresentId;
	WORD wRightWeaponRepresentId;
	WORD wBackPendantRepresentId;
	WORD wHeadPendantRepresentId;
	WORD wBottomPendantRepresentId;
	WORD wLeftShoulderRepresentId;
	WORD wRightShoulerRepresentId;
	WORD wRideRepresentId;
	WORD wPetRepresentId;

	// 其他
	INT nArenaRank;		// 竞技场排名
	INT nRelation;	 // 关系
	INT nCamp;	 // 玩家阵营
	INT nVipLevel;    // vip等级
};

// 离线玩家基本数据
struct KDelayRoleData
{
	// 基础数据
	KDelayBaseData sBaseRoleData;

	// 装备数据
	KDelayEquipData aryEquips[MAX_DELAY_EQUIP_COUNT];

	// 宠物数据
	KDelayPetData aryPets[MAX_DELAY_PET_COUNT];

	// 坐骑数据
	KDelayRideData sRideData;

	// 技能数据
	QDelaySkillData sSkillData;

	// 因离线数据采用定长方式，有游戏上线后扩展数据导致之前存的数据不可用的风险，
	// 因此这里加个扩展数据块，之后如需要新增离线数据，写到这里
	INT nExtendData[MAX_DELAY_ROLE_EXTEND_DATA_NUM];          // 扩展数据
};

#pragma pack()

#define	GET_DB_PLAYER_BASE_DATA(pRoleData)  (DB_PLAYER_BASE_DATA*)&((KROLE_DATA*)(pRoleData))->BlockList[((KROLE_DATA*)(pRoleData))->wBlockCount]

#define	GET_ROLE_BLOCK_HEADER(pRoleData, eBlockType) &(pRoleData)->BlockList[(eBlockType) - 1]
