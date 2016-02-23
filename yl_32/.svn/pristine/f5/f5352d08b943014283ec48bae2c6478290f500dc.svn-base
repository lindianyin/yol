#ifndef _RANKDEF_H__
#define _RANKDEF_H__

#include "serverbase/RankDef.h"

#pragma pack(1)

enum KE_RANK_UP_OR_DOWN
{
	emRANK_COMP_NONE = 0,
	emRANK_COMP_FAIR = 1,	// 持平
	emRANK_COMP_UP,
	emRANK_COMP_DOWN,
};

struct KRoleRankInfo
{
	CHAR szRoleName[MAX_NAME_LEN];
	BYTE byLevel;
	INT nFightScore;
	CHAR szTongName[MAX_NAME_LEN];
	BYTE byUporDown;
	BYTE byUporDownCount;
	DWORD dwLastModifyTime;
	DWORD dwExValue;	// 附加值
	BYTE bySex;
};

struct KPetRankInfo
{
	CHAR szPetName[MAX_NAME_LEN];
	BYTE byLevel;
	INT nFightScore;
	CHAR szOwnerName[MAX_NAME_LEN];
	CHAR szTongName[MAX_NAME_LEN];
	BYTE byUporDown;
	BYTE byUporDownCount;
	DWORD dwLastModifyTime;
	INT nQuality;
	BYTE bySex;
};

struct KRideRankInfo
{
	BYTE byLevel;
	CHAR szOwnerName[MAX_NAME_LEN];
	INT nFightScore;
	CHAR szTongName[MAX_NAME_LEN];
	BYTE byUporDown;
	BYTE byUporDownCount;
	DWORD dwLastModifyTime;
	BYTE bySex;
};

struct KTongRankInfo
{
	INT nMemberNum;
	CHAR szTongName[MAX_NAME_LEN];
	CHAR szMasterName[MAX_NAME_LEN];
	DWORD dwFightScore;
	DWORD dwSalary;
	BYTE byUporDown;
	BYTE byUporDownCount;
	//BYTE bySex;
};

#define RANK_OUTDATE_TIME (7 * 3600 * 24)			// 7天超时时间

#pragma pack()

#endif // _RANKDEF_H__