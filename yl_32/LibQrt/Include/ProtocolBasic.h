
#pragma once
#include<stdint.h>


//字符串长度定义
#define _NAME_LEN               32
//身份证字符串长度
#define KG_STRING_SIZE_ID_CARD  19

#pragma	pack(1)
#pragma warning(disable: 4200)

#define PROTOCOL_MSG_TYPE unsigned char
#define PROTOCOL_MSG_SIZE (sizeof(PROTOCOL_MSG_TYPE))

#define INTRANER_IP					0
#define INTERNET_IP					1
#define LOGINCFG_IP					"\\setting\\IPALL\\ip.ini"
#define LOGINCFG_IP_LEN 16
typedef struct tagProtocolHeader
{
    union
    {
	    PROTOCOL_MSG_TYPE	cProtocol;
	    PROTOCOL_MSG_TYPE	ProtocolType;
	};
} ProtocolHeader, tagProtoHeader;

typedef struct tagProtocolHeader2 :  public tagProtocolHeader
{
	uint32_t ulIdentity;
} ProtocolHeader2;

struct tagSubProtoHeader
{
	PROTOCOL_MSG_TYPE	cSubProtocol;
};

struct PrtocolLengthHeader : tagProtoHeader
{
	unsigned short		Length;
};

struct tagBiProtoHeader : PrtocolLengthHeader
{
	PROTOCOL_MSG_TYPE	cSubProtocol;
};

struct tagBiProtoHeaderNoLen : tagProtoHeader
{
	PROTOCOL_MSG_TYPE	cSubProtocol;
};

typedef struct _INTERNAL_PROTOCOL_HEADER
{
	WORD	wProtocolID;			//协议号
	//BYTE	bReserved;
}INTERNAL_PROTOCOL_HEADER;

typedef struct _IDENTITY_HEADER : INTERNAL_PROTOCOL_HEADER
{
	DWORD		dwPacketIdentity;		//数据包的唯一标记，供验证返回路由使用
}IDENTITY_HEADER;

template<WORD WPROTOCOL_ID>
struct SERVER_PROTOCOL : public INTERNAL_PROTOCOL_HEADER
{
	enum { protocol = WPROTOCOL_ID };
	SERVER_PROTOCOL() { wProtocolID = WPROTOCOL_ID; }
	operator INTERNAL_PROTOCOL_HEADER&() { return *this; }
};

struct TProcessData
{
	unsigned char	nProtoId;
	uint32_t		nDataLen;
	uint32_t		ulIdentity;
	bool			bLeave;
	char			pDataBuffer[1];//实际的数据
};

#define KSG_PASSWORD_MAX_SIZE 64

typedef struct tagKSG_PASSWORD
{
    char szPassword[KSG_PASSWORD_MAX_SIZE];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
} KSG_PASSWORD;

//扩展点
#define MAX_EXT_POINT_COUNT   8

struct tagExtPointInfo
{
	DWORD nExtPoint[MAX_EXT_POINT_COUNT];
};

enum Account_State_Type
{
    OFFLINE = 0x00000001,                   // 离线挂机
    ACTIVED_DYNAMIC_PASSWORD = 0x00000002,  // 激活了动态密码服务
	ACCOUNT_STATE_LOCK = 0x00000004,		// 账号锁定
};

struct tagGuidableInfo : public tagProtocolHeader2
{
	GUID            guid;
    tagExtPointInfo ExtPointInfo;       //可用的附送点
	tagExtPointInfo ChangePointInfo;	//变化的附送点
	DWORD			nTotalLeftTime;		//总的可游戏时间,以秒为单位(包括包月和计点)
	DWORD			nLeftTimeOfPoint;	//点数对应的剩余时间,以秒为单位，nTotalLeftTime-nLeftTimeOfPoint可以得到包月时间	
    DWORD           dwLeftMoney;
    uint32_t   		uLimitPlayTimeFlag;  // 防沉迷标志位
    uint32_t		uLimitOnlineSecond;  // 防沉迷累计在线时间
    uint32_t		uLimitOfflineSecond; // 防沉迷累计离线时间
    int             nChargeFlag;
    unsigned int    uAccountState;       // 按位使用，每位的意义在枚举Account_State_Type中定义
	BYTE			byPasspodVersion;
	BYTE			byPasspodMode;
    char            szPhoneNumber[20];
	uint32_t        datalength;
	char	        szData[0];
};

#define	FAIL_REASON_NONE		                0
#define FAIL_REASON_VALID_PWD	                1
#define	FAIL_REASON_NOT_ALLOW	                2
#define	FAIL_REASON_UNKNOWN                     3
#define FAIL_REASON_NAME_FILTER                 4
#define FAIL_REASON_NAME_EXIST                  5
#define FAIL_REASON_OVER_MAX_ROLE_PER_GATEWAY   6
#define FAIL_REASON_OVER_MAX_ROLE_PER_ZONE      7


//删除与新建角色的返回消息带的数据
struct tagNewDelRoleResponse : tagProtocolHeader2
{
	char	szRoleName[_NAME_LEN];
    bool	bSucceeded;	
	BYTE	cFailReason;
};

struct tagQueryMapInfo : public tagProtocolHeader
{
	unsigned int		uTime;
	char				szGatewayName[32];	// 临时加的，为了让gameserver知道GatewayName。以后不能用这个方案。
};

struct tagUpdateMapID : public tagProtocolHeader
{	
	//  For example : Are your clear older information when it update local information.	 
	bool bRelay;	//如果是relay则，填1，如果是gameserver则填0
	int nMapCount;	//如果是relay则，填0
	int snMapID[0];	// C4200 warning
};

typedef struct tagPING_COMMAND : public tagProtocolHeader
{
	DWORD dwTime;
} PING_COMMAND;

#pragma pack()

