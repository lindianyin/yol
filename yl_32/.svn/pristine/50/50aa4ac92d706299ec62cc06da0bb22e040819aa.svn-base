/* -------------------------------------------------------------------------
//	文件名		：	kpetsetting.h
//	创建者		：	zhaoyu
//	创建时间	：	2013-2-25
//	功能描述	：	宠物配置读取
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kpetsetting.h"
#include "../item2/kmagicattribmanager.h"

#define SZBUFLEN_0 64

#define KD_RIDE_BASE_ATTR "BasicNature%d"
#define KD_RIDE_BASE_VALUE "BasicNature%dValue"
#define KD_RIDE_ATTR "RideNature%d"
#define KD_RIDE_VALUE "RideNature%dValue"

typedef CHAR KMAGICATTRIB_NAME[SZBUFLEN_0];		// 存放魔法属性名的临时字符串

BOOL KPetTemplate::Init(ITabFile* piFile, INT nRow)
{
	BOOL bRet = FALSE;

	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	piFile->GetString(nRow,	"Name", "", m_szName, countof(m_szName));
	piFile->GetInteger(nRow, "PetId",				0 ,		(INT*)&m_nType);
	piFile->GetInteger(nRow, "Hungry",				0 ,		(INT*)&m_nHungry);
	piFile->GetInteger(nRow, "Quality",             0 ,     (INT*)&nQuality);
	piFile->GetInteger(nRow, "PetSkill1",			0,		(INT*)&m_PetSkill1);
	piFile->GetInteger(nRow, "PetSkill2",			0,		(INT*)&m_PetSkill2);
	piFile->GetInteger(nRow, "PetNpcId",			0,		(INT*)&m_PetNpcId);

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KPetSkillTemplate::Init(ITabFile* piFile, INT nRow)
{
	BOOL bRet = FALSE;
	CHAR szBuf[SZBUFLEN_0];

	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	piFile->GetInteger(nRow, "ID",			0,		(INT*)&m_nTemplateId);
	piFile->GetString(nRow,	"SkillName",		"",		m_szSkillName, countof(m_szSkillName));
	piFile->GetInteger(nRow, "SkillType",       0,       &m_nSkillType);
	piFile->GetInteger(nRow, "SkillCD",			0,		(INT*)&m_nSkillCD);
	piFile->GetInteger(nRow, "ToSelf",			0,		(INT*)&m_bToSelf);

	for(INT i = 0; i < emPET_COUNT_SKILL; ++i)
	{
		snprintf(szBuf, countof(szBuf), "SkillId%d", i + 1);
		piFile->GetInteger(nRow, szBuf,	0,		(INT*)&m_arySkillId[i]);
	}

	for(INT i = 0; i < emPET_COUNT_SKILL; ++i)
	{
		snprintf(szBuf, countof(szBuf), "BuffId%d", i + 1);
		piFile->GetInteger(nRow, szBuf,	0,		(INT*)&m_aryBuffId[i]);
	}

	bRet = TRUE;
EXIT0:
	return bRet;
}

KPetSetting g_cPetSetting;

KPetSetting::KPetSetting()
{

}

KPetSetting::~KPetSetting()
{

}

BOOL KPetSetting::Init()
{
	INT nHeight = 0;

	// 读取宠物模板表
	ITabFile* tabFile = g_OpenTabFile(PET_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		KPetTemplate* pPetTemplate = new KPetTemplate();
		LOG_PROCESS_ERROR(pPetTemplate->Init(tabFile, i));
		m_mapPetTemplates[pPetTemplate->m_nType] = pPetTemplate;
	}

	// 读取宠物技能
	tabFile = g_OpenTabFile(PET_SKILL_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		KPetSkillTemplate* pSkillTemplate = new KPetSkillTemplate();
		LOG_PROCESS_ERROR(pSkillTemplate->Init(tabFile, i));
		m_mapPetSkillTemplates[pSkillTemplate->m_nTemplateId] = pSkillTemplate;
	}

	// 读取坐骑表
	tabFile = g_OpenTabFile(RIDE_TABLE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		KRideTemplate* pRideTemplate = new KRideTemplate();
		LOG_PROCESS_ERROR(pRideTemplate->Init(tabFile, i));
		KITEM_INDEX sIndex = {pRideTemplate->m_nGenre, 
			pRideTemplate->m_nDetail, 
			pRideTemplate->m_nParticular, 
			pRideTemplate->m_nLevel};

		m_mapRideTemplates[sIndex] = pRideTemplate;
	}

	// 读取坐骑升阶属性表
	tabFile = g_OpenTabFile(RIDE_TABLE_RANK_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; i++)
	{
		KRideRankTemplate* pRankTemplate = new KRideRankTemplate();
		LOG_PROCESS_ERROR(pRankTemplate->Init(tabFile, i));
		m_mapRideRankTemplates[pRankTemplate->m_nGrowId] = pRankTemplate;
	}


	// 读取宠物属性战斗力表
	tabFile = g_OpenTabFile(PET_ATTR_SCORE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; ++i)
	{
		INT nAttrib = 0;
		INT nScore = 0;
		CHAR szBuf[MAX_NAME_LEN] = "";

		tabFile->GetString(i, "Attrib", "", szBuf, countof(szBuf));

		nAttrib = g_cMagicAttribMgr.GetIdFormString(szBuf);
		LOG_PROCESS_ERROR(nAttrib > 0);

		nScore = 0;
		tabFile->GetInteger(i, "Score", 0, &nScore);

		m_mapPetAttrScore[nAttrib] = nScore;
	}

	// 读取坐骑属性战斗力表
	tabFile = g_OpenTabFile(RIDE_ATTR_SCORE_FILE);
	LOG_PROCESS_ERROR(tabFile);

	nHeight = tabFile->GetHeight();
	for (INT i = 3; i <= nHeight; ++i)
	{
		INT nAttrib = 0;
		INT nScore = 0;
		CHAR szBuf[MAX_NAME_LEN] = "";

		tabFile->GetInteger(i, "Attrib", 0, &nAttrib);

		nScore = 0;
		tabFile->GetInteger(i, "Score", 0, &nScore);

		m_mapRideAttrScore[nAttrib] = nScore;
	}

	return TRUE;
EXIT0:
	UnInit();
	return FALSE;
}

BOOL KRideRankTemplate::Init(ITabFile* piFile, INT nRow)
{
	BOOL bRet = FALSE;
	KMAGICATTRIB_NAME szBaseAttrName[emRIDE_COUNT_BASE];
	KMAGICATTRIB_NAME szRideAttrName[emRIDE_COUNT_RIDE];
	CHAR szBuf[SZBUFLEN_0];

	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	piFile->GetInteger(nRow, "RankId",				0 ,		(INT*)&m_nGrowId);
	for(INT i = 1; i <= emRIDE_COUNT_BASE; ++i)
	{
		snprintf(szBuf, countof(szBuf), KD_RIDE_BASE_ATTR, i);
		piFile->GetString(nRow, szBuf, "", szBaseAttrName[i-1], countof(szBaseAttrName[i-1]));
		m_aryRankAttr[i-1].nAttrib = g_cMagicAttribMgr.GetIdFormString(szBaseAttrName[i-1]);

		snprintf(szBuf, countof(szBuf), KD_RIDE_BASE_VALUE, i);
		piFile->GetInteger(nRow, szBuf, 0, (INT*)&m_aryRankAttr[i-1].nValue);
	}

	
	for(INT i = 1; i <= emRIDE_COUNT_RIDE; ++i)
	{
		snprintf(szBuf, countof(szBuf), KD_RIDE_ATTR, i);
		piFile->GetString(nRow, szBuf, "", szRideAttrName[i-1], countof(szRideAttrName[i-1]));
		m_aryRideAttr[i-1].nAttrib = g_cMagicAttribMgr.GetIdFormString(szRideAttrName[i-1]);

		snprintf(szBuf, countof(szBuf), KD_RIDE_VALUE, i);
		piFile->GetInteger(nRow, szBuf, 0, (INT*)&m_aryRideAttr[i-1].nValue);
	}

	bRet = TRUE;
EXIT0:
	return bRet;
}

BOOL KPetSetting::UnInit()
{
	PET_TEMPLATE_MAP::iterator it_pet;
	for (it_pet = m_mapPetTemplates.begin(); it_pet != m_mapPetTemplates.end(); ++it_pet)
	{
		SAFE_DELETE((*it_pet).second);
	}
	m_mapPetTemplates.clear();

	RIDE_TEMPLATE_MAP::iterator it_ride;
	for (it_ride = m_mapRideTemplates.begin(); it_ride != m_mapRideTemplates.end(); ++it_ride)
	{
		SAFE_DELETE((*it_ride).second);
	}
	m_mapRideTemplates.clear();

	RIDE_RANK_TEMPLATE_MAP::iterator it_ride_rank;
	for (it_ride_rank = m_mapRideRankTemplates.begin(); it_ride_rank != m_mapRideRankTemplates.end(); ++it_ride_rank)
	{
		SAFE_DELETE((*it_ride_rank).second);
	}
	m_mapRideRankTemplates.clear();

	return TRUE;
}

INT KPetSetting::GetBuffIdBySkillId(INT nSkillId)
{
	PET_SKILL_TEMPLATE_MAP::iterator it;
    for (it = m_mapPetSkillTemplates.begin(); it != m_mapPetSkillTemplates.end(); ++it)
	{
		for (INT i = 0; i < emPET_COUNT_SKILL; i++)
		{
            if (it->second->m_arySkillId[i] == nSkillId)
			{
				return (INT)it->second->m_aryBuffId[i];
			}
		}
	}

	return 0;
}

KPetTemplate* KPetSetting::GetPetTemplate(INT nType)
{
	if (m_mapPetTemplates.find(nType) != m_mapPetTemplates.end())
	{
		return m_mapPetTemplates[nType];
	}
	return NULL;
}

// 获取宠物的对应NPC
INT KPetSetting::GetPetNpcIdByType(INT nType)
{
	KPetTemplate* pTemplate = this->GetPetTemplate(nType);
	LOG_PROCESS_ERROR(pTemplate);
	
	return pTemplate->m_PetNpcId;

EXIT0:
	return 0;
}

KPetSkillTemplate* KPetSetting::GetPetSkillTemplate(INT nSkillId)
{
	if (m_mapPetSkillTemplates.find(nSkillId) != m_mapPetSkillTemplates.end())
	{
		return m_mapPetSkillTemplates[nSkillId];
	}
	return NULL;
}

BOOL KRideTemplate::Init(ITabFile* piFile, INT nRow)
{
	INT i = 0;
	BOOL bRet = FALSE;

	LOG_PROCESS_ERROR(piFile);
	LOG_PROCESS_ERROR(nRow > 0);

	piFile->GetString(nRow,	"Name", "", m_szName, countof(m_szName));
	piFile->GetInteger(nRow, "Genre",				0 ,		(INT*)&m_nGenre);
	piFile->GetInteger(nRow, "Detail",				0 ,		(INT*)&m_nDetail);
	piFile->GetInteger(nRow, "Particular",			0 ,		(INT*)&m_nParticular);
	piFile->GetInteger(nRow, "Level",				0 ,		(INT*)&m_nLevel);
	piFile->GetInteger(nRow, "Quality",				0 ,		(INT*)&m_nQuality);
	piFile->GetInteger(nRow, "RankId",				0 ,		(INT*)&m_nRankId);
	piFile->GetInteger(nRow, "RankConsume",			0 ,		(INT*)&m_nRankConsume);
	piFile->GetInteger(nRow, "MaxLevel",			0 ,		(INT*)&m_nMaxLevel);
	piFile->GetInteger(nRow, "NpcId",				0 ,		(INT*)&m_nRepresentId);

	bRet = TRUE;
EXIT0:
	return bRet;
}

KRideTemplate* KPetSetting::GetRideTemplate(KITEM_INDEX sIndex)
{
	if (m_mapRideTemplates.find(sIndex) != m_mapRideTemplates.end())
	{
		return m_mapRideTemplates[sIndex];
	}
	return NULL;
}

KRideRankTemplate* KPetSetting::GetRideRankTemplate(INT nGrowId)
{
	if (m_mapRideRankTemplates.find(nGrowId) != m_mapRideRankTemplates.end())
	{
		return m_mapRideRankTemplates[nGrowId];
	}
	return NULL;
}

INT KPetSetting::GetPetAttribScore(INT nAttrib)
{
	KMAP_PET_ATTR_SCORE::iterator it = m_mapPetAttrScore.find(nAttrib);

	if(it != m_mapPetAttrScore.end())
	{
		return it->second;
	}

	return 0;
}

INT KPetSetting::GetRideAttribScore(INT nAttrib)
{
	KMAP_PET_ATTR_SCORE::iterator it = m_mapRideAttrScore.find(nAttrib);

	if(it != m_mapRideAttrScore.end())
	{
		return it->second;
	}

	return 0;
}