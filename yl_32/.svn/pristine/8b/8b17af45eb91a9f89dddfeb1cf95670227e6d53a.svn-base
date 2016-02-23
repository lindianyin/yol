#pragma once
#define MAX_VIP_LEVEL (10)

struct VipGift
{
	INT	nGenre;
	INT	nDetail;
	INT	nParticular;
	INT	nLevel;
	INT nAwardId;
};

struct VipOverview
{
	INT nNeedCoins;
	INT nAwardId;
	std::map<INT, VipGift*> mapVipGift;
};

struct VipAttributeInfo
{
	INT nLevel;			//vip等级
	INT nNum;			//次数
	INT nType;			//类型
	INT nIsOpen;		//是否开启
	INT nRate;		//倍率
};

struct VipAttribute
{
	CHAR szModuleName[MAX_SENTENCE_LENGTH];
	std::map<INT, VipAttributeInfo*> mapVipInfo;
};

enum QE_VIP_FUNCTION
{
	emVIP_ID_BUY_PRESTIGE = 21,
	emVIP_ID_PET_SOUL_CHANGE = 22,
};