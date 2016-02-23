
#include "stdafx.h"
#include "onlinegamemodule/ai/kaictrl.h"
#include "kluaai.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


DWORD       KAICtrl::dwAIEventSrc                              = ERROR_ID;
int         KAICtrl::nAIEventParam                             = 0;
int         KAICtrl::nTargetGroupCount                         = 0;
KCharacter* KAICtrl::TargetGroup[AI_TARGET_GROUP_MAX_COUNT]    = {NULL};

KAICtrl::KAICtrl(KCharacter& rcCharacter) : 
KCharacterCtrlBase(rcCharacter), m_pLuna(NULL)
{
}

BOOL KAICtrl::Init()
{
	BOOL bRetCode = FALSE;

	m_pLuna = new KLuaAI(m_rcCharacter, this, &m_rcCharacter.m_AIVM);
	LOG_PROCESS_ERROR(m_pLuna);

	ResetData();

	bRetCode = TRUE;
EXIT0:
	return bRetCode;
}

BOOL KAICtrl::UnInit()
{
	ResetData();
	SAFE_DELETE(m_pLuna);

	return TRUE;
}


BOOL KAICtrl::CallScript(LPCSTR pszFunName, LPCSTR pszFormat, ...)
{
	BOOL bOK = FALSE;

	va_list vlist;
	va_start(vlist, pszFormat);
	ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
	bOK = cSafeScript->CallTableFunctionV("AI", pszFunName, 0, pszFormat, vlist);
	va_end(vlist);
	return bOK;
}

void KAICtrl::ResetData()
{
	nAIType                        = 0;

	nOriginPosX                    = -1;
	nOriginPosY                    = -1;
	nOriginPosZ                    = -1;
	nOriginDirection               = -1;

	nReturnPosX                    = -1;
	nReturnPosY                    = -1;
	nReturnPosZ                    = -1;

	nPatrolPathID                  = 0;
	pPatrolPath                    = NULL;
	nPatrolOrderIndex              = 0;

	nEscapeCount                   = 0;
	nCannotAttactCount             = 0;
	nSkillFailedCount              = 0;
	nSkillType                     = 0;
	dwRespondCharacterID           = 0;
	nAITotalDamageValue            = 0;
	dwProtectCampMark              = 0;
	dwProtectForceMark             = 0;
	bSpecialSelectTarget           = false;

	memset(nRecordSkillID, 0, sizeof(nRecordSkillID));
	memset(nRecordSkillLevel, 0, sizeof(nRecordSkillLevel));

	for (int i = 0; i < aittTotal; i++)
		ChoiceTarget[i].ClearTarget();

	m_nSkillSelectIndex = 0;
}
