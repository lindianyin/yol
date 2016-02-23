
#pragma once

#include "scene_def.h"


// -------------------------------------------------------------------------
class KSceneObject;

class IKTraverseHelper
{
public:
	BOOL operator()(KSceneObject* pObj) { return OnTraverse(pObj); }
	virtual BOOL OnTraverse(KSceneObject* pObj) PURE;
};
//class IKTraverseHelper2
//{
//public:
//	virtual BOOL operator()(KSceneObject* pObj, INT nEnterFrame) PURE;
//};

struct KSCENE_CONFIG
{
	DWORD	dwTemplateId;
	CHAR		szClass[MAX_NAME_LEN];					// 场景类名
	CHAR		szName[MAX_NAME_LEN];		// 显示场景名
	CHAR		szPath[MAX_PATH];					// 逻辑场景路径
	CHAR		szSceneType[MAX_NAME_LEN];			// 场景类型名称（小类型）
	INT			nBroadcastRange;		// 同步范围
	BOOL		bCanPK;                // 是否允许PK

	INT			nBattleSceneId;         // 改场景默认使用的战斗场景Id
	INT		nType;					// 场景类别
	BYTE	bFightMap;				// 是否战斗地图  add by NN
	BYTE	bCanTeam;				// 是否可组队 add by GG
	BYTE	bSetFightMode;			// 是否可以设置战斗模式
};

class IKScene
{
public:
	virtual	BOOL	Init(CONST KSCENE_CONFIG& rCfg)	PURE;
	virtual	BOOL	Uninit()	PURE;
	virtual	BOOL	Activate()	PURE;

	// 添加Player、Npc、Doodad并没有什么不同，只是为了性能放到了不同的列表里
	virtual	BOOL	AddPlayer(KSceneObject* pSceneObj, INT x, INT y, INT z)	PURE;
	virtual	BOOL	RemovePlayer(KSceneObject* pSceneObj)	PURE;
	virtual	BOOL	AddNpc(KSceneObject* pSceneObj, INT x, INT y, INT z)	PURE;
	virtual	BOOL	RemoveNpc(KSceneObject* pSceneObj)	PURE;
	virtual	BOOL	AddDoodad(KSceneObject* pSceneObj, INT x, INT y, INT z)	PURE;
	virtual	BOOL	RemoveDoodad(KSceneObject* pSceneObj)	PURE;

	virtual	DWORD	GetId()	PURE;
	virtual	DWORD	GetTemplateId()	PURE;
	virtual	LPCSTR		GetClass()	PURE;
	virtual	LPCSTR		GetName()	PURE;
	virtual	BOOL		GetCanPK()	PURE;
	virtual INT         GetSceneBattleId() PURE;

	virtual	KSceneObject*	GetByNickName(LPCSTR pszNickName)	PURE;
	virtual INT		GetPlayerCount()	PURE;
	virtual INT		GetNpcCount()	PURE;
	virtual INT		GetDoodadCount()	PURE;
	virtual INT		GetRegionCount()	PURE;

	virtual	BOOL	SetBroadcastRange(INT nRange)	PURE; // 目前没有通知客户端，想在运行时动态改变同步范围要加协议

	virtual	BOOL	TraverseNearbyPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE; // 遍历附近玩家
	virtual	BOOL	TraverseAllPlayers(IKTraverseHelper& rcHelper)	PURE; // 遍历场景内的所有玩家
	virtual	BOOL	TraverseNearbyNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE; // 遍历附近npc
	virtual	BOOL	TraverseAllNpcs(IKTraverseHelper& rcHelper)	PURE; // 遍历场景内的所有npc
	virtual	BOOL	TraverseNearbyDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE; // 遍历附近doodad
	virtual	BOOL	TraverseAllDoodads(IKTraverseHelper& rcHelper)	PURE; // 遍历场景内的所有doodad

#ifdef GAME_SERVER
	// 以下接口是方便同步功能使用的，都是指检查某场景物件(sobj)移动后见到的新"邻居"，不移动就没有任何新"邻居"
	// 即运动者自己要负责广播移动、向刚刚到达的区域广播自己的到来和“看到”周围新玩家、Npc和Doodad。
	// 原地不动的人被动地接受别人的广播，不会再有剑世里的那种隔几帧获取周围玩家的情况，是一种"Lazy-Sync"。
	virtual	BOOL	FindNewNeighborPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
	virtual	BOOL	FindNewNeighborNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
	virtual	BOOL	FindNewNeighborDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
	virtual	BOOL	FindOldNeighborPlayers(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
	virtual	BOOL	FindOldNeighborNpcs(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
	virtual	BOOL	FindOldNeighborDoodads(IKTraverseHelper& rcHelper, KSceneObject* pCenterObj)	PURE;
#endif // GAME_SERVER
};

// -------------------------------------------------------------------------

