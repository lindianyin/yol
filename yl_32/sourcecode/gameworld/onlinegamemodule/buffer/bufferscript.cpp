/* -------------------------------------------------------------------------
//	文件名		：	bufferscript.cpp
//	创建者		：	zhangzhixiong
//	创建时间	：	2012/7/6
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#include "stdafx.h"
#include "buffer/buffermanager.h"
#include "onlinegameworld/kplayermgr.h"
#include "onlinegamemodule/characterskill.h"

namespace BufferScript
{


	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: GetBufferNumber
	// @Description		: 获得Buffer数值
	// @ReturnCode		: 
	// @ArgumentFlag	: dds
	// @ArgumentComment	: nSkillId
	// @ArgumentComment	: nSkillLevel
	// @ArgumentComment	: szBufferCommand
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaGetBufferNumber(QLuaScript &sc)
	{
		BufferNumber* bufNumber = g_cBufferSetting.GetBufferNumber(sc.GetInt(1), sc.GetInt(2), sc.GetStr(3));
		PROCESS_ERROR(bufNumber);
				
		sc.PushNumber(bufNumber->m_nValue);
		sc.PushNumber(bufNumber->m_nSkillCoefficientMin);
		sc.PushNumber(bufNumber->m_nSkillCoefficientMax);

		return 3;
EXIT0:
		sc.PushNumber(0);
		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddBufferToCharacter
	// @Description		: 添加buffer到角色
	// @ReturnCode		: 
	// @ArgumentFlag	: ddddd
	// @ArgumentComment	: 释放者ID
	// @ArgumentComment	: 释放技能模版ID
	// @ArgumentComment	: 接受者ID
	// @ArgumentComment	: 添加BUFFER
	// @ArgumentComment	: 是否同步
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddBufferToCharacter(QLuaScript& sc)
	{
        INT nCasterID   = sc.GetInt(1);
		INT nSkillID    = sc.GetInt(2);
		INT nReceiverID = sc.GetInt(3);
		INT nBufferID   = sc.GetInt(4);
		INT nSync       = sc.GetInt(5);
		INT nPersist    = 0;

		if (sc.GetTopIndex() >= 6)
		{
            nPersist = sc.GetInt(6);
		}

		g_cBufferMgr.AddBufferToCharacter((DWORD)nCasterID, nSkillID, (DWORD)nReceiverID, nBufferID, nPersist, nSync);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: AddGroundBuffer
	// @Description		: 添加地面buffer
	// @ReturnCode		: 
	// @ArgumentFlag	: dddddddd
    // @ArgumentComment	: BUFFER ID
	// @ArgumentComment	: 释放者ID
	// @ArgumentComment	: 释放技能模版ID
	// @ArgumentComment	: 场景ID
	// @ArgumentComment	: buffer坐标X
	// @ArgumentComment	: buffer坐标Y
	// @ArgumentComment	: buffer坐标Z
	// @ArgumentComment	: 是否同步
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaAddGroundBuffer(QLuaScript& sc)
	{
		INT nBufferID   = sc.GetInt(1);
		INT nCasterID   = sc.GetInt(2);
		INT nSkillID    = sc.GetInt(3);
		INT nSceneID    = sc.GetInt(4);
		INT nX          = sc.GetInt(5);
		INT nY          = sc.GetInt(6);
		INT nZ          = sc.GetInt(7);
		INT nSync       = sc.GetInt(8);

		g_cBufferMgr.AddGroundBuffer(nBufferID, (DWORD)nCasterID, nSkillID, nSceneID, nX, nY, nZ, nSync);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RemoveGroundBufferBySceneId
	// @Description		: 删除场景里所有groundbuffer
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nSceneId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRemoveGroundBufferBySceneId(QLuaScript& sc)
	{
		INT nSceneId = sc.GetInt(1);
		g_cBufferMgr.RemoveGroundBufferBySceneId(nSceneId);

		return 0;
	}

	// -------------------------------------------------------------------------
	// @ClientSvr(C/S)	: S
	// @ClassName		: --
	// @LuaApiName		: RemoveGroundBufferByCasterId
	// @Description		: 删除由某个角色释放的GroundBuffer
	// @ReturnCode		: 
	// @ArgumentFlag	: d
	// @ArgumentComment	: nCasterId
	// @LuaMarkEnd
	// -------------------------------------------------------------------------
	INT LuaRemoveGroundBufferByCasterId(QLuaScript& sc)
	{
		INT nCasterId = sc.GetInt(1);
		g_cBufferMgr.RemoveGroundBufferByCasterId(nCasterId);

		return 0;
	}

	#include "bufferscript_table.hpp"
}

BOOL g_RegisterBufferScriptFuns()
{
	return g_cScriptManager.RegisterTableFuncs("BufferApi", BufferScript::arFunction, countof(BufferScript::arFunction));
}