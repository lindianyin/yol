#include "stdafx.h"
#include "onlinegamebase/performancestat.h"
#include "onlinegameworld/krequireinterface.h"
#include "onlinegameworld/kcharacter.h"
#include "onlinegameworld/kluacharacter.h"
#include "onlinegameworld/koverloadextension.h"
#include "kplayerbaseprocess.h"
#include "onlinegameworld/synchelper.h"
#include "kgamecenterprocess.h"
#include "kenemycondition.h"
#include "onlinegameworld/knpc.h"
#include "onlinegameworld/kplayer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
VOID KTCharacterCtrlFactory::CreateCtrls(KCharacter& rcContainer, DWORD dwTypeMask)
{
	memset(m_aCtrl, 0, sizeof(m_aCtrl));

	for (INT i = 0; i < emKCHARACTER_CTRL_PLAYER_BEGIN; ++i)
	{
		if (m_saCtrlCtor[i])
			m_aCtrl[i] = static_cast<KCharacterCtrlBase*>(m_saCtrlCtor[i]->Create(rcContainer));
	}
	if (IS_PLAYER(dwTypeMask))
	{
		for (INT i = emKCHARACTER_CTRL_PLAYER_BEGIN; i < emKCHARACTER_CTRL_NPC_BEGIN; ++i)
		{
			if (m_saCtrlCtor[i])
				m_aCtrl[i] = static_cast<KCharacterCtrlBase*>(m_saCtrlCtor[i]->Create(rcContainer));
		}
	}
	else if (IS_NPC(dwTypeMask))
	{
		for (INT i = emKCHARACTER_CTRL_NPC_BEGIN; i < emKCHARACTER_CTRL_MAX; ++i)
		{
			if (m_saCtrlCtor[i])
				m_aCtrl[i] = static_cast<KCharacterCtrlBase*>(m_saCtrlCtor[i]->Create(rcContainer));
		}
	}
	else
	{
		ASSERT(FALSE);
	}
}
// -------------------------------------------------------------------------
KT_CHARACTER_SET_FUN_DW g_aCharacterSetFunDw[256] = { 0 };
KT_CHARACTER_SET_FUN_W g_aCharacterSetFunW[256] = { 0 };
KT_CHARACTER_SET_FUN_BY g_aCharacterSetFunBy[256] = { 0 };
// -------------------------------------------------------------------------
BOOL KCharacter::Init()
{
	KSceneObject::Init();
	m_pLuna->ClearTempTable();

	BOOL bRet = m_cContainer.Init();
	QCONFIRM_RET_FALSE(bRet);	
	bRet = m_cRoleValue.Init();
	QCONFIRM_RET_FALSE(bRet);

	bRet = m_cCtrlSet.InitCtrls();
	QCONFIRM_RET_FALSE(bRet);

	SetMoveSpeed(KD_BASE_RUN_SPEED, FALSE); // TODO

	m_bSystemShield = FALSE;
	m_nSelfAlarmRangeRevise = 0;
	m_nOtherAlarmRangeRevise = 0;
	m_nIgnoreOtherAlarmRange = 0;

	m_nOverThreatPercent = 120; // 默认OT百分比120%
	m_nThreatTime = GAME_FPS * 60;

	m_bSkillHidden = FALSE;

	return TRUE;
}

#ifdef GAME_SERVER
KCharacter::KCharacter(KCharacterServer& rCtrl, DWORD dwTypeMask, INT nRoleVarMax)
: m_cCharacterServer(rCtrl)
, m_bFistTimeLogin(FALSE)
#else
KCharacter::KCharacter(KCharacterClient& rCtrl, DWORD dwTypeMask, INT nRoleVarMax)
: m_cCharacterClient(rCtrl)
#endif
{
	m_pLuna = new KLuaCharacter(this);
	// 这时候KPlayer或KNpc还没构造，有的子控件要求强制downcast转换，所以挪到Npc和Player构造函数里
	//m_cCtrlSet.CreateCtrls(*this);
	const KENV_CONFIG* pConfig = g_piNsBaseRequire->GetEnvConfig();
	m_cContainer.Create(emKCHARACTER_TASK_BYTE_MAX,
		emKCHARACTER_TASK_WORD_MAX, emKCHARACTER_TASK_DWORD_MAX);
	m_cRoleValue.Create(nRoleVarMax);
}
KCharacter::~KCharacter()
{
	SAFE_DELETE(m_pLuna);
}

BOOL KCharacter::Uninit()
{
	m_cCtrlSet.UnInitCtrls();
	m_cContainer.UnInit();
	KSceneObject::Uninit();
	return TRUE;
}

BOOL KCharacter::Activate()
{
	if (!KSceneObject::Activate())
		return FALSE;  // 可能这帧已经激活过

	m_cCtrlSet.ActiveCtrls();

	if (!m_pScene)
		return FALSE;

	return TRUE;
}

INT KCharacter::GetVar( WORD wVarGroup, WORD wVarId ) const
{
	INT nValue;
	switch (wVarGroup)
	{
	case emKVARGROUP_CHARACTER_BYTE:
		nValue = (CHAR)m_cContainer.m_cByArrayValue.GetValue(wVarId);
		break;
	case emKVARGROUP_CHARACTER_WORD:
		nValue = (SHORT)m_cContainer.m_cWArrayValue.GetValue(wVarId);
		break;
	case emKVARGROUP_CHARACTER_DWORD:
		nValue = (INT)m_cContainer.m_cDwArrayValue.GetValue(wVarId);
		break;
	case emKVARGROUP_PLAYER:
	case emKVARGROUP_NPC:
		nValue = (INT)m_cRoleValue.GetValue(wVarId);
		break;
	case emKVARGROUP_CHARACTER_MAP:
	default:
		nValue = (INT)m_cContainer.GetMapValue(KD_VAR_KEY(wVarGroup, wVarId));
	}
	return nValue;
}

BOOL KCharacter::SetVar( WORD wVarGroup, WORD wVarId, INT nValue, BOOL bSync/* = TRUE*/ )
{
	// 目前只支持group和id均为BYTE
	QCONFIRM_RET_FALSE((wVarGroup & 0xff00) == 0);
	QCONFIRM_RET_FALSE((wVarId & 0xff00) == 0);
	INT nOldValue = GetVar(wVarGroup, wVarId);
	BOOL bRet = FALSE;
	switch (wVarGroup)
	{
	case emKVARGROUP_CHARACTER_BYTE:
		if (g_aCharacterSetFunBy[wVarId] && IS_PLAYER(GetId()))
		{
			// 调用重载Set函数 
			bRet = (*g_aCharacterSetFunBy[wVarId])
			(*this, m_cContainer.m_cByArrayValue.RefValue(wVarId), (BYTE)nValue);
		}
		else
		{
			bRet = m_cContainer.m_cByArrayValue.SetValue(wVarId, (BYTE)nValue);
		}
		break;
	case emKVARGROUP_CHARACTER_WORD:
		if (g_aCharacterSetFunW[wVarId] && IS_PLAYER(GetId()))
		{
			// 调用重载Set函数 
			bRet = (*g_aCharacterSetFunW[wVarId])
				(*this, m_cContainer.m_cWArrayValue.RefValue(wVarId), (WORD)nValue);
		}
		else 
		{
			bRet = m_cContainer.m_cWArrayValue.SetValue(wVarId, (WORD)nValue);
		}
		break;
	case emKVARGROUP_CHARACTER_DWORD:
		if (g_aCharacterSetFunDw[wVarId] && IS_PLAYER(GetId()))
		{
			// 调用重载Set函数 
			bRet = (*g_aCharacterSetFunDw[wVarId])
				(*this, m_cContainer.m_cDwArrayValue.RefValue(wVarId), (DWORD)nValue);
		}
		else
		{
			bRet = m_cContainer.m_cDwArrayValue.SetValue(wVarId, (DWORD)nValue);
		}
		break;
	case emKVARGROUP_PLAYER:
	case emKVARGROUP_NPC:
		bRet = m_cRoleValue.SetValue(wVarId, (DWORD)nValue);
		break;
	case emKVARGROUP_CHARACTER_MAP:
	default:
		bRet = m_cContainer.SetMapValue(KD_VAR_KEY(wVarGroup, wVarId), nValue);
	}

#ifdef GAME_SERVER
	if (bSync && bRet)
	{
		m_cCharacterServer.SyncVarChange(wVarGroup, wVarId);
		if (IS_PLAYER(m_dwId))
			g_cGameCenterProcess.OnPlayerVarChange((KPlayer&)*this, wVarGroup, wVarId, nValue);
	}
#endif

	if (bSync && bRet)
	{
		//printf("CHARACTER_ATTRIBUTE_CHANGED: 角色: %X, 组: %d, 变量: %d\n", m_dwId, wVarGroup, wVarId);
		FireEvent(emKOBJEVENTTYPE_CHARACTER_ATTRIBUTE_CHANGED, m_dwId, (DWORD)wVarGroup, (DWORD)wVarId);
	}
	return bRet;
}

BOOL KCharacter::AddVar( WORD wVarGroup, WORD wVarId, INT nAdd, BOOL bSync)
{
	INT nNewValue = GetVar(wVarGroup, wVarId) + nAdd;
	return SetVar(wVarGroup, wVarId, nNewValue, bSync);
}

BOOL KCharacter::AddVarU( WORD wVarGroup, WORD wVarId, INT nAdd, BOOL bSync)
{
	INT nNewValue = GetVar(wVarGroup, wVarId) + nAdd;
	return SetVar(wVarGroup, wVarId, nNewValue, bSync);
}

BOOL KCharacter::RawSetVar( WORD wVarGroup, WORD wVarId, INT nValue )
{
	// 目前只支持group和id均为BYTE
	QCONFIRM_RET_FALSE((wVarGroup & 0xff00) == 0);
	QCONFIRM_RET_FALSE((wVarId & 0xff00) == 0);
	BOOL bRet;
	switch (wVarGroup)
	{
	case emKVARGROUP_CHARACTER_BYTE:
		bRet = m_cContainer.m_cByArrayValue.SetValue(wVarId, (BYTE)nValue);
		break;
	case emKVARGROUP_CHARACTER_WORD:
		bRet = m_cContainer.m_cWArrayValue.SetValue(wVarId, (WORD)nValue);
		break;
	case emKVARGROUP_CHARACTER_DWORD:
		bRet = m_cContainer.m_cDwArrayValue.SetValue(wVarId, (DWORD)nValue);
		break;
	case emKVARGROUP_PLAYER:
	case emKVARGROUP_NPC:
		bRet = m_cRoleValue.SetValue(wVarId, (DWORD)nValue);
		break;
	case emKVARGROUP_CHARACTER_MAP:
	default:
		bRet = m_cContainer.SetMapValue(KD_VAR_KEY(wVarGroup, wVarId), nValue);
	}
	return bRet;
}

BOOL KCharacter::OnMoveStateChanged(KE_MOVE_STATE eMoveState)
{
	if (eMoveState != cmsOnStand)
	{//事件触发
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		cSafeScript->CallTableFunction("BufferCondition", "Move", 0, "d",
			GetId());
	}

	return TRUE;
}

BOOL KCharacter::SetMoveSpeed(INT nMoveSpeed, BOOL bSyncToClient/* = TRUE */)
{
    m_nMoveSpeed = nMoveSpeed;
	if (bSyncToClient)
	{
		SetSpeed(nMoveSpeed);
	}
	else
	{
		RawSetSpeed(nMoveSpeed);
	}

	return TRUE;
}

INT KCharacter::GetMoveSpeed() CONST
{
	//INT nDelaySetSpeed = GetSpeedDelaySet();
	//if (nDelaySetSpeed > 0)  // 为保证速度改变不会引起MoveAdjust，速度的改变由客户端发起，即服务端改变DelaySpeed，客户端确认并重发goto协议
	//{
 //       SetSpeed(nDelaySetSpeed);
	//	SetSpeedDelaySet(0);

	//	return nDelaySetSpeed;
	//}
	
	return GetSpeed();
}

BOOL KCharacter::OnReachTarget()
{
	INT nX = 0, nY = 0, nZ = 0;
	GetPosition(nX, nY, nZ);

	//停止运动需要修正朝向
	if (m_nStopedFaceDirection >= 0)
	{
		ChangeFaceDirection(m_nStopedFaceDirection);
		m_nStopedFaceDirection = -1;
	}

	//需要知道事件源，只需要传 nX, nY就可以了
	FireEvent(emKOBJEVENTTYPE_CHARACTER_REACH_TARGET, this->m_dwId, nX, nY);//, nZ);
	return TRUE;
}

BOOL KCharacter::ChangeFaceDirection( INT nFaceDirection, BOOL bSync)
{
	m_nFaceDirection = nFaceDirection;
	if (m_eMoveState == cmsOnStand)
		m_nDirectionXY = nFaceDirection;
#ifdef GAME_SERVER
	if (bSync)
	{
		g_cPlayerBaseProcess.DoSyncFaceDirection(this, nFaceDirection, TRUE);
	}
#endif // GAME_SERVER
	return TRUE;
}

#ifdef GAME_SERVER
INT KCharacter::GetNearByNpcInSector(INT nRange, INT nAngle, INT nCount, DWORD* pNpcList)
{
	IKScene* pScene = this->GetScene();
	TKTraverseHelper<KCharacter> cTravHelper;
	pScene->TraverseNearbyNpcs(cTravHelper, this);
	INT nIndex = 0;
	for (std::vector<KCharacter*>::iterator it = cTravHelper.m_vObjList.begin();
		it != cTravHelper.m_vObjList.end();
		++it)
	{
		KCharacter* pCharacter = *it;
		QCONFIRM_CONTINUE(pCharacter);

		if (!IS_NPC(pCharacter->m_dwId))
			continue;

		INT nX1 = 0, nY1 = 0, nZ1 = 0;
		INT nX2 = 0, nY2 = 0, nZ2 = 0;
		this->GetPosition(nX1, nY1, nZ1);
		pCharacter->GetPosition(nX2, nY2, nZ2);

		BOOL bResult = g_InRange(nX1, nY1, ZPOINT_TO_XYPOINT(nZ1),
			nX2, nY2, ZPOINT_TO_XYPOINT(nZ2),
			nRange);
		if(bResult)
		{
			INT nFaceTo = this->GetFaceDirection();
			INT nCurAngle = 0;
			if(nX1 != nX2 || nY1 != nY2)
			{
				INT nDirection = g_GetDirection(nX1, nY1, nX2, nY2);
				nCurAngle = abs(nDirection - nFaceTo);
				if (nCurAngle > DIRECTION_COUNT / 2)
				{
					nCurAngle = DIRECTION_COUNT - nCurAngle;
				}
			}
			
			if (nCurAngle <= nAngle)
			{
				pNpcList[nIndex] = pCharacter->GetId();
				nIndex++;
				if(nIndex >= nCount)
				{
					break;
				}
			}
		}
	}
	return nIndex;
}

BOOL KCharacter::SetRideState(BYTE bRide)
{
	SetIsRide(bRide);
	return TRUE;
}

BOOL KCharacter::GetRideState()
{
	return GetIsRide();
}

BOOL KCharacter::IsIgnoreDebuff(INT nBuffType)
{
	BOOL bIsIgnore = FALSE;
	if(IS_NPC(GetId()))
	{
		KNpc* pNpc = (KNpc*)this;
		bIsIgnore = pNpc->IsIgnoreDebuff(nBuffType);
	}
	else
	{
		bIsIgnore = (GetIgnorDebuff() == nBuffType);
	}

	return bIsIgnore;
}

INT KCharacter::GetNearByPlayerInSector(INT nRange, INT nAngle, INT nCount, DWORD* pdwPlayerList)
{
	IKScene* pScene = this->GetScene();
	TKTraverseHelper<KCharacter> cTravHelper;
	pScene->TraverseNearbyPlayers(cTravHelper, this);

	INT nIndex = 0;
	for (std::vector<KCharacter*>::iterator it = cTravHelper.m_vObjList.begin();
		it != cTravHelper.m_vObjList.end();
		++it)
	{
		KCharacter* pCharacter = *it;
		QCONFIRM_CONTINUE(pCharacter);

		if (!IS_PLAYER(pCharacter->m_dwId) || pCharacter->GetId() == this->GetId())
		{
			continue;
		}

		INT nX1 = 0, nY1 = 0, nZ1 = 0;
		INT nX2 = 0, nY2 = 0, nZ2 = 0;
		this->GetPosition(nX1, nY1, nZ1);
		pCharacter->GetPosition(nX2, nY2, nZ2);

		BOOL bResult = g_InRange(nX1, nY1, ZPOINT_TO_XYPOINT(nZ1),
			nX2, nY2, ZPOINT_TO_XYPOINT(nZ2),
			nRange);
		if(bResult)
		{
			INT nFaceTo = this->GetFaceDirection();
			INT nCurAngle = 0;
			if(nX1 != nX2 || nY1 != nY2)
			{
				INT nDirection = g_GetDirection(nX1, nY1, nX2, nY2);
				nCurAngle = abs(nDirection - nFaceTo);
				if (nCurAngle > DIRECTION_COUNT / 2)
				{
					nCurAngle = DIRECTION_COUNT - nCurAngle;
				}
			}

			if (nCurAngle <= nAngle)
			{
				pdwPlayerList[nIndex] = pCharacter->GetId();
				nIndex++;
				if(nIndex >= nCount)
				{
					break;
				}
			}
		}
	}
	return nIndex;
}
BOOL KCharacter::GetFrontPosition(INT nDistance, INT nDirection, INT &nX, INT &nY, INT &nZ)
{
	INT nCurX = 0, nCurY = 0, nCurZ = 0;
	this->GetPosition(nCurX, nCurY, nCurZ);

	nX = nCurX + nDistance * g_Cos(nDirection) / SIN_COS_NUMBER;
	nY = nCurY + nDistance * g_Sin(nDirection) / SIN_COS_NUMBER;
	nZ = nCurZ;

	return TRUE;
}
#endif

BOOL KCharacter::CanMove()
{
	BOOL bResult = FALSE;

	//PROCESS_ERROR(KSceneObject::CanMove());

	bResult = TRUE;
EXIT0:
	return bResult;
}

BOOL KCharacter::ModifyThreat(THREAT_TYPE eType, int nThreatValue)
{
	BOOL        bResult     = true;
	BOOL        bRetCode    = true;
	KCharacter* pSkillSrc   = NULL;

	//LOG_PROCESS_ERROR(m_pSkillAttributeParam);

	//pSkillSrc = m_pSkillAttributeParam->pSkillSrc;
	//PROCESS_ERROR(pSkillSrc);

	//switch (eType)
	//{
	//case thtBaseThreat:
	//	nThreatValue = nThreatValue * m_pSkillAttributeParam->nBaseThreatCoefficient / KILO_NUM;

	//	bRetCode = m_SimpThreatList.ModifyThreat(thtBaseThreat, pSkillSrc, nThreatValue);
	//	LOG_CHECK_ERROR(bRetCode);

	//	break;

	//case thtDamageThreat:
	//	nThreatValue = nThreatValue * m_pSkillAttributeParam->nDamageThreatCoefficient / KILO_NUM;

	//	bRetCode = m_SimpThreatList.ModifyThreat(thtDamageThreat, pSkillSrc, nThreatValue);
	//	LOG_CHECK_ERROR(bRetCode);

	//	break;

	//case thtTherapyThreat:
	//	nThreatValue = nThreatValue * m_pSkillAttributeParam->nTherapyThreatCoefficient / KILO_NUM;

	//	bRetCode = m_SimpThreatList.ModifyThreat(thtTherapyThreat, pSkillSrc, nThreatValue);
	//	LOG_CHECK_ERROR(bRetCode);

	//	break;

	//case thtMainThreat:

	//	bRetCode = m_SimpThreatList.ModifyThreat(thtMainThreat, pSkillSrc, nThreatValue);
	//	LOG_CHECK_ERROR(bRetCode);

	//	break;

	//case thtSpecialThreat:

	//	bRetCode = m_SimpThreatList.ModifyThreat(thtSpecialThreat, pSkillSrc, nThreatValue);
	//	LOG_CHECK_ERROR(bRetCode);

	//	break;

	//default:
	//	QLogPrintf(LOG_ERR, "[Skill] Modify threat, unexpected threat type(%d)\n", eType);
	//	goto EXIT0;
	//}

	bResult = true;
EXIT0:
	return bResult;
}

int KCharacter::AdjustThreat(THREAT_TYPE eTheatType, int nThreatValue)
{
	int     nResult     = 0;
	int     nRetThreat  = 0;

	switch (eTheatType)
	{
	case thtMainThreat:
	case thtSpecialThreat:
		nRetThreat = nThreatValue;
		break;

	//case thtBaseThreat:
	//	nRetThreat = nThreatValue * m_nPassiveBaseThreatCoefficient / KILO_NUM;
	//	break;

	//case thtDamageThreat:
	//	nRetThreat = nThreatValue * m_nPassiveDamageThreatCoefficient / KILO_NUM;
	//	break;

	//case thtTherapyThreat:
	//	nRetThreat = nThreatValue * m_nPassiveTherapyThreatCoefficient / KILO_NUM;
	//	break;

	default:
		QLogPrintf(LOG_ERR, "[Skill] Adjust threat, unexpected threat type(%d)\n", eTheatType);
		goto EXIT0;
	}

	nResult = nRetThreat;
EXIT0:
	return nResult;
}

BOOL KCharacter::SelectTarget(TARGET_TYPE eTargetType, DWORD dwTargetID)
{
	BOOL bResult    = false;
	BOOL bRetCode   = false;

	bRetCode = m_SelectTarget.SetTarget(eTargetType, dwTargetID);
	PROCESS_ERROR(bRetCode);

#ifdef GAME_SERVER
	//if (IS_PLAYER(m_dwID))
	//{
	//	KPlayer* pPlayer = (KPlayer*)this;
	//	g_PlayerServer.DoSetTarget(pPlayer, pPlayer);
	//}

	//g_PlayerServer.DoBroadcastTarget(this);
#else
	//g_PlayerClient.DoRequestTarget(eTargetType, dwTargetID);

	//g_pGameWorldUIHandler->OnUpdateSelectTarget();
#endif

	bResult = true;
EXIT0:
	return bResult;
}

BOOL KCharacter::Stand(BOOL bSyncSelf)
{
	BOOL bResult            = false;
	BOOL bRetCode           = false;
	int  nGradientDirection = 0;

	if (m_eMoveState != cmsOnSit && m_eMoveState != cmsOnWalk && m_eMoveState != cmsOnRun)
	{
		goto EXIT0;
	}

	StopMove(bSyncSelf);

	bResult = true;
EXIT0:
	return bResult;
}

// 判断自己是否对pCharacter隐身，有敌友关系判断
BOOL KCharacter::IsSkillHidden(KCharacter *pCharacter)
{
	/*if(!m_bSkillHidden)
		return FALSE;*/
	if (this == pCharacter)
		return FALSE;

	if(IS_NPC(pCharacter->GetId()) && !GetIsVisiontoNPC())
		return FALSE;
	if(IS_PLAYER(pCharacter->GetId()) && !GetIsVisiontoPlayer())
		return FALSE;

	//if(g_GetRelation(this, pCharacter) == sortEnemy)
	//	return TRUE;

	return TRUE;
}

FLOAT KCharacter::GetFactionAttrValue(INT nAttr)
{
	FLOAT fValue = 0;
	std::map<INT, FLOAT>::iterator it = m_mapFactionAttrValue.find(nAttr);
	if(it != m_mapFactionAttrValue.end())
	{
		fValue = it->second;
	}
	return fValue;
}

BOOL KCharacter::SetFactionAttrValue(INT nAttr, FLOAT fValue)
{
	m_mapFactionAttrValue[nAttr] = fValue;
	return TRUE;
}

BOOL KCharacter::ApplyNumericalAward(CONST QNumericalAward* pAward)
{
	INT nMaxLife = GetMaxLife() + pAward->nLife;
	SetMaxLife(nMaxLife);

	INT nMaxMana = GetMaxMana() + pAward->nAbility;
	SetMaxMana(nMaxMana);

	INT nAttack = GetAttack() + pAward->nAttack;
	SetAttack(nAttack);

	INT nDef = GetDef() + pAward->nDefense;
	SetDef(nDef);

	INT nHit = GetHitCoefficient() + pAward->nHit;
	SetHitCoefficient(nHit);

	INT nDodge = GetDodge() + pAward->nDodge;
	SetDodge(nDodge);

	INT nCrit = GetCriticalValue() + pAward->nCrit;
	SetCriticalValue(nCrit);

	INT nResist = GetResist() + pAward->nResist;
	SetResist(nResist);

	return TRUE;
}

BOOL KCharacter::RemoveNumericalAward(CONST QNumericalAward* pAward, BOOL bRawSet /* = FALSE */)
{
	INT nMaxLife = GetMaxLife() - pAward->nLife;
	QCONFIRM_RET_FALSE(nMaxLife >= 0);
	if(bRawSet)
		RawSetMaxLife(nMaxLife);
	else
		SetMaxLife(nMaxLife);

	INT nMaxMana = GetMaxMana() - pAward->nAbility;
	QCONFIRM_RET_FALSE(nMaxMana >= 0);
	if(bRawSet)
		RawSetMaxMana(nMaxMana);
	else
		SetMaxMana(nMaxMana);

	INT nAttack = GetAttack() - pAward->nAttack;
	QCONFIRM_RET_FALSE(nAttack >= 0);
	if(bRawSet)
		RawSetAttack(nAttack);
	else
		SetAttack(nAttack);

	INT nDef = GetDef() - pAward->nDefense;
	QCONFIRM_RET_FALSE(nDef >= 0);
	if(bRawSet)
		RawSetDef(nDef);
	else
		SetDef(nDef);

	INT nHit = GetHitCoefficient() - pAward->nHit;
	QCONFIRM_RET_FALSE(nHit >= 0);
	if(bRawSet)
		RawSetHitCoefficient(nHit);
	else
		SetHitCoefficient(nHit);

	INT nDodge = GetDodge() - pAward->nDodge;
	QCONFIRM_RET_FALSE(nDodge >= 0);
	if(bRawSet)
		RawSetDodge(nDodge);
	else
		SetDodge(nDodge);

	INT nCrit = GetCriticalValue() - pAward->nCrit;
	QCONFIRM_RET_FALSE(nCrit >= 0);
	if(bRawSet)
		RawSetCriticalValue(nCrit);
	else
		SetCriticalValue(nCrit);

	INT nResist = GetResist() - pAward->nResist;
	QCONFIRM_RET_FALSE(nResist >= 0);
	if(bRawSet)
		RawSetResist(nResist);
	else
		SetResist(nResist);

	return TRUE;
}

int g_GetRelation(KCharacter* pSrcCharacter, KCharacter* pDstCharacter)
{
	if (pSrcCharacter == pDstCharacter)
		return sortSelf;

	CHARACTER_RELATION_TYPE eRelation = sortAlly;  // 默认为友方
	INT nSrcCamp = pSrcCharacter->GetCurrentCamp();
	INT nDstCamp = pDstCharacter->GetCurrentCamp();

	if (nSrcCamp == emKCHARACTER_CAMP_SLAUGHTER ||  // 屠杀
		nDstCamp == emKCHARACTER_CAMP_SLAUGHTER)
	{
		eRelation = sortEnemy;
		goto EXIT0;
	}

	// 双方都是玩家，pk判断
	if(IS_PLAYER(pSrcCharacter->GetId()) && IS_PLAYER(pDstCharacter->GetId()))
	{
		if (nSrcCamp == emKCHARACTER_CAMP_PK && nDstCamp == emKCHARACTER_CAMP_PK)
		{
			eRelation = sortEnemy;
			goto EXIT0;
		}
		if (nSrcCamp == emKCHARACTER_CAMP_PK || nDstCamp == emKCHARACTER_CAMP_PK)
		{
			eRelation = sortAlly;
			goto EXIT0;
		}
	}

	if (nSrcCamp >= emKCHARACTER_CAMP_ACTIVITY1 || nDstCamp >= emKCHARACTER_CAMP_ACTIVITY1) // 两者之一属于 活动阵营， 使用活动阵营判断模式
	{
		if (nSrcCamp == nDstCamp)
		{
			eRelation = sortAlly; // 活动队友关系
		}
		else
		{
			eRelation = sortEnemy; // 活动敌人关系
		}
		goto EXIT0;
	}

	if (pSrcCharacter->GetEnableEnemyCondition() && pDstCharacter->GetEnableEnemyCondition())
	{
		if (QEnemyCondition::GetInstance().IsEnemy(pSrcCharacter, pDstCharacter))
		{
			eRelation = sortEnemy;
			goto EXIT0;
		}
	}

	// 双方都是玩家，队友判断
	if(IS_PLAYER(pSrcCharacter->GetId()) && IS_PLAYER(pDstCharacter->GetId()))
	{
		INT nTeamId1 = ((KPlayer*)pSrcCharacter)->GetTeamId();
		INT nTeamId2 = ((KPlayer*)pDstCharacter)->GetTeamId();
		if (nTeamId1 != 0 && nTeamId1 == nTeamId2)
		{
			eRelation = sortTeammate;
			goto EXIT0;
		}

		// 双方都是玩家阵营，友方
		if (nSrcCamp == emKCHARACTER_CAMP_PLAYER && 
			nDstCamp == emKCHARACTER_CAMP_PLAYER)
		{
			eRelation = sortAlly;
			goto EXIT0;
		}
	}

	if (nSrcCamp == emKCHARACTER_CAMP_NEUTRAL ||
		nDstCamp == emKCHARACTER_CAMP_NEUTRAL)
	{
		eRelation = sortNeutrality; // 中立阵营
		goto EXIT0;
	}

	if (nSrcCamp == emKCHARACTER_CAMP_PLAYER &&
		nDstCamp == emKCHARACTER_CAMP_FIELD)  // 玩家 + 野外
	{
		eRelation = sortEnemy;
		goto EXIT0;
	}
	if (nSrcCamp == emKCHARACTER_CAMP_FIELD &&
		nDstCamp == emKCHARACTER_CAMP_PLAYER)  // 野外 + 玩家
	{
		eRelation = sortEnemy;
		goto EXIT0;
	}

	if (nSrcCamp == emKCHARACTER_CAMP_PLAYER &&
		nDstCamp == emKCHARACTER_CAMP_BATTLECAMP1)  // 玩家 + 公共战场
	{
		eRelation = sortEnemy;
		goto EXIT0;
	}
	if (nSrcCamp == emKCHARACTER_CAMP_BATTLECAMP1 &&  // 公共战场 + 玩家
		nDstCamp == emKCHARACTER_CAMP_PLAYER)
	{
		eRelation = sortEnemy;
		goto EXIT0;
	}

	
EXIT0:
	return eRelation;
}
// -------------------------------------------------------------------------
