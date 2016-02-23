
#pragma once

#include "config/MailDef.h"

class MailRelayClient
{
public:
	MailRelayClient(void);
	~MailRelayClient(void);

	static BOOL DoSendMailRequest(INT nRequestID, const CHAR cszDstName[], KMail* pMail, size_t uMailLen);
	static BOOL DoSendGlobalMailRequest(KMail* pMail, size_t uMailLen, INT nLifeTime);
	static BOOL DoGetMailListRequest(DWORD dwPlayerID, DWORD dwStartID);
	static BOOL DoQueryMailContent(DWORD dwPlayer, DWORD dwMailID, INT nMailType);
	static BOOL DoAcquireMailMoneyRequest(DWORD dwPlayer, DWORD dwMailID);
	static BOOL DoAcquireMailItemRequest(DWORD dwPlayer, DWORD dwMailID, INT nIndex);
	static BOOL DoGiveMailMoneyToPlayerRespond(DWORD dwPlayerID, DWORD dwMailID, KMAIL_RESULT_CODE eResult);
	static BOOL DoGiveMailItemToPlayerRespond(DWORD dwPlayerID, DWORD dwMailID, INT nIndex, KMAIL_RESULT_CODE eResult);
	static BOOL DoSetMailRead(DWORD dwSrcID, DWORD dwMailID);
	static BOOL DoDeleteMail(DWORD dwSrcID, DWORD dwMailID);
	static BOOL DoReturnMail(DWORD dwSrcID, DWORD dwMailID);


	// ----------------------------------------------------------------------
	static void OnSendMailRespond(BYTE* pbyData, size_t uDataLen);
	static void OnGetMailListRespond(BYTE* pbyData, size_t uDataLen);
	static void OnSyncMailContent(BYTE* pbyData, size_t uDataLen);
	static void OnGiveMailMoneyToPlayer(BYTE* pbyData, size_t uDataLen);
	static void OnGiveMailItemToPlayer(BYTE* pbyData, size_t uDataLen);
	static void OnNewMailNotify(BYTE* pbyData, size_t uDataLen);
	static void OnMailGeneralRespond(BYTE* pbyData, size_t uDataLen);
};
