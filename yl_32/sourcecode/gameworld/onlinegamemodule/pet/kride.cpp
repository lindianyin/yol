/* -------------------------------------------------------------------------
//	文件名		：	kride.cpp
//	创建者		：	zhaoyu
//	创建时间	：	2013-3-15
//	功能描述	：	坐骑
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kride.h"
#include "kridectrl.h"
#include "kluaride.h"

#include "onlinegameworld/kplayer.h"
#include "onlinegameworld/kplayermgr.h"
#include "../gameworld/nsworld/kcharacterhelper.h"
#include "award/kawardsetting.h"
#include "onlinegamebase/ksysservice.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"

KRide::KRide()
{
	m_pLuna = new KLuaRide(this);

	memset(&m_cCurrAddAttr, 0, sizeof(QRIDE_ADD_ATTR));
}

KRide::~KRide()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KRide::Init(KITEM_INDEX sIndex, INT nHungry, INT nMagicRate)
{
	KRideTemplate* pTemplate = g_cPetSetting.GetRideTemplate(sIndex);
	QCONFIRM_RET_FALSE(pTemplate);

	m_nGenre = sIndex.nGenre;
	m_nDetail = sIndex.nDetailType;
	m_nParticular = sIndex.nParticular;
	m_nLevel = sIndex.nLevel;

	m_nQuality = pTemplate->m_nQuality;
	m_nRankId = pTemplate->m_nRankId;
	m_nRepresentId = pTemplate->m_nRepresentId;

	QCONFIRM_RET_FALSE(nHungry >= 0 && nHungry <= KD_RIDE_HUNGRY_MAX);
	m_nHungry = nHungry;

	m_byState = emRIDE_STATE_REST;

	m_nMagicRate = nMagicRate;

	return TRUE;
}

BOOL KRide::Gen_Ride(KITEM_INDEX sIndex)
{
	KRideTemplate* pTemplate = g_cPetSetting.GetRideTemplate(sIndex);
	QCONFIRM_RET_FALSE(pTemplate);

	// 升阶数不能超过本坐骑的最大阶数
	QCONFIRM_RET_FALSE(sIndex.nLevel <= pTemplate->m_nMaxLevel);

	m_nGenre = sIndex.nGenre;
	m_nDetail = sIndex.nDetailType;
	m_nParticular = sIndex.nParticular;
	m_nLevel = sIndex.nLevel;

	m_nQuality = pTemplate->m_nQuality;
	m_nRankId = pTemplate->m_nRankId;
	m_nRepresentId = pTemplate->m_nRepresentId;

	// 幻化果清0
	m_nMagicRate = 0;

	return TRUE;
}

BOOL KRide::CopyRideData(CONST KRide* pRide)
{
	if (!pRide)
		return	FALSE;

	KLuaRide* pLuna = m_pLuna;
	*this = *pRide;
	m_pLuna	= pLuna;
	m_dwOwnerId = 0;
	return TRUE;
}

BOOL KRide::Regenerate(INT nGenre, INT nDetail, INT nParticular, INT nLevel)
{
	KRide cNewRide;
	if (!cNewRide.CopyRideData(this))
		return	FALSE;

	KITEM_INDEX sIndex = { nGenre, nDetail, nParticular, nLevel };

	if(!cNewRide.Gen_Ride(sIndex))
		return FALSE;

	INT nPos = -1;
	KPlayer* pOwner = m_dwOwnerId ? g_cPlayerMgr.GetById(m_dwOwnerId) : NULL;
	KRideCtrl* pRideCtrl = pOwner ? pOwner->GetCtrl<KRideCtrl>() : NULL;

	if (pRideCtrl)
	{
		if (!pRideCtrl->RemoveOnly())
			return FALSE;
	}

	KRide cOldRide;
	cOldRide.CopyRideData(this);	// 备份原坐骑
	CopyRideData(&cNewRide);		// 使用新坐骑

	if (pRideCtrl && !pRideCtrl->Add(this))		// 添加失败，恢复原坐骑
	{
		CopyRideData(&cOldRide);
		if(!pRideCtrl->Add(this))
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	if(pRideCtrl)
	{
		pRideCtrl->BroadcastRideLevel(this);
	}

	// 重新计算战斗力
	CalcRideFightScore();

	g_cCharacterHelper.CalcFightScore(*pOwner);

	return TRUE;
}

INT KRide::GetNextLevelFightScore()
{
	INT nFightScore = 0;

	CONST QNumericalAward* pNextAward = g_cAwardSetting.RequestNumericalAward("horse_level", 1, m_nLevel + 1);
	if (pNextAward == NULL)
		return 0;

	nFightScore += pNextAward->nAttack * g_cPetSetting.GetRideAttribScore(1);	
	nFightScore += pNextAward->nDefense * g_cPetSetting.GetRideAttribScore(2);	
	nFightScore += pNextAward->nLife * g_cPetSetting.GetRideAttribScore(3);	
	nFightScore += pNextAward->nHit * g_cPetSetting.GetRideAttribScore(4);	
	nFightScore += pNextAward->nAbility * g_cPetSetting.GetRideAttribScore(5);
	nFightScore += pNextAward->nDodge * g_cPetSetting.GetRideAttribScore(6);

EXIT0:
	return nFightScore / 100;
}

BOOL KRide::SetMagicRate(INT nRate)
{
	m_nMagicRate = nRate;

	CONST QNumericalAward* pCurrAward = g_cAwardSetting.RequestNumericalAward("horse_level", 1, m_nLevel);
	QCONFIRM_RET_FALSE(pCurrAward);

	CONST QNumericalAward* pNextAward = g_cAwardSetting.RequestNumericalAward("horse_level", 1, m_nLevel + 1);

	RemoveRideAddAttr();

	if (pNextAward != NULL)
	{
		INT nScale = (m_nMagicRate * 1000) / pNextAward->nCost;
		nScale = nScale > 1000 ? 1000 : nScale;

		INT nAddLife = pCurrAward->nLife + (nScale * (pNextAward->nLife - pCurrAward->nLife)) / 1000;
		INT nAddDef  = pCurrAward->nDefense + (nScale * (pNextAward->nDefense - pCurrAward->nDefense)) / 1000;
		INT nAddAttack  = pCurrAward->nAttack + (nScale * (pNextAward->nAttack - pCurrAward->nAttack)) / 1000;
		INT nAddSpeed = pCurrAward->nHit + (nScale * (pNextAward->nHit - pCurrAward->nHit)) / 1000;  // hit用作速度值
		INT nAddIgnoreHarm = pCurrAward->nAbility + (nScale * (pNextAward->nAbility - pCurrAward->nAbility)) / 1000;  // Ability用作免疫伤害值
		INT nAddIgnoreDef = pCurrAward->nDodge + (nScale * (pNextAward->nDodge - pCurrAward->nDodge)) / 1000;  // Dodge用作破防值

		m_cCurrAddAttr.nLife = nAddLife;
		m_cCurrAddAttr.nAttack = nAddAttack;
		m_cCurrAddAttr.nDefense = nAddDef;
		m_cCurrAddAttr.nSpeed = nAddSpeed;
		m_cCurrAddAttr.nIgnoreHarmRate = nAddIgnoreHarm;
		m_cCurrAddAttr.nIgnoreDefRate = nAddIgnoreDef;

	}
	else
	{
		m_cCurrAddAttr.nLife = pCurrAward->nLife;
		m_cCurrAddAttr.nAttack = pCurrAward->nAttack;
		m_cCurrAddAttr.nDefense = pCurrAward->nDefense;
		m_cCurrAddAttr.nSpeed = pCurrAward->nHit;
		m_cCurrAddAttr.nIgnoreHarmRate = pCurrAward->nAbility;
		m_cCurrAddAttr.nIgnoreDefRate = pCurrAward->nDodge;

	}

	ApplyRideAddAttr();

	return TRUE;
}

BOOL KRide::RemoveRideAddAttr()
{
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	QCONFIRM_RET_FALSE(pPlayer);

	INT nOldMaxLife = pPlayer->GetMaxLife();
	INT nOldDef = pPlayer->GetDef();
	INT nOldAttack = pPlayer->GetAttack();

	pPlayer->SetMaxLife(nOldMaxLife - m_cCurrAddAttr.nLife);
	pPlayer->SetDef(nOldDef - m_cCurrAddAttr.nDefense);
	pPlayer->SetAttack(nOldAttack - m_cCurrAddAttr.nAttack);
	pPlayer->SetIgnorDmg(0);
	pPlayer->SetIgnoreDef(0);
	pPlayer->SetSpeedDelaySet(KD_BASE_RUN_SPEED);

    memset(&m_cCurrAddAttr, 0, sizeof(QRIDE_ADD_ATTR));

	return TRUE;
}

BOOL KRide::ApplyRideAddAttr()
{
	QLogPrintf(LOG_INFO,"KRide::ApplyRideAddAttr()");
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	QCONFIRM_RET_FALSE(pPlayer);

	INT nOldMaxLife = pPlayer->GetMaxLife();
	INT nOldDef = pPlayer->GetDef();
	INT nOldAttack = pPlayer->GetAttack();

	pPlayer->SetMaxLife(nOldMaxLife + m_cCurrAddAttr.nLife);
	pPlayer->SetDef(nOldDef + m_cCurrAddAttr.nDefense);
	pPlayer->SetAttack(nOldAttack + m_cCurrAddAttr.nAttack);
	pPlayer->SetIgnorDmg(m_cCurrAddAttr.nIgnoreHarmRate);
	pPlayer->SetIgnoreDef(m_cCurrAddAttr.nIgnoreDefRate);

	if (GetState() == emRIDE_STATE_UP)
	{
		INT nNewSpeed = KD_BASE_RUN_SPEED + (KD_BASE_RUN_SPEED * m_cCurrAddAttr.nSpeed / 100);
		pPlayer->SetSpeedDelaySet(nNewSpeed);
	}

	// 重新计算战斗力
	CalcRideFightScore();

	g_cCharacterHelper.CalcFightScore(*pPlayer);

	return TRUE;
}

BOOL KRide::CalcRideFightScore()
{
	INT nFightScore = 0;
	KPlayer* pPlayer = g_cPlayerMgr.GetById(m_dwOwnerId);
	QCONFIRM_RET_FALSE(pPlayer);

    nFightScore += m_cCurrAddAttr.nAttack * g_cPetSetting.GetRideAttribScore(1);	
	nFightScore += m_cCurrAddAttr.nDefense * g_cPetSetting.GetRideAttribScore(2);	
	nFightScore += m_cCurrAddAttr.nLife * g_cPetSetting.GetRideAttribScore(3);	
	nFightScore += m_cCurrAddAttr.nSpeed * g_cPetSetting.GetRideAttribScore(4);	
	nFightScore += m_cCurrAddAttr.nIgnoreHarmRate * g_cPetSetting.GetRideAttribScore(5);
	nFightScore += m_cCurrAddAttr.nIgnoreDefRate * g_cPetSetting.GetRideAttribScore(6);

	SetRideFightScore(nFightScore / 100);

	// 修改排行榜排名
	time_t tNow = KSysService::Time(NULL);
	RankRelayClient::DoModifyRankValueRequest(emRANK_RIDE_FIGHTSCORE, pPlayer->GetId(), nFightScore / 100, tNow);
	CONST QNumericalAward* pNextAward = g_cAwardSetting.RequestNumericalAward("horse_level", 1, m_nLevel + 1);
	QCONFIRM_RET_FALSE(pNextAward);
	INT nScale = 0;
	//zzf
	int nCost = pNextAward->nCost;
	if(nCost == 0){
		nCost = 1;
	}
	nScale = (m_nMagicRate * 100) / (nCost);
	RankRelayClient::DoModifyRankValueRequest(emRANK_RIDE_LV, pPlayer->GetId(), (nFightScore/100*100) +nScale , tNow);
	return TRUE;
}