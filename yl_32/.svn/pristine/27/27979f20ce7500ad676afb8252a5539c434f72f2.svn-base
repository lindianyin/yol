#pragma once
#include "targetdef.h"

#define  TARGET_FILE_PATH		"\\setting\\target\\target.txt"
#define  TARGET_DAY_FOLD		"\\setting\\target\\";

class TargetSetting 
{
public:
	TargetSetting();
	~TargetSetting();

	BOOL Init();
	BOOL UnInit();

	BOOL ReadTargetTemplate();
	BOOL ClearTargetCfg();

	TargetCfg* GetTargetCfg(INT nDay);
	TargetDayCfg* GetTargetDayCfg(INT nDay, INT nId);
	INT GetTargetAwardCount();

private:
	std::map<INT, TargetCfg*> m_MapTargetCfg;
};

extern TargetSetting g_cTargetSetting;