#pragma once
#include "opensedef.h"

#define  OPENSE_FILE_PATH		"\\setting\\opense\\opense.txt"
#define  OPENSE_DAY_FOLD		"\\setting\\opense\\";

class OpenseSetting
{
public:
	OpenseSetting();
	~OpenseSetting();

	BOOL Init();
	BOOL UnInit();

	BOOL ReadOpenseTemplate();
	BOOL ClearOpenseCfg();

	OpenseCfg* GetOpenseCfg(INT nDay);
	OpenseDayCfg* GetOpenseDayCfg(INT nDay, INT nId);
	INT GetOpenseAwardCount();

private:
	std::map<INT, OpenseCfg*> m_MapOpenseCfg;
};

extern OpenseSetting g_cOpenseSetting;