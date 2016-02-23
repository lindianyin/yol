#ifndef _AWARDDEF_H__
#define _AWARDDEF_H__

#include "onlinegamemodule/item2/itemdef.h"

#define AWARD_POOL_MAX_COUNT 10000

#define ASSET_AWARD_MAX_ITEM_NUM 10

#define NUMERICAL_TABLE_FILE                 "\\setting\\award\\moduleaward.txt"
#define AWARD_TABLE_FILE_FOLDER              "\\setting\\award\\numerical_award\\"

#define ASSET_AWARD_FILE                 "\\setting\\award\\assetaward.txt"
#define ASSET_AWARD_FILE_FOLDER          "\\setting\\award\\asset_award\\"

#define EXP_AWARD_FILE				"\\setting\\award\\expaward.txt"
#define EXP_AWARD_FILE_FOLDER		"\\setting\\award\\exp_award\\"

struct KAwardItem
{
	INT nGenre;
	INT nDetail;
	INT nParticular;
	INT nLevel;
	INT nAmount;
	INT nTimeOut;
};

struct KAwardSaveData
{
	CHAR szModuleName[MAX_NAME_LEN];
	INT nLevel;

	DWORD dwOutdateTime;
	INT nRankIdx;

	INT nEnergy;
	INT nFightingSpirit;
	INT nSilver;
	INT nItemNum;

	KAwardItem Items[0];
};

struct QAssetItem
{
	KITEM_INDEX sIndex;
	INT nAmountMin;
	INT nAmountMax;
	INT nRate;
	INT nTimeOut;
};

// 资源奖励
struct QAssetAward
{
	INT nLevel;                 // 系统等级，具体意义由各个系统决定，比如是帮派代表帮派等级，副本则代表副本等级

	INT nSave;
	INT nOverlayDay;
	INT nAutoGive;
	INT nSync;

    INT nSilver;             // 银两奖励数量
	INT nEnergy;             // 真气奖励数量
	INT nFightingSpirit;     // 战魂奖励数量
	INT nExpAwardId;		// 经验比例玩法id
	INT nPrestige;           // 声望奖励

	INT nItemNum;	// 道具数量

	QAssetItem arrAwardItem[ASSET_AWARD_MAX_ITEM_NUM];
};

struct QModuleAssetAward
{
	CHAR szModuleName[MAX_NAME_LEN];
	std::map<INT, QAssetAward*> mapAssetAwards;
};

// 数值奖励
struct QNumericalAward
{
	INT nLevel;      // 等级
	INT nLife;       // 生命   
	INT nAbility;    // 内力
	INT nAttack;     // 攻击
	INT nDefense;    // 防御
	INT nHit;        // 命中
	INT nDodge;      // 闪避
	INT nCrit;       // 暴击
	INT nResist;     // 抗暴
	INT nCost;       // 花费
};

// 数值阶段奖励
struct QNumericalStageAward
{
	INT nStageId;
	INT nStartIndex;
	INT nEndIndex;
};

// 模块数值奖励
struct QModuleNumericalAward
{
	CHAR szModuleName[MAX_NAME_LEN];
	std::map<INT, QNumericalStageAward*> mapNumericalStageAwards;
	std::vector<QNumericalAward*> vecNumericalAwards;  //
	std::vector<INT> vecNumericalIDs;  //
};

// 经验奖励
struct KExpAward
{
	INT nLevel;
	INT nPercent;
};

#endif // _AWARDDEF_H__