#pragma once

#define  TITLE_FILE_PATH		"\\setting\\title\\title.txt"

struct TitleTemplate
{
	INT nId;
	INT nGetWay;
	INT nType;
	INT nMutex;
	INT nMinLevel;
	INT nMaxLevel;
	INT nBuffId;
	INT nEffectiveTime;
};

class TitleSetting 
{
	typedef std::vector<TitleTemplate*> TITLE_TEMPLATE_VECTOR;
public:
	TitleSetting();
	~TitleSetting();

	BOOL Init();
	BOOL UnInit();

	TitleTemplate* GetTitleTemplate(INT nId);
	TitleTemplate* GetTitleTemplate(INT nGetWay, INT nType);
	TitleTemplate* GetTitleTemplate(INT nGetWay,INT nLevel, INT nType);

private:
	BOOL ReadTitleTemplate();

private:
	TITLE_TEMPLATE_VECTOR m_vecTitleTemplate;
};

extern TitleSetting g_cTitleSetting;