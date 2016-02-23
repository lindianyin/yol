/* -------------------------------------------------------------------------
//	文件名		：	kglobaldataptc.h
//	创建者		：	liuchang
//	创建时间	：	2010-8-19 14:57:23
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KGLOBALDATAPTC_H__
#define __KGLOBALDATAPTC_H__

#include "onlinegameworld/kprotocol.h"
#include "gclogicbase/kgclogicbaseprotocol.h"
// -------------------------------------------------------------------------
#pragma  pack(push, 1)

// -------------------------------------------------------------------------
// GS->GC
enum KE_GLOBAL_DATA_S2G_PROTOCOL
{
	emKGLOBAL_DATA_PTC_S2G_RESERVE = emKGCBASE_PTC_COUNT,
	emKGLOBAL_DATA_PTC_S2G_SET_INT_VALUE,
	emKGLOBAL_DATA_PTC_S2G_ADD_INT_VALUE,
	emKGLOBAL_DATA_PTC_S2G_SET_BIN_VALUE,
	emKGLOBAL_DATA_PTC_S2G_DEL_BIN_VALUE,
};

struct KGLOBALDATA_S2G_SET_INT_VALUE : KPTC_HEADER<emKGLOBAL_DATA_PTC_S2G_SET_INT_VALUE>
{
	DWORD dwKey;
	INT nValue;
};

struct KGLOBALDATA_S2G_ADD_INT_VALUE : KPTC_HEADER<emKGLOBAL_DATA_PTC_S2G_ADD_INT_VALUE>
{
	DWORD dwKey;
	INT nValue;
};


struct KGLOBALDATA_S2G_SET_BIN_VALUE : KLENPTC_HEADER<emKGLOBAL_DATA_PTC_S2G_SET_BIN_VALUE>
{
	DWORD dwKey;
	WORD wLength;
	BYTE abValue[];
};

struct KGLOBALDATA_S2G_DEL_BIN_VALUE : KPTC_HEADER<emKGLOBAL_DATA_PTC_S2G_DEL_BIN_VALUE>
{
	DWORD dwKey;
};

// -------------------------------------------------------------------------
// GC->GS
enum KE_GLOBAL_DATA_G2S_PROTOCOL
{
	emKGLOBAL_DATA_PTC_G2S_NONE,
	emKGLOBAL_DATA_PTC_G2S_INT_VALUE,
	emKGLOBAL_DATA_PTC_G2S_BIN_VALUE,
	emKGLOBAL_DATA_PTC_G2S_DEL_BIN_VALUE,
	emKGLOBAL_DATA_PTC_G2S_BATCH_INT_VALUE,
	emKGLOBAL_DATA_PTC_G2S_BATCH_BIN_VALUE,
};

struct KGLOBALDATA_G2S_INT_VALUE : KPTC_HEADER<emKGLOBAL_DATA_PTC_G2S_INT_VALUE>
{
	WORD wGroupId;
	WORD wKey;
	INT nValue;
};


struct KGLOBALDATA_G2S_BIN_VALUE : KLENPTC_HEADER<emKGLOBAL_DATA_PTC_G2S_BIN_VALUE>
{
	WORD wGroupId;
	WORD wKey;
	WORD wLength;
	BYTE abValue[];
};

struct KGLOBALDATA_G2S_DEL_BIN_VALUE : KPTC_HEADER<emKGLOBAL_DATA_PTC_G2S_DEL_BIN_VALUE>
{
	WORD wGroupId;
	WORD wKey;
};

struct INT_ITEM
{
	DWORD dwKey;
	INT nValue;
};

struct KGLOBALDATA_G2S_BATCH_INT_VALUE : KLENPTC_HEADER<emKGLOBAL_DATA_PTC_G2S_BATCH_INT_VALUE>
{
	WORD wItemCount;
	INT_ITEM acIntItem[];
};

struct BIN_ITEM
{
	DWORD dwKey;
	WORD wLength;
	BYTE abValue[1];
};

struct KGLOBALDATA_G2S_BATCH_BIN_VALUE : KLENPTC_HEADER<emKGLOBAL_DATA_PTC_G2S_BATCH_BIN_VALUE>
{
	WORD wItemCount;
	BIN_ITEM acBinItem[];
};

#pragma  pack(pop)

// -------------------------------------------------------------------------

#endif /* __KGLOBALDATAPTC_H__ */
