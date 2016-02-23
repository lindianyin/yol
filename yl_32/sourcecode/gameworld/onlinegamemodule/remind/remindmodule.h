#pragma once

#include "onlinegameworld/kmodulebase.h"

class RemindModule : public KModuleBase
{
public:
	RemindModule();
	virtual ~RemindModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
};

extern RemindModule g_cRemindModule;