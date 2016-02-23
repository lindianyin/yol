
#include "stdafx.h"
#include "ksocketclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KSocketClient _gcSocketClient;
IKSocketClientInterface* g_piGcMonitorClient = &_gcSocketClient;


// -------------------------------------------------------------------------
KSocketClient::KSocketClient()
{
	m_pGameSvrStream		= NULL;
	m_pProcessor = NULL;
	m_nReceivePos = 0;
	m_nReading = 0;
}

KSocketClient::~KSocketClient()
{
	// ShutDown();
}

VOID KSocketClient::NotifyGameSvrDisconnect(VOID)
{
	m_bLostConnection = TRUE;

	KGLogPrintf(KGLOG_INFO, "Lost Connection To Server!");
}

INT KSocketClient::Init(IKSocketEventProcessor_Client* piEventProcessor)
{
	m_bRunStatus = FALSE;
	m_pProcessor = piEventProcessor;
	return TRUE;
}

BOOL KSocketClient::Connect(LPCSTR pszIp, USHORT uPort)
{
	KGC_CONFIRM_RET_FALSE(pszIp);

	KG_Printl("Connecting Server " << pszIp << ":" << uPort );
	m_pGameSvrStream = m_sockConnector.Connect(pszIp, uPort);
	if (!m_pGameSvrStream)
		return FALSE;

	m_pProcessor->ProcessEvent(emKSOCKET_EVENT_CREATE, NULL, 0);

	m_bLostConnection = FALSE;
	m_bRunStatus = TRUE;

	m_cSocketThread.Create(WorkThreadFunction, this);
	return TRUE;
}

BOOL KSocketClient::ShutDown()
{
	m_bRunStatus = FALSE;

	m_cShutdownLock.Lock();
	m_bLostConnection = FALSE;

	KG_COM_RELEASE(m_pGameSvrStream);

	m_cShutdownLock.Unlock();

	m_cSocketThread.Destroy();

	m_pProcessor->ProcessEvent(emKSOCKET_EVENT_CLOSE, NULL, 0);
	return TRUE;
}

BOOL KSocketClient::Send(LPCVOID pData, UINT uDataLen)
{
	if (!m_pGameSvrStream)
		return FALSE;

	IKG_Buffer* pBuffer = KG_MemoryCreateBuffer(uDataLen);
	if (!pBuffer)
		return FALSE;

	memcpy(pBuffer->GetData(), pData, uDataLen);
	BOOL bRet = m_pGameSvrStream->Send(pBuffer) == 1;
	pBuffer->Release();
	if ( ! bRet)
		NotifyGameSvrDisconnect();		// 断线

	return bRet;
}

INT KSocketClient::SocketThreadFunction() 
{	
	int				nResult				= FALSE;
	IKG_Buffer*		piRequestPacket		= NULL;
	BYTE*			pbyData				= NULL;
	IKG_SocketStream* pGameSvrStream	= NULL;
	unsigned int	uSize				= 0;
	int				nProtocolSize		= 0;
	IKG_Buffer*		piPacket			= NULL;
	timeval			tValCheckReceive	= {0, 10000}; //因为单位是百万分之一秒，所以原来的1000太小了，改成100000 == 100ms

	m_cShutdownLock.Lock();

	while (m_bRunStatus)
	{
		if (m_pGameSvrStream->CheckCanRecv(&tValCheckReceive) != 1)
			continue;
		INT nRetCode = m_pGameSvrStream->Recv(&piRequestPacket);
		if (nRetCode == -1)	
		{
			//因为socket CLOSE_WAIT的bug，接收失败就当断线了，不再判断Alive
			NotifyGameSvrDisconnect();
			break;
		}

		if (!piRequestPacket)   // if no data arrive
			continue;

		if (nRetCode != 1)
		{
			KG_COM_RELEASE(piRequestPacket);
			continue;
		}

		//pbyData = (BYTE*)piRequestPacket->GetData();
		//uSize = piRequestPacket->GetSize();
		ProcessPacket(piRequestPacket);
		KG_COM_RELEASE(piRequestPacket);
	}

	nResult = TRUE;
//Exit0:
	KG_COM_RELEASE(m_pGameSvrStream);
	KG_COM_RELEASE(piRequestPacket);
	KG_COM_RELEASE(piPacket);

	m_bRunStatus = TRUE;
	m_cShutdownLock.Unlock();

	return nResult;
}

VOID KSocketClient::WorkThreadFunction(LPVOID pThis) 
{
	((KSocketClient*)pThis)->SocketThreadFunction();
}

BOOL KSocketClient::Activate()
{
	if (!m_nReading)	// 无数据
		return TRUE;

	// 获取接收线程的数据包
	std::vector<IKG_Buffer* > vecPacket;

	ASSERT(m_pProcessor);

	std::vector<IKG_Buffer* >::iterator it;
	INT nReadPos = !m_nReceivePos;
	for (it = m_vecPackagReceive[nReadPos].begin(); it != m_vecPackagReceive[nReadPos].end(); ++it)
	{
		m_pProcessor->ProcessPacket((LPCBYTE)(*it)->GetData(), (*it)->GetSize());
		KG_COM_RELEASE(*it);
	}

	if (m_bLostConnection)
	{
		// 被动断开的才发出连接断开事件
		m_pProcessor->ProcessEvent(emKSOCKET_EVENT_LOST, NULL, 0);
		m_bLostConnection = FALSE;
	}

	m_vecPackagReceive[nReadPos].clear();
	// 加了多余判断，是为了最大限度防止cpu的out-of-orderexecution导致的不同步问题
	// 事实上包括Intel等大多数cpu都不会允许写操作乱序执行，所以实际上有点多余，但代码上作万全保证
	// 能保证是因为后面的读对前面的写操作作了判断的话，out-of-orderexecution是不可能发生的
	if (m_vecPackagReceive[nReadPos].empty())
		m_nReading = FALSE; // 重置读取状态
	return TRUE;
}

BOOL KSocketClient::ProcessPacket( IKG_Buffer* piBuffer )
{
	piBuffer->AddRef();
	INT nPrePos = m_nReceivePos;
	UINT uPreSize = (UINT)m_vecPackagReceive[m_nReceivePos].size();
	m_vecPackagReceive[m_nReceivePos].push_back(piBuffer);
	// 加了多余判断，是为了最大限度防止cpu的out-of-orderexecution导致的不同步问题
	// 事实上包括Intel等大多数cpu都不会允许写操作乱序执行，所以实际上有点多余，但代码上作万全保证
	// 能保证是因为后面的读对前面的写操作作了判断的话，out-of-orderexecution是不可能发生的
	if (m_vecPackagReceive[m_nReceivePos].size() != uPreSize && !m_nReading)	// 如果不是正在读
	{
		// 切换写入缓冲
		m_nReceivePos = !m_nReceivePos;
		// 多余判断，道理同上
		if (m_nReceivePos != nPrePos)
			m_nReading = TRUE;	// 使读取线程可以开始读数据
	}
	return TRUE;
}
