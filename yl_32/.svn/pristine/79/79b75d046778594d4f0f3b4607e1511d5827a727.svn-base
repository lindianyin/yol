
#pragma once

#include "Misc/objevent.h"
#include "onlinegamebase/kscriptmanager.h"

// -------------------------------------------------------------------------
class KObjGeneralEventCreator : public KEventCreator
{
protected:
	BOOL DispatchEvent(LPCSTR szFormat, ...)
	{
		va_list vlist;
		va_start(vlist, szFormat);

		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		cSafeScript->CallTableFunctionV("CallCenter", "OnEvent", 0, szFormat, vlist);

		va_end(vlist);
		return TRUE;
	}
};

// -------------------------------------------------------------------------

