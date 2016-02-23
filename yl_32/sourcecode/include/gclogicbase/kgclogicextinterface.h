
#pragma once

// 玩家管理接口
class KRole;
// {3C434236-6FB8-4c7e-9B12-2EDA90097EDA}
static const GUID IID_IKG_OnlinePlayerMgr = 
{ 0x3c434236, 0x6fb8, 0x4c7e, { 0x9b, 0x12, 0x2e, 0xda, 0x90, 0x9, 0x7e, 0xda } };

// 枚举玩家接口
interface IKEnumPlayerId
{
	virtual BOOL Next(DWORD &dwPlayerId) PURE;
	virtual VOID Reset() PURE;
};

interface IKOnlinedPlayerMgr
{
	// 获取玩家，可获取在线或服务器启动后曾经登录过的玩家
	virtual KRole* GetPlayerById(DWORD dwPlayerId) PURE;
	// 获取玩家，可获取在线或服务器启动后曾经登录过的玩家
	virtual KRole* GetPlayerByName(LPCSTR szPlayerName) PURE;
	// 枚举玩家，可获取在线或服务器启动后曾经登录过的玩家
	virtual IKEnumPlayerId* EnumPlayer(DWORD dwPlayerId = 0) PURE;
	virtual KRole* GetPlayerByAccount(LPCSTR szAccount) = 0;
};

// -------------------------------------------------------------------------

