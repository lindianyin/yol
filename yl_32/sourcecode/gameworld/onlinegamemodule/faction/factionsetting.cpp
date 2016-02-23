#include "stdafx.h"
#include "factionsetting.h"

#define FACTION_EIGHT_TRIGRAM_ADDITION_FILE                 "\\setting\\faction\\eight_trigram_addition.txt"
#define FACTION_EIGHT_TRIGRAM_SPIRIT_FILE                 "\\setting\\faction\\eight_trigram_spirit.txt"
//#define FACTION_EIGHT_TRIGRAM_TIME_FILE                 "\\setting\\faction\\eight_trigram_time.txt"

FactionSetting g_cFactionSetting;

FactionSetting::FactionSetting()
{

}

FactionSetting::~FactionSetting()
{

}

BOOL FactionSetting::Init()
{
	return TRUE;
}

BOOL FactionSetting::UnInit()
{
	return TRUE;
}

