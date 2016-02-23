#pragma once
#include "onlinegameworld/kmodulebase.h"

class TitleModule : public KModuleBase
{
public:
	TitleModule();
	virtual ~TitleModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
};

extern TitleModule g_TitleModule;