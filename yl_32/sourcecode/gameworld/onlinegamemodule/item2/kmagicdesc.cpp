#include "stdafx.h"
#include "kmagicattrib.h"
#include "kmagicdesc.h"
#include "kmagicattribgroup.h"
#include "kmagicattribmanager.h"

#include <string>

static LPCSTR s_apszMagicAttrName[magic_end + 1];

// -------------------------------------------------------------------------
// KMagicDesc
// -------------------------------------------------------------------------
LPCSTR KMagicDesc::GetStringFromId(INT nAttrib)
{
	LPCSTR pszMagicName	= NULL;

	if (nAttrib > KD_MAGIC_INVALID2 && nAttrib < magic_end)
		pszMagicName	= s_apszMagicAttrName[nAttrib];

	if (!pszMagicName)
		pszMagicName	= "";
	
	return pszMagicName;
}


// -------------------------------------------------------------------------
INT KMagicDesc::GetIdFormString(LPCSTR pszMagicAttribName)
{
	INT nMagic	= KD_MAGIC_INVALID2;
	KMagicNameIdMap::const_iterator it;

	LOG_PROCESS_ERROR(pszMagicAttribName);
	
	PROCESS_ERROR(pszMagicAttribName[0]);

	it = m_mapMagicNameId.find(pszMagicAttribName);
	if (it == m_mapMagicNameId.end())
		;//QLogPrintf(LOG_ERR, "unknown magic desc '%s'", pszMagicAttribName);
	else
		nMagic	= it->second;

EXIT0:
	return	nMagic;
}


// -------------------------------------------------------------------------
KMagicDesc::KMagicDesc()
{
	ZeroStruct(s_apszMagicAttrName);

	{
		s_apszMagicAttrName[magic_begin]							= "magic_begin";

		s_apszMagicAttrName[modifydmg_p]							= "modifydmg_p";
		s_apszMagicAttrName[modifydmg_v]							= "modifydmg_v";
		s_apszMagicAttrName[addmaxlife_v]							= "addmaxlife_v";
		s_apszMagicAttrName[addmaxlife_p]							= "addmaxlife_p";
		s_apszMagicAttrName[addmaxmana_v]							= "addmaxmana_v";
		s_apszMagicAttrName[addmaxmana_p]							= "addmaxmana_p";
		s_apszMagicAttrName[addmaxpower_v]							= "addmaxpower_v";
		s_apszMagicAttrName[addmaxpower_p]							= "addmaxpower_p";
		s_apszMagicAttrName[addhitrate_v]							= "addhitrate_v";
		s_apszMagicAttrName[addcriticalrate_v]						= "addcriticalrate_v";
		s_apszMagicAttrName[addresist_v]							= "addresist_v";
		s_apszMagicAttrName[addcriticaldmg_p]						= "addcriticaldmg_p";
		s_apszMagicAttrName[addrunspeed_v]							= "addrunspeed_v";
		s_apszMagicAttrName[adddef_v]								= "adddef_v";
		s_apszMagicAttrName[adddodge_v]								= "adddodge_v";
		s_apszMagicAttrName[rebound_dmg_p]							= "rebound_dmg_p";
		s_apszMagicAttrName[rebound_dmg_v]							= "rebound_dmg_v";
		s_apszMagicAttrName[reducedmg]								= "reducedmg";
		s_apszMagicAttrName[transdmgtolife]							= "transdmgtolife";
		s_apszMagicAttrName[transdmgtomana]							= "transdmgtomana";
		s_apszMagicAttrName[addtreatlife_v]							= "addtreatlife_v";
		s_apszMagicAttrName[addtreatlife_p]							= "addtreatlife_p";
		s_apszMagicAttrName[addtreatmana_v]							= "addtreatmana_v";
		s_apszMagicAttrName[addtreatmana_p]							= "addtreatmana_p";
		s_apszMagicAttrName[add_buffer]								= "add_buffer";
		s_apszMagicAttrName[addweapondmg_v]							= "addweapondmg_v";
		s_apszMagicAttrName[addcurlife_v]							= "addcurlife_v";
		s_apszMagicAttrName[addcurmana_v]							= "addcurmana_v";
		s_apszMagicAttrName[addatkdmg]								= "addatkdmg";
		s_apszMagicAttrName[addrunspeed_p]							= "addrunspeed_p";
	}

	// ∑≈»Îmap
	m_mapMagicNameId.clear();

	for (INT i = 0; i < countof(s_apszMagicAttrName); i++)
	{
		if (s_apszMagicAttrName[i])
			m_mapMagicNameId[s_apszMagicAttrName[i]]	= i;
	}
}


// -------------------------------------------------------------------------
LPCSTR KMagicDesc::GetDesc(CONST KMagicAttrib* pAttrib, INT nSkillId)
{
	static CHAR	szDesc[128];
	memset(szDesc, 0, sizeof(szDesc));
	
	if (!pAttrib)
	{
		_ASSERT(FALSE);
		return NULL;
	}

	//if (nSkillId <= 0 || nSkillId > KD_FIGHTSKILL_MAXSKILLCOUNT)
	//{
	//	_ASSERT(FALSE);
	//	return NULL;
	//}

	//LPCSTR pszKeyName = g_cMagicAttribMgr.GetStringFromId(pAttrib->nAttribType);

	//KScriptSafeCall cSafeCall(*g_pMainScript);
	//g_pMainScript->CallTableFunction("FightSkill", "GetMagicDesc", 1, "s{ddd}{d}", pszKeyName,
	//				pAttrib->nValue[0], pAttrib->nValue[1], pAttrib->nValue[2], nSkillId);
	//LPCSTR pszDesc	= g_pMainScript->GetStr(-1);
	//if (pszDesc)
	//{
	//	QStrCpyLen(szDesc, pszDesc, sizeof(szDesc));
	//	szDesc[sizeof(szDesc)-1]	= '\0';
	//}
	//else
	//{
	//	szDesc[0]	= '\0';
	//}
	//g_pMainScript->PopStack(1);

	return szDesc;
}


// -------------------------------------------------------------------------
// end
