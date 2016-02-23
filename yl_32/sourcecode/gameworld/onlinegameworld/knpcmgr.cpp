/* -------------------------------------------------------------------------
//	文件名		：	KNpcMgr.cpp
//	创建者		：	simon
//	创建时间	：	2009/11/21 12:11:22
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/knpcmgr.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/korpgworld.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/ikscene.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/kregeneration_mgr.h"
#include "onlinegamemodule/ai/kaictrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

KNpcMgr g_cNpcMgr;

// -------------------------------------------------------------------------

BOOL KNpcMgr::Init( )
{
	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	QCONFIRM_RET_FALSE(pConfig);

	m_cObjPool.Init(pConfig->nMaxNpc);

	ReadNpcTemplate();

	return TRUE;
}

INT KNpcMgr::Add(DWORD dwTemplateId, DWORD dwId /*= KD_BAD_ID*/ )
{
	BOOL bOk = FALSE;
	INT nIndex = 0;
	KNpc* pNpc = m_cObjPool.GetById(dwId);
	if (pNpc)
	{
		nIndex = pNpc->m_nIndex;
		IKScene* pScene = pNpc->GetScene();
		if (pScene)
			pScene->RemoveNpc(pNpc);
	}
	else
	{
		KNpcTemplates::iterator it = m_mapNpcTemplate.find(dwTemplateId);
		LOG_PROCESS_ERROR(it != m_mapNpcTemplate.end());

		CONST KNPC_TEMPLATE& sNpcTemplate = it->second;

		nIndex = m_cObjPool.Add(dwId);

		pNpc = m_cObjPool.GetByIndex(nIndex);
		LOG_PROCESS_ERROR(pNpc);

		bOk = pNpc->Init(nIndex, sNpcTemplate);
		LOG_PROCESS_ERROR(bOk && "Npc Init Failed!");
	}
EXIT0:
	return nIndex;
}

BOOL KNpcMgr::Remove( DWORD dwId, BOOL bRegenerate)
{
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	KNpc* pNpc = m_cObjPool.GetById(dwId);
	QCONFIRM_RET_FALSE(pNpc);

	// 重置NPC AI相关
	KAICtrl* pAICtrl = pNpc->GetCtrl<KAICtrl>();
	pAICtrl->UnInit();

#ifdef GAME_SERVER
	//如果有重生信息，注册重生事件
	KREGENERATION_INFO* pRegenerationInfo = pNpc->GetRegenerationInfo();
	if (pRegenerationInfo && bRegenerate)
	{
		//只有在场景中的NPC才需要重生
		if (pNpc->GetScene())
		{
			KS_REGENERATION_INFO info;
			info.dwIdExpect = pNpc->GetId();
			info.byType = emKOBJ_NPC;
			info.dwFrames = pRegenerationInfo->nGenerationTime;
			info.dwSceneId = pNpc->GetScene()->GetId();
			info.dwTemplateId = pNpc->GetTemplateId();
			info.nFaceDirection = pRegenerationInfo->nFaceDirection;
			info.nX = pRegenerationInfo->nX;
			info.nY = pRegenerationInfo->nY;
			info.nZ = pRegenerationInfo->nZ;
			info.nNormalAIType = pRegenerationInfo->nNormalAIType;
			info.nRelation = pRegenerationInfo->nRelation;
			info.szNickName = pNpc->GetNickName();
			info.dwExValue = pRegenerationInfo->dwExValue;
			strncpy(info.szExInfo, pRegenerationInfo->szExInfo, MAX_NAME_LEN);
			KRegenerationMgr::GetInstance().Register(info);
		}	
	}
	else if(pRegenerationInfo && !bRegenerate)
	{
		KRegenerationMgr::GetInstance().UnRegister(pNpc->GetTemplateId(), pNpc->GetScene()->GetId());
	}
	g_cPlayerBaseProcess.DoSyncLeave(*pNpc);
#endif // GAME_SERVER

	cSafeScript->CallTableFunction("Scene", "OnLeaveScene", 0, "dd",
		pNpc->GetId(), 1);
	pNpc->Uninit();

	return m_cObjPool.Remove(dwId);
}

BOOL KNpcMgr::Uninit()
{
	// 所有Npc应该都在场景里，如果有例外需要在这里显式调用Remove函数
	m_cObjPool.Uninit();
	m_mapNpcTemplate.clear();
	return TRUE;
}

KNpc* KNpcMgr::GetById( DWORD dwId )
{
	return m_cObjPool.GetById(dwId);
}

KNpc* KNpcMgr::GetByIndex( INT nIndex )
{
	return m_cObjPool.GetByIndex(nIndex);
}

BOOL KNpcMgr::ReadNpcTemplate()
{
	BOOL bResult = FALSE;
	ITabFile* pFile = g_OpenTabFile(KDF_NPC_LIST);
	QCONFIRM_RET_FALSE(pFile);

	INT nLines = pFile->GetHeight();

	// 第一行是英文字段头描述
	for (INT n = 2; n <= nLines; ++n)
	{
		INT nTmp = 0;
		KNPC_TEMPLATE sNpcInfo;
		CHAR szBuf[MAX_NAME_LEN] = "";
		CHAR szKeyName[MAX_PATH];

		bResult = pFile->GetInteger(n, "TemplateId", KD_BAD_ID, &nTmp);
		sNpcInfo.dwTemplateId = (DWORD)nTmp;

		bResult = pFile->GetString(n, "DisplayName", "", szBuf, countof(szBuf));
		sNpcInfo.strDisplayName = szBuf;

		bResult = pFile->GetString(n, "Title", "", szBuf, countof(szBuf));
		sNpcInfo.strTitle = szBuf;

		bResult = pFile->GetString(n, "ClassName", "", szBuf, countof(szBuf));
		sNpcInfo.strClassName = szBuf;

		bResult = pFile->GetInteger(n, "Kind", 0, &nTmp);
		sNpcInfo.byKind = (BYTE)nTmp;

		bResult = pFile->GetInteger(n, "TouchRange", 0, &sNpcInfo.nTouchRange);

		bResult = pFile->GetInteger(n, "DropListId1", 0, &nTmp);
		sNpcInfo.nDropListId1 = nTmp;

		bResult = pFile->GetInteger(n, "DropTimes1", 0, &nTmp);
		sNpcInfo.nDropTimes1 = nTmp;

		{
			INT nIdx = 0;
			memset(sNpcInfo.nIgnoreDebuff, 0, sizeof(sNpcInfo.nIgnoreDebuff));
			char achIgnore[5+1] = { 0 };
			pFile->GetString(n, "IgnoreDebuffType", "", achIgnore, 5+1);
			std::string tIgnore(achIgnore);
			if (tIgnore.size())
			{
				std::basic_string<char>::size_type iBegin = 0, iEnd = 0;
				while ( (iEnd = tIgnore.find("|", iBegin)) != std::string::npos )
				{
					INT nType = atoi(tIgnore.substr(iBegin, iEnd - iBegin).c_str());
					sNpcInfo.nIgnoreDebuff[nIdx++] = nType;
					iBegin = iEnd + 1;
				}
				if (iBegin < tIgnore.size())
				{
					INT nType = atoi(tIgnore.substr(iBegin, tIgnore.size() - iBegin).c_str());
					sNpcInfo.nIgnoreDebuff[nIdx++] = nType;
				}
			}
		}

		for (INT i = 0;i < MAX_NPC_AI_SKILL; ++i)
		{
			snprintf(szKeyName, sizeof(szKeyName), "SkillId%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, (INT*)&sNpcInfo.dwSkillIDList[i]);

			sNpcInfo.dwSkillLevelList[i] = 1;

			snprintf(szKeyName, sizeof(szKeyName), "SkillCastInterval%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, &sNpcInfo.nSkillCastInterval[i]);

			snprintf(szKeyName, sizeof(szKeyName), "AISkillType%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, &sNpcInfo.nAISkillType[i]);

			snprintf(szKeyName, sizeof(szKeyName), "SkillRate%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, &sNpcInfo.nSkillRate[i]);

			snprintf(szKeyName, sizeof(szKeyName), "SkillAniFrame%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, &sNpcInfo.nSkillAniFrame[i]);

			snprintf(szKeyName, sizeof(szKeyName), "SkillRestFrame%d", i + 1);
			bResult = pFile->GetInteger(n, szKeyName, 0, &sNpcInfo.nSkillRestFrame[i]);
		}

		bResult = pFile->GetInteger(n, "ThreatTime", 0, &sNpcInfo.nThreatTime);
		bResult = pFile->GetInteger(n, "ThreatLinkId", 0, &sNpcInfo.nThreatLinkId);
		bResult = pFile->GetInteger(n, "OverThreatPercent", 0, &sNpcInfo.nOverThreatPercent);

		bResult = pFile->GetInteger(n, "PursuitRange", 0, &sNpcInfo.nPursuitRange);

		QCONFIRM_RET_FALSE(m_mapNpcTemplate.insert(std::make_pair(sNpcInfo.dwTemplateId, sNpcInfo)).second);
	}

	return bResult;
}

const KNPC_TEMPLATE* KNpcMgr::GetTemplateInfo( DWORD dwTemplateID )
{
	if (m_mapNpcTemplate.find(dwTemplateID) != m_mapNpcTemplate.end())
	{
		return &m_mapNpcTemplate[dwTemplateID];
	}
	return NULL;
}
