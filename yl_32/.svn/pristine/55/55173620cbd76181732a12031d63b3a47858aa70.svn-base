#include "stdafx.h"
#include "playerartifact.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "artifactsetting.h"
#include "artifactmodule.h"
#include "artifactprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "item2/kplayeritem.h"


PlayerArtifact::PlayerArtifact(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerArtifact::~PlayerArtifact()
{

}

BOOL PlayerArtifact::Init()
{
	m_pLunaObj = new LuaPlayerArtifact(m_rcCharacter);
	m_mapSaveData.clear();
	return TRUE;
}

BOOL PlayerArtifact::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	m_mapSaveData.clear();
	return TRUE;
}

BOOL PlayerArtifact::Active()
{
	// 每秒检测一次
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerArtifact::OnLogin()
{
	return TRUE;
}

BOOL PlayerArtifact::OnLogout()
{
	return TRUE;
}


BOOL PlayerArtifact::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	LPCBYTE pBuffer = pData;
	SIZE_T uUsedSize = uSize;
	if (uSize == 0)
		return TRUE;

	if (pData == NULL)
	{
		QLogPrintf(LOG_ERR, "[PlayerTitle::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(BufferSaveData)");
		return FALSE;
	}

	/*memcpy(&m_nSignedIn, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_dwLastSignedInTime, pBuffer, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize -= sizeof(DWORD);

	memcpy(&m_nLoginDays, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_nMaxLoginDays, pBuffer, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize -= sizeof(INT);

	memcpy(&m_dwLastLoginTime, pBuffer, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize -= sizeof(DWORD);
	*/
	while(uUsedSize > 0)
	{
		ArtifactSaveData sSaveData;
		memcpy(&sSaveData, pBuffer, sizeof(ArtifactSaveData));
		pBuffer += sizeof(ArtifactSaveData);
		uUsedSize -= sizeof(ArtifactSaveData);
		m_mapSaveData[sSaveData.nId] = sSaveData;

		AddAttributeOfPiece(sSaveData.nId, 1, sSaveData.nCount);
		AddAttributeOfLevel(sSaveData.nId, 1, sSaveData.nLevel);
	}

EXIT0:
	return TRUE;
}

BOOL PlayerArtifact::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	/*memcpy(pBuffer, &m_nSignedIn, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_dwLastSignedInTime, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);

	memcpy(pBuffer, &m_nLoginDays, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_nMaxLoginDays, sizeof(INT));
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	memcpy(pBuffer, &m_dwLastLoginTime, sizeof(DWORD));
	pBuffer += sizeof(DWORD);
	uUsedSize += sizeof(DWORD);
	*/

	for (std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.begin(); it != m_mapSaveData.end(); ++it)
	{
		memcpy(pBuffer, &(it->second), sizeof(ArtifactSaveData));

		pBuffer += sizeof(ArtifactSaveData);
		uUsedSize += sizeof(ArtifactSaveData);
	}


EXIT0:
	return TRUE;
}

LuaPlayerArtifact* PlayerArtifact::GetScriptInterface() CONST
{
	return m_pLunaObj;
}

BOOL PlayerArtifact::AddPiece(INT nId, INT nCount)
{
	std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.find(nId);
	if (it == m_mapSaveData.end())
	{
		ArtifactSaveData sSaveData;
		sSaveData.nId = nId;
		sSaveData.nCount = 0;
		sSaveData.nLevel = 0;
		sSaveData.nCanLevelUp = 0;
		m_mapSaveData[nId] = sSaveData;
	}
	else
	{
	}

	SyncArtifact();
	return TRUE;
}

BOOL PlayerArtifact::UsePiece(INT nId, INT nCount)
{
	QLogPrintf(LOG_INFO,"..PlayerArtifact::UsePiece(INT nId=%d, INT nCount=%d)",nId,nCount);
	CONST QNumericalAward *pNumericalAward = NULL;
	INT gdpl[4] = {18, 1, 550, 0};
	std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.find(nId);
	KPlayerItem* pItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	INT nItemCount = pItem->GetItemCountInBags(gdpl[0], gdpl[1], gdpl[2] + nId - 1);
	if (nItemCount < nCount)
		return FALSE;

	if (it == m_mapSaveData.end())
	{
		ArtifactSaveData sSaveData;
		sSaveData.nId = nId;
		sSaveData.nCount = 0;
		sSaveData.nLevel = 0;
		sSaveData.nCanLevelUp = 0;
		m_mapSaveData[nId] = sSaveData;

		it = m_mapSaveData.find(nId);
	}

	//满级或者需要先升级
	if (it->second.nLevel >= MAX_LEVEL || it->second.nCanLevelUp == 1)
		return FALSE;

	INT nMinCount = it->second.nCount + 1;
	INT nMinLevel = it->second.nLevel + 1;

	it->second.nCount += nCount;
	INT nUseCount = nCount;
	INT nTmpMinLevel = START_LEVEL_ID;
	INT nSum = 0;
	while(nTmpMinLevel < START_LEVEL_ID + MAX_LEVEL)
	{
		switch(nId)
		{
		case 1://金箍棒
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(JINGUBANG_MODULE_NAME, 1, nTmpMinLevel);
			break;
		case 2://后羿弓
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(HOUYIGONG_MODULE_NAME, 1, nTmpMinLevel);
			break;
		case 3://玉净瓶
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(YUJINPING_MODULE_NAME, 1, nTmpMinLevel);
			break;
		case 4://如来印
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(RULAIYIN_MODULE_NAME, 1, nTmpMinLevel);
			break;
		default:
			break;
		}
		LOG_PROCESS_ERROR(pNumericalAward);
		nSum += pNumericalAward->nCost;
		if (it->second.nCount >= nSum && it->second.nLevel + START_LEVEL_ID  == nTmpMinLevel)
		{
			//it->second.nLevel = nTmpMinLevel - START_LEVEL_ID + 1;
			it->second.nCanLevelUp = 1;
			break;
		}
		nTmpMinLevel++;
	}

	if (it->second.nCount > nSum)
	{
		nUseCount = MAX(0, (nCount - (it->second.nCount - nSum)));
		it->second.nCount = nSum;
	}

	pItem->ConsumeItemInBags(nUseCount, gdpl[0], gdpl[1], gdpl[2] + nId - 1);
	AddAttributeOfPiece(nId, nMinCount, it->second.nCount);

	//AddAttributeOfLevel(nId, nMinLevel, it->second.nLevel);
	SyncArtifact();
	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerArtifact::LevelUp(INT nId)
{
	QLogPrintf(LOG_INFO,"..PlayerArtifact::LevelUp(INT nId=%d)",nId);
    char szStr[1024];
	KPlayerPurse *pPlayerPurse = NULL;
	KE_PURSE_RESULT nResult = emKPURSE_S_SUCCESS;
	
	std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.find(nId);
	if (it == m_mapSaveData.end() || it->second.nLevel >= MAX_LEVEL || it->second.nCanLevelUp == 0)
		return FALSE;
	KPlayerItem* pItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	INT nItemCount = pItem->GetItemCountInBags(18, 1, 283);
	if (nItemCount <= 0)
		return FALSE;


	
	it->second.nCanLevelUp = 0;
	it->second.nLevel++;

	
	int nLevel = it->second.nLevel;
	int nCount = nLevel * (nLevel+1) / 2;
	QLogPrintf(LOG_INFO,"PlayerArtifact::LevelUp nCount=%d",nCount);


	

	pItem->ConsumeItemInBags(nCount, 18, 1, 283);
	AddAttributeOfLevel(nId, it->second.nLevel, it->second.nLevel);
	SyncArtifact();


	//统计点 ldy
	pPlayerPurse = m_rcPlayer.GetCtrl<KPlayerPurse>();
	LOG_PROCESS_ERROR(pPlayerPurse);
	ZeroMemory(szStr,sizeof(szStr));
	sprintf(szStr,"nLevel=%d",it->second.nLevel);
	pPlayerPurse->LogMoney(EM_MONEY_COIN,0,59,&nResult,1,szStr);

	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerArtifact::AddAttributeOfPiece(INT nId, INT nMinValue, INT nMaxValue)
{
	CONST QNumericalAward *pNumericalAward = NULL;
	INT nAddAttribute = 0;
	INT nTmpMinValue = nMinValue;

	switch(nId)
	{
	case 1://金箍棒
		while(nTmpMinValue <= nMaxValue)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(JINGUBANG_MODULE_NAME, 1, nTmpMinValue);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nAttack;
			nTmpMinValue++;
		}
		m_rcPlayer.AddAttack(nAddAttribute);
		break;
	case 2://后羿弓
		while(nTmpMinValue <= nMaxValue)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(HOUYIGONG_MODULE_NAME, 1, nTmpMinValue);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nCrit;
			nTmpMinValue++;
		}
		m_rcPlayer.AddCriticalValue(nAddAttribute);
		break;
	case 3://玉净瓶
		while(nTmpMinValue <= nMaxValue)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(YUJINPING_MODULE_NAME, 1, nTmpMinValue);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nLife;
			nTmpMinValue++;
		}
		m_rcPlayer.AddMaxLife(nAddAttribute);
		break;
	case 4://如来印
		while(nTmpMinValue <= nMaxValue)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(RULAIYIN_MODULE_NAME, 1, nTmpMinValue);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nDefense;
			nTmpMinValue++;
		}
		m_rcPlayer.AddDef(nAddAttribute);
		break;
	default:
		break;
	}

	g_cCharacterHelper.CalcFightScore(m_rcPlayer);
	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerArtifact::AddAttributeOfLevel(INT nId, INT nMinLevel, INT nMaxLevel)
{
	CONST QNumericalAward *pNumericalAward = NULL;
	INT nAddAttribute = 0;
	INT nTmpMinLevel = nMinLevel + START_LEVEL_ID - 1;

	switch(nId)
	{
	case 1://金箍棒
		while(nTmpMinLevel < nMaxLevel + START_LEVEL_ID)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(JINGUBANG_MODULE_NAME, 1, nTmpMinLevel);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nAttack;
			nTmpMinLevel++;
		}
		m_rcPlayer.AddAttack(nAddAttribute);
		break;
	case 2://后羿弓
		while(nTmpMinLevel < nMaxLevel + START_LEVEL_ID)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(HOUYIGONG_MODULE_NAME, 1, nTmpMinLevel);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nCrit;
			nTmpMinLevel++;
		}
		m_rcPlayer.AddCriticalValue(nAddAttribute);
		break;
	case 3://玉净瓶
		while(nTmpMinLevel < nMaxLevel + START_LEVEL_ID)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(YUJINPING_MODULE_NAME, 1, nTmpMinLevel);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nLife;
			nTmpMinLevel++;
		}
		m_rcPlayer.AddMaxLife(nAddAttribute);
		break;
	case 4://如来印
		while(nTmpMinLevel < nMaxLevel + START_LEVEL_ID)
		{
			pNumericalAward = g_cAwardSetting.RequestNumericalAward(RULAIYIN_MODULE_NAME, 1, nTmpMinLevel);
			LOG_PROCESS_ERROR(pNumericalAward);
			nAddAttribute += pNumericalAward->nDefense;
			nTmpMinLevel++;
		}
		m_rcPlayer.AddDef(nAddAttribute);
		break;
	default:
		break;
	}

	g_cCharacterHelper.CalcFightScore(m_rcPlayer);
	return TRUE;
EXIT0:
	return FALSE;
}

BOOL PlayerArtifact::SyncArtifact()
{
	QLogPrintf(LOG_INFO,"..PlayerArtifact::SyncArtifact()");
	IMemBlock* piPackage					= NULL;
	KS2C_SYNC_ARTIFACT_INFO* ptcArtifact	= NULL;
	INT nInx								= m_mapSaveData.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(KS2C_SYNC_ARTIFACT_INFO)+sizeof(KArtifactInfo) * nInx);
	ptcArtifact = (KS2C_SYNC_ARTIFACT_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcArtifact);

	ptcArtifact->byProtocol					= (BYTE)s2c_sync_artifact_info;
	ptcArtifact->nCount						= nInx;

	nInx = 0;
	for (std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.begin(); it != m_mapSaveData.end(); ++it)
	{
		ptcArtifact->arrInfo[nInx].nId = it->second.nId;
		ptcArtifact->arrInfo[nInx].nCount = it->second.nCount;
		ptcArtifact->arrInfo[nInx].nLevel = it->second.nLevel;
		ptcArtifact->arrInfo[nInx].nLevelUp = it->second.nCanLevelUp;
		nInx++;

	}

	bOk =  g_cArtifactModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcArtifact, sizeof(KS2C_SYNC_ARTIFACT_INFO)+sizeof(KArtifactInfo) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

INT PlayerArtifact::GetArtifactNumByLevel(INT nLevel)
{
	INT nCount = 0;
	for (std::map<INT, ArtifactSaveData>::iterator it = m_mapSaveData.begin(); it != m_mapSaveData.end(); ++it)
	{
		if (it->second.nLevel >= nLevel)
			nCount++;
	}
	return nCount;
}