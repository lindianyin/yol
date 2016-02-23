/* -------------------------------------------------------------------------
//	文件名		：	buftriggereffect.h
//	创建者		：	zhangzhixiong
//	创建时间	：	2012-5-7 
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "buffer/buftriggereffect.h"
#include "onlinegamemodule/bufferdef.h"
#include "onlinegamebase/kscriptmanager.h"

BufTriggerEffect::BufTriggerEffect()
{
	m_nID = 0;
	m_nProbPercent = 0;
	m_szEvent = "";
}

BufTriggerEffect::~BufTriggerEffect()
{

}

BOOL BufTriggerEffect::Init(ITabFile* tabFile, INT nRow)
{
	CHAR szTemp[SCRIPT_MAX_LENGTH] = {0};
	LOG_PROCESS_ERROR(tabFile);
	LOG_PROCESS_ERROR(nRow > 0);
	tabFile->GetInteger(nRow, "ID", 0 ,(INT*)&m_nID);
	tabFile->GetInteger(nRow, "Percent", 0 , (INT*)&m_nProbPercent);
	tabFile->GetString(nRow,  "Event" , "", szTemp, countof(szTemp));
	m_szEvent = m_szEvent + szTemp;
	ZeroMemory(szTemp,SCRIPT_MAX_LENGTH);
	tabFile->GetString(nRow, "EventOP" , "", szTemp, countof(szTemp));
	tabFile->GetInteger(nRow, "Interval", 0, &m_nIntervalTime);

	m_cCondition.Init(m_szEvent);

	m_cEffect.Init(szTemp);

	return TRUE;
EXIT0:
	return FALSE;
}

