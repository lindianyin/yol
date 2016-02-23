
#pragma once

#include "Core/luna.h"
#include "gclogicbase/kgcplayercommon.h"
#include "LogServerAgency.h"




enum KROLE_STATE
{
	rsInvalid = 0,

	rsOffline,
	rsOnline,

	//LG = Login，登录流程相关状
	rsLG_WaitForPermit,
	rsLG_WaitForLogin,

	//CG = ChangeGameserver，跨服流程相关状态
	rsCG_WaitForCreateMap,
	rsCG_WaitForPlayerData,
	rsCG_WaitForGuid,
	rsCG_WaitForEnter_Leave,
	rsCG_WaitForSrcLeave,
	rsCG_WaitForDestEnter,
	rsCG_WaitForDestTimeout,
	rsCG_WaitForSrcTimeout,

	rsBZ_Request,
	rsBZ_WaitForPermit,
	rsBZ_WaitForLogin,
	rsBZ_Kick,

	rsTotal
};

#pragma pack(1)
class KRole : public KGcPlayerCommon
{
public:
	KRole(void);
	~KRole(void);

	KROLE_STATE GetRoleState(void);
	void SetRoleState(KROLE_STATE eDestState);

    BOOL IsOnline();

    BOOL UpdateMapCopyOwnerForTeamLeader();

    BOOL OnLeaveGS(int nConnIndex);

    BOOL CallLoginScript();
    BOOL CallLogoutScript();

public:    
    //int  LuaIsOnline(Lua_State* L);

public:

	const char* 			m_pszAccount;	            // 角色的帐号名
	char					m_szName[_NAME_LEN];	    // 角色的角色名
	DWORD					m_dwPlayerID;				// 角色的数据库流水号

	int						m_nConnIndex;				// 角色所在的GS的连接号
	DWORD					m_dwMapID;					// 角色所在的地图编号
	int					    m_nMapCopyIndex;			// 角色所在的地图副本号

    int                     m_nDestConnIndex;
    DWORD                   m_dwDestMapID;
    int                     m_nDestMapCopyIndex;

    DWORD                   m_dwSystemTeamID;           // 系统创建的队伍的ID
    DWORD                   m_dwTeamID;                 // 玩家所属队伍的ID
    //KCAMP                   m_eCamp;                    // 玩家所属阵营
    DWORD                   m_dwTongID;                 // 所属帮会ID

    DWORD                   m_dwForceID;                // 所属势力ID
	INT                     m_nFightScore;              // 战斗力
    time_t                  m_nLastSaveTime;
    time_t                  m_nCreateTime;
	time_t					m_nLastTeamActiveTime;		// 上次还在队伍中的时间
    BYTE                    m_byType;                   // 角色类型，取ROLE_TYPE枚举值
    BYTE                    m_byLevel;                  // 等级

private:
	KROLE_STATE				m_eRoleState;			    // 角色的游戏状态

public:
	DWORD GetTeamId() { return m_dwTeamID; }
	DWORD GetAutoTeamId() { return m_dwSystemTeamID ? m_dwSystemTeamID : m_dwTeamID; }

    const char* getAccount(void) { return m_pszAccount; }
    int getLevel(void) {return (int)m_byLevel; }
    int getType(void) {return (int)m_byType; }
	const char* getName(void) { return m_szName; }

    //DECLARE_LUA_CLASS(KRole);    
    //DECLARE_LUA_STRING(Name, sizeof(m_szName));
    //DECLARE_LUA_DWORD(PlayerID);
    //DECLARE_LUA_DWORD(MapID);
    //DECLARE_LUA_INTEGER(ConnIndex);
    //DECLARE_LUA_INTEGER(MapCopyIndex);
    //DECLARE_LUA_DWORD(TeamID);
    //DECLARE_LUA_ENUM(Camp);
    //DECLARE_LUA_DWORD(TongID);
    //DECLARE_LUA_TIME(LastSaveTime);
    //DECLARE_LUA_DWORD(ForceID);
};
#pragma pack()

