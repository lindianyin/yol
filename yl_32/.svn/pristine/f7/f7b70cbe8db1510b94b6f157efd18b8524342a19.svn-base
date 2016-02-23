
#ifndef __KTASKMODULE_H__
#define __KTASKMODULE_H__

// -------------------------------------------------------------------------
#include "onlinegameworld/kmodulebase.h"
// -------------------------------------------------------------------------
class KTaskModule : public KModuleBase
{
public:
	KTaskModule();
	virtual ~KTaskModule(){}

public:	
	BOOL OnInit();
	BOOL OnUnInit();
	BOOL Breath();

#ifdef GAME_SERVER
	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
#endif
#ifdef GAME_CLIENT
	BOOL ProcessProtocol(LPCBYTE pbData, UINT uDataLen);
#endif

	BOOL InitProtocol();
};

extern KTaskModule g_sTaskModule;

inline KTaskModule& GetTaskModule()
{	
	return g_sTaskModule;
}

// -------------------------------------------------------------------------

#endif /* __KTASKMODULE_H__ */
