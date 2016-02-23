/* -------------------------------------------------------------------------
//	文件名		：	synchelper.h
//	创建者		：	simon
//	创建时间	：	2009/12/19 22:12:47
//	功能描述	：	Synchronization Helpers
//
// -----------------------------------------------------------------------*/
#pragma once

#include "onlinegameworld/ikscene.h"
#include <vector>


class KPlayer;
class KSceneObject;
class KPlayerProtocolProcessor;

// -------------------------------------------------------------------------

template<class T>
struct TKTraverseHelper : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj)
	{
		m_vObjList.push_back(static_cast<T*>(pObj));
		return TRUE;
	}

	std::vector<T*> m_vObjList;
};

#ifdef GAME_SERVER
extern BYTE g_abySyncPackBuffer[KD_SYNC_PACK_MAX]; // 全局共享的同步包发送缓存

////////////////////////////////////////////Synchronization Helper Function//////////////////////////////
struct KBroadcastFunc : public IKTraverseHelper
{
	KBroadcastFunc();
	BOOL OnTraverse(KSceneObject* pObj);

	LPCVOID			m_pvData;
	SIZE_T			m_uSize;			
	INT				m_nMaxSyncCount;
	KSceneObject*	m_pExcept;         // 广播时排除这个对象
	KSceneObject*   m_pCenterObject;    // 以此对象为中心来计算距离
	INT				m_nRange;           // 距离在此范围内才广播(单位点),除非该值为0
	KPlayerProtocolProcessor* m_pProcessor;
};

// 跟技能相关的广播
struct KBroadcastPlayerFunc : public IKTraverseHelper
{
	KBroadcastPlayerFunc();
	BOOL OnTraverse(KSceneObject* pObj);

	LPCVOID			m_pvData;
	SIZE_T			m_uSize;			
	INT				m_nMaxSyncCount;
	KSceneObject*	m_pExcept;         // 广播时排除这个对象
	KSceneObject*   m_pCenterObject;    // 以此对象为中心来计算距离
	INT				m_nRange;           // 距离在此范围内才广播(单位点),除非该值为0
	KPlayerProtocolProcessor* m_pProcessor;
};

struct KSyncRegionPlayerFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj/*, INT nEnterFrame*/);
	//int     m_nEnterRegionFrame;
	//int     m_nSyncPlayerCount;
	//DWORD   m_dwDestPlayerID;
	//S2C_SYNC_REGION_PLAYER::KPLAYER_INFO* m_pPlayerInfo;

	VOID*			m_pvData;			
	SIZE_T			m_uSize;			
	KPlayer*		m_pThisPlayer;
	std::vector<KPlayer*> m_vPlayerList;
};

struct KSyncRegionNpcFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj);
	//int m_nEnterRegionFrame;
	//int m_nSyncNpcCount;
	//KPlayer* m_pDestPlayer;
	//S2C_SYNC_REGION_NPC::KNPC_INFO* m_pNpcInfo;
	
	VOID*			m_pvData;			
	SIZE_T			m_uSize;			
	KPlayer*		m_pThisPlayer;
};

struct KSyncRegionNpcLeaveFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj);

	VOID*			m_pvData;			
	SIZE_T			m_uSize;			
	KPlayer*		m_pThisPlayer;
};

struct KSyncRegionDoodadLeaveFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj);

	VOID*			m_pvData;			
	SIZE_T			m_uSize;			
	KPlayer*		m_pThisPlayer;
};

struct KSyncRegionPlayerLeaveFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj);

	VOID*			m_pvData;			
	SIZE_T			m_uSize;			
	KPlayer*		m_pThisPlayer;
	std::vector<KPlayer*> m_vPlayerList;
};

struct KSyncRegionDoodadFunc : public IKTraverseHelper
{
	BOOL OnTraverse(KSceneObject* pObj);
	//int m_nSyncDoodadCount;
	//S2C_SYNC_REGION_DOODAD::KDOODAD_INFO* m_pDoodadInfo;

	VOID*			m_pvData;
	SIZE_T			m_uSize;
	KPlayer*		m_pThisPlayer;
};

////////////////////////////////////////////Synchronization Helper Function Objects//////////////////////////////

#endif // GAME_SERVER
