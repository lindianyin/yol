#include "stdafx.h"
#include "playertitle.h"
#include "titlesetting.h"
#include "onlinegamebase/ksysservice.h"
#include "buffer/buffermanager.h"
#include "titleprotocol.h"
#include "titlemodule.h"
#include "onlinegamemodule/characterbuffer.h"
//#include "../gameworld/nsworld/kcharacterhelper.h"

PlayerTitle::PlayerTitle(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerTitle::~PlayerTitle()
{

}

BOOL PlayerTitle::Init()
{
	m_pLunaObj = new LuaPlayerTitle(m_rcCharacter);
	m_mapTitleSaveData.clear();
	return TRUE;
}

BOOL PlayerTitle::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	return TRUE;
}

BOOL PlayerTitle::OnLogin()
{
	return TRUE;
}

BOOL PlayerTitle::OnLogout()
{
	return TRUE;
}

BOOL PlayerTitle::Active()
{
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;
	TIME_T tNow = KSysService::GameTime(NULL);
	INT nTitleCarry1				= m_rcPlayer.GetTitleCarry1();
	INT nTitleCarry2				= m_rcPlayer.GetTitleCarry2();
	INT nTitleCarry3				= m_rcPlayer.GetTitleCarry3();
	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end();)
	{
		TitleTemplate* pTitle = g_cTitleSetting.GetTitleTemplate(it->second.nTitleId);
		if (pTitle == NULL) continue;

		//删除过期的称号
		if (it->second.bOwn == TRUE && pTitle->nEffectiveTime > 0 && (INT)(tNow - it->second.dwGetTime) > pTitle->nEffectiveTime)
		{
			if (it->second.nTitleId == nTitleCarry1)
				m_rcPlayer.SetTitleCarry1(0);
			else if (it->second.nTitleId == nTitleCarry2)
				m_rcPlayer.SetTitleCarry2(0);
			else if (it->second.nTitleId == nTitleCarry3)
				m_rcPlayer.SetTitleCarry3(0);
			else
			{
				m_mapTitleSaveData.erase(it++);
				SyncTitle();
			}
			break;
		}
		else
			++it;

	}

	return TRUE;
}


BOOL PlayerTitle::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	if (uSize == 0)
		return TRUE;

	if (pData == NULL || uSize < sizeof(TitleSaveData))
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	INT nTitleCarry1				= m_rcPlayer.GetTitleCarry1();
	INT nTitleCarry2				= m_rcPlayer.GetTitleCarry2();
	INT nTitleCarry3				= m_rcPlayer.GetTitleCarry3();
	m_mapTitleSaveData.clear();
	SIZE_T uLoadBuffSize = 0;
	while (uLoadBuffSize < uSize)
	{
		if (uLoadBuffSize + sizeof(TitleSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		TitleSaveData* pBufData = (TitleSaveData*)pData;
		
		//过滤
		TitleTemplate* pTitle = g_cTitleSetting.GetTitleTemplate(pBufData->nTitleId);
		if (pTitle != NULL)
		{
			if (pBufData->bUse == TRUE)
			{
				g_cBufferMgr.AddBufferToCharacter(m_rcPlayer.GetId(), 0, m_rcPlayer.GetId(), pTitle->nBuffId);
			}
			m_mapTitleSaveData.insert(make_pair(pBufData->nTitleId, *pBufData));
		}

		pData += sizeof(TitleSaveData);
		uLoadBuffSize += sizeof(TitleSaveData);
	}

	if (nTitleCarry1 > 0 && m_mapTitleSaveData.find(nTitleCarry1) == m_mapTitleSaveData.end())
		m_rcPlayer.SetTitleCarry1(0);
	if (nTitleCarry2 > 0 && m_mapTitleSaveData.find(nTitleCarry2) == m_mapTitleSaveData.end())
		m_rcPlayer.SetTitleCarry2(0);
	if (nTitleCarry3 > 0 && m_mapTitleSaveData.find(nTitleCarry3) == m_mapTitleSaveData.end())
		m_rcPlayer.SetTitleCarry3(0);

	return TRUE;
}

BOOL PlayerTitle::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	INT nTotalSize = 0;
	TIME_T tNow = 0;
	LOG_PROCESS_ERROR(pBuffer);
	tNow = KSysService::GameTime(NULL);
	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end(); ++it)
	{
		TitleSaveData* pBufferData = (TitleSaveData*)pBuffer;

		pBufferData->nTitleId = it->second.nTitleId;
		pBufferData->dwGetTime = it->second.dwGetTime;
		pBufferData->nCount = it->second.nCount;
		pBufferData->bOwn = it->second.bOwn;
		pBufferData->bUse = it->second.bUse;

		pBuffer += sizeof(TitleSaveData);
		uUsedSize += sizeof(TitleSaveData);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerTitle::RemoveTitleToCharacter(INT nGetWay, INT nLevel, INT nType)
{
	INT nTitleCarry1				= m_rcPlayer.GetTitleCarry1();
	INT nTitleCarry2				= m_rcPlayer.GetTitleCarry2();
	INT nTitleCarry3				= m_rcPlayer.GetTitleCarry3();
	std::map<INT, TitleSaveData>::iterator it;
	TitleTemplate* pTitle			= g_cTitleSetting.GetTitleTemplate(nGetWay, nLevel, nType);
	LOG_PROCESS_ERROR(pTitle);
	

	if (m_mapTitleSaveData.find(pTitle->nId) != m_mapTitleSaveData.end())
	{
		if (pTitle->nId == nTitleCarry1)
			m_rcPlayer.SetTitleCarry1(0);
		else if (pTitle->nId == nTitleCarry2)
			m_rcPlayer.SetTitleCarry2(0);
		else if (pTitle->nId == nTitleCarry3)
			m_rcPlayer.SetTitleCarry3(0);
		/*else
		{
			m_mapTitleSaveData.erase(m_mapTitleSaveData.find(pTitle->nId));
			SyncTitle();
		}*/
		it = m_mapTitleSaveData.find(pTitle->nId);
		if(it != m_mapTitleSaveData.end())
		{
			m_mapTitleSaveData.erase(it);
			SyncTitle();
		}
	}

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerTitle::AddTitleToCharacter1(INT nGetWay, INT nLevel, INT nType, DWORD dwPassTime)
{
	TitleSaveData saveDate;
	BOOL bExist						= FALSE;
	INT nTitleId					= -1;
	INT nTitleCarry1				= 0;
	INT nTitleCarry2				= 0;
	INT nTitleCarry3				= 0;
	TitleTemplate* pTitle			= g_cTitleSetting.GetTitleTemplate(nGetWay, nLevel, nType);
	if(!pTitle)
	{
		printf("找不到称号, GetWay:%d, Level:%d, Type:%d\n", nGetWay, nLevel, nType);
	}
	QCONFIRM_RET_FALSE(pTitle);

	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end();)
	{
		TitleTemplate* pTitleType = g_cTitleSetting.GetTitleTemplate(it->second.nTitleId);
		LOG_PROCESS_ERROR(pTitleType);

		//删除已有相同称号(永久称号除外)
		if (it->second.nTitleId == pTitle->nId && it->second.bOwn == TRUE)
		{
			if (pTitle->nEffectiveTime < 0) return TRUE;
			nTitleId = it->second.nTitleId;
			bExist= TRUE;
			m_mapTitleSaveData.erase(it++);
		}

		//删除互斥的称号
		else if (pTitleType->nMutex == pTitle->nMutex && it->second.bOwn == TRUE)
		{
			nTitleId = it->second.nTitleId;
			m_mapTitleSaveData.erase(it++);
		}
		else
			++it;
	}

	saveDate.dwGetTime = KSysService::GameTime(NULL) - dwPassTime;
	saveDate.nTitleId = pTitle->nId;
	saveDate.bOwn = TRUE;
	saveDate.nCount = 1;
	saveDate.bUse = FALSE;
	m_mapTitleSaveData.insert(make_pair(saveDate.nTitleId, saveDate));

	nTitleCarry1				= m_rcPlayer.GetTitleCarry1();
	nTitleCarry2				= m_rcPlayer.GetTitleCarry2();
	nTitleCarry3				= m_rcPlayer.GetTitleCarry3();
	if (nTitleCarry1 == nTitleId)
		m_rcPlayer.SetTitleCarry1(pTitle->nId);
	else if (nTitleCarry2 == nTitleId)
		m_rcPlayer.SetTitleCarry2(pTitle->nId);
	else if (nTitleCarry3 == nTitleId)
		m_rcPlayer.SetTitleCarry3(pTitle->nId);
	else if (nTitleCarry1 == 0)
		m_rcPlayer.SetTitleCarry1(pTitle->nId);
	else if (nTitleCarry2 == 0)
		m_rcPlayer.SetTitleCarry2(pTitle->nId);
	else if (nTitleCarry3 == 0)
		m_rcPlayer.SetTitleCarry3(pTitle->nId);
	else
		SyncTitle();

	if (bExist == FALSE)
		SendAddTitle(pTitle->nId);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerTitle::AddTitleToCharacter2(INT nGetWay, INT nLevel, INT nType, DWORD dwPassTime)
{
	TitleSaveData saveDate;
	BOOL bExist						= FALSE;
	INT nTitleId					= -1;
	INT nTitleCarry1				= 0;
	INT nTitleCarry2				= 0;
	INT nTitleCarry3				= 0;
	TitleTemplate* pTitle			= NULL;
	TitleTemplate* pTitleType		= g_cTitleSetting.GetTitleTemplate(nGetWay, nType);
	QCONFIRM_RET_FALSE(pTitleType);
	if (m_mapTitleSaveData.find(pTitleType->nId) == m_mapTitleSaveData.end())
	{
		saveDate.dwGetTime = KSysService::GameTime(NULL) - dwPassTime;
		saveDate.nTitleId = pTitleType->nId;
		saveDate.bOwn = FALSE;
		saveDate.nCount = 1;
		saveDate.bUse = FALSE;
		m_mapTitleSaveData.insert(make_pair(saveDate.nTitleId, saveDate));
	}
	else
	{
		m_mapTitleSaveData[pTitleType->nId].nCount++;
		m_mapTitleSaveData[pTitleType->nId].dwGetTime = KSysService::GameTime(NULL);
	}

	pTitle	= g_cTitleSetting.GetTitleTemplate(nGetWay, m_mapTitleSaveData[pTitleType->nId].nCount, nType);
	LOG_PROCESS_ERROR(pTitle);

	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end();)
	{
		TitleTemplate* pTitleType = g_cTitleSetting.GetTitleTemplate(it->second.nTitleId);
		LOG_PROCESS_ERROR(pTitleType);

		//删除已有相同称号(永久称号除外)
		if (it->second.nTitleId == pTitle->nId && it->second.bOwn == TRUE)
		{
			if (pTitle->nEffectiveTime < 0) return TRUE;
			nTitleId = it->second.nTitleId;
			bExist= TRUE;
			m_mapTitleSaveData.erase(it++);
		}

		//删除互斥的称号
		else if (pTitleType->nMutex == pTitle->nMutex && it->second.bOwn == TRUE)
		{
			nTitleId = it->second.nTitleId;
			m_mapTitleSaveData.erase(it++);

		}
		else
			++it;
	}

	m_mapTitleSaveData[pTitle->nId].bOwn = TRUE;
	m_mapTitleSaveData[pTitle->nId].dwGetTime = KSysService::GameTime(NULL);

	nTitleCarry1				= m_rcPlayer.GetTitleCarry1();
	nTitleCarry2				= m_rcPlayer.GetTitleCarry2();
	nTitleCarry3				= m_rcPlayer.GetTitleCarry3();

	if (nTitleCarry1 == nTitleId)
		m_rcPlayer.SetTitleCarry1(pTitle->nId);
	else if (nTitleCarry2 == nTitleId)
		m_rcPlayer.SetTitleCarry2(pTitle->nId);
	else if (nTitleCarry3 == nTitleId)
		m_rcPlayer.SetTitleCarry3(pTitle->nId);
	else if (nTitleCarry1 == 0)
		m_rcPlayer.SetTitleCarry1(pTitle->nId);
	else if (nTitleCarry2 == 0)
		m_rcPlayer.SetTitleCarry2(pTitle->nId);
	else if (nTitleCarry3 == 0)
		m_rcPlayer.SetTitleCarry3(pTitle->nId);
	else
		SyncTitle();

	if (bExist == FALSE)
		SendAddTitle(pTitle->nId);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerTitle::SyncTitle()
{
	IMemBlock* piPackage					= NULL;
	TITLE_S2C_SYNC_TITLE* ptcTitle			= NULL;
	INT nInx								= 0;
	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end(); ++it)
	{
		if (it->second.bOwn == FALSE) continue;
		nInx++;
	}
	piPackage = QCreateMemBlock(sizeof(TITLE_S2C_SYNC_TITLE)+sizeof(TITLE_INFO) * nInx);
	ptcTitle = (TITLE_S2C_SYNC_TITLE*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcTitle);

	ptcTitle->byProtocol					= (BYTE)emTITLE_PTC_S2C_SYNC_TITLE;
	ptcTitle->nCount						= nInx;

	nInx = 0;
	for (std::map<INT, TitleSaveData>::iterator it = m_mapTitleSaveData.begin(); it != m_mapTitleSaveData.end(); ++it)
	{
		if (it->second.bOwn == FALSE) continue;

		ptcTitle->arrInfo[nInx].nTitleId = it->second.nTitleId;
		ptcTitle->arrInfo[nInx].byUse = (BYTE)(it->second.bUse);
		nInx++;

	}
	g_TitleModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcTitle, sizeof(TITLE_S2C_SYNC_TITLE)+sizeof(TITLE_INFO) * ptcTitle->nCount);
	return TRUE;

EXIT0:
	SAFE_RELEASE(piPackage);
	return FALSE;
}

BOOL PlayerTitle::SendAddTitle(INT nTitleId)
{
	TITLE_S2C_ADD_TITLE ptcResult;
	ptcResult.byProtocol = (BYTE)emTITLE_PTC_S2C_ADD_TILLE;
	ptcResult.nTitleId = nTitleId;

	return g_TitleModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), ptcResult);
}

BOOL PlayerTitle::SetTitleCarray_1(INT nOldValue, INT nNewValue)
{
	//旧携带称号处理
	TIME_T tNow = 0;
	TitleTemplate* pTitleOld = g_cTitleSetting.GetTitleTemplate(nOldValue);
	if (pTitleOld != NULL)
	{
		CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
		if (pCharacterBuffer != NULL)
		{
			Buffer* pBuffer = pCharacterBuffer->GetBufferByTemplateId(pTitleOld->nBuffId);
			if (pBuffer != NULL)
			{
				INT nIdx = g_cBufferMgr.GetIndexById(pBuffer->GetID());
				g_cBufferMgr.RemoveBufferFromCharacter(m_rcPlayer.GetId(), nIdx);
			}
		}

		std::map<INT, TitleSaveData>::iterator itOld = m_mapTitleSaveData.find(nOldValue);
		if (itOld != m_mapTitleSaveData.end()) 
		{
			itOld->second.bUse = FALSE;
			//m_mapTitleSaveData.erase(itOld);
			tNow = KSysService::GameTime(NULL);
			if (pTitleOld->nEffectiveTime > 0)
			{
				if ((INT)(tNow - itOld->second.dwGetTime) >= pTitleOld->nEffectiveTime)
					m_mapTitleSaveData.erase(itOld);
			}
		}
	}

	//新携带称号处理
	TitleTemplate* pTitleNew = g_cTitleSetting.GetTitleTemplate(nNewValue);
	if (pTitleNew != NULL)
	{
		std::map<INT, TitleSaveData>::iterator itNew = m_mapTitleSaveData.find(nNewValue); 
		if (itNew != m_mapTitleSaveData.end())
		{
			if (itNew->second.bOwn == TRUE && pTitleNew->nEffectiveTime > 0 && (INT)(tNow - itNew->second.dwGetTime) >= pTitleNew->nEffectiveTime)
			{
				m_mapTitleSaveData.erase(itNew);
				m_rcPlayer.SetTitleCarry1(0);

			}
			else
			{
				itNew->second.bUse = TRUE;
				g_cBufferMgr.AddBufferToCharacter(m_rcPlayer.GetId(), 0, m_rcPlayer.GetId(), pTitleNew->nBuffId);
			}
		}
		else
		{
			m_rcPlayer.SetTitleCarry1(0);
		}
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction( "Player", "DelayCalcFightScore", 0, "d", m_rcPlayer.GetId());
	SyncTitle();

EXIT0:
	return TRUE;
}

BOOL PlayerTitle::SetTitleCarray_2(INT nOldValue, INT nNewValue)
{
	//旧携带称号处理
	TIME_T tNow = 0;
	TitleTemplate* pTitleOld = g_cTitleSetting.GetTitleTemplate(nOldValue);
	if (pTitleOld != NULL)
	{
		CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
		if (pCharacterBuffer != NULL)
		{
			Buffer* pBuffer = pCharacterBuffer->GetBufferByTemplateId(pTitleOld->nBuffId);
			if (pBuffer != NULL)
			{
				INT nIdx = g_cBufferMgr.GetIndexById(pBuffer->GetID());
				g_cBufferMgr.RemoveBufferFromCharacter(m_rcPlayer.GetId(), nIdx);
			}
		}

		std::map<INT, TitleSaveData>::iterator itOld = m_mapTitleSaveData.find(nOldValue);
		if (itOld != m_mapTitleSaveData.end()) 
		{
			itOld->second.bUse = FALSE;
			//m_mapTitleSaveData.erase(itOld);
			tNow = KSysService::GameTime(NULL);
			if (pTitleOld->nEffectiveTime > 0)
			{
				if ((INT)(tNow - itOld->second.dwGetTime) >= pTitleOld->nEffectiveTime)
					m_mapTitleSaveData.erase(itOld);
			}
		}
	}

	//新携带称号处理
	TitleTemplate* pTitleNew = g_cTitleSetting.GetTitleTemplate(nNewValue);
	if (pTitleNew != NULL)
	{
		std::map<INT, TitleSaveData>::iterator itNew = m_mapTitleSaveData.find(nNewValue); 
		if (itNew != m_mapTitleSaveData.end())
		{
			if (itNew->second.bOwn == TRUE && pTitleNew->nEffectiveTime > 0 && (INT)(tNow - itNew->second.dwGetTime) >= pTitleNew->nEffectiveTime)
			{
				m_mapTitleSaveData.erase(itNew);
				m_rcPlayer.SetTitleCarry2(0);

			}
			else
			{
				itNew->second.bUse = TRUE;
				g_cBufferMgr.AddBufferToCharacter(m_rcPlayer.GetId(), 0, m_rcPlayer.GetId(), pTitleNew->nBuffId);
			}
		}
		else
		{
			m_rcPlayer.SetTitleCarry2(0);
		}
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction( "Player", "DelayCalcFightScore", 0, "d", m_rcPlayer.GetId());
	SyncTitle();

EXIT0:
	return TRUE;
}

BOOL PlayerTitle::SetTitleCarray_3(INT nOldValue, INT nNewValue)
{
	//旧携带称号处理
	TIME_T tNow = 0;
	TitleTemplate* pTitleOld = g_cTitleSetting.GetTitleTemplate(nOldValue);
	if (pTitleOld != NULL)
	{
		CharacterBuffer* pCharacterBuffer = m_rcCharacter.GetCtrl<CharacterBuffer>();
		if (pCharacterBuffer != NULL)
		{
			Buffer* pBuffer = pCharacterBuffer->GetBufferByTemplateId(pTitleOld->nBuffId);
			if (pBuffer != NULL)
			{
				INT nIdx = g_cBufferMgr.GetIndexById(pBuffer->GetID());
				g_cBufferMgr.RemoveBufferFromCharacter(m_rcPlayer.GetId(), nIdx);
			}
		}

		std::map<INT, TitleSaveData>::iterator itOld = m_mapTitleSaveData.find(nOldValue);
		if (itOld != m_mapTitleSaveData.end()) 
		{
			itOld->second.bUse = FALSE;
			//m_mapTitleSaveData.erase(itOld);
			tNow = KSysService::GameTime(NULL);
			if (pTitleOld->nEffectiveTime > 0)
			{
				if ((INT)(tNow - itOld->second.dwGetTime) >= pTitleOld->nEffectiveTime)
					m_mapTitleSaveData.erase(itOld);
			}
		}
	}

	//新携带称号处理
	TitleTemplate* pTitleNew = g_cTitleSetting.GetTitleTemplate(nNewValue);
	if (pTitleNew != NULL)
	{
		std::map<INT, TitleSaveData>::iterator itNew = m_mapTitleSaveData.find(nNewValue); 
		if (itNew != m_mapTitleSaveData.end())
		{
			if (itNew->second.bOwn == TRUE && pTitleNew->nEffectiveTime > 0 && (INT)(tNow - itNew->second.dwGetTime) >= pTitleNew->nEffectiveTime)
			{
				m_mapTitleSaveData.erase(itNew);
				m_rcPlayer.SetTitleCarry3(0);

			}
			else
			{
				itNew->second.bUse = TRUE;
				g_cBufferMgr.AddBufferToCharacter(m_rcPlayer.GetId(), 0, m_rcPlayer.GetId(), pTitleNew->nBuffId);
			}
		}
		else
		{
			m_rcPlayer.SetTitleCarry3(0);
		}
	}

	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	cSafeScript->CallTableFunction( "Player", "DelayCalcFightScore", 0, "d", m_rcPlayer.GetId());
	//g_cCharacterHelper.CalcFightScore(m_rcCharacter);
	SyncTitle();

EXIT0:
	return TRUE;
}

BOOL PlayerTitle::ClearSavaData()
{
	m_mapTitleSaveData.clear();
	return TRUE;
}


LuaPlayerTitle* PlayerTitle::GetScriptInterface() CONST
{
	return m_pLunaObj;
}