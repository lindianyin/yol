
#pragma once
// -------------------------------------------------------------------------
class KCharacter;
class KPlayer;
class KNpc;

namespace KNSBASE_EXT
{
	// Npc初始化时
	extern BOOL (*DoNpcInit)(KNpc& rcNpc);

	// Doing协议过滤，return FALSE表示协议不通过
	extern BOOL (*DoingProtocolFilter[])(KPlayer& rcPlayer, LPCBYTE pbyData, UINT uDataLen);
};

