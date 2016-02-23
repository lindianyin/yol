
#include "stdafx.h"
#include "kmagicattribmanager.h"
#include "kmagicdesc.h"
#include "kmagicattrib.h"
#include "onlinegamemodule/item2/itemdef.h"
#include "onlinegamebase/kscriptmanager.h"
#include "onlinegameworld/kcharacter.h"

KMagicAttribMgr g_cMagicAttribMgr;

// -------------------------------------------------------------------------
// KMagicAttribMgr
// -------------------------------------------------------------------------
PCSTR KMagicAttribMgr::GetStringFromId(INT nAttrib)
{
	return m_cMagicDesc.GetStringFromId(nAttrib);
}


// -------------------------------------------------------------------------
INT KMagicAttribMgr::GetIdFormString(PCSTR szMagicAttribName)
{
	return m_cMagicDesc.GetIdFormString(szMagicAttribName);
}


// -------------------------------------------------------------------------
LPCSTR KMagicAttribMgr::GetMagicDesc(CONST KMagicAttrib *pData, INT nSkillId)
{
	return m_cMagicDesc.GetDesc(pData, nSkillId);
}


// -------------------------------------------------------------------------
VOID KMagicAttribMgr::ModifyAttrib(KCharacter *pCharacterLauncher, KCharacter *pCharacter, INT nSkillId, CONST KMagicAttrib& rcMagic, BOOL bIsEndProcess)
{
	LPCSTR pszMagicEffectName = GetStringFromId(rcMagic.nAttribType);
	if (pszMagicEffectName && *pszMagicEffectName)
	{
		KMagicAttrib sMagic = rcMagic;
		if (bIsEndProcess)
		{
			for (UINT i = 0; i < countof(sMagic.nValue); ++i)
			{
				sMagic.nValue[i] = - rcMagic.nValue[i];
			}
		}

		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();

		if(pCharacterLauncher)
		{
			g_cScriptManager.SetMe((QLunaBase*)pCharacterLauncher->GetScriptInterface());
		}

		cSafeScript->CallTableFunction(
			"BufferMagicEffect", pszMagicEffectName,
			0,
			"dd",
			sMagic.nValue[0],
			sMagic.nValue[1]
		);
	}
	return;
}


// -------------------------------------------------------------------------
BOOL KMagicAttribMgr::IsEndProcess() CONST
{
	//return m_cNpcAttribModify.IsEndProcess();
	// TODO:
	return 0;
}


// -------------------------------------------------------------------------
KNpc* KMagicAttribMgr::GetMagicLauncher() CONST
{
	//return m_cNpcAttribModify.GetLauncher();
	// TODO:
	return NULL;
}

// -------------------------------------------------------------------------
// end
