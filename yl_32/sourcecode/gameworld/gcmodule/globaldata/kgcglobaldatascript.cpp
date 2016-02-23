/* -------------------------------------------------------------------------
//	文件名		：	kglobaldatascript.cpp
//	创建者		：	liuchang
//	创建时间	：	2010-8-20 14:27:39
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifdef GAME_CENTER

#include "stdafx.h"
#include "kgcglobaldatamodule.h"
#include "onlinegamebase/kscriptmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

namespace KGCGlobalDataNameSpace
{
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: SetIntValue
// @Description		: 设置一个全局整形变量
// @ReturnCode		: d
// @ArgumentFlag	: dd
// @ArgumentComment	: Key
// @ArgumentComment	: Value
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaSetIntValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		BOOL bRet = g_cGlobalDataModule.SetIntValue(HIWORD(dwKey), LOWORD(dwKey), sc.GetInt(2), -1);
		sc.PushNumber(bRet);
		return 1;
	}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: AddIntValue
// @Description		: 累加一个全局整形变量
// @ReturnCode		: d
// @ArgumentFlag	: dd
// @ArgumentComment	: Key
// @ArgumentComment	: Value
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaAddIntValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		BOOL bRet = g_cGlobalDataModule.AddIntValue(HIWORD(dwKey), LOWORD(dwKey), sc.GetInt(2), -1);
		sc.PushNumber(bRet);
		return 1;
	}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: GetIntValue
// @Description		: 获得一个全局整形变量
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: Key
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaGetIntValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		INT nValue = 0;
		BOOL bRet = g_cGlobalDataModule.GetIntValue(HIWORD(dwKey), LOWORD(dwKey), nValue);
		if (!bRet)
			return 0;
		sc.PushNumber(nValue);

		return 1;
	}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: SetStrValue
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: ds
// @ArgumentComment	: Key
// @ArgumentComment	: Value
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaSetStrValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		INT nDataLen = 0;
		LPCSTR pszData = sc.GetLStr(2, nDataLen);
		if (nDataLen > KD_BIN_DATA_MAX_LEN)
			return 0;

		BOOL bRet = g_cGlobalDataModule.SetBinaryValue(HIWORD(dwKey), LOWORD(dwKey), (BYTE*)pszData, nDataLen, -1);

		sc.PushNumber(bRet);
		return 1;
	}

// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: GetStrValue
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: Key
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaGetStrValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		CHAR acStrData[KD_BIN_DATA_MAX_LEN];
		WORD wStrLen = sizeof(acStrData);
		memset(acStrData, 0, wStrLen);

		BOOL bRet = g_cGlobalDataModule.GetBinaryValue(HIWORD(dwKey), LOWORD(dwKey), (BYTE*)acStrData, wStrLen);
		if (!bRet)
			return 0;
		
		acStrData[KD_BIN_DATA_MAX_LEN-1] = '\0';
		sc.PushString(acStrData);

		return 1;
	}
// -------------------------------------------------------------------------
// @ClientSvr(C/S)	: S
// @ClassName		: --
// @LuaApiName		: DelStrValue
// @Description		: 
// @ReturnCode		: 
// @ArgumentFlag	: d
// @ArgumentComment	: Key
// @ArgumentComment	: 
// @LuaMarkEnd
// -------------------------------------------------------------------------
	INT LuaDelStrValue(QLuaScript &sc)
	{
		DWORD dwKey = sc.GetInt(1);
		BOOL bRet = g_cGlobalDataModule.DeleteBinaryValue(HIWORD(dwKey), LOWORD(dwKey), -1);
		sc.PushNumber(bRet);

		return 1;
	}
	
	#include "kgcglobaldatascript_table.hpp"
}


BOOL g_RegisterKGlobalDataScriptFun(KScriptManager &rcScriptMgr)
{
	return rcScriptMgr.RegisterTableFuncs("KGlobalData", KGCGlobalDataNameSpace::arFunction, countof(KGCGlobalDataNameSpace::arFunction));
}
// -------------------------------------------------------------------------
#endif
