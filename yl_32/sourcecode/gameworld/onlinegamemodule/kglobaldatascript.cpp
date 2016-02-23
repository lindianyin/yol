
#ifdef GAME_SERVER

#include "stdafx.h"
#include "onlinegamebase/kscriptmanager.h"
#include "kgsglobaldatamodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------

namespace KGlobalDataNameSpace
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
		BOOL bRet = g_cGlobalDataModule.SetIntValue(sc.GetInt(1), sc.GetInt(2));
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
		BOOL bRet = g_cGlobalDataModule.AddIntValue(sc.GetInt(1), sc.GetInt(2));
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
		INT nValue = 0;
		BOOL bRet = g_cGlobalDataModule.GetIntValue(sc.GetInt(1), nValue);
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
		INT nDataLen = 0;
		LPCSTR pszData = sc.GetLStr(2, nDataLen);
		if (nDataLen > KD_BIN_DATA_MAX_LEN)
			return 0;

		BOOL bRet = g_cGlobalDataModule.SetBinaryValue(sc.GetInt(1), (BYTE*)pszData, nDataLen);

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
		CHAR acStrData[KD_BIN_DATA_MAX_LEN];
		WORD wStrLen = sizeof(acStrData);
		memset(acStrData, 0, wStrLen);

		BOOL bRet = g_cGlobalDataModule.GetBinaryValue(sc.GetInt(1), (BYTE*)acStrData, wStrLen);
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
		BOOL bRet = g_cGlobalDataModule.DeleteBinaryValue(sc.GetInt(1));
		sc.PushNumber(bRet);

		return 1;
	}
	
	#include "kglobaldatascript_table.hpp"
}


BOOL g_RegisterKGlobalDataScriptFun()
{
	return g_cScriptManager.RegisterTableFuncs("KGlobalData", KGlobalDataNameSpace::arFunction, countof(KGlobalDataNameSpace::arFunction));
}
// -------------------------------------------------------------------------
#endif
