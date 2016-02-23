/* -------------------------------------------------------------------------
//	文件名		：	kchatapply_gc.h
//	创建者		：	liuchang
//	创建时间	：	2010-6-4 19:40:37
//	功能描述	：	
//
// -----------------------------------------------------------------------*/
#ifndef __KCHATAPPLY_GC_H__
#define __KCHATAPPLY_GC_H__

// -------------------------------------------------------------------------

#ifdef GAME_CENTER

// -------------------------------------------------------------------------
// 全局消息广播给各个服务器
class KGCChatProcess_Global : public IKChatChannelProcessor
{
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};


// -------------------------------------------------------------------------
// 密聊
class KGCChatProcess_Private : public IKChatChannelProcessor
{	
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL ProcessSendMsgToPlayer(INT nChannelId, INT nSenderId, const char* pszReceiver, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);	
};

//队聊
class KGCChatProcess_Team : public IKChatChannelProcessor
{
	virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};

//家族聊
class KGCChatProcess_Kin : public IKChatChannelProcessor
{
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	//virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};

//群聊
class KGCChatProcess_Group : public IKChatChannelProcessor
{
	virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};

class KGCChatProcess_GM : public IKChatChannelProcessor
{
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BOOL bSysMsg);
};

// -------------------------------------------------------------------------


extern KGCChatProcess_Global g_cGCChatGlobal;
extern KGCChatProcess_Private g_cGCChatPrivate;
extern KGCChatProcess_Team g_cGCChatTeam;
extern KGCChatProcess_Kin g_cGCChatKin;
extern KGCChatProcess_Group g_cGCChatGroup;
extern KGCChatProcess_GM g_cGCChatGM;


class KProcessorFactory_GC : public IKChatChannelProcessorFactory
{
	IKChatChannelProcessor* GetChannelProcessor(KE_CHATCHANNEL_TYPE emChannelType)
	{
		QCONFIRM_RET_FALSE(emChannelType > emKCHATCHANNEL_NONE && emChannelType < emKCHATCHANNEL_NUM);

		switch (emChannelType)
		{
		case emKCHATCHANNEL_GLOBAL:
			return &g_cGCChatGlobal;
		case emKCHATCHANNEL_TEAM:
			return &g_cGCChatTeam;
		case emKCHATCHANNEL_KIN:
			return &g_cGCChatKin;
		case emKCHATCHANNEL_GROUP:
			return &g_cGCChatGroup;
		case emKCHATCHANNEL_PRIVATE:
			return &g_cGCChatPrivate;
		case emKCHATCHANNEL_GM:
			return &g_cGCChatGM;
		default:
			return NULL;
		}
	}
};

extern KProcessorFactory_GC g_cChatChannelProcessorFactory_GC;

#endif


// -------------------------------------------------------------------------

#endif /* __KCHATAPPLY_GC_H__ */
