#pragma once
#include "artifactdef.h"

//#define  AWARD_LOBBY_PATH		"\\setting\\awardlobby\\awardlobby.txt"
//#define  LOBBY_TARGET_FOLD		"\\setting\\awardlobby\\";
//#define  SIGNIN_AWARD_PATH		"\\setting\\awardlobby\\signInAward.txt"
//#define  SEVENDAYS_AWARD_PATH	"\\setting\\awardlobby\\sevenDaysAward.txt"

class ArtifactSetting 
{
public:
	ArtifactSetting();
	~ArtifactSetting();

	BOOL Init();
	BOOL UnInit();

private:
};

extern ArtifactSetting g_cArtifactSetting;