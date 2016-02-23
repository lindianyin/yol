/* -------------------------------------------------------------------------
//	文件名		：	KRegion_NS.h
//	创建者		：	simon
//	创建时间	：	2009-9-23 16:47:47
//	功能描述	：	adapted from Sword Online III, originally by Zhu JianQiu
//
// -----------------------------------------------------------------------*/
#ifndef __KREGION_NS_H__
#define __KREGION_NS_H__

// -------------------------------------------------------------------------
#include "kcell.h"
#include "onlinegameworld/ikscene.h"

#define SCRIPT_COUNT_PER_REGION		16
#define SCRIPT_DATA_SIZE (sizeof(KRegionTrapScript) * SCRIPT_COUNT_PER_REGION)

class KScene;
class KSceneObject;
class KPlayer;
class KNpc;
class KDoodad;

class KRegion
{
public:
	KRegion(void);
	virtual ~KRegion(void);

	BOOL Init(KScene* pScene, INT nRegionX, INT nRegionY);
	VOID UnInit();

	VOID Activate(VOID);

	BOOL LoadTerrain();
	//BOOL SaveTerrain(); // for the scene editor

	BOOL AddPlayer(KSceneObject* pSceneObj);
	BOOL RemovePlayer(KSceneObject* pSceneObj);
	BOOL AddNpc(KSceneObject* pSceneObj);
	BOOL RemoveNpc(KSceneObject* pSceneObj);
	BOOL AddDoodad(KSceneObject* pSceneObj);
	BOOL RemoveDoodad(KSceneObject* pSceneObj);

	BOOL DeleteAllNpc(VOID);
	BOOL DeleteAllDoodad(VOID);
	BOOL DeleteAllPlayer(VOID);
	BOOL DeleteAllObject(VOID);

	BOOL DeleteAllCell(VOID);	// 此函数客户端和服务器都不用
	LPCSTR GetScriptByIndex(DWORD dwIndex)
	{
		ASSERT(m_pScriptList);
		LPCTSTR pszScript = NULL;

		if (dwIndex < SCRIPT_COUNT_PER_REGION)
			pszScript = m_pScriptList[dwIndex].szScript;

		return pszScript;
	}


	// 遍历Region中的Cell
	template <class TFunc> BOOL TraverseCell(TFunc& Func);
	BOOL TraversePlayer(IKTraverseHelper& rcHelper);
	BOOL TraverseNpc(IKTraverseHelper& rcHelper);
	BOOL TraverseCharacter(IKTraverseHelper& rcHelper);
	BOOL TraverseDoodad(IKTraverseHelper& rcHelper);

	// 遍历Region中的场景物体
	template <class TFunc> BOOL TraverseNpc(TFunc& Func);
	template <class TFunc> BOOL TraverseDoodad(TFunc& Func);
	template <class TFunc> BOOL TraversePlayer(TFunc& Func);
public:
	// 本Region中的左下角格子，在整个地图上的坐标（单位：格子）
	INT					m_nLeftCell;
	INT					m_nBottomCell;

	// 本Region中的左下角格子的左下点，在整个地图上的坐标（单位：点）
	INT					m_nLeft;
	INT					m_nBottom;

	// 本Region在整个地图上的坐标（单位：Region）
	INT				    m_nRegionX;
	INT				    m_nRegionY;

	KScene*			m_pScene;										
	KCell*			m_pCells;
	KRegionTrapScript*		m_pScriptList;
	INT				m_nEnterFrame;
	INT             m_nLeaveFrame;     // 最后一个角色离开的帧数
	DWORD		m_dwActiveFrame;

	QList			m_cNpcList;				// Npc列表
	QList			m_cDoodadList;			// Doodad列表
	QList			m_cPlayerList;		// 玩家列表

public:
	// optimization
#ifdef GAME_SERVER
	inline BOOL IsActive(DWORD dwFrame) { return dwFrame < m_dwActiveFrame + GAME_FPS * 3; }
#else
	inline BOOL IsActive(DWORD dwFrame) { return (UINT64)dwFrame < (UINT64)m_dwActiveFrame + GAME_FPS / 2; }
#endif

	VOID SetActive();
	BOOL IsActive();

	// 添加障碍
	BOOL AddObstacle(INT nXCell, INT nYCell, KCell* pCell);
	BOOL AddObstacle(INT nXCell, INT nYCell, const KCell& crCellInfo, INT nDynamicObstacleGroupIndex);
	// 删除障碍
	KCell* RemoveObstacle(INT nXCell, INT nYCell, INT nZ);

	BOOL GetBaseCellInfo(INT nXCell, INT nYCell, KCell* pCell);
	BOOL SetBaseCellInfo(INT nXCell, INT nYCell, KCell& rCell);

	// 获取障碍: 下表面 <= nZ <= 上表面
	KCell* GetObstacle(INT nXCell, INT nYCell, INT nZ);

	KCell* GetLowestObstacle(INT nXCell, INT nYCell);

	// 获取Z轴往下第一个低于或等于nZ的Cell, nZ: 绝对坐标,单位:点
	KCell* GetLowerObstacle(INT nXCell, INT nYCell, INT nZ);

	// 从pnRelativeZ输出成功后的Z轴相对坐标,如果没有成功,则不会修改*pnRelativeZ
	// 如果遇到目标Cell是障碍而无法进入,则*pnObstacleDirection返回障碍的法线方向
	// 注意,不见得一定能够得到障碍法线方向,所以要在调此函数前将*pnObstacleDirection设为-1
	// 调用后,如果返回Cell为空,并且*pnObstacleDirection在[0, DIRECTION_COUNT]内,则*pnObstacleDirection才表示障碍方向
	KCell* GetAdaptedCellForObj(
		INT nXCell, INT nYCell, INT nFootPosZ, INT nClimb, 
		INT nHeight, BOOL bIgnoreDynamicSwitch,
		INT* pnRelativeZ, INT *pnObstacleDirection);

	// 注意,上面的函数严格计算高度,而下面这个函数是寻找一个最接近的Cell,并不严格匹配高度

	// 从pnRelativeZ输出成功后的Z轴相对坐标,如果没有成功,则不会修改*pnRelativeZ
	KCell* GetProximalCellForObj(
		INT nXCell, INT nYCell, INT nFootPosZ, INT nHeight, 
		BOOL bIgnoreDynamicSwitch, 
		BOOL bIgnoreBlock, 
		INT* pnRelativeZ
		);

	// 取得Cell链表中的上一个Cell,内部会动态障碍的开关
	KCell* GetPreCell(INT nCellX, INT nCellY, KCell* pCell);
	// 取得下一个有效的Cell,内部会考虑动态障碍开关
	KCell* GetNextCell(KCell* pCell, BOOL bIgnoreDynamicSwitch);

	BOOL CheckObstacle(INT nXCell, INT nYCell, INT nLayer);

private:
	enum KTERRAIN_DATA_VERSION
	{
		eTerrainDataVersionPrevious = 1,
		eTerrainDataVersionCurrent
	};

	struct KRegionHeader
	{
		INT nVersion;
		INT nRegionX;
		INT nRegionY;
		INT nReserved;
	};

	//KCell   m_Cells[REGION_GRID_WIDTH * REGION_GRID_HEIGHT];	
	KRegionTrapScript   m_dwScriptList[SCRIPT_COUNT_PER_REGION];

	BOOL LoadTerrainBufferV2(const BYTE* pbyData, size_t uDataLen);

	BOOL AddSceneObject(QList& rcList, KSceneObject* pSceneObj);
	BOOL RemoveSceneObject(KSceneObject* pSceneObj);

	friend class KScene;
};

template <class TFunc> 
BOOL KRegion::TraverseNpc(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cNpcList.GetHead();
	while (pNode != NULL)
	{
		KNpc* pNpc = (KNpc*)(pNode->m_pSceneObject);

		bRetCode = Func(pNpc);
		PROCESS_ERROR(bRetCode);

		pNode = (SceneObjNode*)pNode->GetNext();
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc> 
BOOL KRegion::TraverseDoodad(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cDoodadList.GetHead();
	while (pNode != NULL)
	{
		KDoodad* pDoodad = (KDoodad*)(pNode->m_pSceneObject);

		bRetCode = Func(pDoodad);
		PROCESS_ERROR(bRetCode);

		pNode = (SceneObjNode*)pNode->GetNext();
	}

	bResult = true;
EXIT0:
	return bResult;
}

template <class TFunc> 
BOOL KRegion::TraversePlayer(TFunc& Func)
{
	BOOL bResult  = false;
	BOOL bRetCode = false;

	SceneObjNode* pNode = (SceneObjNode*)m_cPlayerList.GetHead();
	while (pNode != NULL)
	{
		KPlayer* pPlayer = (KPlayer*)(pNode->m_pSceneObject);

		bRetCode = Func(pPlayer);
		PROCESS_ERROR(bRetCode);

		pNode = (SceneObjNode*)pNode->GetNext();
	}

	bResult = true;
EXIT0:
	return bResult;
}


// -------------------------------------------------------------------------

#endif /* __KREGION_NS_H__ */
