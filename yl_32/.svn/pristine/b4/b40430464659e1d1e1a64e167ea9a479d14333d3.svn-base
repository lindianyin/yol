
#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "Misc/randomgen.h"
#include "Misc/Misc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
namespace KLibScriptNameSpace
{

// 格式化输出时间（返回：字符串）
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: FormatTime2String
// @Description		: 根据给定的秒数，转换成制定格式的字符串返回
// @ReturnCode		: str：格式化后的时间表示
// @ArgumentFlag	: sd
// @ArgumentComment	: szParam：格式
// @ArgumentComment	: tTime：秒数
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaFormatTime2String(QLuaScript& sc)
{
	LPCSTR szParam = sc.GetStr(1);
	time_t tTime = (time_t)sc.GetInt(2);
	tm* pStm = localtime(&tTime);

	CHAR str[256] = "";
	if (!strftime(str, sizeof(str), szParam, pStm))
		puts("invalid \'date\' format");

	sc.PushString(str);
	return 1;
}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetRootPath
// @Description		: 
// @ReturnCode		: 返回根目录
// @ArgumentFlag	: 
// @ArgumentComment	: 
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetRootPath(QLuaScript& sc)
{
	CHAR lszCurrentDirectory[200];
	g_GetRootPath(lszCurrentDirectory);
	INT nLen = strlen(lszCurrentDirectory);
	if (lszCurrentDirectory[nLen - 1] == '\\' || lszCurrentDirectory[nLen - 1] == '/')
		lszCurrentDirectory[nLen - 1] = 0;
	sc.PushString(lszCurrentDirectory);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SplitStr
// @Description		: 以一个分隔符切分字符串
// @ReturnCode		: 切分的子字符串数组，失败返回nil
// @ArgumentFlag	: ss
// @ArgumentComment	: 要切分的字符串
// @ArgumentComment	: 切分符（取参数第一个字符）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSplitStr(QLuaScript& sc)
{
	INT nResult		= 0;
	LPCSTR pszSrc	= sc.GetStr(1);
	CHAR cSplit		= *sc.GetStr(2);	// 参数表中的ss已经可以保证GetStr(1)、GetStr(2)必然得到非空指针，这里不再检查
	INT nCount		= 0;

	LOG_PROCESS_ERROR(cSplit);

	sc.PushTable();
	nResult ++;

	for (LPCSTR pszSubStr = pszSrc; ; ++pszSubStr)
	{
		if (*pszSubStr && cSplit != *pszSubStr)
			continue;

		sc.PushLString(pszSrc, pszSubStr - pszSrc);
		sc.SetTableIndex(++nCount);

		if (!*pszSubStr)
			break;
		
		pszSrc = pszSubStr + 1;
	}

EXIT0:
	return nResult;
}


// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: Number2Int
// @Description		: 把数字转换成INT（取整的同时转为有符号数字）
// @ReturnCode		: 整数
// @ArgumentFlag	: d
// @ArgumentComment	: dNumber:要取整的数
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaNumber2Int(QLuaScript& sc)
{
	sc.PushNumber(sc.GetInt(1));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: Number2UInt
// @Description		: 把数字转换成UINT（取整的同时转为无符号数字）
// @ReturnCode		: 转换后的数
// @ArgumentFlag	: d
// @ArgumentComment	: dNumber:被转换的数
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaNumber2UInt(QLuaScript& sc)
{
	sc.PushNumber((DWORD)sc.GetNum(1));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: FileName2Id
// @Description		: 返回指定文件的哈希ID
// @ReturnCode		: DWORD 哈希ID, 失败返回nil
// @ArgumentFlag	: s
// @ArgumentComment	: pszFile：文件名
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaFileName2Id(QLuaScript& sc)
{
	LPCSTR pszFile = (CHAR*)sc.GetStr(1);
	INT bRet = 0;
	PROCESS_ERROR(pszFile);
	sc.PushNumber(g_FileNameHash(pszFile));
	bRet = 1;
EXIT0:
	if (bRet == 0)
		sc.PushNull();
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: String2Id
// @Description		: 返回指定字符串的哈希ID
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: str: 字符串
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaString2Id(QLuaScript& sc)
{
	DWORD dwId		= 0;
	LPCSTR szName	= NULL;
	szName	= sc.GetStr(1);
	PROCESS_ERROR(szName && szName[0]);
	dwId	= g_StringHash((LPSTR)szName);
EXIT0:
	sc.PushNumber(dwId);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetValByStr
// @Description		: 按照字符串取得Table成员变量，支持多级“.”和“:”。
// @ReturnCode		: value
// @ArgumentFlag	: s
// @ArgumentComment	: 字符串
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetValByStr(QLuaScript& sc)
{
	return sc.GetGlobalF(sc.GetStr(1));
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: SaveValue2Buffer
// @Description		: 将一个值转换成可以储存、传送的Buffer
// @ReturnCode		: Buffer
// @ArgumentFlag	: {}
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaSaveValue2Buffer(QLuaScript& sc)
{
	CHAR szBuf[1024];
	INT nLen	= sc.SaveValue2Buffer((LPBYTE)szBuf, sizeof(szBuf), 1);
	if (nLen <= 0)
		return 0;

	sc.PushLString(szBuf, nLen);
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: LoadBuffer2Value
// @Description		: 将一个Buffer换成值
// @ReturnCode		: value
// @ArgumentFlag	: s
// @ArgumentComment	: Buffer
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaLoadBuffer2Value(QLuaScript& sc)
{
	INT nLen;
	LPCSTR pszBuf	= sc.GetLStr(1, nLen);
	if (!pszBuf)
		return 0;

	if (!sc.LoadBuffer2Value((LPCBYTE)pszBuf, nLen))
		return 0;

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetBitTB
// @Description		: 将一个数值装换成一个表示每个二进制位的Table
// @ReturnCode		: table
// @ArgumentFlag	: d
// @ArgumentComment	: Number
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetBitTB(QLuaScript& sc)
{
	if (!sc.IsNumber(1))
		return 0;
	DWORD dwValue	= (DWORD)sc.GetInt(1);
	sc.PushTable();
	for (INT i = 0; i < 32; i++)
	{
		sc.PushNumber((dwValue >> i) & 1);
		sc.SetTableIndex(i);
	}

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: LoadTabFile
// @Description		: 加载table文件
// @ReturnCode		: table对象
// @ArgumentFlag	: s
// @ArgumentComment	: 文件名
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaLoadTabFile(QLuaScript& sc)
{
	LPCSTR pszFileName = sc.GetStr(1);
	CHECK_REPORT(pszFileName);

	{
		QTabFile tabFile;
		if (!tabFile.Load(pszFileName))
			return 0;

		INT nWidth	= tabFile.GetWidth();
		INT nHeight	= tabFile.GetHeight();
		CHAR szBuffer[10240];

		sc.PushTable();
		for (INT i = 1; i <= nHeight; i++)
		{
			sc.PushTable();
			for (INT j = 1; j <= nWidth; j++)
			{
				tabFile.GetString(i, j, "", szBuffer, sizeof(szBuffer));
				sc.PushString(szBuffer);
				sc.SetTableIndex(j);
			}
			sc.SetTableIndex(i);
		}

		return 1;
	}

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: LoadIniFile
// @Description		: 加载ini文件
// @ReturnCode		: ini对象
// @ArgumentFlag	: s
// @ArgumentComment	: 文件名
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaLoadIniFile(QLuaScript& sc)
{
	LPCSTR pszFileName	= sc.GetStr(1);
	CHECK_REPORT(pszFileName);

	{
		QIniFile iniFile;
		if (!iniFile.Load(pszFileName))
			return 0;

		CHAR szSection[256];
		CHAR szKey[256];
		CHAR szValue[10240];

		sc.PushTable();
		szSection[0]	= '\0';
		while (iniFile.GetNextSection(szSection, szSection))
		{
			sc.PushTable();
			szKey[0]	= '\0';
			while (iniFile.GetNextKey(szSection, szKey, szKey))
			{
				iniFile.GetString(szSection, szKey, "", szValue, sizeof(szValue));
				sc.PushString(szValue);
				sc.SetTableField(szKey);
			}
			sc.SetTableField(szSection);
		}

		return 1;
	}

EXIT0:
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: FindStr
// @Description		: 在字符串中搜索子串，会跳过不安全的双字节汉字
// @ReturnCode		: nil表示未找到；否则返回找到的子串起始位置（下标从1开始，不是0）
// @ArgumentFlag	: ss[d]
// @ArgumentComment	: szStr: 原字符串
// @ArgumentComment	: szSub: 要查找的子串
// @ArgumentComment	: nStart: 搜索起始位置（下标从1开始，不是0）
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaFindStr(QLuaScript& sc)
{
	INT nResult		= 0;

	INT nStrLen		= 0;
	LPCSTR pszStr	= sc.GetLStr(1, nStrLen);
	INT nSubLen		= 0;
	LPCSTR pszSub	= sc.GetLStr(2, nSubLen);
	INT nStart		= 0;

	LPCSTR pszFindEnd	= NULL;

	LOG_PROCESS_ERROR(nStrLen > 0);
	LOG_PROCESS_ERROR(nSubLen > 0);

	if (sc.GetTopIndex() >= 3)
	{
		nStart	= sc.GetInt(3) - 1;
		LOG_PROCESS_ERROR(nStart >= 0);
		LOG_PROCESS_ERROR(nStart <= nStrLen);
	}

	pszFindEnd	= pszStr + nStrLen - nSubLen;

	for (LPCSTR pszFind = pszStr + nStart; pszFind <= pszFindEnd; ++pszFind)
	{
		if (memcmp(pszFind, pszSub, nSubLen) == 0)
		{
			sc.PushNumber(pszFind - pszStr + 1);
			nResult	++;
			break;
		}

		// TODO: FanZai	未进行多语言的判断
		if (*pszFind < 0)	// 汉字多加1
			pszFind	++;
	}

EXIT0:
	return nResult;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetScriptMemSize
// @Description		: 得到脚本已分配内存大小
// @ReturnCode		: nSize
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetScriptMemSize(QLuaScript& sc)
{
	sc.PushNumber(sc.GetMemSize());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetScriptAllocTimes
// @Description		: 得到脚本已分配内存次数
// @ReturnCode		: nTimes
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetScriptAllocTimes(QLuaScript& sc)
{
	sc.PushNumber(sc.GetAllocTimes());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetScriptBlockMaxSize
// @Description		: 得到脚本已分配的内存块的最大大小
// @ReturnCode		: nSize
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetScriptBlockMaxSize(QLuaScript& sc)
{
	sc.PushNumber(sc.GetBlockMaxSize());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetScriptAllocMemSize
// @Description		: 得到脚本全部已分配的内存块大小
// @ReturnCode		: nSize
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetScriptAllocMemSize(QLuaScript& sc)
{
	sc.PushNumber(sc.GetAllocMemSize());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetScriptFreeMemSize
// @Description		: 得到脚本未使用的缓冲内存块大小
// @ReturnCode		: nSize
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetScriptFreeMemSize(QLuaScript& sc)
{
	sc.PushNumber(sc.GetFreeMemSize());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ReleaseScriptFreeMemory
// @Description		: 释放脚本不用的缓冲内存
// @ReturnCode		: 1、成功；0、失败
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaReleaseScriptFreeMemory(QLuaScript& sc)
{
	sc.PushNumber(sc.ReleaseAllFreeMemory());

	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: ClrScr
// @Description		: 清空控制台信息
// @ReturnCode		: 
// @ArgumentFlag	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaClrScr(QLuaScript& sc)
{
#ifdef _DEBUG
#ifdef WIN32
	system("cls");
#else
	printf("\33[2J");			// 未测试过
#endif
#endif
	return 0;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: HashDword
// @Description		: 获得参数对应的32位Hash值，若连续用上一个值获取，相当于得到一连串伪随机数
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: 32位参数(随机种子)
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaHashDword(QLuaScript& sc)
{
	DWORD dwSeed = (DWORD)sc.GetInt(1);
	sc.PushNumber(g_HashWord(dwSeed));
	return 1;
}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: 
// @ClassName		: --
// @LuaApiName		: GetMD5String
// @Description		: 用MD5加密一个字符串
// @ReturnCode		: 
// @ArgumentFlag	: s
// @ArgumentComment	: 一个要加密的字符串
// @LuaMarkEnd
// -------------------------------------------------------------------------
INT LuaGetMD5String(QLuaScript& sc)
{
	LPCSTR pszString = sc.GetStr(1);
	if (pszString == NULL)
		return 0;

	CHAR szMD5[64] = {0};

	Misc_MD5String(szMD5, (BYTE*)pszString, strlen(pszString));

	sc.PushString(szMD5);
	return 1;
}
//
//// -------------------------------------------------------------------------
//// @ClientSvr(C/S)	: 
//// @ClassName		: --
//// @LuaApiName		: Base64Encode
//// @Description		: base64编码
//// @ReturnCode		: 
//// @ArgumentFlag	: s
//// @ArgumentComment	: 一个要编码的字符串
//// @LuaMarkEnd
//// -------------------------------------------------------------------------
//INT LuaBase64Encode(QLuaScript& sc)
//{
//	LPCSTR pszString = sc.GetStr(1);
//	if (pszString == NULL)
//		return 0;
//	INT nLen = strlen(pszString);
//	LPCSTR pszBuffer = NULL;
//	INT nEstimateSize = 0;
//	INT nGetSize = 0;
//	EstimateEncodeBufferSize(nLen, &nEstimateSize);
//	pszBuffer = new CHAR[nEstimateSize + 1];
//	if (pszBuffer == NULL)
//		return 0;
//	Base64Encode(pszString, nLen, (char*)pszBuffer, nEstimateSize, &nGetSize, TRUE);
//	sc.PushLString(pszBuffer, nGetSize);
//	SAFE_DELETE_ARRAY(pszBuffer);
//	return 1;
//}
//
//// -------------------------------------------------------------------------
//// @ClientSvr(C/S)	: 
//// @ClassName		: --
//// @LuaApiName		: Base64Decode
//// @Description		: base64解码
//// @ReturnCode		: 
//// @ArgumentFlag	: s
//// @ArgumentComment	: 一个要解码的字符串
//// @LuaMarkEnd
//// -------------------------------------------------------------------------
//INT LuaBase64Decode(QLuaScript& sc)
//{
//	LPCSTR pszIn = sc.GetStr(1);
//	if (pszIn == NULL)
//		return 0;
//	INT nLen = strlen(pszIn);
//	LPCSTR pszOut = NULL;
//	INT nMaxSize = nLen + 1;
//	INT nGetSize = 0;
//	pszOut = new CHAR[nMaxSize];
//	if (pszOut == NULL)
//		return 0;
//	Base64Decode(pszIn, nLen, (char*)pszOut, nMaxSize, &nGetSize);
//	sc.PushLString(pszOut, nGetSize);
//	SAFE_DELETE_ARRAY(pszOut);
//	return 1;
//}

#include "klibscript_table.hpp"
}; // KLibScriptNameSpace

BOOL g_RegisterKLibScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KLib", KLibScriptNameSpace::arFunction, countof(KLibScriptNameSpace::arFunction));
}
// -------------------------------------------------------------------------
