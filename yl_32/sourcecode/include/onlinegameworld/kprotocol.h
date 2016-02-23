
#pragma once

#include "kprotocolsize.h"
#include "ProtocolBasic.h"

#define	KD_PROTOCOL_VERSION					28
#define	KD_PROTOCOL_VERSION_LOWER_LIMIT		28

#define	MAX_PROTOCOL_NUM	256
// 基础模块使用的最大协议号
#define	KD_MAX_BASE_PROTOCOL_HEADER	64

#pragma pack(push, 1)
// -------------------------------------------------------------------------
// 定长协议
struct KPTC_HEADER_BASE
{
	KPTC_HEADER_BASE(BYTE byPtc) : byProtocol(byPtc){}
	// 填协议结构时以下的不用填
	BYTE byProtocolFamily;
	BYTE byProtocol;
};

template<BYTE BY_PROTOCOL>
struct KPTC_HEADER : public KPTC_HEADER_BASE
{
	enum { protocol = BY_PROTOCOL };
	KPTC_HEADER() : KPTC_HEADER_BASE(BY_PROTOCOL) { }
	operator KPTC_HEADER_BASE&() { return *this; }
};


// 变长协议(防止隐式转换不继承KPTC_HEADER_BASE)
struct KLENPTC_HEADER_BASE
{
	KLENPTC_HEADER_BASE(BYTE byPtc) : byProtocol(byPtc){}
	// 填协议结构时以下的不用填
	BYTE byProtocolFamily;
	BYTE byProtocol;
	WORD wDataLen;
};

template<BYTE BY_PROTOCOL>
struct KLENPTC_HEADER : public KLENPTC_HEADER_BASE
{
	enum { protocol = BY_PROTOCOL };
	KLENPTC_HEADER() : KLENPTC_HEADER_BASE(BY_PROTOCOL) { }
	operator KLENPTC_HEADER_BASE&() { return *this; }
};

// -------------------------------------------------------------------------
#pragma pack(pop)

