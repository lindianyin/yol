
#include "CorePrivate.h"
#include "QStrBase.h"
#ifdef WIN32
#include <string.h>
#else
#include <strings.h>
#endif

int QStrLen(LPCSTR lpStr)
{
	return strlen(lpStr);
}
//---------------------------------------------------------------------------
// 函数:	StrEnd
// 功能:	返回字符串结尾指针
// 参数:	lpStr	:	字符串开头的指针
// 返回:	lpEnd	:	字符串末尾的指针
//---------------------------------------------------------------------------
LPSTR QStrEnd(LPCSTR lpStr)
{
	return (char *)lpStr + strlen(lpStr);
}
//---------------------------------------------------------------------------
// 函数:	StrCpy
// 功能:	字符串拷贝
// 参数:	lpDest	:	目标字符串
//			lpSrc	:	源字符串
// 返回:	void
//---------------------------------------------------------------------------
void QStrCpy(LPSTR lpDest, LPCSTR lpSrc)
{
	strcpy(lpDest, lpSrc);
}
//---------------------------------------------------------------------------
// 函数:	StrCpyLen
// 功能:	字符串拷贝,有最大长度限制
// 参数:	lpDest	:	目标字符串
//			lpSrc	:	源字符串
//			nMaxLen	:	最大长度
// 返回:	void
//---------------------------------------------------------------------------
void QStrCpyLen(LPSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
	strncpy(lpDest, lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// 函数:	StrCat
// 功能:	字符串末尾追加另一个字符串
// 参数:	lpDest	:	目标字符串
//			lpSrc	:	源字符串
// 返回:	void
//---------------------------------------------------------------------------
void QStrCat(LPSTR lpDest, LPCSTR lpSrc)
{
	register LPSTR lpEnd;

	lpEnd = QStrEnd(lpDest);
	QStrCpy(lpEnd, lpSrc);
}
//---------------------------------------------------------------------------
// 函数:	StrCatLen
// 功能:	字符串末尾追加另一个字符串,有最大长度限制
// 参数:	lpDest	:	目标字符串
//			lpSrc	:	源字符串
//			nMaxLen	:	最大长度
// 返回:	void
//---------------------------------------------------------------------------
void QStrCatLen(LPSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
	register LPSTR lpEnd;

	lpEnd = QStrEnd(lpDest);
	QStrCpyLen(lpEnd, lpSrc, nMaxLen);
}
//---------------------------------------------------------------------------
// 函数:	StrCmp
// 功能:	字符串比较
// 参数:	lpDest	:	字符串1	
//			lpSrc	:	字符串2
// 返回:	TRUE	:	相同
//			FALSE	:	不同
//---------------------------------------------------------------------------
BOOL QStrCmp(LPCSTR lpDest, LPCSTR lpSrc)
{
	return strcmp(lpDest, lpSrc) == 0;
}
//---------------------------------------------------------------------------
// 函数:	StrCmpLen
// 功能:	字符串比较,限定长度
// 参数:	lpDest	:	字符串1	
//			lpSrc	:	字符串2
//			nLen	:	长度
// 返回:	TRUE	:	相同
//			FALSE	:	不同
//---------------------------------------------------------------------------
BOOL QStrCmpLen(LPCSTR lpDest, LPCSTR lpSrc, int nMaxLen)
{
	return strncmp(lpDest, lpSrc, nMaxLen) == 0;
}
//---------------------------------------------------------------------------
// 函数:	StrUpper
// 功能:	小写字母转大写字母
// 参数:	lpDest	:	字符串
// 返回:	void
//---------------------------------------------------------------------------
void QStrUpper(LPSTR lpDest)
{
     char *ptr = lpDest;
     while(*ptr) {
         if(*ptr >= 'a' && *ptr <= 'z') 
			 *ptr += 'A' - 'a';
//          *ptr = toupper(*ptr);
          ptr++;
     }
}
//---------------------------------------------------------------------------
// 函数:	StrLower
// 功能:	大写字母转小写字母
// 参数:	lpDest	:	字符串
// 返回:	void
//---------------------------------------------------------------------------
void QStrLower(LPSTR lpDest)
{
     char *ptr = lpDest;
     while(*ptr) {
         if(*ptr >= 'A' && *ptr <= 'Z')
			*ptr += 'a' - 'A';
//          *ptr = tolower(*ptr);
          ptr++;
     }
}
//---------------------------------------------------------------------------
void QStrRep(LPSTR lpDest, LPSTR lpSrc, LPSTR lpRep)
{
	int		nSrcLen = QStrLen(lpSrc);
	int		nDestLen = QStrLen(lpDest);
	int		nMaxLen = nDestLen - nSrcLen + QStrLen(lpRep) + 1;
	char	*pStart = NULL;
        int i;
	for (i = 0; i < nDestLen - nSrcLen; i++)
	{
		if (QStrCmpLen(&lpDest[i], lpSrc, nSrcLen))
			break;
	}
	if (i == nDestLen - nSrcLen)
		return;

	pStart = new char[nMaxLen];

	if (i != 0)
	{
		QStrCpyLen(pStart, lpDest, i);
		QStrCat(pStart, lpRep);
		QStrCat(pStart, &lpDest[i + nSrcLen]);
	}
	else
	{
		QStrCpy(pStart, lpRep);
		QStrCat(pStart, &lpDest[nSrcLen]);
	}
	QStrCpy(lpDest, pStart);
	if (pStart)
	{
		delete [] pStart;
		pStart = NULL;
	}
}

int QStrCaseCmp(LPCSTR pszSrc, LPCSTR pszDst)
{
#ifdef WIN32
	return stricmp(pszSrc, pszDst);
#else
	return strcasecmp(pszSrc, pszDst);
#endif
}

int QStrNCaseCmp(LPCSTR pszSrc, LPCSTR pszDst, int nLen)
{
#ifdef WIN32
	return strnicmp(pszSrc, pszDst, nLen);
#else
	return strncasecmp(pszSrc, pszDst, nLen);
#endif
}

