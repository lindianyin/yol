/* -------------------------------------------------------------------------
//	文件名		：	kscenescript.cpp
//	创建者		：	simon
//	创建时间	：	2009/12/16 10:26:16
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include <sstream>
#include <string>
#include "onlinegameworld/korpgworld.h"
#include "onlinegamebase/kscriptmanager.h"

#include "../onlinegameworld/scene/kscene.h"
#include "onlinegameworld/ikscenemgr.h"
#include "onlinegameworld/kluaplayer.h"
#include "onlinegameworld/synchelper.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kdoodad.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "../onlinegameworld/KRelayClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------


// -------------------------------------------------------------------------

namespace KSceneScript
{
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetNameById
	// @Description		: 通过场景Id(模板)获得名字
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景Id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetNameById(QLuaScript& sc)
	{
		INT nSceneId = sc.GetInt(1);
		INT nMapId = GET_MAP_ID(nSceneId);
		INT nMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);
		IKScene* pScene = g_pSceneMgr->GetScene(nSceneId, nMapCopyIndex);
		QCONFIRM_RET_FALSE(pScene);

		sc.PushString(pScene->GetName());
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetMapOwnerId
	// @Description		: 通过场景nSceneId获得场景OwnerId
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景Id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMapOwnerId(QLuaScript& sc)
	{
		INT nSceneId = sc.GetInt(1);
		INT nMapId = GET_MAP_ID(nSceneId);
		INT nMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);
		KScene* pScene = (KScene*)g_pSceneMgr->GetScene(nMapId, nMapCopyIndex);
		QCONFIRM_RET_FALSE(pScene);

		sc.PushNumber(pScene->m_dwOwnerID);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetMapCopyIndex
	// @Description		: 通过场景nSceneId获得场景CopyIndex
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景Id
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetMapCopyIndex(QLuaScript& sc)
	{
		INT nSceneId = sc.GetInt(1);
		INT nMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);

		sc.PushNumber(nMapCopyIndex);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: Dump
	// @Description		: 输出场景调试信息，可以当系统消息发给指定玩家
	// @ReturnCode		: 
	// @ArgumentFlag	: [ou]
	// @ArgumentComment	: pLuaPlayer 向此玩家发送sysmsg
	// @ArgumentComment	: dwSceneId 只输出此场景信息
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDump(QLuaScript& sc)
	{
		KPlayer* pPlayer = NULL;
		DWORD dwSceneId = 0;
		INT nStackIndex = sc.GetTopIndex();
		if (nStackIndex >= 1)
		{
			KLuaPlayer* pLuaPlayer = (KLuaPlayer*)sc.GetObj(1);
			pPlayer = pLuaPlayer ? pLuaPlayer->GetHim() : NULL;
		}
		if (nStackIndex >= 2)
			dwSceneId = sc.GetInt(2);

		std::stringstream stmBuff;
		stmBuff << "\n__________________________________________________________\n";
		stmBuff << "SceneStaticsDump: " << g_pSceneMgr->GetCount() << " Loaded.\n\n";

		IKScene* pScene = g_pSceneMgr->FirstScene();
		while (pScene)
		{
			IKScene* pNextScene = g_pSceneMgr->NextScene();
			if (dwSceneId && pScene->GetId() != dwSceneId)
			{
				pScene = pNextScene;
				continue;
			}

			stmBuff << "Scene[" << pScene->GetId() << "][" << pScene->GetName()  << "] Tid["
				<< pScene->GetTemplateId() << "]:\n";

			stmBuff << "Player: \n";
			// 玩家列表
			TKTraverseHelper<KPlayer> cTravHelper;
			pScene->TraverseAllPlayers(cTravHelper);
			for(std::vector<KPlayer*>::iterator it = cTravHelper.m_vObjList.begin();
				it != cTravHelper.m_vObjList.end(); ++it)
			{
				KPlayer* pPlayer = *it;
				QCONFIRM_CONTINUE(pPlayer);

				INT nX = 0, nY = 0, nZ = 0;
				pPlayer->GetPosition(nX, nY, nZ);

				stmBuff << std::hex << "[0x" << pPlayer->GetId() << "][" << pPlayer->GetName()  << "] @"
					<< std::dec << nX << "," << nY << "," << nZ << "\n";
			}

			stmBuff << "Npc: \n";
			// Npc列表
			TKTraverseHelper<KNpc> cTravHelperNpc;
			pScene->TraverseAllNpcs(cTravHelperNpc);
			for(std::vector<KNpc*>::iterator it = cTravHelperNpc.m_vObjList.begin();
				it != cTravHelperNpc.m_vObjList.end(); ++it)
			{
				KNpc* pNpc = *it;
				QCONFIRM_CONTINUE(pNpc);

				INT nX = 0, nY = 0, nZ = 0;
				pNpc->GetPosition(nX, nY, nZ);

				stmBuff << std::hex << "[0x" << pNpc->GetId() << "][" << pNpc->GetName()  << "] @"
					<< std::dec << nX << "," << nY << "," << nZ << " Nick[" << pNpc->GetNickName()
					<< "] Tid[" << pNpc->GetTemplateId() << "]\n";
			}

			stmBuff << "Doodad: \n";
			// Doodad列表
			TKTraverseHelper<KDoodad> cTravHelperDoodad;
			pScene->TraverseAllDoodads(cTravHelperDoodad);
			for(std::vector<KDoodad*>::iterator it = cTravHelperDoodad.m_vObjList.begin();
				it != cTravHelperDoodad.m_vObjList.end(); ++it)
			{
				KDoodad* pDoodad = *it;
				QCONFIRM_CONTINUE(pDoodad);

				INT nX = 0, nY = 0, nZ = 0;
				pDoodad->GetPosition(nX, nY, nZ);

				stmBuff << std::hex << "[0x" << pDoodad->GetId() << "][" << pDoodad->GetName()  << "] @"
					<< std::dec << nX << "," << nY << "," << nZ << " Tid[" << pDoodad->m_dwTemplateId << "]\n";
			}

			stmBuff << "\n";
			pScene = pNextScene;
		}
		stmBuff << "End of scene dump.\n";

		const std::string& strContent = stmBuff.str();
		std::cout << strContent; // 如需入log修改此处
		if (pPlayer)
		{
			SIZE_T uSizePerPack = KD_CHANNEL_PACKET_MAX_LEN - KD_CHANNEL_MSG_MAX_LEN; // 不知道包大小，取保守值
			for (SIZE_T uLen = 0; uLen < strContent.length(); uLen += uSizePerPack)
			{
				g_ChatChannelManager.ProcessRespond(trLocalSys, 0, pPlayer->GetId(), "",
					strContent.substr(uLen, uSizePerPack).c_str());
			}
		}
		sc.PushString(strContent.c_str());
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetPlayerNum
	// @Description		: 获取场景内玩家人数
	// @ReturnCode		: 
	// @ArgumentFlag	: d[d]
	// @ArgumentComment	: dwSceneId 场景ID
	// @ArgumentComment	: nCopyIndex 副本序号
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetPlayerNum(QLuaScript& sc)
	{
		DWORD dwSceneId = sc.GetInt(1);
		INT nMapCopyIndex = 0;

		if (sc.GetTopIndex() >= 2)
			nMapCopyIndex = sc.GetInt(2);

		IKScene* pScene = g_pSceneMgr->GetScene(dwSceneId, nMapCopyIndex);
		//如果场景不存在，返回nil
		if (pScene)
		{
			TKTraverseHelper<KPlayer> cTravHelper;
			pScene->TraverseAllPlayers(cTravHelper);

			sc.PushNumber(cTravHelper.m_vObjList.size());
		}
		else
		{
			sc.PushNull();
		}
		
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: ResetMap
	// @Description		: 重置场景
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nMapID
	// @ArgumentComment	: nSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaResetMap(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		INT nSceneId = sc.GetInt(2);
		INT nMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);
		KScene* pScene = (KScene*)g_pSceneMgr->GetScene(nMapId, nMapCopyIndex);
		if(NULL == pScene)
		{
			QLogPrintf(LOG_ERR,"..LuaResetMap nMapId=%d,nSceneId=%d,nMapCopyIndex=%d",nMapId,nSceneId,nMapCopyIndex);
		}
		LOG_PROCESS_ERROR(pScene);
		g_RelayClient.DoResetMapCopyRequest(pScene->m_dwOwnerID, nMapId, nMapCopyIndex);
EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: DeleteAllNpc
	// @Description		: 删除场景所有npc
	// @ReturnCode		: 
	// @ArgumentFlag	: dd
	// @ArgumentComment	: nMapID
	// @ArgumentComment	: nSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaDeleteAllNpc(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		INT nSceneId = sc.GetInt(2);
		INT nMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);
		KScene* pScene = (KScene*)g_pSceneMgr->GetScene(nMapId, nMapCopyIndex);
		LOG_PROCESS_ERROR(pScene);
		pScene->DeleteAllNpc();
EXIT0:
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetNameByTemplateId
	// @Description		: 通过场景的TemplateId获得名字
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景TemplateId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetNameByTemplateId(QLuaScript& sc)
	{
		CONST KSCENE_CONFIG* pSceneConfig = g_pSceneMgr->GetSceneInfo(sc.GetInt(1));
		QCONFIRM_RET_FALSE(pSceneConfig);

		sc.PushString(pSceneConfig->szName);
		return 1;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: 
	// @ClassName		: --
	// @LuaApiName		: GetFilePathByTemplateId
	// @Description		: 通过场景的TemplateId获得场景文件的存放路径
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景TemplateId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetFilePathByTemplateId(QLuaScript& sc)
	{
		CONST KSCENE_CONFIG* pSceneConfig = g_pSceneMgr->GetSceneInfo(sc.GetInt(1));
		QCONFIRM_RET_FALSE(pSceneConfig);

		sc.PushString(pSceneConfig->szPath);
		return 1;
	}
#ifdef GAME_SERVER

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: UnloadScene
	// @Description		: 卸载地图(小心！若非必要请勿使用,会导致其中的玩家退出场景但仍留在游戏世界)
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaUnloadScene(QLuaScript& sc)
	{
		INT nRet = 0;
		INT nMapCopy = sc.GetInt(1);

		nRet = g_cOrpgWorld.UnloadSceneDelay(nMapCopy);

		sc.PushNumber(nRet);
		return nRet;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: UnloadBattleScene
	// @Description		: 卸载战斗地图
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: dwSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaUnloadBattleScene(QLuaScript& sc)
	{
		INT nRet = 0;
		INT nSceneId = sc.GetInt(1);
        DWORD dwMapId = GET_MAP_ID(nSceneId);
		DWORD dwMapCopyIndex = GET_MAP_COPY_INDEX(nSceneId);

		g_RelayClient.DoDeleteBattleMapRequest(dwMapId, dwMapCopyIndex);
	 
		return nRet;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: LoadScene
	// @Description		: 载入地图 debug用
	// @ReturnCode		: 返回0：导入失败，返回1：导入成功
	// @ArgumentFlag	: dd[d]
	// @ArgumentComment	: nSceneId
	// @ArgumentComment	: nMapOwnerId 
	// @ArgumentComment	: nUserParam1 用户参数，加载完成返回
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaLoadScene(QLuaScript& sc)
	{
		INT nRet = 0;
		INT nMapId = sc.GetInt(1);
		INT nMapOwnerId = sc.GetInt(2);
		INT nParam1 = sc.GetTopIndex() >= 3 ? sc.GetInt(3) : 0;
		BOOL bToThisServer = sc.GetTopIndex() >= 4 ? sc.GetInt(4) : 0;

		nRet = g_RelayClient.DoLoadMapRequest(nMapId, nMapOwnerId, nParam1, bToThisServer);

		sc.PushNumber(nRet);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSceneType
	// @Description		: 得到地图Type
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nMapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSceneType(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(nMapId);
		QCONFIRM_RET_NULL(pCfg);
		sc.PushNumber(pCfg->nType);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSceneTypeName
	// @Description		: 得到地图SceneTypeName
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nMapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSceneTypeName(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(nMapId);
		if(!pCfg)
		{
			sc.OutPutErrMsgF("..%s","LuaGetSceneTypeName");
			//printf("ffffffffffffffffffffffffffffffffffffff:%d\n", nMapId);
		}
		if(NULL == pCfg)
		{
			QLogPrintf(LOG_ERR,"nMapId = %d",nMapId);
		}
		QCONFIRM_RET_NULL(pCfg);
		sc.PushString(pCfg->szSceneType);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSceneFightState
	// @Description		: 判断一个地图是否是可战斗地图
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nMapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSceneFightState(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(nMapId);
		QCONFIRM_RET_NULL(pCfg);
		sc.PushNumber(pCfg->bFightMap);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetSceneSetFightMode
	// @Description		: 判断一个地图是否是可战斗地图
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nMapId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetSceneSetFightMode(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		CONST KSCENE_CONFIG* pCfg = g_pSceneMgr->GetSceneInfo(nMapId);
		QCONFIRM_RET_NULL(pCfg);
		sc.PushNumber(pCfg->bSetFightMode);
		return 1;
	}
	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: IsDynamicScene
	// @Description		: 判断一个场景是否是动态场景
	// @ReturnCode		: 1 是动态场景 0 不是 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsDynamicScene(QLuaScript& sc)
	{
		INT nRet = 0;
		//INT nSceneId = sc.GetInt(1);
		//if (IS_DYNAMIC_SCENE(nSceneId))
		//{
		//	nRet = 1;
		//}
		sc.PushNumber(nRet);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetBattleSceneIdByTemplateId
	// @Description		: 通过场景的TemplateId获得该场景默认使用的战斗场景Id
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景TemplateId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetBattleSceneIdByTemplateId(QLuaScript& sc)
	{
		CONST KSCENE_CONFIG* pSceneConfig = g_pSceneMgr->GetSceneInfo(sc.GetInt(1));
		QCONFIRM_RET_FALSE(pSceneConfig);

		sc.PushNumber(pSceneConfig->nBattleSceneId);
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: IsDynamicSceneLoad
	// @Description		: 通过模版id查询动态场景是否已加载过
	// @ReturnCode		: 如存在
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景TemplateId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaIsDynamicSceneLoad(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		INT nMapCopyIndex = 0;

		if (sc.GetTopIndex() > 1)
		{
			nMapCopyIndex = sc.GetInt(2);
		}

		IKScene* pScene = g_pSceneMgr->GetScene(sc.GetInt(1), nMapCopyIndex);

		if (pScene == NULL)
		{
			sc.PushNumber(0);
		}
		else
		{
            sc.PushNumber(1);
		}
		
		return 1;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetDynamicSceneIdByTemplateId
	// @Description		: 通过模版id得到普通动态场景的id
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: 场景TemplateId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetDynamicSceneIdByTemplateId(QLuaScript& sc)
	{
		INT nMapId = sc.GetInt(1);
		INT nMapCopyIndex = 0;

		if (sc.GetTopIndex() > 1)
		{
			nMapCopyIndex = sc.GetInt(2);
		}

		IKScene* pScene = g_pSceneMgr->GetScene(sc.GetInt(1), nMapCopyIndex);

		if (pScene == NULL)
		{
			sc.PushNumber(0);
		}
		else
		{
			sc.PushNumber(pScene->GetId());
		}

		return 1;
	}


#endif // GAME_SERVER

#include "kscenescript_table.hpp"
}

BOOL g_RegisterSceneScript()
{
	return g_cScriptManager.RegisterTableFuncs("KScene", KSceneScript::arFunction, countof(KSceneScript::arFunction));
}

