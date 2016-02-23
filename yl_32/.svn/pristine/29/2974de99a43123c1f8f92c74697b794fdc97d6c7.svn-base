///////////////////////////////////////////////////////////////////////

///	File Name	: kregeneration_mgr.cpp
///	Author		: Dai Zejun
///	Created Date: 2010-10-29
///
/// Version history:
///		2010-10-29dzj: Created
///	
///	Purpose		: 
///
///////////////////////////////////////////////////////////////////////

#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegameworld/kregeneration_mgr.h"
#include "onlinegameworld/ikscenemgr.h"
//#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/ksceneobject.h"
#include "onlinegameworld/ikscene.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kdoodadmgr.h"
#include "onlinegamemodule/ai/kaictrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

KRegenerationMgr KRegenerationMgr::ms_instance;

KRegenerationMgr::KRegenerationMgr()
{
	Clear();
	m_pObjPool = new KRegenerationMemPool();//生成Pool对象
}

KRegenerationMgr::~KRegenerationMgr()
{
	Clear();
	SAFE_DELETE(m_pObjPool);//清空对象池
}

BOOL KRegenerationMgr::Activate()
{
	//当前帧计数器+1
	++m_dwFrameCounter;

	QCONFIRM_RET_FALSE(m_pObjPool);

	if (m_dwFrameCounter >= m_dwFirstRegenerationFrame)
	{//将第一个重生的帧记录下来，提高效率
		//检查有没有等待超时的数据，重生
		KRegenerationMMap::iterator it = m_cDataMMap.begin();
		for (; it != m_cDataMMap.end(); ++it)
		{
			KS_REGENERATION_INFO* pInfo = it->second;
			QCONFIRM_CONTINUE(pInfo);
			if (m_dwFrameCounter >= it->first && pInfo->bRegnerate)
			{//时间到，触发重生
				//重生场景物件
				this->_ActionRegeneration(*pInfo);

				//删除已经重生的对象
				m_pObjPool->Free(pInfo);
			}
			else if(m_dwFrameCounter >= it->first && !pInfo->bRegnerate)
			{
				m_pObjPool->Free(pInfo);
			}
			else
			{//当前记录未超时，后面的也不会超时
				break;
			}
		}
		m_cDataMMap.erase(m_cDataMMap.begin(), it);//删除已经无效的节点
	}

	return TRUE;
}

BOOL KRegenerationMgr::UnRegister(DWORD dwTemplateId, DWORD dwSceneId)
{
	KRegenerationMMap::iterator it = m_cDataMMap.begin();
	for (; it != m_cDataMMap.end(); ++it)
	{
		KS_REGENERATION_INFO* pInfo = it->second;
		QCONFIRM_CONTINUE(pInfo);
		if(pInfo->dwTemplateId == dwTemplateId && pInfo->dwSceneId == dwSceneId)
		{
			pInfo->bRegnerate = FALSE;
		}
	}

	return TRUE;
}

BOOL KRegenerationMgr::Register( const KS_REGENERATION_INFO& sInfo )
{
	DWORD	dwActionFrame = sInfo.dwFrames + m_dwFrameCounter;
	KS_REGENERATION_INFO* pNewInfo = m_pObjPool->New();
	QCONFIRM_RET_FALSE(pNewInfo);
	//::memcpy(pNewInfo, &sInfo, sizeof(KS_REGENERATION_INFO));
	*pNewInfo = sInfo; // bugfix 非基本类型的struct不能memcpy
	KRegenerationMMap::iterator it_insert = m_cDataMMap.insert(KRegenerationMMap::value_type(dwActionFrame, pNewInfo));
	if ((it_insert == m_cDataMMap.begin()) && !m_cDataMMap.empty())
	{//新插入数据是第一个需要触发的，记录下来
		m_dwFirstRegenerationFrame = dwActionFrame;
	}
//#ifdef _DEBUG
//	printf("KRegenerationMgr::Register(dwIdExpect=%08X,dwSceneId=%d,dwTemplateId=%d,dwFrames=%d,x=%d,y=%d,z=%d)\n"
//		,sInfo.dwIdExpect,sInfo.dwSceneId, sInfo.dwTemplateId,sInfo.dwFrames,sInfo.nX,sInfo.nY,sInfo.nZ);
//#endif
	return TRUE;
}

VOID KRegenerationMgr::Clear()
{
	m_dwFrameCounter = 0;
	m_dwFirstRegenerationFrame = 0xFFFFFFFF;//无穷大，什么都不触发
	if (m_pObjPool)
	{
		KRegenerationMMap::iterator it = m_cDataMMap.begin();
		for (; it != m_cDataMMap.end(); ++it)
		{
			KS_REGENERATION_INFO* pInfo = it->second;
			QCONFIRM_CONTINUE(pInfo);
			m_pObjPool->Free(pInfo);
		}
	}
	m_cDataMMap.clear();
	SAFE_DELETE(m_pObjPool);//清空对象池
}

BOOL KRegenerationMgr::_ActionRegeneration( const KS_REGENERATION_INFO& sInfo )
{
	BOOL bRet = FALSE;
	switch (sInfo.byType)
	{
	case emKOBJ_PLAYER:
		break;
	case emKOBJ_NPC:
		bRet = this->_ActionRegenerationNPC(sInfo);
		break;
	case emKOBJ_DOODAD:
		bRet = this->_ActionRegenerationDoodad(sInfo);
		break;
	default:
		break;
	}
	return bRet;
}

BOOL KRegenerationMgr::_ActionRegenerationNPC( const KS_REGENERATION_INFO& sInfo )
{
//#ifdef _DEBUG
//	printf("KRegenerationMgr::_ActionRegenerationNPC(dwIdExpect=%08X,dwSceneId=%d,dwTemplateId=%d,dwFrames=%d,nX=%d,nY=%d,nZ=%d) begin\n"
//		,sInfo.dwIdExpect,sInfo.dwSceneId,sInfo.dwTemplateId,sInfo.dwFrames,sInfo.nX,sInfo.nY,sInfo.nZ);
//#endif
	BOOL bResult = FALSE;
	IKScene* pScence = NULL;
	KNpc* pNpc = NULL;

	LOG_PROCESS_ERROR(sInfo.byType == emKOBJ_NPC);
	LOG_PROCESS_ERROR(g_pSceneMgr);

	pScence = g_pSceneMgr->GetScene(sInfo.dwSceneId);
	if(pScence)
	{
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		//if(IS_DYNAMIC_SCENE(sInfo.dwSceneId))
		//{
		//	// TODO: 测试需要，临时先这样(zzx)
		//	/*cSafeScript->CallTableFunction("ExpScene","RegenerateNpc",0, "usdddd",
		//		sInfo.dwSceneId, sInfo.szNickName.CStr(), sInfo.nX, sInfo.nY, sInfo.nZ, sInfo.nFaceDirection);*/

		//	// commented by zy 2012-10-27 副本等动态场景NPC不需要重生
		//	/*
		//	CONST KSCENE_CONFIG* pSceneCfg = g_pSceneMgr->GetSceneInfo(pScence->GetTemplateId());
		//	PROCESS_ERROR(pSceneCfg);
		//	cSafeScript->CallTableFunction("Scene", "RegenerateNpc", 0, "uusd",
		//		sInfo.dwSceneId, pSceneCfg->dwTemplateId, pSceneCfg->szPath, sInfo.dwTemplateId);
		//	*/
		//}
		//else
		{
			CONST KSCENE_CONFIG* pSceneCfg = g_pSceneMgr->GetSceneInfo(pScence->GetTemplateId());
			PROCESS_ERROR(pSceneCfg);
			
			cSafeScript->CallTableFunction("Scene", "RegenerateNpc", 0, "uusdddddds",
				sInfo.dwSceneId, pSceneCfg->dwTemplateId, pSceneCfg->szPath, sInfo.dwTemplateId, sInfo.nX, sInfo.nY, sInfo.nZ, sInfo.nFaceDirection, sInfo.dwExValue, sInfo.szExInfo);
		}
	}
//#ifdef _DEBUG
//	printf("KRegenerationMgr::_ActionRegenerationNPC(dwIdExpect=%08X,dwSceneId=%d,dwTemplateId=%d,dwFrames=%d,nX=%d,nY=%d,nZ=%d) end success\n"
//		,sInfo.dwIdExpect,sInfo.dwSceneId,sInfo.dwTemplateId,sInfo.dwFrames,sInfo.nX,sInfo.nY,sInfo.nZ);
//#endif

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KRegenerationMgr::_ActionRegenerationDoodad( const KS_REGENERATION_INFO& sInfo )
{
	BOOL bResult = FALSE;
	IKScene* pScence = NULL;
	KSceneObject* pDoodad = NULL;

	QCONFIRM_RET_FALSE(sInfo.byType == emKOBJ_DOODAD);
	QCONFIRM_RET_FALSE(g_pSceneMgr);
	pScence = g_pSceneMgr->GetScene(sInfo.dwSceneId);
	LOG_PROCESS_ERROR(pScence);

	/*
	pDoodad = (KSceneObject*)g_cOrpgWorld.AddDoodad(sInfo.dwTemplateId, sInfo.dwIdExpect);
	LOG_PROCESS_ERROR(pDoodad);

	bResult = pScence->AddDoodad(pDoodad, sInfo.nX, sInfo.nY, sInfo.nZ);
	LOG_PROCESS_ERROR(bResult);

	pDoodad->m_nFaceDirection =  sInfo.nFaceDirection;
	pDoodad->m_nDirectionXY =  sInfo.nFaceDirection;
	*/
	LOG_PROCESS_ERROR(pDoodad = g_cDoodadMgr.CreateDoodad(sInfo.dwTemplateId, sInfo.dwIdExpect));
	LOG_PROCESS_ERROR(g_cDoodadMgr.AddToScene((KDoodad*)pDoodad, sInfo.dwSceneId, sInfo.nX, sInfo.nY, sInfo.nZ));
	//LOG_PROCESS_ERROR(pDoodad = (KSceneObject*)g_cOrpgWorld.AddDoodadToScene(sInfo.dwTemplateId, sInfo.dwSceneId, sInfo.nX, sInfo.nY, sInfo.nZ, sInfo.nFaceDirection, sInfo.dwIdExpect));

	if (true)
	{
		//设置重生信息
		KREGENERATION_INFO regeneration_info;
		//regeneration_info.dwTemplateId = sInfo.dwTemplateId;
		regeneration_info.nGenerationTime = sInfo.dwFrames;
		regeneration_info.nX = sInfo.nX;
		regeneration_info.nY = sInfo.nY;
		regeneration_info.nZ = sInfo.nZ;
		regeneration_info.nFaceDirection = sInfo.nFaceDirection;
		regeneration_info.nNormalAIType = sInfo.nNormalAIType;
		regeneration_info.nRelation = sInfo.nRelation;
		pDoodad->SetRegenerationInfo(regeneration_info);
	}

	bResult = TRUE;
EXIT0:
	return bResult;
}

#endif
