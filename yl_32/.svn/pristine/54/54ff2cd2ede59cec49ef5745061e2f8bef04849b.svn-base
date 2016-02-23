#pragma once
#include "onlinegameworld/kmodulebase.h"

class TargetModule : public KModuleBase
{
public:
	TargetModule();
	virtual ~TargetModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
};

extern TargetModule g_TargetModule;