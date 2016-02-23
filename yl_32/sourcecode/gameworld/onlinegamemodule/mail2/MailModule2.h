#pragma once

#include "onlinegameworld/kgcmodulebase.h"
#include "MailProtocol.h"

struct KMailListInfo;
struct KMailContent;
class KPlayer;

class QMailModule : public KModuleBase
{
protected:
	virtual BOOL InitProtocol();
	virtual BOOL OnInit();
	virtual BOOL OnUnInit();
	virtual BOOL ProcessProtocol(INT nPlayerIndex, LPCBYTE pbData, UINT uDataLen);

public:
	QMailModule(void);
	~QMailModule(void);

	// -------------------------------------------------------
	BOOL DoSendMailRespond(KPlayer* pPlayer, int nRespondID, int nResult);
	BOOL DoGetMailListRespond(KPlayer* pPlayer, KMailListInfo* pMailListInfo, int nMailCount);
	BOOL DoNewMailNotify(KPlayer* pPlayer, KMailListInfo* pMailListInfo);
	BOOL DoSyncMailContent(KPlayer* pPlayer, DWORD dwMailID, int nResult, int nDataLen, BYTE* pData);
	BOOL DoMailGeneralRespond(KPlayer* pPlayer, DWORD dwMailID, int nResult);

	// ---------------------------------------------
private:
	void OnSendMailRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnGetMaillistRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnQueryMailContent(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnAcquireMailMoneyRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnAcquireMailItemRequest(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnSetMailRead(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnDeleteMail(KPlayer* pPlayer, char* pData, size_t uDataLen);
	void OnReturnMail(KPlayer* pPlayer, char* pData, size_t uDataLen);

	// 发送固定长度协议
	BOOL SendData(INT nPlayerIndex, KPTC_HEADER_BASE& rPtcData);
	// 发送变长协议
	BOOL SendLenData(INT nPlayerIndex, KLENPTC_HEADER_BASE& rPtcData, UINT uDataSize);
	BOOL SendUnknownData(INT nPlayerIndex, LPSTR pData, UINT uDataSize);

	BYTE m_byTempData[32*1024];

protected:
	typedef void (QMailModule::*PROCESS_PROTOCOL_FUNC)(KPlayer* pPlayer, char* pData, size_t uDataLen);

	PROCESS_PROTOCOL_FUNC m_ProcessProtocolFuns[c2s_mail_end];
};

extern QMailModule g_cMailModule2;
