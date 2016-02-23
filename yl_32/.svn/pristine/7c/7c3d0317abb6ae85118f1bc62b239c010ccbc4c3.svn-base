#include "stdafx.h"
#include "playertarget.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "targetsetting.h"
#include "targetmodule.h"
#include "targetprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"


PlayerTarget::PlayerTarget(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerTarget::~PlayerTarget()
{

}

BOOL PlayerTarget::Init()
{
	m_pLunaObj = new LuaPlayerTarget(m_rcCharacter);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerTarget::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerTarget::OnLogin()
{
	return TRUE;
}

BOOL PlayerTarget::OnLogout()
{
	return TRUE;
}

BOOL PlayerTarget::Active()
{
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerTarget::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	if (uSize == 0)
		return TRUE;

	if (pData == NULL)
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}
	m_vecSaveData.clear();
	SIZE_T uLoadBuffSize = 0;
	while (uLoadBuffSize < uSize)
	{
		if (uLoadBuffSize + sizeof(TargetSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}
		TargetSaveData bufData;
		memcpy(&bufData,pData,sizeof(TargetSaveData));
		m_vecSaveData.push_back(bufData);
		pData += sizeof(TargetSaveData);
		uLoadBuffSize += sizeof(TargetSaveData);
	}

	return TRUE;
}

BOOL PlayerTarget::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	INT nTotalSize = 0;
	TIME_T tNow = 0;
	LOG_PROCESS_ERROR(pBuffer);
	for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		TargetSaveData* pBufferData = (TargetSaveData*)pBuffer;
		pBufferData->nDay = it->nDay;
		pBufferData->nFinish = it->nFinish;
		pBufferData->nGive = it->nGive;
		pBufferData->nId = it->nId;
		pBufferData->nCount = it->nCount;

		pBuffer += sizeof(TargetSaveData);
		uUsedSize += sizeof(TargetSaveData);
	}

	return TRUE;
EXIT0:
	return FALSE;
}

/**/

BOOL PlayerTarget::DoTarget(INT nDay, INT nId)
{
	BOOL bResult 	= FALSE;
	TargetSaveData* pSaveData = NULL;
	TargetDayCfg* 	pTargetDay = NULL;
	KAward*			pAward  = NULL;
	TargetCfg* 		pTarget = NULL;
	BOOL isContain = FALSE;
	pSaveData 	= GetTargetSaveData(nDay, nId);
	pTargetDay 	= g_cTargetSetting.GetTargetDayCfg(nDay, nId);
	PROCESS_ERROR(pTargetDay);
	pTarget 	= g_cTargetSetting.GetTargetCfg(nDay);
	PROCESS_ERROR(pTarget);
	
	if (NULL == pSaveData)
	{
		LOG_PROCESS_ERROR(pTargetDay);
		TargetSaveData saveData;
		saveData.nCount = 1;
		saveData.nDay = nDay;
		saveData.nGive = 0;
		saveData.nId = nId;
		saveData.nFinish = pTargetDay->nFinishCount <= 1 ? 1 : 0;
		for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
		{
			if(it->nId == nId && it->nDay == nDay)
			{
				isContain = TRUE;
				break;
			}
		}
		if(!isContain)
		{
			m_vecSaveData.push_back(saveData);
		}
	}
	else
	{
		if(pSaveData->nFinish != 0)
		{
			goto EXIT0;
		}
		pSaveData->nCount++;
		if (pSaveData->nCount >= pTargetDay->nFinishCount)
		{
			pSaveData->nFinish = 1;
		}
	}
	SyncTarget();
	bResult = TRUE;
EXIT0:
	return bResult;
}




BOOL PlayerTarget::GetAward(INT nDay, INT nId)
{
	//QLogPrintf(LOG_INFO,"PlayerTarget::GetAward(INT nDay = %d, INT nId = %d)",nDay,nId);
	BOOL bResult = FALSE;
	TargetDayCfg* pTargetDay = NULL;
	TargetCfg* pTarget = NULL;
	TargetSaveData* pSaveData = NULL;
	KAward* pAward = NULL;
	pTargetDay = g_cTargetSetting.GetTargetDayCfg(nDay, nId);
	PROCESS_ERROR(pTargetDay);
	pTarget = g_cTargetSetting.GetTargetCfg(nDay);
	PROCESS_ERROR(pTarget);
	pSaveData = GetTargetSaveData(nDay, nId);
	PROCESS_ERROR(pSaveData);
	PROCESS_ERROR(pSaveData->nGive == 0);
	PROCESS_ERROR(pSaveData->nFinish == 1);
	pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer, "Target", pTargetDay->nAwardId);
	PROCESS_ERROR(pAward);
	pSaveData->nGive = 1;//标记已经领取
	SyncTarget();
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerTarget::SyncTarget()
{
	IMemBlock* piPackage					= NULL;
	TARGET_S2C_SYNC* ptcTarget				= NULL;
	INT nInx								= 0;
	BOOL bOk								= FALSE;
	for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0)
		{
			continue;
		}
		nInx++;
	}
	piPackage = QCreateMemBlock(sizeof(TARGET_S2C_SYNC)+sizeof(TARGET_INFO) * nInx);
	ptcTarget = (TARGET_S2C_SYNC*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcTarget);

	ptcTarget->byProtocol					= (BYTE)emTARGET_PTC_S2C_SYNC;
	ptcTarget->nCount						= nInx;
	//QLogPrintf(LOG_INFO,"..emTARGET_PTC_S2C_SYNC ptcTarget->nCount=%d..",ptcTarget->nCount);
	
	nInx = 0;
	for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0) 
		{
			continue;
		}

		ptcTarget->arrInfo[nInx].byDay = (BYTE)it->nDay;
		ptcTarget->arrInfo[nInx].byFinish = (BYTE)(it->nFinish);
		ptcTarget->arrInfo[nInx].byGive = (BYTE)(it->nGive);
		ptcTarget->arrInfo[nInx].byId = (BYTE)(it->nId);
		ptcTarget->arrInfo[nInx].nCount = it->nCount;
		nInx++;
	}
	bOk =  g_TargetModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcTarget, sizeof(TARGET_S2C_SYNC)+sizeof(TARGET_INFO) * nInx);
	SendAllFinish();
EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

BOOL PlayerTarget::SendAllFinish()
{
	//没完成判断
	INT count = g_cTargetSetting.GetTargetAwardCount();
	if (m_vecSaveData.size() < count) 
		return FALSE;
	for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0 || it->nGive == 0)
			return FALSE;
	}

	TARGET_S2C_ALL_FINISH ptcFinish;
	ptcFinish.byProtocol = (BYTE)emTARGET_PTC_S2C_ALL_FINISH;
	ptcFinish.nFinish = 1;

	return g_TargetModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), ptcFinish);
}

TargetSaveData* PlayerTarget::GetTargetSaveData(INT nDay, INT nId)
{
	for (std::vector<TargetSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nDay == nDay && it->nId == nId)
		{
			return &(*it);
		}
	}

	return NULL;
}

LuaPlayerTarget* PlayerTarget::GetScriptInterface() CONST
{
	return m_pLunaObj;
}
