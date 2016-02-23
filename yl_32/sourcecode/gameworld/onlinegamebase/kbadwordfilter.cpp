/* -------------------------------------------------------------------------
//	文件名		：	kbadwordfilter.cpp
//	创建者		：	simon
//	创建时间	：	2011/4/11 14:49:13
//	功能描述	：	
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "kbadwordfilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

IKBadwordFilter* CreateBadWordFilter()
{
	KBadwordFilter* pBadWordFilter = new KBadwordFilter();
	return pBadWordFilter;
}

// -------------------------------------------------------------------------

//KBadwordFilter g_cBadwordFilter;
//IKBadwordFilter* g_pBadwordFilter = &g_cBadwordFilter;
