#pragma once

#include "onlinegameworld/kprotocol.h"
#include "artifactdef.h"

#pragma pack(push, 1)

enum KC2S_ARTIFACT_PROTOCOL
{
	c2s_artifact_sync_request,
	c2s_artifact_piece_use_request,
	c2s_artiface_level_up_request,
};

enum KS2C_ARTIFACT_PROTOCOL
{
	s2c_sync_artifact_info,
};


// c2s
struct KC2S_SYNC_ARTIFACT_REQUEST : KPTC_HEADER<c2s_artifact_sync_request>
{
	INT nSync;
};

struct KC2S_PIECE_USE_REQUEST : KPTC_HEADER<c2s_artifact_piece_use_request>
{
	INT nId;
	INT nCount;
};

struct KC2S_LEVEL_UP_REQUEST : KPTC_HEADER<c2s_artiface_level_up_request>
{
	INT nId;
}; 

// s2c
struct KArtifactInfo
{
	INT nId;
	INT nLevel;
	INT nCount;			//已注入数量
	INT nLevelUp;
};

struct KS2C_SYNC_ARTIFACT_INFO : KLENPTC_HEADER<s2c_sync_artifact_info>
{
	INT nCount;
	KArtifactInfo arrInfo[0];
};

#pragma pack(pop)