#ifndef _GODDESS_PROTOCOL_H_
#define _GODDESS_PROTOCOL_H_

#include "../ProtocolBasic.h"

enum GODDESS_C2S_PROTOCOL_ID    // Goddess client ==> server
{
    GODDESS_C2S_PROTOCOL_BEGIN = PROTOCOL_BIG_PACKAGE_BEGIN,
    GDC2S_SAVE_ROLE_DATA,         // 存储角色数据, ref : TProcessData
    GDC2S_SET_STATISTIC_INFO,     // 更新统计信息, ref : TProcessData

    GDC2S_PING = PROTOCOL_NORMAL_PACKAGE_BEGIN + 1, // ping, ref : PING_COMMAND
    GDC2S_QUERY_ROLE_LIST,        // 获取角色列表, ref : TProcessData
    GDC2S_QUERY_ROLE_DATA,        // 获取角色数据, ref : TProcessData
    GDC2S_CREATE_ROLE,            // 创建角色, ref : TProcessData
    GDC2S_DELETE_ROLE,            // 删除角色, ref : TProcessData

    GDC2S_LOCK_OR_UNLOCK_ROLE,    // 角色数据加/解锁操作, ref : LOCK_OR_UNLOCK_ROLE
    GDC2S_PLAYER_ENTER_GAME,      // 角色进入游戏, ref : TProcessData
    GDC2S_PLAYER_LEAVE_GAME,      // 角色离开游戏或者是进入游戏失败, ref : TProcessData
    GDC2S_SHUTDOWN_GODDESS,       // 从网关关闭Goddess, ref : TProcessData
    GDC2S_QUERY_STATISTIC_INFO,   // 获取统计信息, ref : TProcessData
    GDC2S_TRANSFER_ROLE,          // 角色数据转服, ref : ???
    GDC2S_QUERY_ROLE_LIST_ITEM,   // 获取单个角色列表信息, ref : TProcessData

    GDC2S_LOCK_OR_UNLOCK_ROLE_EXT,// 角色数据加/解锁操作, 该操作有返回包 ref : LOCK_OR_UNLOCK_ROLE_EXT
    GDC2S_CREATE_TONGNAME,        // 创建帮会名，ref : CREATE_TONG_NAME
    GDC2S_QUERY_ROLE_LIST_IN_ZONE // 获取一个大区内一个账号的所有角色名和其所在服务器，ref : TProcessData
};

enum GODDESS_S2C_PROTOCOL_ID     // Goddess server ==> client
{
    GODDESS_S2C_PROTOCOL_BEGIN = PROTOCOL_BIG_PACKAGE_BEGIN,
    GDS2C_RETURN_ROLE_DATA,       // 返回角色数据, ref : TProcessData + role data
    GDS2C_CREATE_ROLE_RESULT,     // 创建角色数据的操作结果, ref : TProcessData,pDataBuf[0]取值参见enum CREATE_ROLE_RESULT
    GDS2C_RETURN_ROLE_LIST_ITEM,  // 返回单个角色列表信息, ref : TProcessData

    GDS2C_PING = PROTOCOL_NORMAL_PACKAGE_BEGIN + 1, // ping back, ref : PING_COMMAND    

    GDS2C_RETURN_ROLE_LIST,       // 返回角色列表, ref : TProcessData + role list data
    GDS2C_SAVE_ROLE_DATA_RESULT,  // 存储角色数据返回结果, ref : TProcessData
    GDS2C_DELETE_ROLE_RESULT,     // 删除角色数据返回结果, ref : TProcessData
    GDS2C_SHUTDOWN_CMD_RESPOND,   // 剑网新加协议: 关闭命令的响应结果, ref : TProcessData
    GDS2C_RETURN_STATISTIC_INFO,  // 返回统计信息, ref : TProcessData

    GDS2C_RETURN_LOCK_OR_UNLOCK_ROLE_EXT, // 返回角色数据加/解锁的操作结果, ref : TProcessData,  
                                          // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success

    GDS2C_CREATE_TONGNAME_RESULT, // 返回创建帮会名的结果, ref : TProcessData
                                  // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success

    GDS2C_SET_STATISTIC_INFO_RESULT,   // 更新统计信息返回结果, ref : TProcessData,
                                       // TProcessData.pDataBuffer[0]:0 failed, TProcessData.pDataBuffer[0]:1 success
    GDS2C_RETURN_ROLE_LIST_IN_ZONE     // 返回一个大区内一个账号的所有角色名及其所在服务器，ref : TProcessData,ROLE_IN_ZONE
                                       // TProcessData.pDataBuffer[0]  : 0 failed, 1 success
                                       // &TProcessData.pDataBuffer[1] : (unsigned)uRoleCount + ROLE_IN_ZONE[uRoleCount]
};

#pragma    pack(push, 1)
enum CREATE_ROLE_RESULT
{
    CREATE_ROLE_SUCCESS = 1,
    CREATE_ROLE_UNKNOW_FAILED  = -1,
    CREATE_ROLE_NAME_FILTER    = -2,
    CREATE_ROLE_NAME_EXIST     = -3,
    CREATE_ROLE_OVER_MAX_ROLE_PER_GATEWAY = -4,
    CREATE_ROLE_OVER_MAX_ROLE_PER_ZONE    = -5
};

struct LOCK_OR_UNLOCK_ROLE : tagProtocolHeader
{
    bool bLock;
    char szRoleName[_NAME_LEN];
};

struct LOCK_OR_UNLOCK_ROLE_EXT : tagProtocolHeader2
{
    bool bLock;
    char szRoleName[_NAME_LEN];
};

struct CREATE_TONG_NAME : tagProtocolHeader2
{
    char szTongName[_NAME_LEN];
};

struct ROLE_IN_ZONE
{
    char szRoleName[_NAME_LEN];
    char szGatewayName[_NAME_LEN];
};

#pragma pack(pop)

#endif


