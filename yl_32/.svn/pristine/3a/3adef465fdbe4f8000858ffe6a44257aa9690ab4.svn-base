
#pragma once


#include <map>

#include "kchanneldef.h"
#include "onlinegamebase/kscriptmanager.h"

// -------------------------------------------------------------------------
class IKChatChannelProcessor
{
public:
	virtual BOOL ProcessSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType) { return FALSE; };
	virtual BOOL TransferToServer(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType) { return TRUE; }
	virtual BOOL ProcessRespondMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage,BYTE byShownType = emQCHAT_PANEL) { return TRUE; };
	virtual BOOL TransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType) { return TRUE; }
};

class IKChatChannelProcessorFactory
{
public:
	virtual IKChatChannelProcessor* GetChannelProcessor(CHAT_CHANNEL_TYPE emChannelType) = 0;
};

class KChatChannel
{
public:
	KChatChannel():m_pProcessorFactory(NULL) {};
	virtual ~KChatChannel(){};

public:
	inline BOOL Init()
	{
		QCONFIRM_RET_FALSE(m_pProcessorFactory);

		QIniFile iniChannelCfg;
		if (!iniChannelCfg.Load(KD_CHANNEL_CONFIG_FILE))
		{
			Q_Printl("Failed To Open" << KD_CHANNEL_CONFIG_FILE);
			return FALSE;
		}

		INT nChannelCount = iniChannelCfg.GetSectionCount();

		for (INT i = 0; i < nChannelCount; i++)
		{
			KChannelInfo cChannel;
			ZeroStruct(cChannel);

			INT nChannelId = i + 1;
			cChannel.nChannelId = nChannelId;

			CHAR aSection[3];
			sprintf(aSection, "%d", nChannelId);
			INT nTemp = 0;

			iniChannelCfg.GetInteger(aSection, "nOffLineMsg", 0, &nTemp);
			cChannel.nOffLineMsg = (BYTE)nTemp;

			iniChannelCfg.GetInteger(aSection, "nChannelType", 0, &nTemp);
			cChannel.pProcessor = m_pProcessorFactory->GetChannelProcessor((CHAT_CHANNEL_TYPE)nTemp);

			m_mapChannel[nChannelId] = cChannel;
		}

		return TRUE;
	}

public:
    inline BOOL RequestBroadcastBlackMsg(LPCSTR pszMessage)
	{
		return RequestSendMsg(trWorld, 0, 0, "", pszMessage, emBLACK_STRIPE);
	}

	inline BOOL RequestBroadcastSystemMsg(INT nChannelId, LPCSTR pszMessage, DWORD dwSenderId = 0, LPCTSTR szSenderName = NULL)
	{
		BOOL bResult = FALSE;
		if(!szSenderName)
		{
			bResult = RequestSendMsg(nChannelId, dwSenderId, 0, "", pszMessage, emQCHAT_PANEL);
		}
		else
		{
			bResult = RequestSendMsg(nChannelId, dwSenderId, 0, szSenderName, pszMessage, emQCHAT_PANEL);
		}
		return bResult;
	}

	inline BOOL RequestBroadcastNoticeMsg(LPCSTR pszMessage)
	{
		return RequestSendMsg(trWorld, 0, 0, "", pszMessage, emQNOTICE_BOARD);
	}

	inline BOOL RequestSendKinNoticeMsg(INT nSenderID, LPCSTR pszMessage)
	{
        return RequestSendMsg(trTong, nSenderID, 0, "", pszMessage, emQNOTICE_BOARD);
	}

	inline BOOL RequestSendMsg(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
	{
		BOOL bRet = FALSE;
		BOOL bFinish = FALSE;
		IKChatChannelProcessor *pProcessor = m_pProcessorFactory->GetChannelProcessor((CHAT_CHANNEL_TYPE)nChannelId);
		QCONFIRM_RET_FALSE(pProcessor);

		bRet = CheckSendConditon(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);
		QCONFIRM_RET_FALSE(bRet);

		bFinish = pProcessor->ProcessSendMsg(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, byShownType);

		if (!bFinish)
		{
			pProcessor->TransferToServer(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, byShownType);
		}

		return TRUE;
	}

	inline BOOL RequestTransferToClient(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType)
	{
		BOOL bRet = FALSE;
		IKChatChannelProcessor *pProcessor = m_pProcessorFactory->GetChannelProcessor((CHAT_CHANNEL_TYPE)nChannelId);
		QCONFIRM_RET_FALSE(pProcessor);

		bRet = CheckSendConditon(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage);
		QCONFIRM_RET_FALSE(bRet);

		pProcessor->TransferToClient(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, byShownType);

		return TRUE;
	}


	inline BOOL ProcessRespond(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage, BYTE byShownType = emQCHAT_PANEL)
	{
		IKChatChannelProcessor *pProcessor = m_pProcessorFactory->GetChannelProcessor((CHAT_CHANNEL_TYPE)nChannelId);
		QCONFIRM_RET_FALSE(pProcessor);

		pProcessor->ProcessRespondMsg(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, byShownType);
		pProcessor->TransferToClient(nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage, byShownType);

		return TRUE;
	}

public:
	inline BOOL SetProcessorFactory(IKChatChannelProcessorFactory* pFactory)
	{
		QCONFIRM_RET_FALSE(pFactory && !m_pProcessorFactory);
		m_pProcessorFactory = pFactory;

		return TRUE;
	}

	inline CONST KChannelInfo* GetChannelInfo(INT nChannelId)
	{
		KT_CHANNEL_INFO::iterator it = m_mapChannel.find(nChannelId);
		if (it == m_mapChannel.end())
			return NULL;

		return &(it->second);
	}

protected:
	inline BOOL RegisterMsgProcessor(INT nChannelId, IKChatChannelProcessor* pProcessor)
	{
		_ASSERT(pProcessor);
		KT_CHANNEL_INFO::iterator it = m_mapChannel.find(nChannelId);
		if (it == m_mapChannel.end())
		{
			_ASSERT(FALSE);
			return FALSE;
		}

		it->second.pProcessor = pProcessor;

		return TRUE;
	}

	inline virtual BOOL CheckSendConditon(INT nChannelId, INT nSenderId, INT nReceiverId, LPCSTR pszSenderName, LPCSTR pszMessage)
	{
#ifdef GAME_CENTER
		return TRUE;
#else
		ScriptSafe cSafeScript = g_cScriptManager.GetSafeScript();
		VERIFY(cSafeScript->CallTableFunction("Chat", "CheckSendCondition", 1, "dddss", nChannelId, nSenderId, nReceiverId, pszSenderName, pszMessage));
		BOOL bRet = cSafeScript->GetInt(-1);

		return bRet;
#endif // GAME_CENTER
	}

protected:
	KT_CHANNEL_INFO m_mapChannel;
	IKChatChannelProcessorFactory*	m_pProcessorFactory;
};

