#pragma once
#include "vipdef.h"

#define  WELFARE_OVERVIEW_PATH		"\\setting\\vip\\welfare_overview.txt"
#define  WELFARE_OVERVIEW_FOLD		"\\setting\\vip\\overview\\";
#define  WELFARE_CONSTRAST_PATH		"\\setting\\vip\\welfare_contrast.txt"
#define  WELFARE_CONSTRAST_FOLD		"\\setting\\vip\\contrast\\";

class VipSetting 
{
public:
	VipSetting();
	~VipSetting();

	BOOL Init();
	BOOL UnInit();

	VipOverview* GetVipAttribute(INT nLevel);
	BOOL GetVipGift(INT nLevel, std::map<INT, VipGift*>& mapVipGifts);
	VipAttributeInfo* GetVipAttribute(INT nId, INT nLevel);
	LPCSTR GetModuleName(INT nId);

	BOOL ReadVipGifts();
	BOOL ClearVipGifts();

	BOOL ReadVipAttribute();
	BOOL ClearVipAttribute();


private:

	std::map<INT, VipOverview*> m_MapVipGifts;				//‘˘ÀÕ¿Ò∞¸
	std::map<INT, VipAttribute*> m_MapVipAttribute;		//vip Ù–‘
};

extern VipSetting g_cVipSetting;