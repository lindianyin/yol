
#ifndef __KGC_COORDINATORCLIENT_H__
#define __KGC_COORDINATORCLIENT_H__

// -------------------------------------------------------------------------


class KGC_CoordinatorClient
{
public:
	static KGC_CoordinatorClient& Inst();
	BOOL Init(PCSTR pszIp, INT nPort);

	INT Connect(PCSTR pszIp, INT nPort);

	BOOL Uninit();

	INT Activate();
	INT Send(IMemBlock* piBuffer);

	INT WaitForModuleInfo();
	INT SendModuleInfoRequest( ISocketStream* piSocket );

private:
	typedef void (KGC_CoordinatorClient::*PPROTOCOL_FUNC)(BYTE* pbyData, size_t uDataLen);
	PPROTOCOL_FUNC m_PakProcessor[UCHAR_MAX + 1];
	size_t m_uPakSize[UCHAR_MAX + 1];

	BOOL ProcessPackage();
	void OnSyncTime(BYTE* pbyData, size_t uDataLen);
	void OnGcIntercomm(BYTE* pbyData, size_t uDataLen);

	KGC_CoordinatorClient();
private:
	ISocketStream* m_piSocket;
	std::string m_strCoordinatorIp;
	INT m_nCoordinatorPort;
};

// -------------------------------------------------------------------------

#endif /* __KGC_COORDINATORCLIENT_H__ */
