
#include "onlinegameworld/kprotocol.h"
#pragma pack(push, 1)
enum FACTION_C2S_PROTOCOL
{
	emC2S_SYNC_CLIENT_SETTING,
	emC2S_Count,
};
enum FACTION_S2C_PROTOCOL
{
	emS2C_SYNC_CLIENT_SETTING,
	emS2C_Count,
};

// -------------------------------------------------------------------------
// 客户端同步设置信息
struct FACTION_C2S_SYNC_CLIENT_SETTING : KPTC_HEADER<emC2S_SYNC_CLIENT_SETTING>
{
	BYTE  byRepresentQuality;
	BYTE  byHideOtherPlayers;
	BYTE  byHideParticleEffect;
	BYTE  byBackgroundMusicVolume;
	BYTE  byGameSoundVolume;
	BYTE  byVolumeLock;
	BYTE  byQualityLock;
	BYTE  byAtuoAddHp;
	BYTE  byAtuoAddMp;
};

// 服务端同步设置信息
struct FACTION_S2C_SYNC_CLIENT_SETTING : KPTC_HEADER<emS2C_SYNC_CLIENT_SETTING>
{
	BYTE  byRepresentQuality;
	BYTE  byHideOtherPlayers;
	BYTE  byHideParticleEffect;
	BYTE  byBackgroundMusicVolume;
	BYTE  byGameSoundVolume;
	BYTE  byVolumeLock;
	BYTE  byQualityLock;
	BYTE  byAtuoAddHp;
	BYTE  byAtuoAddMp;
};

#pragma pack(pop)