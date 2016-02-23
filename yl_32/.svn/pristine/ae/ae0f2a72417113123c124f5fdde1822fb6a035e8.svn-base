#pragma once
#include "onlinegameworld/kmodulebase.h"

class VipModule : public KModuleBase
{
public:
	VipModule();
	virtual ~VipModule();

	BOOL OnInit();

	BOOL OnUnInit();

	BOOL Breath();

	virtual BOOL InitProtocol();

	BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);
};

extern VipModule g_VipModule;