#pragma once
#include "onlinegameworld/kmodulebase.h"

class OpenseModule : public KModuleBase
{
public:
	OpenseModule();
	virtual ~OpenseModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
};

extern OpenseModule g_OpenseModule;