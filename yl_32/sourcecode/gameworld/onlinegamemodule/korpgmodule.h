
#pragma once

#include "onlinegameworld/kmodulebase.h"
// -------------------------------------------------------------------------
class KOrpgModule : public KModuleBase
{
public:
	KOrpgModule();
	virtual BOOL PreInit();

	virtual BOOL OnInit();
	virtual BOOL OnUnInit();
	virtual BOOL Breath();

protected:
	void _RegisterCtrl();
};



