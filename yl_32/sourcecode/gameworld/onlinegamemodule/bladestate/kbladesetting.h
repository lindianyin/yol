#ifndef _BLADE_SETTING_H__
#define _BLADE_SETTING_H__

#include "bladedef.h"

class KBladeSetting
{
public:
	KBladeSetting();
	~KBladeSetting();

	BOOL Init();
	BOOL UnInit();

public:
	KBLADE_ATTR_PERCENT m_cBladeAttrPercent;

};

extern KBladeSetting g_cBladeSetting;

#endif