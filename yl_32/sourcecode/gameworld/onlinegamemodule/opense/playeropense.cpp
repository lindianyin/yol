#include "stdafx.h"
#include "playeropense.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "opensesetting.h"
#include "opensemodule.h"
#include "openseprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"
#include "onlinegameworld/korpgworld.h"


//#include "../gameworld/nsworld/kcharacterhelper.h"


PlayerOpense::PlayerOpense(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerOpense::~PlayerOpense()
{

}

BOOL PlayerOpense::Init()
{
	m_pLunaObj = new LuaPlayerOpense(m_rcCharacter);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerOpense::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	m_vecSaveData.clear();
	return TRUE;
}

BOOL PlayerOpense::OnLogin()
{
	return TRUE;
}

BOOL PlayerOpense::OnLogout()
{
	return TRUE;
}

BOOL PlayerOpense::Active()
{
	if (g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerOpense::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
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
		if (uLoadBuffSize + sizeof(OpenseSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}
		OpenseSaveData bufData;
		memcpy(&bufData,pData,sizeof(OpenseSaveData));
		//OpenseSaveData* pBufData = (OpenseSaveData*)pData;
		m_vecSaveData.push_back(bufData);
		pData += sizeof(OpenseSaveData);
		uLoadBuffSize += sizeof(OpenseSaveData);
	}
	return TRUE;
}

BOOL PlayerOpense::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	BOOL bReslut = FALSE;
	LOG_PROCESS_ERROR(pBuffer);
	for (std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		OpenseSaveData* pBufferData = (OpenseSaveData*)pBuffer;
		pBufferData->nDay = it->nDay;
		pBufferData->nFinish = it->nFinish;
		pBufferData->nGive = it->nGive;
		pBufferData->nId = it->nId;
		pBufferData->nCount = it->nCount;

		pBuffer += sizeof(OpenseSaveData);
		uUsedSize += sizeof(OpenseSaveData);
	}
	bReslut =  TRUE;
EXIT0:
	return bReslut;
}

BOOL PlayerOpense::DoOpense(INT nDay, INT nId)
{
	//QLogPrintf(LOG_INFO,"PlayerOpense::DoOpense(INT nDay=%d, INT nId=%d)",nDay,nId);
	BOOL bResult = FALSE;
	OpenseSaveData* pSaveData = NULL;
	OpenseDayCfg* pOpenseDay = NULL;
	BOOL isContain = FALSE;
	pOpenseDay = g_cOpenseSetting.GetOpenseDayCfg(nDay, nId);
	PROCESS_ERROR(pOpenseDay);
	pSaveData = GetOpenseSaveData(nDay, nId);
	if (NULL == pSaveData)
	{
			OpenseSaveData saveData;
			saveData.nCount = 1;
			saveData.nDay = nDay;
			saveData.nGive = 0;
			saveData.nId = nId;
			saveData.nFinish = 0;
			//m_vecSaveData.push_back(saveData);
			for (std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
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
			if(0 == pSaveData->nFinish)//未完成加次数变完成
			{
				pSaveData->nCount++;
				if (pSaveData->nCount >= pOpenseDay->nFinishCount)
				{
					pSaveData->nFinish = 1;
				}
			}

	}
	//SyncOpense();
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerOpense::GetAward(INT nDay, INT nId)
{
	//QLogPrintf(LOG_INFO,"PlayerOpense::GetAward(INT nDay=%d, INT nId=%d)",nDay,nId);
	BOOL bResult = FALSE;
	OpenseDayCfg* pOpenseDay = NULL; 
	OpenseCfg* pOpense = NULL; 
	OpenseSaveData* pSaveData = NULL ;
	KAward* pAward = NULL;
	pOpenseDay = g_cOpenseSetting.GetOpenseDayCfg(nDay, nId);
	PROCESS_ERROR(pOpenseDay);
	pOpense = g_cOpenseSetting.GetOpenseCfg(nDay);
	PROCESS_ERROR(pOpense);
	pSaveData = GetOpenseSaveData(nDay, nId);
	PROCESS_ERROR(pSaveData);
	PROCESS_ERROR(pSaveData->nGive == 0 );	//1 给了  0表示没给
	PROCESS_ERROR(pSaveData->nFinish == 1); //1 完成  0表示没完成
	//QLogPrintf(LOG_INFO,"pOpense->nAwardId=%d",pOpense->nAwardId);
	pAward = g_cAwardMgr.AddPlayerAward(&m_rcPlayer, "Opense", pOpenseDay->nAwardId);
	PROCESS_ERROR(pAward);
	pSaveData->nGive = 1;
	SyncOpense();
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL PlayerOpense::SyncOpense()
{
	IMemBlock* piPackage = NULL;
	OPENSE_S2C_SYNC* ptcOpense = NULL;
	INT nInx = 0;
	BOOL bOk = FALSE;
	QIniFile cIniFile;
	INT nTimeNow = KSysService::Time(NULL);
	int opentime = KSysService::GetDayDiff(nTimeNow,g_cOrpgWorld.m_dwServerStartTime) + 1;
	Q_Printl("..SyncOpense .. opentime=" << opentime);
	for(std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0) 
		{
			continue;
		}
		nInx++;
	}
	piPackage = QCreateMemBlock(sizeof(OPENSE_S2C_SYNC)+sizeof(OPENSE_INFO)* nInx);
	ptcOpense = (OPENSE_S2C_SYNC*)piPackage->GetData();
	PROCESS_ERROR(ptcOpense);

	ptcOpense->byProtocol = (BYTE)emOPENSE_PTC_S2C_SYNC;
	ptcOpense->nCount = nInx;
	ptcOpense->day = opentime;
	nInx = 0;
	for (std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0) 
		{
			continue;
		}
		
		ptcOpense->arrInfo[nInx].byDay = (BYTE)it->nDay;
		ptcOpense->arrInfo[nInx].byFinish = (BYTE)(it->nFinish);
		ptcOpense->arrInfo[nInx].byGive = (BYTE)(it->nGive);
		ptcOpense->arrInfo[nInx].byId = (BYTE)(it->nId);
		ptcOpense->arrInfo[nInx].nCount = it->nCount;
		nInx++;
		if (it->nDay == ptcOpense->day)
		{
			if (it->nId == 1)
			{
				ptcOpense->one = it->nGive;
			}
			else
			{
				ptcOpense->two = it->nGive;
			}
		}
	}

	bOk = g_OpenseModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcOpense, sizeof(OPENSE_S2C_SYNC)+sizeof(OPENSE_INFO)* nInx);
	SendAllFinish();
EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

BOOL PlayerOpense::SendAllFinish()
{
	INT count = g_cOpenseSetting.GetOpenseAwardCount();
	if (m_vecSaveData.size() < count)
		return FALSE;
	for (std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nFinish == 0 || it->nGive == 0)
			return FALSE;
	}

	OPENSE_S2C_ALL_FINISH ptcFinish;
	ptcFinish.byProtocol = (BYTE)emOPENSE_PTC_S2C_ALL_FINISH;
	ptcFinish.nFinish = 1;

	return g_OpenseModule.GetProtocolProcessor()->SendData(m_rcPlayer.GetPlayerIndex(), ptcFinish);
}

OpenseSaveData* PlayerOpense::GetOpenseSaveData(INT nDay, INT nId)
{
	for (std::vector<OpenseSaveData>::iterator it = m_vecSaveData.begin(); it != m_vecSaveData.end(); ++it)
	{
		if (it->nDay == nDay && it->nId == nId)
		{
			return &(*it);
		}
	}
	return NULL;
}

LuaPlayerOpense* PlayerOpense::GetScriptInterface() CONST
{
	return m_pLunaObj;
}
