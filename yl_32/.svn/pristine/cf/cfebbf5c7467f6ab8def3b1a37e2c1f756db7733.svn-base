#ifndef __KRANKMANAGER_H__
#define __KRANKMANAGER_H__

#include "stdafx.h"
#include "serverbase/protocol/Relay_GS_Protocol.h"
#include "rankdef.h"

class KRankManager
{
public:
	KRankManager();
	~KRankManager();

public:
	BOOL Init();
	BOOL UnInit();

public:
	VOID InitRanks();
	VOID InitOpenServerRanks();


	//刷新 个人排行->战力榜
	BOOL InitRoleRank();
	
	BOOL InitPetLvRank();
	BOOL InitPetFSRank();
	BOOL InitRideRank();
	BOOL InitMpjjRank();
	BOOL InitLGRank();
	BOOL InitSmzcRank();

	BOOL InitTongFSRank();
	BOOL InitTongSrRank();

	
	//开服活动
	BOOL InitPlayerLevel();
	BOOL InitRideLvRank();
	BOOL InitArenaRank();	
	BOOL InitPetLvSaveRank();
	BOOL InitLGLvRank();
	BOOL InitTowerRank();
	BOOL InitRoleFsRank();

	
	//BOOL InitTower();
	BOOL InitOpenServerRank();		//初始化保存的排行榜
public:
	BOOL OnGetRoleRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetPetLvRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetPetFSRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetRideRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetMpjjRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue);
	BOOL OnGetLGRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue);
	BOOL OnGetSmzcRankIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue);

	BOOL OnGetTongFSIdRespond(DWORD dwTongId, INT nRankIdx, DWORD dwRankValue);
	BOOL OnGetTongSrIdRespond(DWORD dwTongId, INT nRankIdx, DWORD dwRankValue);

	BOOL OnGetPlayerLevelIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetTowerIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);

	BOOL OnetGetRideLVRanksSaveIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetAreanRanksSaveIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue);
	BOOL OnGetGetPetLVSaveIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);
	BOOL OnGetLGSaveIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwRankValue);
	BOOL OnGetRoleFightScoreSaveIdRespond(DWORD dwRoleId, INT nRankIdx, DWORD dwLastModifyTime = 0);


	BOOL OnDelayLoadRespond(IMemBlock* piBuffer, DWORD dwSrcId, DWORD dwRoleId);
	//获得战斗力榜人数
	INT GetRoleRankNum() { return m_nRoleRanksNum; }
	//获得神仙等级榜人数
	INT GetPetLvRankNum() { return m_nPetLvRanksNum; }
	INT GetPetFSRankNum() { return m_nPetFSRanksNum; }
	INT GetRideRankNum() { return m_nRideRanksNum; }
	INT GetMpjjRankNum() { return m_nMpjjRanksNum; }
	INT GetLGRankNum() { return m_nLGRanksNum; }
	INT GetSmzcRankNum() { return m_nSmzcRanksNum; }

	INT GetTongFsRankNum() { return m_nTongFSRanksNum; }
	INT GetTongSrRankNum() { return m_nTongSrRanksNum; }

	INT GetPlayerLevelNum() { return m_nRoleLevelRanksNum; }
	INT GetTowerkNum() { return m_nTowerRanksNum; }

	INT GetRideLVRanksSaveNum() { return m_nRideLvRanksNum; }
	INT GetAreanRanksSaveNum() { return m_nAreanRanksNum; }
	INT GetPetLVSaveNum() { return m_nPetLvNum; }
	INT GetLGSaveNum() { return m_nLGNum; }
	INT GetRoleFightScoreSaveNum() { return m_nRoleFightScoreNum; }

	INT GetPlayerRank(INT nRankId, DWORD dwRoleId);
	BOOL GetTongRank(INT nRankId, DWORD dwTongId);

private:
	BOOL OnDelayLoadRoleRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadPetLvRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadPetFSRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadRideRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadMpjjRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadLGRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadSmzcRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadPlayerLevelRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadTowerRankRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);

	BOOL OnDelayLoadRideLVRanksSaveRanksRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadAreanSaveRanksRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadPetLVSaveRanksRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadLGSaveRanksRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
	BOOL OnDelayLoadRoleFightSaveRanksRespond(KDelayRoleData* pDelayData, DWORD dwRoleId);
public:
	BOOL OnGetTongFsRankInfoRespond(DWORD dwTongId, INT nTongMemberNum, LPCTSTR szTongeName, LPCTSTR szMasterName);
	BOOL OnGetTongSrRankInfoRespond(DWORD dwTongId, INT nTongMemberNum, LPCTSTR szTongName, LPCTSTR szMasterName);

public:
	KRoleRankInfo m_arrRoleRanks[emRANK_MAX_COUNT];		// 个人排行->战斗力榜
	
	KPetRankInfo m_arrPetLvRanks[emRANK_MAX_COUNT];		// 个人排行->神仙等级榜
	KPetRankInfo m_arrPetFSRanks[emRANK_MAX_COUNT];		// 神仙战斗力
	KRideRankInfo m_arrRideRanks[emRANK_MAX_COUNT];		// 坐骑战斗力
	KRoleRankInfo m_arrMpjjRanks[emRANK_MAX_COUNT];		// 门派竞技
	KRoleRankInfo m_arrLGRanks[emRANK_MAX_COUNT];       // 炼卦
	KRoleRankInfo m_arrSmzcRanks[emRANK_MAX_COUNT];     // 神魔战场

	KTongRankInfo m_arrTongFSRanks[emRANK_MAX_COUNT];		// 家族总战斗力
	KTongRankInfo m_arrTongSrRanks[emRANK_MAX_COUNT];		// 家族总贡献度

	//开服活动排行榜
	KRoleRankInfo m_arrRoleLevelRanks[emRANK_MAX_COUNT_OPEN_SERVICE];			// 玩家等级排行榜
	KRideRankInfo m_arrRideLvRanks[emRANK_MAX_COUNT_OPEN_SERVICE];				//坐骑等级
	KRoleRankInfo m_arrAreanSaveRanks[emRANK_MAX_COUNT_OPEN_SERVICE];			//竞技场	
	KPetRankInfo m_arrPetLVSaveRanks[emRANK_MAX_COUNT_OPEN_SERVICE];			//宠物等级	
	KRoleRankInfo m_arrLGSaveRanks[emRANK_MAX_COUNT_OPEN_SERVICE];				//炼卦
	KRoleRankInfo m_arrTowerRanks[emRANK_MAX_COUNT_OPEN_SERVICE];				// 伏魔塔排行榜
	KRoleRankInfo m_arrRoleFightSaveRanks[emRANK_MAX_COUNT_OPEN_SERVICE];		//战斗力

private:
	std::map<INT, INT> m_mapRoleRanks;
	std::map<INT, INT> m_mapRoleOldRanks;
	std::map<INT, DWORD> m_mapRoleFsModifyTime;

	std::map<INT, INT> m_mapPetFSRanks;
	std::map<INT, INT> m_mapPetFSOldRanks;
	std::map<INT, DWORD> m_mapPetFSModifyTime;

	std::map<INT, INT> m_mapPetLvRanks;
	std::map<INT, INT> m_mapPetLvOldRanks;
	std::map<INT, DWORD> m_mapPetLvModifyTime;

	std::map<INT, INT> m_mapRideRanks;
	std::map<INT, INT> m_mapRideOldRanks;
	std::map<INT, DWORD> m_mapRideFSModifyTime;

	std::map<INT, INT> m_mapMpjjRanks;
	std::map<INT, DWORD> m_mapMpjjValues;	// 门派竞技积分
	std::map<INT, INT> m_mapMpjjOldRanks;

	std::map<INT, INT> m_mapLGRanks;		// 炼卦
	std::map<INT, DWORD> m_mapLGValues;
	std::map<INT, INT> m_mapLGOldRanks;

	std::map<INT, INT> m_mapSmzcRanks;
	std::map<INT, DWORD> m_mapSmzcValues;	// 门派竞技积分
	std::map<INT, INT> m_mapSmzcOldRanks;

	std::map<INT, INT> m_mapTongFSRanks;
	std::map<INT, INT> m_mapTongFSOldRanks;
	std::map<INT, DWORD> m_mapTongFSValues;

	std::map<INT, INT> m_mapTongSrRanks;
	std::map<INT, INT> m_mapTongSrOldRanks;
	std::map<INT, DWORD> m_mapTongSrValues;


	//开服保存排行榜
	std::map<INT, INT> m_mapRoleLevelRanks;//玩家等级
	std::map<INT, INT> m_mapRoleLevelOldRanks;
	std::map<INT, DWORD> m_mapRoleLevelModifyTime;
	
	std::map<INT, INT> m_mapRideLvRanks;//坐骑等级
	std::map<INT, INT> m_mapRideLvOldRanks;
	std::map<INT, DWORD> m_mapRideLvRanksModifyTime;

	std::map<INT, INT> m_mapAreanSaveRanks;//竞技场
	std::map<INT, INT> m_mapAreanSaveOldRanks;
	std::map<INT, DWORD> m_mapAreanSaveModifyTime;

	std::map<INT, INT> m_mapPetLVSaveRanks;//宠物等级
	std::map<INT, INT> m_mapPetLVSaveOldRanks;
	std::map<INT, DWORD> m_mapPetLVSaveModifyTime;

	std::map<INT, INT> m_mapLGSaveRanks;//炼卦
	std::map<INT, INT> m_mapLGSaveOldRanks;
	std::map<INT, INT> m_mapLGSaveModifyTime;

	std::map<INT, INT> m_mapTowerRanks; //伐魔塔
	std::map<INT, INT> m_mapTowerOldRanks;
	std::map<INT, DWORD> m_mapTowerModifyTime;


	std::map<INT, INT> m_mapRoleFightSaveRanks;//战斗力
	std::map<INT, INT> m_mapRoleFightSaveOldRanks;
	std::map<INT, DWORD> m_mapRoleFightSaveModifyTime;

private:
	INT m_nRoleRanksNum;
	INT m_nPetLvRanksNum;
	INT m_nPetFSRanksNum;
	INT m_nRideRanksNum;
	INT m_nMpjjRanksNum;
	INT m_nLGRanksNum;
	INT m_nSmzcRanksNum;
	INT m_nTongFSRanksNum;
	INT m_nTongSrRanksNum;

	//开服活动排行榜
	INT m_nRoleLevelRanksNum;
	INT m_nRideLvRanksNum;
	INT m_nAreanRanksNum;
	INT m_nPetLvNum;
	INT m_nLGNum;
	INT m_nTowerRanksNum;
	INT m_nRoleFightScoreNum;

private:
	std::map<INT, DWORD> m_mapLastUpdateTime;

public:
	DWORD GetLastUpdateTime(INT nRankId);
};

extern KRankManager g_cRankManager;

#endif