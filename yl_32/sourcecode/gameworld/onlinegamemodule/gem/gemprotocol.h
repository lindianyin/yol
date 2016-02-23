#pragma once

#include "onlinegameworld/kprotocol.h"
#include "gemdef.h"

#pragma pack(push, 1)

enum KS2C_GEM_PROTOCOL
{
	s2c_sync_gem_info,
};

struct KGemInfo
{
	INT nGener;
	INT nDetail;
	INT nParticular;
	INT nLevel;
	INT nCount;
};

struct KS2C_SYNC_GEM_INFO : KLENPTC_HEADER<s2c_sync_gem_info>
{
	INT nCount;
	KGemInfo arrInfo[0];
};



#pragma pack(pop)