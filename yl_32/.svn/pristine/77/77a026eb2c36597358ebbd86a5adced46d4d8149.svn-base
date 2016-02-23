

#include "stdafx.h"
#include "onlinegameworld/kprotocol.h"
#include "onlinegameworld/kprotocolsize.h"
// 包含各模块的协议定义头文件
#include "onlinegameworld/kbaseprotocol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

template<INT N>
struct KTPTC_SIZE_C2S { };
template<INT N>
struct KTPTC_SIZE_S2C { };

//template<BYTE BY_PROTOCOL>
//const INT KPTC_HEADER_C2S<BY_PROTOCOL>::nProtocolSize = KTPTC_SIZE_C2S<BY_PROTOCOL>::Size;
//template<BYTE BY_PROTOCOL>
//const INT KPTC_HEADER_S2C<BY_PROTOCOL>::nProtocolSize = KTPTC_SIZE_S2C<BY_PROTOCOL>::Size;
// 一旦定义了协议结构就必须用KDEF_PROTOCOL_SIZE指定协议大小否则编不过
#define KD_FIX_PTC_C2S(PTC_STRUCT) \
	template<> struct KTPTC_SIZE_SET_C2S<PTC_STRUCT::protocol> { enum { Size = sizeof(PTC_STRUCT) }; };
#define KD_LENTHEN_PTC_C2S(PTC_STRUCT) \
	template<> struct KTPTC_SIZE_SET_C2S<PTC_STRUCT::protocol> { enum { Size = -1 }; };\

#define KD_FIX_PTC_S2C(PTC_STRUCT) \
	template<> struct KTPTC_SIZE_SET_S2C<PTC_STRUCT::protocol> { enum { Size = sizeof(PTC_STRUCT) }; };
#define KD_LENTHEN_PTC_S2C(PTC_STRUCT) \
	template<> struct KTPTC_SIZE_SET_S2C<PTC_STRUCT::protocol> { enum { Size = -1 }; };\

static INT s_anProtocolSize_C2S[256] = { 0 };
static INT s_anProtocolSize_S2C[256] = { 0 };
static LPINT s_pnProtocolSizeC2S[256] = { NULL };
static LPINT s_pnProtocolSizeS2C[256] = { NULL };

template<INT N>
struct KTPTC_SIZE_SET_C2S { enum { Size = 0 }; };
template<INT N>
struct KTPTC_SIZE_SET_S2C { enum { Size = 0 }; };

template<INT N>
struct KPTC_SIZE_ININT
{
	static VOID Init()
	{
		s_anProtocolSize_C2S[N] = KTPTC_SIZE_SET_C2S<N>::Size; 
		s_anProtocolSize_S2C[N] = KTPTC_SIZE_SET_S2C<N>::Size; 
		KPTC_SIZE_ININT<N + 1>::Init();
	}
};

template<>
VOID KPTC_SIZE_ININT<256>::Init() {}


VOID KPTC_SIZE::Init()
{
	KPTC_SIZE_ININT<1>::Init();
	s_pnProtocolSizeC2S[0] = s_anProtocolSize_C2S;
	s_pnProtocolSizeS2C[0] = s_anProtocolSize_S2C;
}

INT KPTC_SIZE::GetSizeC2S(BYTE byPf, BYTE byPtc)
{
	QCONFIRM_RET_FALSE(s_pnProtocolSizeC2S[byPf]);
	return s_pnProtocolSizeC2S[byPf][byPtc];
}

INT KPTC_SIZE::GetSizeS2C(BYTE byPf, BYTE byPtc)
{
	QCONFIRM_RET_FALSE(s_pnProtocolSizeS2C[byPf]);
	return s_pnProtocolSizeS2C[byPf][byPtc];
}

VOID KPTC_SIZE::SetSizeCheckC2S( BYTE byPf, LPINT pnSizeArray )
{
	QCONFIRM(s_pnProtocolSizeC2S[byPf] == NULL);
	s_pnProtocolSizeC2S[byPf] = pnSizeArray;
}

VOID KPTC_SIZE::SetSizeCheckS2C( BYTE byPf, LPINT pnSizeArray )
{
	QCONFIRM(s_pnProtocolSizeS2C[byPf] == NULL);
	s_pnProtocolSizeS2C[byPf] = pnSizeArray;
}
// 下面定义协议大小------ client->server ------------
KD_FIX_PTC_C2S(KC2S_LOGIN)
KD_FIX_PTC_C2S(KC2SSYNC_END)
KD_FIX_PTC_C2S(KC2S_PING)
KD_FIX_PTC_C2S(KC2S_JUMP)
KD_FIX_PTC_C2S(KC2S_MOVE)
KD_FIX_PTC_C2S(KC2S_MOVE_CTRL)
KD_FIX_PTC_C2S(KC2S_DIALOG_DOODAD)
KD_FIX_PTC_C2S(KC2S_REQUESTOFFLINE)
KD_FIX_PTC_C2S(KC2S_APPLYNPCTALK)
KD_LENTHEN_PTC_C2S(KC2S_SCRIPT_REMOTECALL)
KD_FIX_PTC_C2S(KC2S_APPLY_SYNC_NEIGHBOR)
KD_FIX_PTC_C2S(KC2S_APPLY_SET_VAR)
KD_FIX_PTC_C2S(KC2S_APPLY_REVIVE)
KD_FIX_PTC_C2S(KC2S_DIALOG_SELECT)
KD_FIX_PTC_C2S(KC2S_PICKUP_DOODAD)
KD_FIX_PTC_C2S(KC2S_GET_DELAYDATA)
KD_FIX_PTC_C2S(KC2S_ACTION_AROUND)


// 下面定义协议大小------ server->client------------
KD_LENTHEN_PTC_S2C(KS2C_PLAYERDATA)
KD_FIX_PTC_S2C(KS2C_PING)
KD_FIX_PTC_S2C(KS2C_FRAME_SIGNAL)
KD_FIX_PTC_S2C(KS2C_CHANGE_SERVER)
KD_FIX_PTC_S2C(KS2C_SYNC_SELF)
KD_LENTHEN_PTC_S2C(KS2C_SYNC_DELAYROLEDATA)
KD_FIX_PTC_S2C(KS2C_SYNC_JUMP)
KD_FIX_PTC_S2C(KS2C_SYNC_JUMP_TO)
KD_FIX_PTC_S2C(KS2C_SYNC_MOVE)
KD_FIX_PTC_S2C(KS2C_SYNC_FACE_DIRECTION)
KD_FIX_PTC_S2C(KS2C_SYNC_ACTION_AROUND)
KD_LENTHEN_PTC_S2C(KS2C_SYNC_PLAYER)
KD_LENTHEN_PTC_S2C(KS2C_SYNC_NPC)
KD_FIX_PTC_S2C(KS2C_SYNC_DOODAD)
KD_FIX_PTC_S2C(KS2C_SYNC_LEAVE)
KD_FIX_PTC_S2C(KS2C_SYNC_ADJUST_MOVE)
KD_LENTHEN_PTC_S2C(KS2C_SCRIPT_REMOTECALL)
KD_FIX_PTC_S2C(KS2C_SYNC_TELEPORT)
KD_FIX_PTC_S2C(KS2C_SYNC_VAR_SELF)
KD_FIX_PTC_S2C(KS2C_SYNC_VAR_AROUND)
KD_FIX_PTC_S2C(KS2C_RESPONSE_OFFLINE)

KD_FIX_PTC_S2C(KS2C_SYNC_VISIBLE)
KD_FIX_PTC_S2C(KS2C_SYNC_HEAD_TEXT)
KD_FIX_PTC_S2C(KS2C_SYNC_DEGREE)

KD_LENTHEN_PTC_S2C(KS2C_SHOW_DIALOG)
KD_LENTHEN_PTC_S2C(KS2C_FACE_TALK)
KD_LENTHEN_PTC_S2C(S2C_MESSAGE_NOTIFY)
KD_LENTHEN_PTC_S2C(KS2C_BLACK_MSG)
KD_LENTHEN_PTC_S2C(KS2C_NOTICE_MSG)
