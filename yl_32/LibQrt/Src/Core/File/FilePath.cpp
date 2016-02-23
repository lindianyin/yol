
#include "CorePrivate.h"
#include "File.h"
#ifdef WIN32
	#include <direct.h>
	#include <mbstring.h>
#else
	#include <unistd.h>
	#include <sys/stat.h>
#endif

#ifdef __GNUC__
void strlwr(char* str)
{
	char c;
	for (int i = 0; 0 != (c = str[i]); i++)
	{
		if (c >= 'A' && c <= 'Z')
			str[i] = c + ('a' - 'A');
	}
}

#endif //#ifdef __GNUC__
//---------------------------------------------------------------------------
#ifdef WIN32
	static char s_szRootPath[MAX_PATH] = "C:";		// 启动路径
	static char s_szCurrPath[MAX_PATH] = "\\";		// 当前路径
	#define		PATH_SPLITTER		'\\'
	#define		PATH_SPLITTER_STR	"\\"
#else
	static char s_szRootPath[MAX_PATH] = "/";		// 启动路径
	static char s_szCurrPath[MAX_PATH] = "/";		// 当前路径
	#define		PATH_SPLITTER		'/'
	#define		PATH_SPLITTER_STR	"/"
#endif

int RemoveTwoPointPath(char* szPath, int nLength)
{
	int nRemove = 0;
	assert(szPath);

#ifdef WIN32
	const char* lpszOld = "\\..\\";
#else
	const char* lpszOld = "/../";
#endif

	char* lpszTarget = strstr(szPath, lpszOld);

	if (lpszTarget)
	{
		const char* lpszAfter = lpszTarget + 3;
		while(lpszTarget > szPath)
		{
			lpszTarget--;
			if ((*lpszTarget) == '\\' ||(*lpszTarget) == '/')
				break;
		}
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (int)(lpszAfter - lpszTarget);
		return RemoveTwoPointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

int RemoveOnePointPath(char* szPath, int nLength)
{
	int nRemove = 0;
	assert(szPath);
#ifdef WIN32
	const char* lpszOld = "\\.\\";
#else
	const char* lpszOld = "/./";
#endif
	char* lpszTarget = strstr(szPath, lpszOld);
	if (lpszTarget)
	{
		char* lpszAfter = lpszTarget + 2;
		memmove(lpszTarget, lpszAfter, (nLength - (lpszAfter - szPath) + 1) * sizeof(char));
		nRemove = (int)(lpszAfter - lpszTarget);
		return RemoveOnePointPath(szPath, nLength - nRemove);
	}

	return nLength - nRemove;
}

int RemoveAllPointPath(char* szPath, int nLength)
{
	return RemoveOnePointPath(szPath, RemoveTwoPointPath(szPath, nLength));
}

//---------------------------------------------------------------------------
// 功能:	设置程序的根路径
// 参数:	lpPathName	必须是argv[0]
//---------------------------------------------------------------------------
void g_SetRootPath(const char* lpPathName)
{
	char excutePath[MAX_PATH];
	g_ExtractFilePath(excutePath, lpPathName);
	if (excutePath[0] != '\0')
	{
		int ret =chdir(excutePath);
		assert(ret == 0);
	}
	char* ret = getcwd(s_szRootPath, MAX_PATH);
	assert(ret != NULL);
}

//---------------------------------------------------------------------------
// 函数:	GetRootPath
// 功能:	取得程序的根路径
// 参数:	lpPathName	路径名
//---------------------------------------------------------------------------
void g_GetRootPath(char* lpPathName)
{
	strcpy(lpPathName, s_szRootPath);
}

//---------------------------------------------------------------------------
// 函数:	SetFilePath
// 功能:	设置当前文件路径
// 参数:	lpPathName	路径名
// 返回:	void
//---------------------------------------------------------------------------
void g_SetFilePath(const char* lpPathName)
{
	// 去掉前面的 "\\"
	if (lpPathName[0] == '\\' ||lpPathName[0] == '/')
	{
		strcpy(s_szCurrPath, lpPathName + 1);
	}
	else
	{
		strcpy(s_szCurrPath, lpPathName);
	}

	// 末尾加上 "\\"
	int len = (int)strlen(s_szCurrPath);
	if (len > 0 && s_szCurrPath[len - 1] != '\\' && s_szCurrPath[len - 1] != '/')
	{
		s_szCurrPath[len] = PATH_SPLITTER;
		s_szCurrPath[len + 1] = 0;
	}
	RemoveAllPointPath(s_szCurrPath, len + 1);
#ifndef WIN32
	//'\\' -> '/' [wxb 2003-7-29]
	for (len = 0; s_szCurrPath[len]; len++)
	{
		if (s_szCurrPath[len] == '\\')
			s_szCurrPath[len] = '/';
	}
#endif
}

//---------------------------------------------------------------------------
// 函数:	GetFilePath
// 功能:	取得当前文件路径
// 参数:	lpPathName	路径名
// 返回:	void
//---------------------------------------------------------------------------
void g_GetFilePath(char* lpPathName)
{
	strcpy(lpPathName, s_szCurrPath);
}

//---------------------------------------------------------------------------
// 函数:	GetFullPath
// 功能:	取得文件的全路径名
// 参数:	lpPathName	路径名
//			lpFileName	文件名
// 返回:	void
//---------------------------------------------------------------------------
void g_GetFullPath(char* lpPathName, const char* lpFileName)
{
#ifdef WIN32
	if (lpFileName[1] == ':' ||	// 文件带有全路径
		(lpFileName[0] == '\\' && lpFileName[1] == '\\'))// 跳过有'\\'这种局域网路径
	{
		strcpy(lpPathName, lpFileName);
		return;
	}
#endif

	// 文件带有部分路径
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		strcpy(lpPathName, s_szRootPath);
		strcat(lpPathName, lpFileName);
		return;
	}

	// 当前路径为全路径
#ifdef WIN32
	if (s_szCurrPath[1] == ':')
	{
		strcpy(lpPathName, s_szCurrPath);
		strcat(lpPathName, lpFileName);
		return;
	}
#endif
	// 当前路径为部分路径
	strcpy(lpPathName, s_szRootPath);
	if(s_szCurrPath[0] != '\\' && s_szCurrPath[0] != '/')
	{
		strcat(lpPathName, PATH_SPLITTER_STR);
	}
	strcat(lpPathName, s_szCurrPath);

	if (lpFileName[0] == '.' && (lpFileName[1] == '\\'||lpFileName[1] == '/') )
		strcat(lpPathName, lpFileName + 2);
	else
		strcat(lpPathName, lpFileName);
}

//---------------------------------------------------------------------------
// 函数:	GetHalfPath
// 功能:	取得文件的半路径名，不带根路径
// 参数:	lpPathName	路径名
//			lpFileName	文件名
// 返回:	void
//---------------------------------------------------------------------------
void g_GetHalfPath(char* lpPathName, const char* lpFileName)
{
	// 文件带有部分路径
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		strcpy(lpPathName, lpFileName);
	}
	else
	{
		strcpy(lpPathName, PATH_SPLITTER_STR);
		strcat(lpPathName, s_szCurrPath);
		strcat(lpPathName, lpFileName);
	}
}

//---------------------------------------------------------------------------
// 函数:	GetPackPath
// 功能:	取得文件在压缩包中的路径名
// 参数:	lpPathName	路径名
//			lpFileName	文件名
// 返回:	void
//---------------------------------------------------------------------------
void g_GetPackPath(char* lpPathName, const char* lpFileName)
{
	// 文件带有部分路径
	if (lpFileName[0] == '\\' || lpFileName[0] == '/')
	{
		strcpy(lpPathName, lpFileName + 1);
	}
	else
	{
		strcpy(lpPathName, s_szCurrPath);
		strcat(lpPathName, lpFileName);
	}
	int len = (int)strlen(lpPathName);
	RemoveAllPointPath(lpPathName, len + 1);
	// 全部转换为小写字母
	strlwr(lpPathName);
}

//---------------------------------------------------------------------------
// 函数:	GetDiskPath
// 功能:	取得CDROM对应的文件路径名
// 参数:	lpPathName	路径名
//			lpFileName	文件名
// 返回:	void
//---------------------------------------------------------------------------
/*CORE_API void g_GetDiskPath(char* lpPathName, char* lpFileName)
{
	strcpy(lpPathName, "C:");
	for (int i = 0; i < 24; lpPathName[0]++, i++)
	{
//		if (GetDriveType(lpPathName) == DRIVE_CDROM)
//			break;
	}
	if (lpFileName[0] == '\\' || lpPathName[0] == '/')
	{
		strcat(lpPathName, lpFileName);
	}
	else
	{
#ifdef WIN32
		strcat(lpPathName, "\\");
#else
		strcat(lpPathName, "/");
#endif
		strcat(lpPathName, s_szCurrPath);
		strcat(lpPathName, lpFileName);
	}
}
*/

//---------------------------------------------------------------------------
// 函数:	CreatePath
// 功能:	在游戏根目录下建立一条路径
// 参数:	lpPathName	路径名
//---------------------------------------------------------------------------
int	g_CreatePath(const char* lpPathName)
{
	if (!lpPathName || !lpPathName[0])
		return false;

	char szFullPath[MAX_PATH] = "";
	g_GetFullPath(szFullPath, lpPathName);
#ifdef unix
	{
		char *ptr = szFullPath;
		while(*ptr)
		{
			if (*ptr == '\\')
				*ptr = '/';
			ptr++;
		}
	}
#endif	// #ifdef unix

	int nResult = true;
	#ifdef WIN32
		CreateDirectoryA(szFullPath, NULL);
		DWORD dwAtt = GetFileAttributesA(szFullPath);
		nResult = ((dwAtt != 0xFFFFFFFF)  && (dwAtt & FILE_ATTRIBUTE_DIRECTORY));
	#else
		mkdir(szFullPath, 0777);
	#endif

	return nResult;
}

//---------------------------------------------------------------------------
// 函数:	g_UnitePathAndName
// 功能:	一个路径和一个文件名，合并到lpGet中形成一个完整的路径文件名
// 参数:	pcszPath 传入路径名 pcszFile 传入文件名 pszFullName 获得的最终完整文件名
// 返回:	void
// 注意：   这里没有考虑字符串的长度，使用的时候要保证字符串的长度足够
//---------------------------------------------------------------------------
void	g_UnitePathAndName(const char *pcszPath, const char *pcszFile, char *pszRetFullName)
{
    if (pszRetFullName)
        pszRetFullName[0] = '\0';

	if (
        (!pcszPath) || 
        (!pcszFile) || 
        (!pszRetFullName)
    )
		return;

	strcpy(pszRetFullName, pcszPath);
	int	nSize = (int)strlen(pszRetFullName);
    if (nSize > 0)
    {
	    if (pszRetFullName[nSize - 1] != '\\')
	    {
		    pszRetFullName[nSize] = '\\';
            nSize++;
		    pszRetFullName[nSize] = '\0';
	    }
    }

	if (pcszFile[0] != '\\')
	{
		strcat(pszRetFullName + nSize, pcszFile);
	}
	else
	{
		strcat(pszRetFullName + nSize, &pcszFile[1]);
	}
}

//---------------------------------------------------------------------------
// 函数:	find if file exists in pak or in hard disk
// 功能:	返回指定的文件是否存在
// 参数:	lpPathName	路径名＋文件名
// 返回:	TRUE－成功，false－失败。
//---------------------------------------------------------------------------
int g_IsFileExist(const char* FileName)
{
	int	bExist = false;
	char szFullName[MAX_PATH];

	if (FileName && FileName[0])
	{
      //  bExist = KG_IsFileExist(FileName);

      //  if (!bExist)
      //  {
		    ////先查是是否在打包文件中
		    //XPackFile::XPackElemFileRef	PackRef;
		    //bExist = g_EnginePackList.FindElemFile(FileName, PackRef);
      //  }

		//在检查是否单独存在文件系统里
		if (bExist == false)
		{
			g_GetFullPath(szFullName, FileName);
			#ifdef	WIN32
				bExist = !(GetFileAttributesA(szFullName) & FILE_ATTRIBUTE_DIRECTORY);// || dword == INVALID_FILE_ATTRIBUTES)
			#else
//				bExist = _sopen(szFullName, _O_BINARY, _SH_DENYNO, 0);
//				if (bExist != -1)
//				{
//					_close(bExist);
//					bExist = true;
//				}
//				else
//				{
//					bExist = false;
//				}
			#endif
		}
	}
	return bExist;
}

//---------------------------------------------------------------------------
// 函数:	String to 32bit Id
// 功能:	文件名转换成 Hash 32bit ID
// 参数:	lpFileName	文件名
// 返回:	FileName Hash 32bit ID
// 注意:	游戏世界和主网关交互数据所用的哈希查找索引也是用
//			的这个函数，所以请修改这个函数时也对应修改主网关
//			中相对应的那个函数。这个函数存在于Common.lib工程的Utils.h
//			中，函数声明为 unsigned int HashStr2ID( const char * const pStr );
//---------------------------------------------------------------------------
unsigned int g_StringHash(const char* pString)
{
	unsigned int Id = 0;
	char c = 0;
	for (int i = 0; pString[i]; i++)
	{
		c = pString[i];
		Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
	}
	return (Id ^ 0x12345678);
}

//把一个字符串小写化，再转为hash数值
unsigned int	g_StringLowerHash(const char* pString)
{
	unsigned int Id = 0;
	char c = 0;
	for (int i = 0; pString[i]; i++)
	{
		c = pString[i];
		if (c >= 'A' && c <= 'Z')
			c += 0x20;	//中文字符的后字节亦可能被转换，人为地加大了重码的概率■
		Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
	}
	return (Id ^ 0x12345678);
}

//把一个字符串小写化，再转为hash数值
unsigned int	g_FileNameHash(const char* pString)
{
	unsigned int Id = 0;
	char c = 0;
	for (int i = 0; pString[i]; i++)
	{
		c = pString[i];
		if (c >= 'A' && c <= 'Z')
			c += 0x20;	//中文字符的后字节亦可能被转换，人为地加大了重码的概率■
		else if (c == '/')
			c = '\\';
		Id = (Id + (i + 1) * c) % 0x8000000b * 0xffffffef;
	}
	return (Id ^ 0x12345678);
}

//---------------------------------------------------------------------------
// 函数:	change file extention
// 功能:	改变文件的扩展名
// 参数:	lpFileName	文件名
//			lpNewExt	新扩展名，不能有'.'
// 返回:	void
//---------------------------------------------------------------------------
void g_ChangeFileExt(char* lpFileName, const char* lpNewExt)
{
	char* pDot = strrchr(lpFileName, '.');
	char* pPathSplit = strrchr(lpFileName, PATH_SPLITTER);

	if (pDot && pPathSplit < pDot)
	{
		strcpy(pDot + 1, lpNewExt);
	}
	else
	{
		strcat(lpFileName, ".");
		strcat(lpFileName, lpNewExt);
	}
}

//---------------------------------------------------------------------------
// 函数:	Extract File Name from path name
// 功能:	取得文件名（不包含路径）
// 参数:	lpFileName	文件名（不包含路径）
//			lpFilePath	文件名（包含路径）
// 返回:	void
//---------------------------------------------------------------------------
void g_ExtractFileName(char* lpFileName, const char* lpFilePath)
{
	int nPos = (int)strlen(lpFilePath);
//	if (nPos < 5)
//		return;
	while ((--nPos) >= 0)
	{
		if (lpFilePath[nPos] == '\\' || lpFilePath[nPos] == '/')
			break;
	}
	strcpy(lpFileName, &lpFilePath[nPos + 1]);
}

//---------------------------------------------------------------------------
// 函数:	Extract File Path from path name
// 功能:	取得路径名
// 参数:	lpFileName	路径名
//			lpFilePath	路径名＋文件名
// 返回:	void
//---------------------------------------------------------------------------
void g_ExtractFilePath(char* lpPathName, const char* lpFilePath)
{
	int nPos = (int)strlen(lpFilePath);
//	if (nLen < 5)
//		return;
//	int nPos = nLen;
	while ((--nPos) > 0)
	{
		if (lpFilePath[nPos] == '\\' ||lpFilePath[nPos] == '/')
			break;
	}
	if (nPos > 0)
	{
		memcpy(lpPathName, lpFilePath, nPos);
		lpPathName[nPos] = 0;
	}
	else
	{
		lpPathName[0] = 0;
	}
}

//---------------------------------------------------------------------------
// 函数:	Get File Path from full path name
// 功能:	从全路径中取得相对路径名
// 参数:	lpPathName		相对路径名
//			lpFullFilePath	全路径名
// 返回:	void
//---------------------------------------------------------------------------
BOOL g_GetFilePathFromFullPath(char* lpPathName, const char* lpFullFilePath)
{
	if (!lpPathName || !lpFullFilePath)
		return FALSE;

	int nRootSize = strlen(s_szRootPath);
	int nFullSize = strlen(lpFullFilePath);

	if (nFullSize <= nRootSize)
		return FALSE;

	memcpy(lpPathName, lpFullFilePath, nRootSize);
	lpPathName[nRootSize] = 0;

	if (stricmp(lpPathName, s_szRootPath))
		return FALSE;

	//再跳过路径后的"\\"
	memcpy(lpPathName, &lpFullFilePath[nRootSize + 1], nFullSize - nRootSize - 1);
	lpPathName[nFullSize - nRootSize - 1] = 0;

	return TRUE;
}

//---------------------------------------------------------------------------
// 函数:	Get relative file path from a path
// 功能:	从某个路径中取得相对路径名
// 参数:	lpRelativePathName		相对路径名
//			lpFilePath				路径名
// 返回:	void
//---------------------------------------------------------------------------
// TODO: Fanghao_Wu 提交到Base是否替代g_GetFilePathFromFullPath？
void g_GetRelativePath(char* lpRelativePathName, const char* lpFilePath)
{
	int nLenRoot = strlen(s_szRootPath);
	int nLenPath = strlen(lpFilePath);
	if (nLenPath > nLenRoot && (memcmp(s_szRootPath, lpFilePath, nLenRoot) == 0))
	{
		strcpy(lpRelativePathName, lpFilePath + nLenRoot);
	}
	else
	{
		memcpy(lpRelativePathName, lpFilePath, nLenPath + 1);
	}
	for (int i = 0; lpRelativePathName[i]; i++)
	{
		if (lpRelativePathName[i] == '/')
			lpRelativePathName[i] = '\\';
	}
}
