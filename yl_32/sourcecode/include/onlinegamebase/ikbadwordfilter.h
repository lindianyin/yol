/* -------------------------------------------------------------------------
//	文件名		：	ikbadwordfilter.h
//	创建者		：	simon
//	创建时间	：	2011/4/22 14:48:45
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __IKBADWORDFILTER_H__
#define __IKBADWORDFILTER_H__

class IKBadwordFilter
{
public:
	virtual INT Init() = 0;
	virtual INT Uninit() = 0;
	virtual INT LoadFilterFile(CONST CHAR* pFileName) = 0;
	virtual INT AddPlainText(CONST CHAR* pString) = 0;
	virtual INT HasBadword(CONST CHAR* pszText) = 0;
	virtual INT KillBadword(CONST CHAR* pszText, CHAR* pszFilterText, INT nFilterTextLen) = 0;
	virtual INT AddRegularExpression(CONST CHAR* pString, BOOL bIgnoreCase = FALSE) = 0;
	virtual INT Dump() = 0;
	virtual INT Release() = 0;
};

IKBadwordFilter* CreateBadWordFilter();

//extern IKBadwordFilter* g_pBadwordFilter;

#endif
