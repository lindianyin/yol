#pragma once

class KRole;
struct KRoleBaseInfo;

class BattleZone
{
public:
	BattleZone(void);
	~BattleZone(void);

	bool Init(void);

	void OnPveQueueRequest();
	void OnAcceptPveMatch();


	BOOL DoPveQueueMatchNotify();

	BOOL OnBZoneSaveRoleDataRequest();

	BOOL DoConfirmPlayerLoginRequest();
	BOOL DoConfirmPlayerLoginRespond();

	BOOL DoSaveRoleData();
	BOOL DoLoadRoleDataRequest(DWORD dwRoleID);
	void OnLoadRoleDataRequest(INT nConnIndex);
	void OnLoadRoleData(int nConnIndex, void* pData, size_t uDataLen);

	BOOL SendGlobal(IMemBlock* piBuffer); // 

	BOOL LoadRoleData(int nSourceServerId, DWORD dwRoleID, BOOL bSuccess, BYTE* pbyData, size_t uDataLen);
private:
	void OnReceivedProtocol();
private:
	int			m_eRunMode;

	ISocketStream*	m_pMasterSocket;
};

extern BattleZone g_BZone;
