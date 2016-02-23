
#ifdef GAME_CLIENT

#include "stdafx.h"
#include "ksocketinterface.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "onlinegameworld/kprotocolsize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
class KSocketClient : public IKSocketClientInterface
{
public:
	KSocketClient();
	virtual ~KSocketClient();

	BOOL	Init(IKSocketEventProcessor_Client* piEventProcessor);
	BOOL	Activate();

	BOOL	Connect(DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden);	// 连接指定Gameserver
	BOOL	ShutDown();								// 主动断开同Gameserver的连接
	BOOL	Send(LPCVOID pData, UINT uDataLen);			// 向gameserver发送消息
private:
	VOID	NotifyGameSvrDisconnect(VOID);
private:
	KG_BufferedSocketConnector	m_sockConnector;
	ISocketStream*			m_pGameSvrStream;
	
	// 注意在x86处理器上m_bRunStatus需要是对齐的
	BOOL						m_bRunStatus;
	IKSocketEventProcessor_Client* m_pProcessor;
};

KSocketClient _gcSocketClient;
IKSocketClientInterface* g_piSocketClient = &_gcSocketClient;


KSocketClient::KSocketClient()
{
	m_pGameSvrStream		= NULL;
	m_pProcessor = NULL;
}

KSocketClient::~KSocketClient()
{
	// ShutDown();
}

VOID KSocketClient::NotifyGameSvrDisconnect(VOID)
{
	QLogPrintf(LOG_INFO, "Lost Connection To Game Server!");
	m_pProcessor->ProcessEvent(emKSOCKET_EVENT_LOST, NULL, 0);
	SAFE_RELEASE(m_pGameSvrStream);
}

INT KSocketClient::Init(IKSocketEventProcessor_Client* piEventProcessor)
{
	m_bRunStatus = FALSE;
	m_pProcessor = piEventProcessor;
	return TRUE;
}

BOOL KSocketClient::Connect(DWORD dwIpAddress, USHORT uPort, const GUID* pGuid, BOOL bHidden)
{
	ASSERT(pGuid);
	SAFE_RELEASE(m_pGameSvrStream);

	CHAR szIpAddress[32];
	LPBYTE pIpAddress = (LPBYTE)&dwIpAddress;
	sprintf(szIpAddress, "%u.%u.%u.%u", pIpAddress[0], pIpAddress[1], pIpAddress[2], pIpAddress[3]);

	Q_Printl("Connecting to GameServer " << szIpAddress << ":" << uPort );
	m_pGameSvrStream = m_sockConnector.ConnectSecurity(szIpAddress, uPort, KSG_ENCODE_DECODE);
	if (!m_pGameSvrStream)
		return FALSE;

	m_pProcessor->ProcessEvent(emKSOCKET_EVENT_CREATE, NULL, 0);

	// 设置超时时间
	timeval sTimeout;
	sTimeout.tv_sec = 0;
	sTimeout.tv_usec = 2000;
	m_pGameSvrStream->SetTimeout(&sTimeout);

	KC2S_LOGIN sLoginProtocol;
	sLoginProtocol.byProtocolFamily = 0;

	memcpy(&sLoginProtocol.guid, pGuid, sizeof(GUID));
	sLoginProtocol.nHidden = bHidden ? 1 : 0;
	sLoginProtocol.byGameServerVersion = KD_PROTOCOL_VERSION;

	IMemBlock* pBuffer = QCreateMemBlock(sizeof(sLoginProtocol));
	QCONFIRM_RET_FALSE(pBuffer);

	memcpy(pBuffer->GetData(), &sLoginProtocol, sizeof(sLoginProtocol));

	QCONFIRM_RET_FALSE(m_pGameSvrStream);
	BOOL bRet = m_pGameSvrStream->Send(pBuffer);
	SAFE_RELEASE(pBuffer);

	if (!bRet)
	{
		m_pGameSvrStream->Release();
		m_pGameSvrStream = NULL;
		return FALSE;
	}

	m_bRunStatus = TRUE;

	return TRUE;
}

BOOL KSocketClient::ShutDown()
{
	SAFE_RELEASE(m_pGameSvrStream);
	m_pProcessor->ProcessEvent(emKSOCKET_EVENT_CLOSE, NULL, 0);
	return TRUE;
}

BOOL KSocketClient::Send(LPCVOID pData, UINT uDataLen)
{
	if (!m_pGameSvrStream)
		return FALSE;

	IMemBlock* pBuffer = QCreateMemBlock(uDataLen);
	if (!pBuffer)
		return FALSE;

	memcpy(pBuffer->GetData(), pData, uDataLen);
	BOOL bRet = m_pGameSvrStream->Send(pBuffer) == 1;
	pBuffer->Release();
	if ( ! bRet)
		NotifyGameSvrDisconnect();		// 断线

	return bRet;
}

BOOL KSocketClient::Activate()
{
	int				nResult				= FALSE;
	IMemBlock*		piRequestPacket		= NULL;
	BYTE*			pbyData				= NULL;
	ISocketStream* pGameSvrStream	= NULL;
	unsigned int	uSize				= 0;
	int				nProtocolSize		= 0;
	IMemBlock*		piPacket			= NULL;
	timeval			tValCheckReceive	= {0, 0};

	while (m_pGameSvrStream && m_pGameSvrStream->CheckCanRecv(&tValCheckReceive) == 1)
	{
		INT nRetCode = m_pGameSvrStream->Recv(&piRequestPacket);
		if (nRetCode == -1)	
		{
			//因为socket CLOSE_WAIT的bug，接收失败就当断线了，不再判断Alive
			NotifyGameSvrDisconnect();
			break;
		}

		if (!piRequestPacket)   // if no data arrive
			break;

		LOG_PROCESS_ERROR(nRetCode == 1);

		//拆分已经合并的包
		pbyData = (BYTE*)piRequestPacket->GetData();
		uSize = piRequestPacket->GetSize();

		SAFE_RELEASE(piPacket);
		for (NULL; uSize > 0; uSize -= nProtocolSize, pbyData += nProtocolSize)
		{
			nProtocolSize = KPTC_SIZE::GetSizeS2C(*pbyData, *(pbyData + 1));

			// 等于 0 说明客户端没有此协议
			QCONFIRM_BREAK(nProtocolSize != 0);

			if (nProtocolSize == -1) //-1表示变长的协议
			{
				nProtocolSize = (INT)*(LPWORD)(pbyData + 2);
			}
			QCONFIRM_BREAK((UINT)nProtocolSize <= (int)uSize);

			piPacket = QCreateMemBlock(nProtocolSize);
			QCONFIRM_BREAK(piPacket);

			void* pvData = piPacket->GetData();
			QCONFIRM_BREAK(pvData);

			memcpy(pvData, pbyData, nProtocolSize);
			m_pProcessor->ProcessPacket((LPCBYTE)piPacket->GetData(), piPacket->GetSize());

			SAFE_RELEASE(piPacket);
		}
		SAFE_RELEASE(piRequestPacket);
	}

	nResult = TRUE;
EXIT0:
	SAFE_RELEASE(piPacket);
	SAFE_RELEASE(piRequestPacket);

	return nResult;
}

// -------------------------------------------------------------------------
#endif // #ifdef GAME_CLIENT
