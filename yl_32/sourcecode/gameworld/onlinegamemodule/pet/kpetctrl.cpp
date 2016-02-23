/* -------------------------------------------------------------------------
//	文件名		：	kpetctrl.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-27
//	功能描述	：	宠物管理
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kpetctrl.h"
#include "kpetmanager.h"
#include "kpetprotocol.h"
#include "kpetmodule.h"

#include "item2/kitemmgr.h"
#include "item2/kitem.h"
#include "onlinegameworld/kplayer.h"
#include "onlinegamebase/ksysservice.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegamemodule/characterskill.h"
#include "onlinegameworld/synchelper.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "onlinegamemodule/chat/kchatmodule.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "../onlinegamemodule/bladestate/kbladectrl.h"

KPetCtrl::KPetCtrl(KCharacter& rcPlayer) : KCharacterCtrlBase(rcPlayer), m_rcPlayer(dynamic_cast<KPlayer&>(rcPlayer))
{
	
}

KPetCtrl::~KPetCtrl()
{
	
}

BOOL KPetCtrl::Init()
{
	// 刷新总次数
	m_nPetIslandRefreshCount = 0;

	// 初始化刷新宠物概率, 从lua读取默认值
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	g_cScriptManager.SetMe((QLunaBase*)m_rcPlayer.GetScriptInterface());
	cSafeScript->CallTableFunction("PetIsland", "GetDefaultRefreshRates", 6, "");

	for (INT rateId = 0; rateId < countof(m_arrPetRefreshRate); rateId++)
	{
		cSafeScript->GetTableIndex(1, rateId + 1);			// LUA的Table下标是从1开始的
		INT nIndex = cSafeScript->GetTopIndex();
		INT nRate = cSafeScript->GetInt(nIndex);
		m_arrPetRefreshRate[rateId] = nRate;
	}

	memset(&m_cLastPetIslandInfo, 0 ,sizeof(LastPetIslandInfo));
	m_cLastPetIslandInfo.wLastPetQuality = 1;
	m_cLastPetIslandInfo.wLastPetIndex = 0;

	return TRUE;
}

BOOL KPetCtrl::UnInit()
{
	m_arrPets.clear();

	memset(m_arrPetRefreshRate, 0, sizeof(m_arrPetRefreshRate));

	m_mapTactic.clear();
	return TRUE;
}
BOOL KPetCtrl::OnLogin()
{
	if (m_rcPlayer.IsFirstTimeLogin())
	{
		// 第一次登陆，同步刷新次数
		//this->SyncPetIslandRefreshState();
	}
	return TRUE;
}
BOOL KPetCtrl::OnLogout()
{
	for (PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		QCONFIRM_RET_FALSE(g_cPetMgr.RemovePet((*it)->GetId()));
	}

	return TRUE;
}

BOOL KPetCtrl::SavePetData(PetSaveData* pBuffer, KPet* pPet)
{
	QCONFIRM_RET_FALSE(pBuffer);
	QCONFIRM_RET_FALSE(pPet);

	pBuffer->wType = (WORD)pPet->m_nType;
	pBuffer->wQuality = (WORD)pPet->m_nQuality;
	pBuffer->wCharacter = (WORD)pPet->m_nCharacter;
	memcpy(pBuffer->szName, pPet->m_szPetName, sizeof(pPet->m_szPetName));
	pBuffer->byLevel = (BYTE)pPet->m_nLevel;
	pBuffer->byRankLevel = (BYTE)pPet->m_nRankLevel;
	//pBuffer->byGrade = (BYTE)pPet->m_nGrade;
	pBuffer->byState = (BYTE)pPet->m_byState;
	pBuffer->byLastState = (BYTE)pPet->m_nLastState;
	pBuffer->byHungry = pPet->m_byHungry;
	pBuffer->byEquipSlot = (BYTE)pPet->m_nEquipSlot;
	pBuffer->byTactic = (BYTE)pPet->m_nTactic;
	pBuffer->dwExp = (DWORD)pPet->m_dwExp;
	pBuffer->dwRandSeed = pPet->m_dwRandSeed;
	pBuffer->byRename = (BYTE)pPet->m_nRename;

	for (INT i = 0; i < emPET_COUNT_SKILL; i++)
	{
		INT nSkillId = pPet->GetSkillIdByIndex(i);
		INT nSkillLevel = pPet->GetSkillLevel(nSkillId);

		pBuffer->anSkillLevels[i] = MAKELONG(nSkillId, nSkillLevel);
	}

	for(INT i = 0; i < pPet->m_nEquipSlot; ++i)
	{
		pBuffer->awEquip[i].nParticular = (WORD)pPet->m_aryEquip[i].nParticular;
		pBuffer->awEquip[i].byLock = pPet->m_aryEquip[i].byLock;
		pBuffer->awEquip[i].wNumberIndex = pPet->m_aryEquip[i].wNumberIndex;
	}

	return TRUE;
}

BOOL KPetCtrl::MakePet(KPet* pPet, PetSaveData* pSaveData)
{
	QCONFIRM_RET_FALSE(pSaveData);
	QCONFIRM_RET_FALSE(pPet);
	memcpy(pPet->m_szPetName, pSaveData->szName, sizeof(pSaveData->szName));
	pPet->m_byState = pSaveData->byState;
	pPet->m_nLastState = pSaveData->byLastState;
	pPet->m_byHungry = pSaveData->byHungry;
	pPet->m_nEquipSlot = pSaveData->byEquipSlot;
	pPet->m_dwExp = pSaveData->dwExp;
	pPet->m_nTactic = pSaveData->byTactic;
	pPet->m_nRename = pSaveData->byRename;

	memset(pPet->m_aryEquip, 0, sizeof(pPet->m_aryEquip));
	for(INT i = 0; i < pPet->m_nEquipSlot; ++i)
	{
		pPet->m_aryEquip[i].nParticular = pSaveData->awEquip[i].nParticular;
		pPet->m_aryEquip[i].byLock = pSaveData->awEquip[i].byLock;
		pPet->m_aryEquip[i].wNumberIndex = pSaveData->awEquip[i].wNumberIndex;
	}

	for (INT i = 0; i < emPET_COUNT_SKILL; i++)
	{
		INT nSkillId = pSaveData->anSkillLevels[i] & 0x0000ffff;
		INT nSkillLevel = (pSaveData->anSkillLevels[i] >> 16) & 0x0000ffff;
        pPet->m_mapSkillLevels[nSkillId] = nSkillLevel;
	}

	return TRUE;
}

BOOL KPetCtrl::OnDbSave(LPBYTE pBuffer, SIZE_T uBuffSize, SIZE_T& uUsedSize)
{
	if (m_arrPets.size() * sizeof(PetSaveData) > uBuffSize)
	{
		QLogPrintf(LOG_ERR, "[KPetCtrl::OnDbSave]	The real size is more than uBuffSize");
		return FALSE;
	}
	// 保存刷新总次数
	INT* pPetRefreshCount = (INT *)pBuffer;
	*pPetRefreshCount = m_nPetIslandRefreshCount;
	pBuffer += sizeof(INT);
	uUsedSize += sizeof(INT);

	// 宠物刷新概率
	for (INT i = 0; i < countof(m_arrPetRefreshRate); i++)
	{
		INT* pPetRefreshRate = (INT *)pBuffer;
		*pPetRefreshRate = m_arrPetRefreshRate[i];

		pBuffer += sizeof(m_arrPetRefreshRate[0]);
		uUsedSize += sizeof(m_arrPetRefreshRate[0]);
	}

	// 上次南天门信息
	memcpy(pBuffer, &m_cLastPetIslandInfo, sizeof(LastPetIslandInfo));
	pBuffer += sizeof(LastPetIslandInfo);
	uUsedSize += sizeof(LastPetIslandInfo);


	// 宠物信息
	for (PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		KPet* pPet = *it;
		QCONFIRM_RET_FALSE(pPet);
		PetSaveData* pPetData = (PetSaveData*)pBuffer;
		SavePetData(pPetData, pPet);
		pBuffer += sizeof(PetSaveData);
		uUsedSize += sizeof(PetSaveData);
	}

	return TRUE;
}

BOOL KPetCtrl::OnDbLoad(LPCBYTE pData, SIZE_T uSize)
{
	if(m_rcPlayer.GetPetCapacity() < KD_MIN_PET_COUNT)
		m_rcPlayer.SetPetCapacity(KD_MIN_PET_COUNT);

	BOOL bRet = FALSE;
	if (uSize == 0)
		return TRUE;
	
	if (pData == NULL || uSize < (sizeof(INT) + sizeof(m_arrPetRefreshRate))) // 至少刷新次数和刷新概率
	{
		QLogPrintf(LOG_ERR, "[CharacterSkill::OnDbLoad]	The pData is NULL or the uSize is less than sizeof(m_arrPetRefreshRate)");
		return FALSE;
	}
	// 保存刷新总次数 读取
	INT* pPetRefreshCount = (INT *)pData;
	m_nPetIslandRefreshCount = *pPetRefreshCount;
	pData += sizeof(INT);

	// 宠物刷新概率 读取
	for (INT i = 0; i < countof(m_arrPetRefreshRate); i++)
	{
		INT* pSaveRate = (INT *)pData;
		m_arrPetRefreshRate[i] = *pSaveRate;

		pData += sizeof(INT);
		//uLoadPetSize += sizeof(INT);
	}

	// 上次召唤大神信息
	memcpy(&m_cLastPetIslandInfo, pData, sizeof(LastPetIslandInfo));
	pData += sizeof(LastPetIslandInfo);

	m_nPetIslandRefreshCount = m_cLastPetIslandInfo.wLastPetIndex;
	this->SyncPetIslandRefreshState(m_cLastPetIslandInfo.wLastPetQuality);  // 同步

	SIZE_T uLoadPetSize = sizeof(INT) + sizeof(m_arrPetRefreshRate) + sizeof(LastPetIslandInfo); // 读取宠物时，加上宠物刷新概率以及上次信息偏移

	while (uLoadPetSize < uSize)
	{
		if (uLoadPetSize + sizeof(PetSaveData) > uSize)
		{
			QLogPrintf(LOG_ERR, "[KPetCtrl::OnDbLoad]	The Size is more than uSize");
			return FALSE;
		}

		PetSaveData* pPetSaveData = (PetSaveData*)pData;
		KPET_INDEX sIndex = {pPetSaveData->wType, pPetSaveData->wQuality, pPetSaveData->wCharacter};
		INT nIndex = g_cPetMgr.AddPet();
		QCONFIRM_RET_FALSE(nIndex >= 0);

		KPet* pPet = g_cPetMgr.GetPetByIndex(nIndex);
		QCONFIRM_RET_FALSE(pPet);

		bRet = MakePet(pPet, pPetSaveData);
		QCONFIRM_RET_FALSE(bRet);

		bRet = pPet->Gen_Pet(sIndex, 
			pPetSaveData->byLevel, 
			pPetSaveData->byRankLevel,
			pPetSaveData->byHungry);
		QCONFIRM_RET_FALSE(bRet);

		pPet->CalcFightScore();

		bRet = Add(pPet, TRUE);
		QCONFIRM_RET_FALSE(bRet);

		if(pPet->GetPetState() == emPET_STATE_FIGHT)
		{
			g_cPetMgr.ContactPetWithOwner(pPet, m_rcPlayer.GetId());
		}

		// 把宠物放在阵位上
		if(pPet->GetTactic() != 0)
		{
			OnChangeTactic(pPet->GetTactic(), pPet);
		}

		pData += sizeof(PetSaveData);
		uLoadPetSize += sizeof(PetSaveData);
	}

	return TRUE;
}

BOOL KPetCtrl::TransExpToPet(KPet* pSrcPet, KPet* pTargetPet, INT nPercent)
{
	BOOL bResult = FALSE;
	DWORD dwTotalExp = 0;
	DWORD dwTransExp = 0;
	DWORD dwSkillBookNum = 0;
	DWORD dwUpRankBallNum = 0;
	INT   bRet = 0; 
	ScriptSafe cScript = g_cScriptManager.GetSafeScript();
	
	LOG_PROCESS_ERROR(pSrcPet);
	LOG_PROCESS_ERROR(pTargetPet);

	dwSkillBookNum = pSrcPet->GetTotalSkillBookNum();
	dwUpRankBallNum = pSrcPet->GetTotalUpRankBallNum();

	Q_Printl("..dwSkillBookNum="<<dwSkillBookNum<<" dwUpRankBallNum="<<dwUpRankBallNum);

	//先退还物品，然后再给经验
	cScript->CallTableFunction("Pet", "GiveBackItems", 1, "odd", m_rcPlayer.GetScriptInterface(), dwUpRankBallNum, dwSkillBookNum);
	bRet = cScript->GetInt(-1);
	LOG_PROCESS_ERROR(bRet == 1);
	
	dwTotalExp = pSrcPet->GetTotalExp(0);
	dwTransExp = dwTotalExp * nPercent / 100;
	
	pTargetPet->AddExp(dwTransExp);
	

	pSrcPet->ClearSkillLevel();
	pSrcPet->ClearExp();
	pSrcPet->Regenerate(pSrcPet->GetType(), pSrcPet->GetQuality(), pSrcPet->GetCharacter(), 1, 1, pSrcPet->GetRandSeed());
	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KPetCtrl::Add(KPet* pPet, BOOL bSync /* = FALSE */)
{
	QCONFIRM_RET_FALSE(pPet);

	QCONFIRM_RET_FALSE(m_rcPlayer.GetPetCapacity() > GetPetsCount());

	pPet->m_dwOwnerId = m_rcPlayer.GetId();
	m_arrPets.push_back(pPet);

	if(pPet->GetPetState() == emPET_STATE_FIGHT)
	{
		QCONFIRM_RET_FALSE(ApplyPetAttrib(pPet));

		m_rcPlayer.SetPetRepresentId(pPet->m_nType);
	}

	if(bSync)
	{
		SyncPetInfo(pPet);
	}

	return TRUE;
}

BOOL KPetCtrl::Insert(KPet* pPet, INT nPos)
{
	INT nIdx = 0;
	QCONFIRM_RET_FALSE(pPet);
	QCONFIRM_RET_FALSE(nPos >= 0);
	ASSERT(m_rcPlayer.GetPetCapacity() > GetPetsCount());

	pPet->m_dwOwnerId = m_rcPlayer.GetId();

	if(nPos == m_arrPets.size())
	{
		m_arrPets.push_back(pPet);
	}
	else
	{
		for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
		{
			if(nIdx++ == nPos)
			{
				m_arrPets.insert(it, pPet);
				break;
			}
		}
	}

	if(pPet->GetPetState() == emPET_STATE_FIGHT)
	{
		QCONFIRM_RET_FALSE(ApplyPetProb(pPet));
	}

	return TRUE;
}

INT KPetCtrl::GetPetPos(DWORD dwId)
{
	INT nPos = 0;
	KPet* pPet = NULL;
	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		pPet = *it;
		QCONFIRM_RET_FALSE(pPet);
		if(pPet->GetId() == dwId)
			return nPos;
		++nPos;
	}
	return -1;
}

KPet* KPetCtrl::GetPetByPos(INT nPos)
{
	QCONFIRM_RET_NULL(nPos >= 0 && nPos < (INT)m_arrPets.size());

	return m_arrPets[nPos];
}

BOOL KPetCtrl::RemoveOnly(INT nPos)
{
	INT nIdx = 0;
	KPet* pPet = NULL;
	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		pPet = *it;
		QCONFIRM_RET_FALSE(pPet);
		if(nIdx++ == nPos)
		{
			if(pPet->GetPetState() == emPET_STATE_FIGHT)
			{
				QCONFIRM_RET_FALSE(RemovePetProb(pPet));
			}

			m_arrPets.erase(it);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL KPetCtrl::SyncPetInfo(KPet* pPet)
{
	KS2C_SYNC_PET_INFO sSync;
	ZeroStruct(sSync);
	std::vector<INT> vecSkillIds;
	KBASEATTR TacticAttr;
	ZeroStruct(TacticAttr);

	sSync.byProtocol = (BYTE)s2c_sync_pet_info;
	// 生成同步宠物基本数据
	sSync.dwId = pPet->m_dwId;
	sSync.dwOwnerId = pPet->m_dwOwnerId;
	sSync.wType = (WORD)pPet->m_nType;
	sSync.byQuality = (BYTE)pPet->m_nQuality;
	sSync.byCharacter = (BYTE)pPet->m_nCharacter;
	strncpy(sSync.szName, pPet->m_szPetName, sizeof(pPet->m_szPetName));
	sSync.byLevel = (BYTE)pPet->m_nLevel;
	sSync.byRankLevel = (BYTE)pPet->m_nRankLevel;
	sSync.byState = (BYTE)pPet->m_byState;
	sSync.byHungry = pPet->m_byHungry;
	sSync.byEquipSlot = (BYTE)pPet->m_nEquipSlot;
	sSync.byTactic = (BYTE)pPet->m_nTactic;
	sSync.dwFightScore = pPet->m_nFightScore; 
	sSync.dwExp = pPet->m_dwExp;
	//sSync.dwRandSeed = pPet->m_dwRandSeed;
	sSync.nRenameCount = pPet->m_nRename;

	ZeroMemory(sSync.awSkill, sizeof(sSync.awSkill));
	pPet->GetSkillIds(vecSkillIds);
	for(INT i = 0; i < (INT)vecSkillIds.size(); ++i)
	{
		sSync.awSkill[i] = MAKELONG(vecSkillIds[i], pPet->GetSkillLevel(vecSkillIds[i]));
	}
	for(INT i = 0; i < (INT)vecSkillIds.size(); ++i)
	{
		sSync.wSkillValue1[i] = pPet->GetSkillValue(vecSkillIds[i], 0);
		sSync.wSkillValue2[i] = pPet->GetSkillValue(vecSkillIds[i], 1);
		sSync.wSkillValue3[i] = pPet->GetSkillValue(vecSkillIds[i], 2);

		sSync.wSkillValue1[i + (INT)vecSkillIds.size()] = pPet->GetSkillValue(vecSkillIds[i], 3);
		sSync.wSkillValue2[i + (INT)vecSkillIds.size()] = pPet->GetSkillValue(vecSkillIds[i], 4);
		sSync.wSkillValue3[i + (INT)vecSkillIds.size()] = pPet->GetSkillValue(vecSkillIds[i], 5);
	}

	for(INT i = 0; i < emPET_COUNT_BASE; ++i)
	{
		sSync.awAttrValue[i] = pPet->m_aryBaseAttr[i].nValue;
	}

	sSync.awTacticValue[0] = pPet->m_TacticAttr.nValue;
	//Q_Printl("..sSync.awTacticValue[0]=" << sSync.awTacticValue[0]);
	if(pPet->GetRankLevel() >= pPet->GetMaxRank())
	{
		sSync.awTacticValue[1] = 0;
	}
	else
	{
		pPet->Get_TacticAttrib(pPet->GetQuality(), pPet->GetRankLevel() + 1, TacticAttr);
		sSync.awTacticValue[1] = TacticAttr.nValue;
	}
	
	for(INT i = 0; i < pPet->m_nEquipSlot; ++i)
	{
		sSync.awEquip[i] = pPet->m_aryEquip[i].nParticular;
		sSync.abyEquipLockFlags[i] = pPet->m_aryEquip[i].byLock;
		sSync.wbyNumberIndex[i] = pPet->m_aryEquip[i].wNumberIndex;
	}
	//Q_Printl("..sSync.awTacticValue[1]=" << sSync.awTacticValue[1]);
	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPetCtrl::SetPetFightState(KPet* pPet, INT nState)
{
	QCONFIRM_RET_FALSE(nState >= emPET_STATE_REST && nState <= emPET_STATE_FIGHT);

	if (pPet->m_byState == nState)
		return FALSE;

	if(nState == emPET_STATE_FIGHT)
	{
		for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
		{
			if((*it)->m_byState != emPET_STATE_FIGHT)
				continue;
			// 去除宠物属性效果
			QCONFIRM_RET_FALSE(RemovePetProb(*it));

			(*it)->m_byState = emPET_STATE_REST;
			QCONFIRM_RET_FALSE(BroadcastPetState(*it));
			break;
		}
		// 宠物属性生效
		QCONFIRM_RET_FALSE(ApplyPetProb(pPet));

		// 以下2行顺序不能变
		g_cPetMgr.ContactPetWithOwner(pPet, m_rcPlayer.GetId());
		m_rcPlayer.SetPetRepresentId(pPet->m_nType);
	}
	else
	{
		QCONFIRM_RET_FALSE(RemovePetProb(pPet));

		m_rcPlayer.SetPetRepresentId(0);
	}

	pPet->m_byState = nState;
	QCONFIRM_RET_FALSE(BroadcastPetState(pPet));
	
	return TRUE;
}

VOID KPetCtrl::ReducePetHungry(KPet* pPet)
{
	INT nHungry = pPet->GetHungry();

	PROCESS_ERROR(pPet->GetPetState() == emPET_STATE_FIGHT);

	nHungry -= pPet->GetHungryConsume();
	nHungry = MAX(nHungry, 0);
	pPet->SetHungry(nHungry);

	if(pPet->GetHungry() <= 0)
	{
		// 强制休息
		SetPetFightState(pPet, emPET_STATE_REST);
	}

	SyncPetInfo(pPet);

EXIT0:
	return;
}

BOOL KPetCtrl::Active()
{
	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		(*it)->Breath();

		/*if (g_cOrpgWorld.m_dwLogicFrames % (GAME_FPS * KD_PET_ABRADE_SECOND) == 0)
		{
			ReducePetHungry(*it);
		}*/
	}

	return TRUE;
}

BOOL KPetCtrl::CanChangePetState(KPet* pPet, INT nState)
{
	if(pPet->GetHungry() <= 0 && nState == emPET_STATE_FIGHT)
	{
		g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, "你的宠物饱食度为0不能出战");
		return FALSE;
	}

	return TRUE;
}

BOOL KPetCtrl::OnChangeStateRequest(KPet* pPet, INT nState)
{
	BOOL bRet = FALSE;

	PROCESS_ERROR(CanChangePetState(pPet, nState));

	bRet = SetPetFightState(pPet, nState);
	
	pPet->CalcFightScore();

EXIT0:
	return bRet;
}

BOOL KPetCtrl::CalcPetAttrib(KPet* pPet, KMagicAttrib& sMagicAttib, INT nIdx)
{
	INT nValue = pPet->GetBaseAttrValue(nIdx);
	// 发挥宠物5%的基础属性
	nValue = nValue / 20;
	if(nValue < 1)
		nValue = 1;

	sMagicAttib.nAttribType = pPet->m_aryBaseAttr[nIdx].nAttrib;
	sMagicAttib.nValue[0] = nValue;
	sMagicAttib.nValue[1] = nValue;

	return TRUE;
}

BOOL KPetCtrl::ApplyPetAttrib(KPet* pPet)
{
	BOOL bRet = FALSE;
	KMagicAttrib sMagicAttib;

	for(INT i = 0; i < emPET_COUNT_BASE; ++i)
	{
		bRet = CalcPetAttrib(pPet, sMagicAttib, i);
		QCONFIRM_RET_FALSE(bRet);
		g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttib, FALSE);
	}
	return TRUE;
}

BOOL KPetCtrl::RemovePetAttrib(KPet* pPet)
{
	BOOL bRet = FALSE;
	KMagicAttrib sMagicAttib;

	for(INT i = 0; i < emPET_COUNT_BASE; ++i)
	{
		bRet = CalcPetAttrib(pPet, sMagicAttib, i);
		QCONFIRM_RET_FALSE(bRet);
		g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttib, TRUE);
	}
	return TRUE;
}

BOOL KPetCtrl::ApplyTacticAttr(KPet* pPet)
{
	QLogPrintf(LOG_INFO,"KPetCtrl::ApplyTacticAttr");
	// 更新下阵法加成属性
	pPet->Get_TacticAttrib(pPet->GetQuality(), pPet->GetRankLevel(), pPet->m_TacticAttr);

	KMagicAttrib sMagicAttrib;
	sMagicAttrib.nAttribType = pPet->m_TacticAttr.nAttrib;
	sMagicAttrib.nValue[0] = pPet->m_TacticAttr.nValue;
	sMagicAttrib.nValue[1] = pPet->m_TacticAttr.nValue;

	g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttrib, FALSE);

	g_cCharacterHelper.CalcFightScore(m_rcPlayer);

	return TRUE;
}

BOOL KPetCtrl::RemoveTacticAttr(KPet* pPet)
{
	KMagicAttrib sMagicAttrib;
	sMagicAttrib.nAttribType = pPet->m_TacticAttr.nAttrib;
	sMagicAttrib.nValue[0] = pPet->m_TacticAttr.nValue;
	sMagicAttrib.nValue[1] = pPet->m_TacticAttr.nValue;

	g_cMagicAttribMgr.ModifyAttrib(&m_rcPlayer, &m_rcPlayer, 0, sMagicAttrib, TRUE);

	return TRUE;
}

BOOL KPetCtrl::ApplyPetProb(KPet* pPet)
{
	/*
	BOOL bRet = ApplyPetAttrib(pPet);
	QCONFIRM_RET_FALSE(bRet);
	*/

	return TRUE;
}

BOOL KPetCtrl::RemovePetProb(KPet* pPet)
{
	/*
	BOOL bRet = RemovePetAttrib(pPet);
	QCONFIRM_RET_FALSE(bRet);
	*/

	return TRUE;
}

KPet* KPetCtrl::GetPetById(INT nId)
{
    for (INT i = 0; i < (INT)m_arrPets.size(); i++)
	{
        if (m_arrPets[i]->GetId() == (DWORD)nId)
		{
			return m_arrPets[i];
		}
	}
	return NULL;
}

KPet* KPetCtrl::GetPetByIdx(INT nIdx)
{
	QCONFIRM_RET_NULL(nIdx >= 0 && nIdx < (INT)m_arrPets.size());
	return m_arrPets[nIdx];
}

BOOL KPetCtrl::FreePet(DWORD dwId)
{
	KPet* pPet = NULL;
	BOOL bFind = FALSE;
	KS2C_SYNC_FREE_RESULT sSync;
	time_t tNow = KSysService::Time(NULL);

	INT MaxQuilty = 0;
	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		if((*it)->GetId() != dwId)
			continue;

		pPet = *it;
		QCONFIRM_RET_FALSE(pPet);
		if(pPet->GetPetState() == emPET_STATE_FIGHT)
		{
			QCONFIRM_RET_FALSE(RemovePetProb(pPet));
		}

		m_arrPets.erase(it);

		sSync.dwId = dwId;
		sSync.bResult = g_cPetMgr.RemovePet(dwId);
		bFind = TRUE;
		break;
	}

	QCONFIRM_RET_FALSE(bFind);

	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LEVEL, m_rcPlayer.GetId(), 0, tNow);
	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_FIGHTSCORE, m_rcPlayer.GetId(), 0, tNow);

	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LV_OS, m_rcPlayer.GetId(),0, tNow);//存这个排行榜特殊处理
	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPetCtrl::ChangeName(KPet* pPet, LPCTSTR szName)
{
	BOOL bRet = FALSE;

	if (pPet->m_nRename > 0)
	{
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rcPlayer.GetId(), "DEBUG", "每个宠物只能改名一次");
	}
	else
	{
		strncpy(pPet->m_szPetName, szName, sizeof(pPet->m_szPetName));
		pPet->m_nRename++;
		bRet = SyncPetInfo(pPet);
	}

	return bRet;
}

KPet* KPetCtrl::GetCurrentFightPet()
{
	KPet* pPet = NULL;
	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		if((*it)->GetPetState() == emPET_STATE_FIGHT)
		{
			pPet = *it;
			break;
		}
	}
	return pPet;
}

BOOL KPetCtrl::BroadcastPetEquipInfo(KPet* pPet)
{
	KS2C_SYNC_PET_EQUIP sSync;

	sSync.byProtocol = s2c_sync_pet_equip;
	sSync.dwId = pPet->GetId();
	for(INT i = 0; i < emPET_COUNT_EQUIP; ++i)
	{
		sSync.arywEquip[i] = pPet->GetEquip(i);
		sSync.abyEquipLockFlags[i] = pPet->IsEquipLocked((BYTE)i);
		sSync.wbyNumberIndex[i] = pPet->GetEquipNumberIndex((BYTE)i);
	}

	// broadcast
	KPlayerProtocolProcessor* pProtocolProcessor = g_cPetModule.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&sSync;
	cBroadcaster.m_uSize = sizeof(sSync);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcPlayer); // 向自己和自己周围玩家广播
}

BOOL KPetCtrl::BroadcastPetState(KPet* pPet)
{
	KS2C_SYNC_PET_STATE sSync;

	sSync.byProtocol = s2c_sync_pet_state;
	sSync.dwId = pPet->GetId();
	sSync.dwOwnerId = pPet->GetOwnerId();
	sSync.byState = pPet->GetPetState();

	// broadcast
	KPlayerProtocolProcessor* pProtocolProcessor = g_cPetModule.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&sSync;
	cBroadcaster.m_uSize = sizeof(sSync);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcPlayer); // 向自己和自己周围玩家广播
}

BOOL KPetCtrl::UpgradePetSkill(KPet* pPet, INT nSkillId)
{
	QCONFIRM_RET_FALSE(pPet);
	return pPet->UpgradeSkill(nSkillId);
}

BOOL KPetCtrl::BroadcastCastSkill(KPet* pPet, INT nSkillId, INT nTargetId, INT nMissileTime)
{
	KS2C_PET_CAST_SKILL sSync;

	sSync.byProtocol = s2c_pet_cast_skill;
	sSync.dwId = pPet->GetId();
	sSync.dwTargetId = (DWORD)nTargetId;
	sSync.wSkillId = (WORD)nSkillId;
	sSync.wMissileTime = (WORD)nMissileTime;

	// broadcast
	KPlayerProtocolProcessor* pProtocolProcessor = g_cPetModule.GetProtocolProcessor();
	QCONFIRM_RET_FALSE(pProtocolProcessor);

	KBroadcastFunc cBroadcaster;
	cBroadcaster.m_pvData = (LPCVOID)&sSync;
	cBroadcaster.m_uSize = sizeof(sSync);
	cBroadcaster.m_pProcessor = pProtocolProcessor;
	//cBroadcaster.m_nRange = 1; //by ldy 限制广播范围

	IKScene* pScene = m_rcPlayer.GetScene();
	QCONFIRM_RET_FALSE(pScene);

	return pScene->TraverseNearbyPlayers(cBroadcaster, &m_rcPlayer); // 向自己和自己周围玩家广播
}

BOOL KPetCtrl::SyncPetExp(KPet* pPet)
{
	KS2C_SYNC_PET_EXP sSync;

	sSync.dwId = pPet->GetId();
	sSync.dwExp = pPet->GetExp();

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

// 刷新次数和刷新成功率
BOOL KPetCtrl::SyncPetIslandRefreshState(INT nCurQuality)
{
	QS2C_PET_ISLAND_REFRESH_RATE sSync;
	
	sSync.nRefreshProgress = m_nPetIslandRefreshCount;
	sSync.nCurQuality = nCurQuality;

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPetCtrl::SyncPetCardInfo(std::vector<QPET_CARD>& vecCardInfos)
{
    QS2C_PET_CARDS sSync;
	int nIndex = 0;
	for (INT i = 0; i < (INT)vecCardInfos.size(); i++)
	{
		sSync.petInfos[nIndex++] = (USHORT)vecCardInfos[i].nPetId;
		sSync.petInfos[nIndex++] = (USHORT)vecCardInfos[i].nPetNature;
	}

	return g_cPetModule.SendData(m_rcPlayer.GetPlayerIndex(), sSync);
}

BOOL KPetCtrl::OnPutOnEquipRequest(KPet* pPet, DWORD dwItemId, BYTE byPos)
{
	BOOL bRet = FALSE;
	INT nFitPetType = 0;

	KPlayerItem* pPlayerItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	KItem* pItem = pPlayerItem->GetItemById(dwItemId);
	QCONFIRM_RET_FALSE(pItem);

	if(!KD_ISPETEQUIP(pItem->GetGenre(), pItem->GetDetailType()))
		return FALSE;

	nFitPetType = pItem->GetExtParam(6);
	KPET_INDEX sIndex = {nFitPetType, 1, 1};
	KPetTemplate* pPetTemplate = g_cPetSetting.GetPetTemplate(sIndex.nType);
	QCONFIRM_RET_FALSE(pPetTemplate);

	if(nFitPetType != pPet->GetType())
	{
		CHAR szTips[50];
		::sprintf(szTips, "这件装备只能穿在%s身上", pPetTemplate->m_szName);
		g_ChatChannelManager.ProcessRespond(trLocalSys, emKSYSTEMMSG_NORMAL, m_rcPlayer.GetId(), "DEBUG", szTips);
		return FALSE;
	}

	if(pPet->HasEquip(byPos))
	{
		return FALSE;
	}
	if (pPet->IsEquipLocked(byPos))
	{
		g_cPlayerBaseProcess.DoSyncBlackMsg(&m_rcPlayer, "装备绑定的槽位未解锁！");
		return FALSE;
	}

	INT nParticular = pItem->GetParticular();
	INT nNumberIndex = pItem->GetExtParam(4);

	// 扣除宠物装备
	bRet = pPlayerItem->Remove(pItem->GetGameIndex(), emKLOSEITEM_PUTON_PET_EQUIP);
	QCONFIRM_RET_FALSE(bRet);

	bRet = g_cItemMgr.Remove(pItem->GetGameIndex());
	QCONFIRM_RET_FALSE(bRet);

	bRet = pPet->PutOnEquip(byPos, nParticular, nNumberIndex);
	QCONFIRM_RET_FALSE(bRet);

	return BroadcastPetEquipInfo(pPet);
}

BOOL KPetCtrl::OnTakeOffEquipRequest(KPet* pPet, BYTE byPos)
{
	BOOL bRet = FALSE;

	KPlayerItem* pPlayerItem = m_rcPlayer.GetCtrl<KPlayerItem>();
	QCONFIRM_RET_FALSE(pPlayerItem);

	if(!pPet->HasEquip(byPos)) return FALSE;

	INT nParticular = pPet->GetEquip(byPos);
	QCONFIRM_RET_FALSE(nParticular > 0);

	INT nItemIdx = g_cItemMgr.Add(item_pet, item_pet_equip, nParticular, 0, 0, emITEM_DATARECORD_SYSTEMADD);
	if(nItemIdx < 0)
		return FALSE;

	if (!pPlayerItem->AddItemIntoBag(nItemIdx, emKADDITEM_PET_EQUIP, FALSE))
		return FALSE;

	QCONFIRM_RET_FALSE(pPet->TakeOffEquip(byPos));

	return BroadcastPetEquipInfo(pPet);
}

BOOL KPetCtrl::OnChangeTactic(INT nCharacter, KPet* pPet)
{
	ASSERT(pPet->GetOwnerId() == m_rcPlayer.GetId());
	ASSERT(nCharacter >= emPET_CHARACTER_LIFE && nCharacter <= emPET_CHARACTER_DODGE);

	KBladeCtrl* pBladeCtrl = m_rcPlayer.GetCtrl<KBladeCtrl>();
	QCONFIRM_RET_FALSE(pBladeCtrl);

	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		pBladeCtrl->RemoveBladeAttr();
	}

	PET_TACTIC_MAP::iterator it = m_mapTactic.find(nCharacter);
	if(it != m_mapTactic.end())
	{
		KPet* pOldPet = it->second;
		QCONFIRM_RET_FALSE(pOldPet);
		RemoveTacticAttr(pOldPet);
		
		pOldPet->SetTactic(0);
	}

	m_mapTactic[nCharacter] = pPet;
	ApplyTacticAttr(pPet);
	pPet->SetTactic(nCharacter);

	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		pBladeCtrl->ApplyBladeAttr();
	}

	this->SyncPetInfo(pPet);

	return TRUE;
}

KPet* KPetCtrl::GetPetByTactic(INT nCharacter)
{
	KPet* pPet = NULL;
	ASSERT(nCharacter >= emPET_CHARACTER_LIFE && nCharacter <= emPET_CHARACTER_DODGE);

	PET_TACTIC_MAP::iterator it = m_mapTactic.find(nCharacter);
	if(it != m_mapTactic.end())
	{
		pPet = it->second;
	}

	return pPet;
}

BOOL KPetCtrl::RemoveFromTactic(INT nCharacter)
{
	QLogPrintf(LOG_INFO,"KPetCtrl::RemoveFromTactic");
	ASSERT(nCharacter >= emPET_CHARACTER_LIFE && nCharacter <= emPET_CHARACTER_DODGE);

	KBladeCtrl* pBladeCtrl = m_rcPlayer.GetCtrl<KBladeCtrl>();
	QCONFIRM_RET_FALSE(pBladeCtrl);

	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		pBladeCtrl->RemoveBladeAttr();
	}

	PET_TACTIC_MAP::iterator it = m_mapTactic.find(nCharacter);
	if(it != m_mapTactic.end())
	{
		KPet* pOldPet = it->second;
		QCONFIRM_RET_FALSE(pOldPet);
		RemoveTacticAttr(pOldPet);
		pOldPet->SetTactic(0);
		this->SyncPetInfo(pOldPet);

		m_mapTactic.erase(it);
	}

	if(m_rcPlayer.GetAngerLeftTime() > 0)
	{
		pBladeCtrl->ApplyBladeAttr();
	}

	g_cCharacterHelper.CalcFightScore(m_rcPlayer);

	return TRUE;
}

VOID KPetCtrl::BindPetTarget(DWORD dwPetId, DWORD dwTargetId, INT nPetX, INT nPetY)
{
	KPet* pPet = GetCurrentFightPet();
	PROCESS_ERROR(pPet);

	LOG_PROCESS_ERROR(dwPetId == pPet->GetId());

	pPet->SetSkillTargetId(dwTargetId);
	pPet->SetPosition(nPetX, nPetY);

EXIT0:
	return;
}

BOOL KPetCtrl::SavePetDelayData(KDelayPetData* pAryDelayData)
{
	INT nIdx = 0;
	KPet* pPet = NULL;
	KDelayPetData* pData = NULL;
	std::vector<INT> vecSkillIds;
	std::vector<KPet*> vecPets;
	time_t tNow = KSysService::Time(NULL);
	INT nMaxLevel = 0;
	INT nMaxFightScore = 0;

	INT MaxQuilty = 0;
	pPet = GetCurrentFightPet();
	if(pPet)
	{
		vecPets.push_back(pPet);
	}

	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		pPet = *it;
		if(pPet->GetPetState() != emPET_STATE_FIGHT && pPet->GetTactic() > 0)
		{
			vecPets.push_back(pPet);
		}
	}

	for(PET_ARRAY::iterator it = vecPets.begin(); it != vecPets.end(); ++it)
	{
		pPet = *it;

		pData = &(pAryDelayData[nIdx]);

		pData->wType = (WORD)pPet->GetType();
		pData->byLevel = (BYTE)pPet->GetLevel();
		pData->nQuality = pPet->GetQuality();
		
		if(pPet->GetLevel() > nMaxLevel)
		{
			nMaxLevel = pPet->GetLevel();
		}


		memcpy(pData->szPetName, pPet->m_szPetName, MAX_NAME_LEN);

		pData->byCharacter = (BYTE)pPet->GetCharacter();
		pData->byRankLevel = (BYTE)pPet->GetRankLevel();

		pData->nFightScore = pPet->GetFightScore();
		if(pPet->GetFightScore() > nMaxFightScore)
		{
			nMaxFightScore = pPet->GetFightScore();
		}

		pData->byState = (BYTE)pPet->GetPetState();

		pData->nMaxLife = pPet->m_aryBaseAttr[emPET_ATTR_LIFE].nValue;
		pData->nAttack = pPet->m_aryBaseAttr[emPET_ATTR_ATTACK].nValue;
		pData->nHit = pPet->m_aryBaseAttr[emPET_ATTR_HIT].nValue;
		pData->nDuck = pPet->m_aryBaseAttr[emPET_ATTR_DODGE].nValue;
		pData->nDef = pPet->m_aryBaseAttr[emPET_ATTR_DEFENSE].nValue;
		pData->nCritical = pPet->m_aryBaseAttr[emPET_ATTR_CRIT].nValue;
		pData->nResist = pPet->m_aryBaseAttr[emPET_ATTR_RESIST].nValue;

		memset(pData->arySkillIds, 0, sizeof(pData->arySkillIds));
		pPet->GetSkillIds(vecSkillIds);
		for(INT i = 0; i < (INT)vecSkillIds.size(); ++i)
		{
			pData->arySkillIds[i] = vecSkillIds[i];
			pData->arwSkillLevels[i] = pPet->GetSkillLevel(vecSkillIds[i]);
		}

		for(INT i = 0; i < 2; ++i)
		{
			pData->arySkillValues[i * 6 + 0] = pPet->GetSkillValue(vecSkillIds[i], 0);
			pData->arySkillValues[i * 6 + 1] = pPet->GetSkillValue(vecSkillIds[i], 1);
			pData->arySkillValues[i * 6 + 2] = pPet->GetSkillValue(vecSkillIds[i], 2);

			pData->arySkillValues[i * 6 + 3] = pPet->GetSkillValue(vecSkillIds[i], 3);
			pData->arySkillValues[i * 6 + 4] = pPet->GetSkillValue(vecSkillIds[i], 4);
			pData->arySkillValues[i * 6 + 5] = pPet->GetSkillValue(vecSkillIds[i], 5);
		}

		for(INT i = 0; i < 7; ++i)
		{
			pData->aryEquips[i] = pPet->GetEquip(i);
			pData->aryEquipLocks[i] = pPet->IsEquipLocked(i);
		}

		if (pPet->GetQuality()*100+pPet->GetLevel() > MaxQuilty)
		{
			MaxQuilty = pPet->GetQuality()*100+pPet->GetLevel();
		}
		++nIdx;

		// 最多显示7个
		if(nIdx >= 7)
			break;
	}

	// 修改神仙排行榜
	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LEVEL, m_rcPlayer.GetId(), nMaxLevel, tNow);
	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_FIGHTSCORE, m_rcPlayer.GetId(), nMaxFightScore, tNow);
	//zzf
	RankRelayClient::DoModifyRankValueRequest(emRANK_PET_LV_OS, m_rcPlayer.GetId(),MaxQuilty, tNow);//存这个排行榜特殊处理

	return TRUE;
}

BOOL KPetCtrl::RecordPetsLastState()
{
	KPet* pPet = NULL;

	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		pPet = *it;
		pPet->SetLastState(pPet->GetPetState());
	}

	return TRUE;
}

BOOL KPetCtrl::ResumePetsLastState()
{
	KPet* pPet = NULL;

	for(PET_ARRAY::iterator it = m_arrPets.begin(); it != m_arrPets.end(); ++it)
	{
		pPet = *it;
		if(pPet->GetLastState() == emPET_STATE_FIGHT && pPet->GetPetState() == emPET_STATE_REST)
		{
			OnChangeStateRequest(pPet, emPET_STATE_FIGHT);
		}
	}

	return TRUE;
}

INT KPetCtrl::GetMaxPower()
{
	std::vector<KPet*>::iterator iter = m_arrPets.begin();
	INT maxPower = 0;
	for (iter = m_arrPets.begin(); iter != m_arrPets.end(); ++iter)
	{
		if (maxPower < (*iter)->GetFightScore())
		{
			maxPower = (*iter)->GetFightScore();
		}
	}
	return maxPower;
}