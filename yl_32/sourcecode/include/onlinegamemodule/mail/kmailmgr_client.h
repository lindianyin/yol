/* -------------------------------------------------------------------------
//	文件名		：	kmailmgr_client.h
//	创建者		：	xiewenzhe
//	创建时间	：	2011/5/23 17:52:28
//	功能描述	：	邮件Client模块
//
// -----------------------------------------------------------------------*/
#ifndef __KMAILMGR_CLIENT_H__
#define __KMAILMGR_CLIENT_H__

// -------------------------------------------------------------------------
#ifdef GAME_CLIENT

class KMailManager_Client
{
public:
	KMailManager_Client();
	~KMailManager_Client();

public:
	static KMailManager_Client* Instance() {return &ms_inst;}

public:
	BOOL ProcessGsProtocal(LPCBYTE pbData, UINT uDataLen);
	BOOL OnSendResult(LPCBYTE pbData, UINT uSize);

private:
	BOOL OnMailBoxLoadOK(LPCBYTE pbData, UINT uSize);
	BOOL OnFetchMailInfo(LPCBYTE pbData, UINT uSize);
	BOOL OnFetchMail(LPCBYTE pbData, UINT uSize);
	BOOL OnNewMail(LPCBYTE pbData, UINT uSize);
	BOOL OnFetchItemOK(LPCBYTE pbData, UINT uSize);

private:
	static KMailManager_Client	ms_inst;
	CHAR m_szBuffer[256];
};

#endif /* GAME_CLIENT */
// -------------------------------------------------------------------------

#endif /* __KMAILMGR_CLIENT_H__ */
