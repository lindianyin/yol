
#pragma once

// -------------------------------------------------------------------------

// 频道名长度 
#define KD_CHANNEL_NAME_MAX_LEN	32

// 消息的最大长度（只用来限制客户端发来的消息）
#define KD_CHANNEL_MSG_MAX_LEN	1024

#define KD_CHANNEL_PACKET_MAX_LEN 64000

// 频道配置文件
#define KD_CHANNEL_CONFIG_FILE	"\\setting\\chat\\channelcfg_new.ini"
#define KD_CHANNEL_SETTING_FILE "\\setting\\chat\\channel.ini"

// 系统消息类型
enum KE_SYSTEMMSG_TYPE
{
	emKSYSTEMMSG_NORMAL,        // 普通，聊天信息区
	emKSYSTEMMSG_IMPORTANT,     // 重要，中央提示
	emKSYSTEMMSG_NOTICE,        // 公告，中央滚动提示
	emKSYSTEMMSG_NUM,
};

// 消息显示类型
enum QE_MSG_SHOWN_TYPE
{
    emQCHAT_PANEL,              // 聊天面板显示 
	emQNOTICE_BOARD,            // 公告栏显示
	emBLACK_STRIPE              // 黑条显示
};

class IKChatChannelProcessor;

#ifdef GAME_CENTER
	class IKInterfaceConnector;
#endif

// 频道信息
struct KChannelInfo
{
	INT						nChannelId;
	BOOL					nOffLineMsg;
	IKChatChannelProcessor*	pProcessor;
#ifdef GAME_CENTER
	IKInterfaceConnector*	pInterface;	// 用于GC聊天模块向其它模块发送消息
#endif
};

typedef std::map<INT, KChannelInfo> KT_CHANNEL_INFO;
// -------------------------------------------------------------------------


