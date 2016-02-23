#include "stdafx.h"
#include "playergem.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegamemodule/characterbuffer.h"
#include "gemsetting.h"
#include "gemmodule.h"
#include "gemprotocol.h"
#include "award/kawardmanager.h"
#include "award/awarddef.h"
#include "item2/kitemmgr.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "onlinegamemodule/trade/kplayerpurse.h"
#include "../gameworld/nsworld/kcharacterhelper.h"


PlayerGem::PlayerGem(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	return;
}

PlayerGem::~PlayerGem()
{

}

BOOL PlayerGem::Init()
{
	m_pLunaObj = new LuaPlayerGem(m_rcCharacter);
	m_mapSaveData.clear();
	return TRUE;
}

BOOL PlayerGem::UnInit()
{
	SAFE_DELETE(m_pLunaObj);
	return TRUE;
}

BOOL PlayerGem::Active()
{
	// Ã¿Ãë¼ì²âÒ»´Î
	if(g_cOrpgWorld.m_dwLogicFrames % GAME_FPS != 0)
		return TRUE;

	return TRUE;
}


BOOL PlayerGem::OnLogin()
{
	return TRUE;
}

BOOL PlayerGem::OnLogout()
{
	return TRUE;
}


BOOL PlayerGem::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
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

	while(uUsedSize > 0)
	{
		KITEM_INDEX sItem;
		memcpy(&sItem, pBuffer, sizeof(KITEM_INDEX));
		pBuffer += sizeof(KITEM_INDEX);
		uUsedSize -= sizeof(KITEM_INDEX);

		GemSaveData sSavedata;
		memcpy(&sSavedata, pBuffer, sizeof(GemSaveData));
		pBuffer += sizeof(GemSaveData);
		uUsedSize -= sizeof(GemSaveData);

		m_mapSaveData[sItem] = sSavedata;
		AddAttribute(sItem, sSavedata.nCount);
	}

	SyncGemInfo();

EXIT0:
	return TRUE;
}

BOOL PlayerGem::OnDbSave(LPBYTE pData, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	LPBYTE pBuffer = pData;
	LOG_PROCESS_ERROR(pBuffer);

	for (std::map<KITEM_INDEX, GemSaveData>::iterator it = m_mapSaveData.begin(); it != m_mapSaveData.end(); ++it)
	{
		memcpy(pBuffer, &(it->first), sizeof(KITEM_INDEX));
		pBuffer += sizeof(KITEM_INDEX);
		uUsedSize += sizeof(KITEM_INDEX);

		memcpy(pBuffer, &(it->second), sizeof(GemSaveData));
		pBuffer += sizeof(GemSaveData);
		uUsedSize += sizeof(GemSaveData);
	}


EXIT0:
	return TRUE;
}

BOOL PlayerGem::OnUse(INT nGenre,INT nDetailType, INT nParticular, INT nLevel)
{
	KITEM_INDEX sItem;
	sItem.nGenre = nGenre;
	sItem.nDetailType = nDetailType;
	sItem.nParticular = nParticular;
	sItem.nLevel = nLevel;
	std::map<KITEM_INDEX, GemSaveData>::iterator it = m_mapSaveData.find(sItem);
	if (it == m_mapSaveData.end())
	{
		GemSaveData sSaveData;
		sSaveData.nCount = 1;
		m_mapSaveData.insert(std::make_pair(sItem, sSaveData));
	}
	else
	{
		m_mapSaveData[sItem].nCount++;
	}
	AddAttribute(sItem, 1);

	SyncGemInfo();
	return TRUE;
}

BOOL PlayerGem::AddAttribute(KITEM_INDEX &sIndex, INT nCount)
{
	CONST KItemGenerator* pItemGen = g_cItemMgr.GetGenerator();
	CONST KLibOfBPT* pLibBPT = pItemGen->GetLibOfBPT();
	CONST KBASICPROP_BASEDATA* pBPTBase = (CONST KBASICPROP_BASEDATA*)pLibBPT->GetBasicProp(sIndex);
	if (!pBPTBase)
		return	FALSE;

	switch (pBPTBase->aryExtParam[0])
	{
	case 1:
		m_rcPlayer.AddAttack(pBPTBase->aryExtParam[1] * nCount);//¹¥»÷
		break;

	case 2:
		m_rcPlayer.AddDef(pBPTBase->aryExtParam[1] * nCount);//·ÀÓù
		break;

	case 3:
		m_rcPlayer.AddCriticalValue(pBPTBase->aryExtParam[1] * nCount);//±©»÷
		break;

	case 4:
		m_rcPlayer.AddResist(pBPTBase->aryExtParam[1] * nCount);//¿¹±©
		break;

	case 5:
		m_rcPlayer.AddDodge(pBPTBase->aryExtParam[1] * nCount);//ÉÁ±Ü
		break;

	case 6:
		m_rcPlayer.AddHitCoefficient(pBPTBase->aryExtParam[1] * nCount);//ÃüÖÐ
		break;

	case 7:
		m_rcPlayer.AddMaxLife(pBPTBase->aryExtParam[1] * nCount);//ÉúÃü
		break;

	case 8:
		m_rcPlayer.AddMaxMana(pBPTBase->aryExtParam[1] * nCount);//Ä§·¨
		break;

	default:
		break;
	}

	g_cCharacterHelper.CalcFightScore(m_rcPlayer);
	return TRUE;
}

BOOL PlayerGem::SyncGemInfo()
{
	IMemBlock* piPackage					= NULL;
	KS2C_SYNC_GEM_INFO* ptcGem				= NULL;
	INT nInx								= m_mapSaveData.size();
	BOOL bOk								= FALSE;
	piPackage = QCreateMemBlock(sizeof(KS2C_SYNC_GEM_INFO)+sizeof(KGemInfo) * nInx);
	ptcGem = (KS2C_SYNC_GEM_INFO*)piPackage->GetData();
	LOG_PROCESS_ERROR(ptcGem);

	ptcGem->byProtocol					= (BYTE)s2c_sync_gem_info;
	ptcGem->nCount						= nInx;

	nInx = 0;
	for (std::map<KITEM_INDEX, GemSaveData>::iterator it = m_mapSaveData.begin(); it != m_mapSaveData.end(); ++it)
	{
		ptcGem->arrInfo[nInx].nCount = it->second.nCount;
		ptcGem->arrInfo[nInx].nGener = it->first.nGenre;
		ptcGem->arrInfo[nInx].nDetail = it->first.nDetailType;
		ptcGem->arrInfo[nInx].nParticular = it->first.nParticular;
		ptcGem->arrInfo[nInx].nLevel = it->first.nLevel;
		nInx++;
	}
	
	bOk =  g_cGemModule.GetProtocolProcessor()->SendLenData(m_rcPlayer.GetPlayerIndex(), *ptcGem, sizeof(KS2C_SYNC_GEM_INFO)+sizeof(KGemInfo) * nInx);

EXIT0:
	SAFE_RELEASE(piPackage);
	return bOk;
}

LuaPlayerGem* PlayerGem::GetScriptInterface() CONST
{
	return m_pLunaObj;
}
