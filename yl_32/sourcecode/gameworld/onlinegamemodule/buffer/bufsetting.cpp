/* -------------------------------------------------------------------------
//	文件名		：	bufsetting.cpp
//	创建者		：	zhangjunjie
//	创建时间	：	2012-5-10
//	功能描述	：	buff配置读取
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "bufsetting.h"
#include "onlinegamebase/kscriptmanager.h"

BOOL BufferTemplate::Init(ITabFile* piFile, int nRow)
{
	INT nTemp = 0;
	CHAR szTemp[SCRIPT_MAX_LENGTH] = {0};
	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	m_vecBufTimerEffectID.clear();
	m_vecBufTriggerEffectID.clear();
	piFile->GetInteger(nRow, "ID",				0 ,		(INT*)&m_nBuffTemplateID);
	piFile->GetInteger(nRow, "Persist" ,		0 ,		(INT*)&m_nBuffPersist);
	piFile->GetInteger(nRow, "Group",			0 ,		(INT*)&m_eBuffType);
	piFile->GetInteger(nRow, "Level",			0 ,		(INT*)&m_nBuffLevel);
	piFile->GetInteger(nRow, "Position",		0 ,		(INT*)&m_BuffPosition);
	piFile->GetInteger(nRow, "Debuff",			0 ,		(INT*)&m_nBuffType);
	piFile->GetInteger(nRow, "Category",		0 ,		(INT*)&m_nStackCategory);
	piFile->GetInteger(nRow, "Category1",		0 ,		(INT*)&m_nStackCategory1);
	piFile->GetInteger(nRow, "Category2",		0 ,		(INT*)&m_nStackCategory2);
	piFile->GetInteger(nRow, "SkillMagnification", 0,	(INT*)&m_nMagnification);
	piFile->GetInteger(nRow, "Grow",			0,		(INT*)&m_nGrow);
	piFile->GetInteger(nRow, "Stack",			0 ,		(INT*)&m_nStackCount);
	piFile->GetInteger(nRow, "Sync",			0 ,		(INT*)&m_bSync);
	piFile->GetInteger(nRow, "Save",			0 ,		(INT*)&m_bSave);
	piFile->GetInteger(nRow, "Death",			0 ,		(INT*)&m_bDeath);
	piFile->GetInteger(nRow, "Plus",			0 ,		(INT*)&m_bPlus);
	piFile->GetInteger(nRow, "NeedAgree",		0 ,		(INT*)&m_bNeedAgree);
	piFile->GetFloat(nRow, "BuffRadius",		0 ,		(FLOAT*)&m_fBuffRadius);
	piFile->GetInteger(nRow, "BuffEffectNum",	0 ,		(INT*)&m_nBuffEffectNum);
	piFile->GetInteger(nRow, "GroundBuff",		0 ,		(INT*)&m_bIsGroundBuffer);
	piFile->GetInteger(nRow, "K",				0 ,		(INT*)&m_nK);
	piFile->GetInteger(nRow, "BuffDelayFrame",	0 ,		(INT*)&m_nBuffDelayFrame);
	piFile->GetInteger(nRow, "IfUseSkillLv",	0 ,		(INT*)&m_nIfUseSkillLv);
	piFile->GetString(nRow, "EffectMapType",	"",		m_szEffectMapType,		MAX_NAME_LEN);
	
	{
		memset(m_abRelation, 0, sizeof(m_abRelation));
		char achRelation[5+1] = { 0 };
		piFile->GetString(nRow, "BuffRelation", "", achRelation, 5+1);
		string tRelation(achRelation);
		if(tRelation.size())
		{
			basic_string<char>::size_type iBegin = 0, iEnd = 0;
			while ( (iEnd = tRelation.find("|", iBegin)) != string::npos )
			{
				INT nType = atoi(tRelation.substr(iBegin, iEnd - iBegin).c_str());
				m_abRelation[nType-1] = TRUE;
				iBegin = iEnd + 1;
			}
			if (iBegin < tRelation.size())
			{
				INT nType = atoi(tRelation.substr(iBegin, tRelation.size() - iBegin).c_str());
				m_abRelation[nType-1] = TRUE;
			}
		}
		else
		{
			m_abRelation[0] = TRUE;
			m_abRelation[1] = TRUE;
			m_abRelation[2] = TRUE;
			m_abRelation[3] = TRUE;
		}
	}

	#define LENGTH 20 
	for (int i = 1; i <= BUFF_TIMER_EFFECT_COUNT ; ++i)
	{
		char szTitle[LENGTH] = {0};
		snprintf(szTitle, sizeof(szTitle), "Etimer%d", i);
		piFile->GetInteger(nRow, szTitle, 0 , (INT*)&nTemp);
		if (nTemp > 0)
		{
			m_vecBufTimerEffectID.push_back(nTemp);
		}
	}
	for (int i = 1; i <= BUFF_TRIGGER_EFFECT_COUNT ; ++i)
	{
		char szTitle[LENGTH] = {0};
		snprintf(szTitle, sizeof(szTitle), "Etrigger%d", i);
		piFile->GetInteger(nRow, szTitle, 0 , (INT*)&nTemp);
		if (nTemp > 0)
		{
			m_vecBufTriggerEffectID.push_back(nTemp);
		}
	}
	piFile->GetString(nRow, "Precond" , "", szTemp, countof(szTemp));
	m_cBuffPrecondition.Init(szTemp);
	return TRUE;
EXIT0:
	return FALSE;
}

//  Buffer等级数值读取
BOOL BufferNumber::Init(ITabFile* piFile, int nRow)
{
	INT nTemp = 0;
	CHAR szTemp[SCRIPT_MAX_LENGTH] = {0};
	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	piFile->GetInteger(nRow, "BufferId",				0 ,		(INT*)&m_nBufferId);
	piFile->GetInteger(nRow, "Value1" ,		0 ,		(INT*)&m_nValue);
	piFile->GetInteger(nRow, "SkillLevel" ,		0 ,		(INT*)&m_nSkillLevel);
	piFile->GetInteger(nRow, "SkillCoefficientMin" ,		1000,		(INT*)&m_nSkillCoefficientMin);
	piFile->GetInteger(nRow, "SkillCoefficientMax" ,		1000,		(INT*)&m_nSkillCoefficientMax);
	piFile->GetString(nRow, "Command", "", szTemp, countof(szTemp));
	strncpy(m_szCommand, szTemp, sizeof(m_szCommand));

	return TRUE;
EXIT0:
	return FALSE;
};


BuffSetting g_cBufferSetting;

BuffSetting::BuffSetting()
{

}

BuffSetting::~BuffSetting()
{

}

BOOL BuffSetting::Init()
{
	INT nHeight = 0;
	
	// 读取触发性效果表,第二行是说明
	ITabFile* tabFile = g_OpenTabFile(BUFFER_TRIGGER_EFFECT_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		BufTriggerEffect* triggerEff = new BufTriggerEffect();
		LOG_PROCESS_ERROR(triggerEff->Init(tabFile, i));

		m_mapTriggerEffects[triggerEff->GetID()] = triggerEff;
	}

	// 读取持续性效果表,第二行是说明
	tabFile = g_OpenTabFile(BUFFER_TIMER_EFFECT_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		BufTimerEffect* timerEff = new BufTimerEffect();
		LOG_PROCESS_ERROR(timerEff->Init(tabFile, i));

		m_mapTimerEffects[timerEff->GetID()] = timerEff;
	}

	// 读取Buffer表,第二行是说明
	tabFile = g_OpenTabFile(BUFFER_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		BufferTemplate* buf = new BufferTemplate();
		LOG_PROCESS_ERROR(buf->Init(tabFile, i));
		m_mapBuffers[buf->m_nBuffTemplateID] = buf;
	}
	
	// buffer数值读取
	tabFile = g_OpenTabFile(BUFFER_NUMBER_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		BufferNumber* bufNumber = new BufferNumber();
		LOG_PROCESS_ERROR(bufNumber->Init(tabFile, i));

		// 放入到容器中
		DWORD dwKey = MAKELONG(bufNumber->m_nSkillLevel, bufNumber->m_nBufferId);
		if (m_mapBufferNumbers.find(dwKey) == m_mapBufferNumbers.end())
		{
			// 创建vector并放入
			m_mapBufferNumbers[dwKey] = BUFFER_NUMBER_MAP();
		}
		
		m_mapBufferNumbers[dwKey][bufNumber->m_szCommand] = bufNumber;
			
	}

	return TRUE;
EXIT0:
	UnInit();
	return FALSE;
}

BOOL BuffSetting::UnInit()
{
	BUFFER_TIMER_EFFECT_MAP::iterator it_teff;
	for (it_teff = m_mapTimerEffects.begin(); it_teff != m_mapTimerEffects.end(); ++it_teff)
	{
		SAFE_DELETE((*it_teff).second);
	}
	m_mapTimerEffects.clear();

	BUFFER_TRIGGER_EFFECT_MAP::iterator it_eeff;
	for (it_eeff = m_mapTriggerEffects.begin(); it_eeff != m_mapTriggerEffects.end(); ++it_eeff)
	{
		SAFE_DELETE((*it_eeff).second);
	}
	m_mapTriggerEffects.clear();

	BUFFER_TEMPLATE_MAP::iterator it_buf;
	for (it_buf = m_mapBuffers.begin(); it_buf != m_mapBuffers.end(); ++it_buf)
	{
		SAFE_DELETE((*it_buf).second);
	}
	m_mapBuffers.clear();

	std::map<DWORD, BUFFER_NUMBER_MAP>::iterator it_bn;
	BUFFER_NUMBER_MAP::iterator it_bn2;
	for (it_bn = m_mapBufferNumbers.begin(); it_bn != m_mapBufferNumbers.end(); ++it_bn)
	{
		for (it_bn2 = (*it_bn).second.begin(); it_bn2 != (*it_bn).second.end(); ++it_bn2)
		{
			SAFE_DELETE((*it_bn2).second);
		}

		(*it_bn).second.clear();
	}

	m_mapBufferNumbers.clear();
	return TRUE;
}

BufferNumber* BuffSetting::GetBufferNumber(INT nBufferId, INT nSkillLevel, std::string szCommand)
{
	DWORD dwKey = MAKELONG(nSkillLevel, nBufferId);
	if (m_mapBufferNumbers.find(dwKey) != m_mapBufferNumbers.end())
	{
		BUFFER_NUMBER_MAP* pMapBufferNumbers = &m_mapBufferNumbers[dwKey];
		if (pMapBufferNumbers->find(szCommand) != pMapBufferNumbers->end())
		{
			return (*pMapBufferNumbers)[szCommand];
		}
	}

	return NULL;
}

BufferTemplate* BuffSetting::GetBufferTemplate(INT nID)
{
	if (m_mapBuffers.find(nID) != m_mapBuffers.end())
	{
		return m_mapBuffers[nID];
	}
	return NULL;
}

BufTimerEffect* BuffSetting::GetTimerEffect(INT nID)
{
	if (m_mapTimerEffects.find(nID) != m_mapTimerEffects.end())
	{
		return m_mapTimerEffects[nID];
	}
	return NULL;
}

TriggerData BuffSetting::GetTriggerEffect(INT nID)
{
	if (m_mapTriggerEffects.find(nID) != m_mapTriggerEffects.end())
	{
		return TriggerData(m_mapTriggerEffects[nID]);
	}

	return TriggerData(0);	
}

BOOL BuffSetting::CheckPreCondition(INT nBuffTemplateId,KCharacter* m_pSender,KCharacter* m_pReceiver)
{
	if (m_mapBuffers.find(nBuffTemplateId) != m_mapBuffers.end())
	{
		if(!m_mapBuffers[nBuffTemplateId])
			return FALSE;
		BufferTemplate* pTemplate = m_mapBuffers[nBuffTemplateId];
		QLunaBase* pOldMe = NULL;
		QLunaBase* pOldHim = NULL;
		pOldMe = g_cScriptManager.SetMe((QLunaBase*)m_pReceiver->GetScriptInterface());
		if (m_pSender)
		{
			pOldHim = g_cScriptManager.SetHim((QLunaBase*)m_pSender->GetScriptInterface());
		}
		BOOL bRet = pTemplate->m_cBuffPrecondition.Excecute();
		g_cScriptManager.SetMe(pOldMe);
		g_cScriptManager.SetHim(pOldHim);
		return bRet;
	}
	else
	{
		return FALSE;
	}
}