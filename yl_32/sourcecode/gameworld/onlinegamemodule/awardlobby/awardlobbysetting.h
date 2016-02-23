#pragma once
#include "awardlobbydef.h"

#define  AWARD_LOBBY_PATH		"\\setting\\awardlobby\\awardlobby.txt"
#define  LOBBY_TARGET_FOLD		"\\setting\\awardlobby\\";
#define  SIGNIN_AWARD_PATH		"\\setting\\awardlobby\\signInAward.txt"
#define  SEVENDAYS_AWARD_PATH	"\\setting\\awardlobby\\sevenDaysAward.txt"

class AwardLobbySetting 
{
public:
	AwardLobbySetting();
	~AwardLobbySetting();

	BOOL Init();
	BOOL UnInit();

	SignInAward* GetSignInAwardByIndex(INT nIndex);
	SevenDaysAward* GetSevenDaysAwardByIndex(INT nIndex);
	AwardLobby* GetAwardLobby(INT key);

	BOOL ReadSignIdAward();
	VOID ClearSignIdAward();

	BOOL ReadSevenDaysAward();
	VOID ClearSevenDaysAward();

	BOOL ReadAwardLobby();
	VOID ClearAwardLobby();

private:

	std::vector<SignInAward*> m_signIdAwardVec;
	std::vector<SevenDaysAward*> m_sevenDaysAwardVec;
	std::map<INT, AwardLobby*> m_awardLobbyMap;

};

extern AwardLobbySetting g_cAwardLobbySetting;