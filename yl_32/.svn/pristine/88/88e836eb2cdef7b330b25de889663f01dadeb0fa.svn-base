//---------------------------------------------------------------------------
// 剑侠情缘网络版2 GameMasterCentral 互联互通
// 版权所有:       2006-2007 金山软件(Kingsoft)
// 创建日期与作者: 2006 越子(wooy)
//---------------------------------------------------------------------------
// GameMasterCentral与游戏世界的各种Master服务之间的协议接口
//---------------------------------------------------------------------------
#ifndef __LOG_SERVER_PROTOCOL_H__
#define __LOG_SERVER_PROTOCOL_H__

#pragma	pack(push, 1)

enum LOG_SERVER_PROTOCOL_LIST
{
	LOG_SERVER_P_LOGIN	= 32,
	LOG_SERVER_P_PING,
	LOG_SERVER_P_QUERY_LOG,
};

struct LOG_SERVER_S_LOGIN
{
	unsigned char	cProtocol;	// = LOG_SERVER_P_LOGIN
	char			szGameSpaceName[32];
	char			Reserved[32];
};

//LogServer收到Ping消息包后会原样返回
struct LOG_SERVER_S_PING
{
	unsigned char	cProtocol;	// = LOG_SERVER_P_PING
	unsigned int	uTime;		// = 事件戳
};

struct LOG_SERVER_S_QUERY_LOG
{
	unsigned char	cProtocol;		// = LOG_SERVER_P_QUERY_LOG
	char			szLogSort[32];	// 日志分类
	char			szLogObject[64];// 日志相关的目标对象名
	unsigned int	uParamLen;		// 后面的szParam的长度(含结尾符)
	char			szParam[1];		// szParam的最后一个字节空间为字符串结尾符号'\0'
	
};

#pragma	pack(pop)

#endif// #ifndef __LOG_SERVER_PROTOCOL_H__
