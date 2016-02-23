#pragma once

struct SignInAward
{
	INT nId;
	INT nAwardId;
	INT nSignInCount;
};

struct SevenDaysAward
{
	INT nId;
	INT nAwardId;
};

struct LobbyTarget
{
	INT nId;
	INT nAwardId;
	INT nMinValue;
};

struct AwardLobby
{
	INT nId;
	TIME_T tStartTime;
	TIME_T tEndTime;
	CHAR szModuleName[MAX_SENTENCE_LENGTH];

	std::map<INT, LobbyTarget*> mapLobbyTarget;
};
