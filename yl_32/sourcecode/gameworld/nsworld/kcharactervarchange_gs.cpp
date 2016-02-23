
#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegameworld/kcharacter.h"
#include "kcharacterhelper.h"
#include "../onlinegamemodule/faction/factionmanager.h"
#include "../onlinegameworld/kplayerbaseprocess.h"
#include "../onlinegamemodule/pet/kpetctrl.h"
#include "../onlinegamemodule/pet/kpet.h"
#include "onlinegameworld/kplayer.h"

#include "onlinegamebase/ksysservice.h"
#include "../onlinegamemodule/rank/RankRelayClient.h"
#include "serverbase/RankDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL SetExp(KCharacter& rcCharacter, DWORD& rValue, DWORD nNewValue)
{
	//BOOL bLevelUp = FALSE;
	
	if (IS_PLAYER(rcCharacter.m_dwId))
	{
		/*KPlayer* pPlayer = (KPlayer*)&rcCharacter;
		KPetCtrl* pPetCtrl = pPlayer->GetCtrl<KPetCtrl>();
		QCONFIRM_RET_FALSE(pPetCtrl);
		// 给当前参战宠物20%经验
		KPet* pPet = pPetCtrl->GetCurrentFightPet();
		if(pPet)
		{
			pPet->AddExp((nNewValue - rValue) / 5);
		}*/
		// 传递客户端增加的经验
		INT addedExp = nNewValue - rValue;
		g_cPlayerBaseProcess.DoSyncHeadText((KPlayer *)&rcCharacter, rcCharacter.m_dwId, emHeadTextExp, addedExp);
	}

	rValue = nNewValue;

	INT nNextLevel = rcCharacter.GetLevel() + 1;
	//by ldy 限制60等级
	if(nNextLevel >= 61)
	{
		return TRUE;
	}
	DWORD nLvUpNeedExp = g_cCharacterHelper.GetNextLvExp(nNextLevel);
	while(rValue >= nLvUpNeedExp/* && rcCharacter.GetLevel()<=(60-1)*/)
	{
		rcCharacter.SetLevel(nNextLevel);
		rValue -= nLvUpNeedExp;
		nNextLevel = rcCharacter.GetLevel() + 1;
		nLvUpNeedExp = g_cCharacterHelper.GetNextLvExp(nNextLevel);

		time_t tNow = KSysService::Time(NULL);
		RankRelayClient::DoModifyRankValueRequest(emRANK_ROLE_LEVEL_OPEN_SERVER, rcCharacter.GetId(), rcCharacter.GetLevel(), tNow);
		//bLevelUp = TRUE;
	}

	/*if(bLevelUp)
	{
		g_cCharacterHelper.CalcFightScore(rcCharacter);
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("Player", "OnLevelUp", 0, "od", rcCharacter.GetScriptInterface());
	}*/

	return TRUE;
}

BOOL SetMaxLife(KCharacter& rcCharacter, DWORD& rValue, DWORD nNewValue)
{
	DWORD dwOld = rValue;
	rValue = nNewValue;
	BOOL bRet = g_cFactionManager.SetMaxLife(rcCharacter, dwOld, nNewValue);
	if (rcCharacter.GetIsFightMap() <= 0)
	{
		rcCharacter.SetCurrentLife(nNewValue);
	}
	return bRet;
}

BOOL SetVipLevel(KCharacter& rcCharacter, BYTE& rValue, BYTE byNewValue)
{
	BYTE byOld = rValue;
	rValue = byNewValue;
	return rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_VIP_LEVEL, rcCharacter.GetId(), (INT)byOld, (INT)byNewValue);
}

BOOL SetTitleCarry1(KCharacter& rcCharacter, WORD& rValue, WORD byNewValue)
{
	WORD byOld = rValue;
	rValue = byNewValue;
	return rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_1, rcCharacter.GetId(), (INT)byOld, (INT)byNewValue);
}

BOOL SetTitleCarry2(KCharacter& rcCharacter, WORD& rValue, WORD byNewValue)
{
	WORD byOld = rValue;
	rValue = byNewValue;
	return rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_2, rcCharacter.GetId(), (INT)byOld, (INT)byNewValue);
}

BOOL SetTitleCarry3(KCharacter& rcCharacter, WORD& rValue, WORD byNewValue)
{
	WORD byOld = rValue;
	rValue = byNewValue;
	return rcCharacter.FireEvent(emKOBJEVENTTYPE_CHARACTER_TITLE_CARRY_3, rcCharacter.GetId(), (INT)byOld, (INT)byNewValue);
}

BOOL SetLevel(KCharacter& rcCharacter, WORD& rValue, WORD nNewValue)
{	
	//QLogPrintf(LOG_INFO,"..SetLevel..");
	BOOL bOK = FALSE;
	WORD wOld = rValue;
	rValue = nNewValue;

	//by ldy
	if(rValue >= 61)
	{
		return FALSE;
	}
	
	bOK = g_cFactionManager.SetLevel(rcCharacter, wOld, nNewValue);

	if(wOld && nNewValue > wOld)
	{
		g_cCharacterHelper.CalcFightScore(rcCharacter);
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunction("Player", "OnLevelUp", 0, "od", rcCharacter.GetScriptInterface());
	}

	time_t tNow = KSysService::Time(NULL);
	RankRelayClient::DoModifyRankValueRequest(emRANK_ROLE_LEVEL_OPEN_SERVER, rcCharacter.GetId(), rcCharacter.GetLevel(), tNow);
	return bOK;
}

BOOL SetKinCamp(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	if (rValue == 0) // 家族阵营一旦确定不可再更改
	{
        rValue = nNewValue;
	}

	return TRUE;
}

BOOL SetRoute(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetRoute(rcCharacter, byOld, nNewValue);
}

// 炼卦相关
BOOL SetEightTrigramLevel1(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel1(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel2(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel2(rcCharacter, byOld, nNewValue);
}
BOOL SetEightTrigramLevel3(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel3(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel4(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel4(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel5(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel5(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel6(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel6(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel7(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel7(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevel8(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevel8(rcCharacter, byOld, nNewValue);
}

BOOL SetEightTrigramLevelTaiChi(KCharacter& rcCharacter, BYTE& rValue, BYTE nNewValue)
{
	BYTE byOld = rValue;
	rValue = nNewValue;
	return g_cFactionManager.SetEightTrigramLevelTaiChi(rcCharacter, byOld, nNewValue);
}

BOOL SetSpeedDelaySet(KCharacter& rcCharacter, DWORD& rValue, DWORD nNewValue)
{	
	if (nNewValue == 0)
	{
        rValue = nNewValue;
	}
	else
	{
		INT nLastValue = 0;
		if (rcCharacter.m_SpeedCheckArr.size() > 0)
		{
			nLastValue = rcCharacter.m_SpeedCheckArr[rcCharacter.m_SpeedCheckArr.size() - 1];
		}

		if (nLastValue != nNewValue)
		{
			rcCharacter.m_SpeedCheckArr.push_back(nNewValue);

			INT nIndex = (INT)rcCharacter.m_SpeedCheckArr.size() - 1;// 记下索引
			INT nCombinValue = nNewValue | nIndex << 16;
			rValue = nCombinValue;
		}
	}
	
	return TRUE;
}

BOOL g_InitCharacterVarChange()
{
#define KD_CHARACTER_VAR_FUN_SET_DW(VARID, FUN) g_aCharacterSetFunDw[VARID] = FUN;
#define KD_CHARACTER_VAR_FUN_SET_W(VARID, FUN) g_aCharacterSetFunW[VARID] = FUN;
#define KD_CHARACTER_VAR_FUN_SET_BY(VARID, FUN) g_aCharacterSetFunBy[VARID] = FUN;

	KD_CHARACTER_VAR_FUN_SET_DW(emKCHARACTER_TASK_EXP, SetExp)
	KD_CHARACTER_VAR_FUN_SET_W(emKCHARACTER_TASK_LEVEL, SetLevel)
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_CURRENT_ROUTE, SetRoute)
	KD_CHARACTER_VAR_FUN_SET_DW(emKCHARACTER_TASK_MAX_LIFE,SetMaxLife)

	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_VIP_LEVEL, SetVipLevel)
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_KIN_CAMP,  SetKinCamp)
	KD_CHARACTER_VAR_FUN_SET_W(emKCHARACTER_TASK_TITLE_CARRY_1, SetTitleCarry1)
	KD_CHARACTER_VAR_FUN_SET_W(emKCHARACTER_TASK_TITLE_CARRY_2, SetTitleCarry2)
	KD_CHARACTER_VAR_FUN_SET_W(emKCHARACTER_TASK_TITLE_CARRY_3, SetTitleCarry3)

	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_1, SetEightTrigramLevel1);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_2, SetEightTrigramLevel2);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_3, SetEightTrigramLevel3);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_4, SetEightTrigramLevel4);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_5, SetEightTrigramLevel5);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_6, SetEightTrigramLevel6);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_7, SetEightTrigramLevel7);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_8, SetEightTrigramLevel8);
	KD_CHARACTER_VAR_FUN_SET_BY(emKCHARACTER_TASK_EIGHT_TRIGRAM_LEVEL_TAICHI, SetEightTrigramLevelTaiChi);

	KD_CHARACTER_VAR_FUN_SET_DW(emKCHARACTER_TASK_SPEEDDELAYSET, SetSpeedDelaySet);

	//KD_CHARACTER_VAR_FUN_ADD_OVERFLOW_DW(emKCHARACTER_TASK_CURRENT_LIFE, OnLifeUnderflow)
	return TRUE;
}


// -------------------------------------------------------------------------
#endif //#ifdef GAME_SERVER

