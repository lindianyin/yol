
#ifndef _KSCENE_H_
#define	_KSCENE_H_

#include <list>
#include <map>
#include <string>
#include <bitset>
#include "Core/QList.h"
#include "onlinegameworld/scene_def.h"
#include "onlinegameworld/ksceneobject.h"
#include "kcell.h"
#include "kregion.h"
#include "onlinegameworld/ikscene.h"
#include "../onlinegamemodule/ai/ThreatList.h" // 临时移植
#include "../onlinegamemodule/ai/AIPatrolGroupManager.h" // 临时移植

class KTerrainScene;
struct KPartition;
class KRegion;
class KScene;

struct KREVIVE_POSITION;

#define SCENE_CUSTOM_DATA_SIZE 1024

struct KTRAP_INFO
{
	DWORD	dwId;
	CHAR		szClass[MAX_NAME_LEN]; // Trap脚本类
	INT			nToleranceZ; // Z轴高度差限制

	DWORD	dwTargetSceneId; // 目的地 场景Id
	INT			nTargetX; // 目的地 x
	INT			nTargetY; // 目的地 y
	INT			nTargetZ; // 目的地 z	
};

struct KNPC_INFO
{
	DWORD	dwTemplateId; // Npc模板Id
	INT			nX; // 逻辑坐标x
	INT			nY; // y
	INT			nZ; // z	
	INT			nFaceDirection; // 面部朝向
	CHAR		szNickName[MAX_NAME_LEN];
};

struct KDOODAD_INFO
{
	DWORD	dwTemplateId; // Npc模板Id
	INT			nX; // 逻辑坐标x
	INT			nY; // y
	INT			nZ; // z	
	INT			nFaceDirection; // 面部朝向
};

//typedef std::map<DWORD, KCustomData<64> > KOBJ_CUSTOM_DATA_TABLE;

typedef INT(*FUNC_LOADPROGRESS)(LPCTSTR pName,float a); 

class KScene : public IKScene/* : public KBaseObject*/
{
public:
	KScene();
	~KScene();

	//////////////////////////////////Beginning of IKScene////////////////////////////////////////
	BOOL	Init(CONST KSCENE_CONFIG& rCfg);
    BOOL	Uninit();
	BOOL	Activate();

	DWORD	GetId() { return m_dwId; }
	DWORD	GetTemplateId() { return m_dwTemplateId; }
	LPCSTR		GetClass()	{ return m_szClass; };
	LPCSTR		GetName()	{ return m_szName; };
	BOOL		GetCanPK()  { return m_bCanPK;};
	INT			GetSceneBattleId() { return m_nBattleSceneId; };

	KSceneObject*	GetByNickName(LPCSTR pszNickName);
	INT		GetPlayerCount();
	INT		GetNpcCount();
	INT		GetDoodadCount();
	INT		GetRegionCount();
	BOOL		AddNickName(LPCSTR pszNickName, KSceneObject* pSceneObj);
	BOOL		RemoveNickName(LPCSTR pszNickName);

	BOOL	IsVisible(INT nSrcX, INT nSrcY, INT nSrcZ, INT nDstX, INT nDstY, INT nDstZ);

	BOOL AddPlayer(KSceneObject* pSceneObj, INT x, INT y, INT z);
	BOOL RemovePlayer(KSceneObject* pSceneObj);
	BOOL AddNpc(KSceneObject* pSceneObj, INT x, INT y, INT z);
	BOOL RemoveNpc(KSceneObject* pSceneObj);
	BOOL AddDoodad(KSceneObject* pSceneObj, INT x, INT y, INT z);
	BOOL RemoveDoodad(KSceneObject* pSceneObj);

	BOOL	SetBroadcastRange(INT nRange) { m_nBroadcastRegion = nRange; return TRUE; }

	BOOL	TraverseNearbyPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 遍历某sobj附近所有玩家
	BOOL	TraverseNearbyPlayers(IKTraverseHelper& rcHelper, INT nX, INT nY); // 遍历某坐标点附近所有玩家
	BOOL	TraverseAllPlayers(IKTraverseHelper& rcHelper); // 遍历场景内的所有玩家
	BOOL	TraverseNearbyNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 遍历某sobj附近所有npc
	BOOL	TraverseNearbyNpcs(IKTraverseHelper& rcHelper, INT nX, INT nY); // 遍历某坐标点附近所有npc
	BOOL	TraverseAllNpcs(IKTraverseHelper& rcHelper); // 遍历场景内的所有npc
	BOOL	TraverseNearbyDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 遍历某sobj附近所有doodad
	BOOL	TraverseNearbyDoodads(IKTraverseHelper& rcHelper, INT nX, INT nY); // 遍历某坐标点附近所有doodad
	BOOL	TraverseAllDoodads(IKTraverseHelper& rcHelper); // 遍历场景内的所有doodad

	// 遍历附近sobj
	inline	BOOL	TraverseNearbyObj(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj, DWORD dwType);
	// 根据XY遍历附近sobj
	inline BOOL TraverseNearbyObj(IKTraverseHelper& rcHelper, INT nPointX, INT nPointY, DWORD dwType);
	// 遍历场景内所有sobj
	inline	BOOL	TraverseAll(IKTraverseHelper& rcHelper, DWORD dwType);

#ifdef GAME_SERVER
	BOOL	FindNewNeighborPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 进到新region后检查附近有哪些新玩家
	BOOL	FindNewNeighborNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 进到新region后检查附近有哪些新npc
	BOOL	FindNewNeighborDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); // 进到新region后检查附近有哪些新doodad
	BOOL	FindOldNeighborPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); 
	BOOL	FindOldNeighborNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); 
	BOOL	FindOldNeighborDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj); 
	 // 进到新region后检查附近有哪些新东东
	inline	BOOL	FindNewNeighbors(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj, DWORD dwType);
	inline  BOOL    FindOldNeighbors(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj, DWORD dwType);
#endif // GAME_SERVER
	//////////////////////////////////End of IKScene////////////////////////////////////////

public:
	BOOL	Load(); // ServerLoad和EditorLoad都作废，统一用Load载入
#ifdef GAME_SERVER
	BOOL	CallInitScript();
	BOOL	PrepareToRefresh();
	BOOL	LoadTrap();
	BOOL	LoadNpc();
	BOOL	LoadDoodad();
#endif
	// 使指定区域的Region有效: 未加载的就加载,加载了的就激活
	BOOL ValidateRegions(INT nRegionX, INT nRegionY);

	BOOL GetObstacle(INT nXCell, INT nYCell, INT nZ, KCell::KBaseInfo& BaseInfo, WORD& wLowLayer, WORD& wHighLayer);
	BOOL CheckObstacle(INT nXCell, INT nYCell, INT nLayer);

    BOOL ClearAllScript();
    BOOL ClearScriptSpecial(DWORD dwScriptID);

	// 预处理障碍数据
	typedef VOID (*PREPROCESS_PROGRESS_CALLBACK_FUNC)(INT nYCell, INT nCellWidth, INT nCellHeight);
    BOOL PreprocessCalculateData();

	// 遍历Scene中的Region
	template <class TFunc> BOOL TraverseRegion(TFunc& Func);
	// 遍历Scene中的Cell
	template <class TFunc> BOOL TraverseCell(TFunc& Func);

	// 遍历周围指定范围的Region,可以指定"半径"
	template <class TFunc> BOOL TraverseRangePlayer(KRegion* pCenter, TFunc& Func, int nRange = -1);
	template <class TFunc> BOOL TraverseRangeNpc(KRegion* pCenter, TFunc& Func, int nRange = -1);
	template <class TFunc> BOOL TraverseRangeDoodad(KRegion* pCenter, TFunc& Func, int nRange = -1);

	BOOL Save(const char cszName[], FUNC_LOADPROGRESS);

	BOOL SaveBaseInfo();
	BOOL LoadBaseInfo();

	BOOL		IsPointInSceneSpace(INT nX, INT nY, INT nZ);

	INT			GetRegionWidth(VOID);
	INT			GetRegionHeight(VOID);

	BOOL        SetRegion(INT nRegionX, INT nRegionY, KRegion* pRegion);
	KRegion*    GetRegion(INT nRegionX, INT nRegionY);

	// 由象素点坐标得到Region指针
	KRegion*    GetRegionByPoint(INT nX, INT nY);
	BOOL        DelRegion(INT nRegionX, INT nRegionY);
	BOOL        DeleteAllRegion(VOID);
	BOOL        DeleteAllObstacle(VOID);

	VOID DeleteAllObjects();

	VOID DeleteAllNpc();

	// 所有动态障碍开关默认为"true",true表示这些障碍是enable的
	BOOL SetDynamicObstacleState(INT nGroupIndex, BOOL bState);
	BOOL GetDynamicObstacleState(INT nGroupIndex); 

private:
    BOOL ReviseCalculateGradient();
	BOOL CalculateObstacleRange();
public:
	CHAR							m_szClass[MAX_NAME_LEN];
	CHAR							m_szName[MAX_NAME_LEN];
	CHAR							m_szPath[MAX_PATH];		// 逻辑场景路径
    //BOOL                            m_bReviveInSitu;                // 玩家是否能在原地复活
	DWORD						m_dwScriptID;
    INT								m_nBroadcastRegion;             // 同步广播的半径,以Region为单位
	//DWORD						m_dwBanSkillMask;
	BOOL							m_bCanPK;
	INT						m_nBattleSceneId;

	time_t                  m_nTimeStamp;

	DWORD					m_dwId;
	DWORD					m_dwTemplateId;
	INT						m_nCopyIndex;
	std::vector<KTRAP_INFO>	m_vTrapCfg;
	std::vector<KNPC_INFO>	m_vNpcCfg;
	std::vector<KDOODAD_INFO>	m_vDoodadCfg;
	KTerrainScene*				m_pTerrainScene;

#ifdef GAME_SERVER
    DWORD						m_dwOwnerID;
    BOOL							m_bSaved;
    time_t							m_nDeleteTime;
	DWORD						m_dwCreationTime;
	KThreatLink                 m_ThreatLink;
	KAIPatrolGroupManager		m_AIPatrolGroupManager; // 临时移植
#endif

private:
	INT							m_nRegionWidth;
	INT  							m_nRegionHeight;
	KRegion*					m_RegionList[MAX_REGION_WIDTH][MAX_REGION_HEIGHT];
    KPartition*					m_pPartitionTable;
	std::map<std::string, KSceneObject*>	m_mapNickNames;

    typedef std::bitset<MAX_OBSTACLE_GROUP> KDYNAMIC_OBSTACLE_SWITCH_TABLE;
    KDYNAMIC_OBSTACLE_SWITCH_TABLE  m_DynamicObstacleSwitchTable;

	friend class KSceneObject;
};

// 得到全局象素点坐标所在的Region指针
inline KRegion* KScene::GetRegionByPoint(INT nX, INT nY)
{
	KRegion*    pRegion     = NULL;
	INT         nRegionX    = nX / REGION_GRID_WIDTH / CELL_LENGTH;
	INT         nRegionY    = nY / REGION_GRID_HEIGHT / CELL_LENGTH;

	PROCESS_ERROR(nX >= 0);
    PROCESS_ERROR(nX < CELL_LENGTH * REGION_GRID_WIDTH * m_nRegionWidth);

	PROCESS_ERROR(nY >= 0);
    PROCESS_ERROR(nY < CELL_LENGTH * REGION_GRID_WIDTH * m_nRegionHeight);

	pRegion = m_RegionList[nRegionX][nRegionY];

EXIT0:
	return pRegion;
}

template <class TFunc>
BOOL KScene::TraverseRegion(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (INT nX = 0; nX < m_nRegionWidth; nX)
	{
		for (INT nY = 0; nY < m_nRegionHeight; nY++)
		{
			KRegion* pRegion = m_RegionList[nX][nY];
			if (pRegion == NULL)
				continue;

			bRetCode = Func(pRegion);
			PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc>
BOOL KScene::TraverseCell(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	for (INT i = 0; i < MAX_REGION_WIDTH; ++i)
	{
		for (INT j = 0; j < MAX_REGION_HEIGHT; ++j)
		{
			KRegion* pRegion = m_RegionList[i][j];
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraverseCell(Func);
			PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc>
BOOL KScene::TraverseRangePlayer(KRegion* pCenter, TFunc& Func, int nRange)
{
	BOOL    bResult     = false;
	BOOL    bRetCode    = false;
	int     nLeft       = 0;
	int     nRight      = 0;
	int     nTop        = 0;
	int     nBottom     = 0;

	assert(pCenter);

	if (nRange < 0)
	{
		nRange = m_nBroadcastRegion;
	}

	nLeft   = pCenter->m_nRegionX - nRange;
	nRight  = pCenter->m_nRegionX + nRange;
	nBottom = pCenter->m_nRegionY - nRange;
	nTop    = pCenter->m_nRegionY + nRange;

	for (int nX = nLeft; nX <= nRight; nX++)
	{
		for (int nY = nBottom; nY <= nTop; nY++)
		{
			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraversePlayer(Func);
			PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc>
BOOL KScene::TraverseRangeNpc(KRegion* pCenter, TFunc& Func, int nRange)
{
	BOOL    bResult     = false;
	BOOL    bRetCode   = false;
	int     nLeft       = 0;
	int     nRight      = 0;
	int     nTop        = 0;
	int     nBottom     = 0;

	assert(pCenter);

	if (nRange < 0)
	{
		nRange = m_nBroadcastRegion;
	}

	nLeft   = pCenter->m_nRegionX - nRange;
	nRight  = pCenter->m_nRegionX + nRange;
	nBottom = pCenter->m_nRegionY - nRange;
	nTop    = pCenter->m_nRegionY + nRange;

	for (int nX = nLeft; nX <= nRight; nX++)
	{
		for (int nY = nBottom; nY <= nTop; nY++)
		{
			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraverseNpc(Func);
			PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc>
BOOL KScene::TraverseRangeDoodad(KRegion* pCenter, TFunc& Func, int nRange)
{
	BOOL    bResult     = false;
	BOOL    bRetCode    = false;
	int     nLeft       = 0;
	int     nRight      = 0;
	int     nTop        = 0;
	int     nBottom     = 0;

	assert(pCenter);

	if (nRange < 0)
	{
		nRange = m_nBroadcastRegion;
	}

	nLeft   = pCenter->m_nRegionX - nRange;
	nRight  = pCenter->m_nRegionX + nRange;
	nBottom = pCenter->m_nRegionY - nRange;
	nTop    = pCenter->m_nRegionY + nRange;

	for (int nX = nLeft; nX <= nRight; nX++)
	{
		for (int nY = nBottom; nY <= nTop; nY++)
		{
			KRegion* pRegion = GetRegion(nX, nY);
			if (pRegion == NULL)
				continue;

			bRetCode = pRegion->TraverseDoodad(Func);
			PROCESS_ERROR(bRetCode);
		}
	}

	bResult = true;
EXIT0:
	return bResult;
}

#endif
