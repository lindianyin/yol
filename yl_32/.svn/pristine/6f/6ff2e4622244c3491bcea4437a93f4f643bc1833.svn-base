
#include "stdafx.h"
#include "Core/IpMacAddress.h"
#include "Misc/objectpool.h"
#include "Misc/qidpool.h"
#include "onlinegamebase/ksysservice.h"
#include "config/kconfigfile.h"
#include "onlinegameworld/krequireinterface.h"
#include "ksocketinterface.h"
#include "onlinegameworld/korpgworld.h"
#include "onlinegameworld/kbaseprotocol.h"
#include "Network/QSocketPack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const static INT g_snRecvPacketSize = 32 * 1024;
const static INT g_snSendPacketSize = 960 * 1024;
const static INT g_snMaxEventAccept = 1024 * 10;
const static INT g_snSendInterval = 10; // 发包间隔，毫秒

//#define _USE_SOCKET_PACKER


struct KCONNECTION_DATA
{
	KCONNECTION_DATA() : SocketPacker(4096)
	{
		nConnectId = 0; piSocketStream = NULL;
	}

	INT nConnectId;
	ISocketStream* piSocketStream;
	QSocket_Packer SocketPacker;
};

// -------------------------------------------------------------------------
class KSocketServer : public IKSocketInterface
{
public:
	KSocketServer();
	~KSocketServer();
	BOOL Init(IKSocketEventProcessor* piEventProcessor);
	// 打开所有连接
	BOOL SocketOpen();
	// 关闭所有连接
	BOOL SocketClose();
	BOOL Disconnect(INT nConnectId);
	// 发送数据、事件
	BOOL Send(INT nConnectId, INT nEventId, LPCVOID pData, INT nData);
	// 消息循环，接收数据、事件并处理
	BOOL MessageLoop();
	// 获得本机Actived地址
	BOOL GetHostIP(KHOST_ADDRESS& rcAddress);
	// 获得远端地址
	BOOL GetRemoteAddress(INT nConnectId, DWORD& rdwIp, USHORT& ruPort);

	BOOL GetLocalIpAddress(DWORD dwInAdapterIdx, DWORD dwOutAdapterIdx, CHAR szInIp[], CHAR szOutIp[]);

	BOOL GetSocketInfo(KSOCKET_KINGEYES_INFO* pcSocketInfo);
protected:
	BOOL CloseConnect(INT nConnectId,BOOL bLost);
	// 真正发送
	BOOL RealSend(INT nConnectId);
protected:
	QSocketServerAcceptor	m_cSockServer;	// Server Linstener，用于监听GameClient
	QSOCKET_EVENT* m_pSockEvents;	// 监听事件的缓冲区指针
	INT m_nMaxPlayer;	// 最大连接数
	QIdPool m_cIdPool;	// 连接ID分配器
	KCONNECTION_DATA* m_pConnData;
	IKSocketEventProcessor* m_piEventProcessor;
	UINT m_uLastLoopElapse;
private:
	KHOST_ADDRESS	m_cParam;
	KSOCKET_KINGEYES_INFO	m_cSocketInfo;
};

KSocketServer _gcSocketServer;
IKSocketInterface* g_piSocketServer = &_gcSocketServer;

// -------------------------------------------------------------------------
KSocketServer::KSocketServer()
{
	m_piEventProcessor = NULL;
	m_pSockEvents = NULL;
	m_pConnData = NULL;
	m_nMaxPlayer = 0;
	m_uLastLoopElapse = 0;
}

KSocketServer::~KSocketServer()
{
	SocketClose();
}

BOOL KSocketServer::SocketOpen()
{
	//listen not use m_cParam.szInternetIp
	Q_Printl("Prepare To Listen On [" << "0.0.0.0" << ":" << m_cParam.nListenPort << "]...");
	BOOL bRet = m_cSockServer.Init("0.0.0.0",
			m_cParam.nListenPort,
			g_snMaxEventAccept,
			g_snRecvPacketSize,
			g_snSendPacketSize,
			ENCODE_DECODE_NONE/*KSG_ENCODE_DECODE*/,
			NULL);
	QCONFIRM_RET_FALSE(bRet);
	return TRUE;
}

BOOL KSocketServer::GetLocalIpAddress(DWORD dwInAdapterIdx, DWORD dwOutAdapterIdx, CHAR szInIp[], CHAR szOutIp[])
{
	CONST INT cnAdapterMaxCount		= 10;

	DWORD adwIPAddressIn[cnAdapterMaxCount] = {0};
	DWORD adwIPAddressOut[cnAdapterMaxCount] = {0};

	DWORD dwInIPCount				= 0;
	DWORD dwOutIPCount			= 0;

	gGetIPAddressList(NULL, adwIPAddressIn, dwInIPCount, 
		NULL, adwIPAddressOut, dwOutIPCount, cnAdapterMaxCount);

	QCONFIRM_RET_FALSE(dwInIPCount > 0 && dwOutIPCount > 0);

	// IntranetIp
	if (dwInAdapterIdx <= 0 || dwInAdapterIdx > dwInIPCount)
		dwInAdapterIdx = 1;

	LPBYTE pByte = (LPBYTE)&adwIPAddressIn[dwInAdapterIdx - 1];
	snprintf(szInIp, MAX_PATH, "%u.%u.%u.%u", pByte[0], pByte[1], pByte[2], pByte[3]);

	// InternetIp
	if (dwOutAdapterIdx <= 0 || dwOutAdapterIdx > dwOutIPCount)
		dwOutAdapterIdx = 1;

	pByte = (LPBYTE)&adwIPAddressOut[dwOutAdapterIdx - 1];
	snprintf(szOutIp, MAX_PATH, "%u.%u.%u.%u", pByte[0], pByte[1], pByte[2], pByte[3]);

	return TRUE;
}

BOOL KSocketServer::GetSocketInfo(KSOCKET_KINGEYES_INFO* pcSocketInfo)
{
	if (pcSocketInfo)
	{
		*pcSocketInfo = m_cSocketInfo;
		return TRUE;
	}
	return FALSE;
}

BOOL KSocketServer::Init( IKSocketEventProcessor* piEventProcessor )
{
	QIniFile cIniFile;
	QCONFIRM_RET_FALSE(cIniFile.Load(KDF_SERVER_CFG));

	DWORD 		dwInAdapterIdx	= 0;
	DWORD		dwOutAdapterIdx = 0;
	CHAR szIntranetIp[KD_IPADDRESS_LEN] = {0}; // 内网Ip
	CHAR szInternetIp[KD_IPADDRESS_LEN] = {0}; // 外网Ip

	cIniFile.GetInteger("GameServer", "InAdapterIdx",	1,		(INT*)&dwInAdapterIdx);
	cIniFile.GetInteger("GameServer", "OutAdapterIdx",	1,		(INT*)&dwOutAdapterIdx);

	cIniFile.GetString("GameServer", "InIp",			"",	m_cParam.szIntranetIp, sizeof(m_cParam.szIntranetIp));
	cIniFile.GetString("GameServer", "OutIp",		"",	m_cParam.szInternetIp, sizeof(m_cParam.szInternetIp));
	cIniFile.GetInteger("GameServer", "Port", 5103, &m_cParam.nListenPort);

	if (g_cOrpgWorld.m_nServerId > 0)
		m_cParam.nListenPort += g_cOrpgWorld.m_nServerId - 1;

	// 获取本机内网外网IP
	if (!GetLocalIpAddress(dwInAdapterIdx, dwOutAdapterIdx, szIntranetIp, szInternetIp))
	{
		Q_Error("Failed to initiate server Intranet/Internet IP.");
		//return FALSE;
	}
	if (!m_cParam.szIntranetIp[0])
		QStrCpyLen(m_cParam.szIntranetIp, szIntranetIp, countof(m_cParam.szIntranetIp));
	if (!m_cParam.szInternetIp[0])
		QStrCpyLen(m_cParam.szInternetIp, szInternetIp, countof(m_cParam.szInternetIp));
	Q_Printl("Host Intranet IP: " << m_cParam.szIntranetIp);
	Q_Printl("Host Internet IP: " << m_cParam.szInternetIp);
	if (inet_addr(m_cParam.szInternetIp) == INADDR_ANY || inet_addr(m_cParam.szInternetIp) == INADDR_NONE)
	{
		QLogPrintf(LOG_WARNING,
			"Server绑定IP为%s，因为servercfg.ini配置项OutAdapterIdx和OutIp错误。",
			m_cParam.szInternetIp);
	}

	m_nMaxPlayer = g_piNsBaseRequire->GetEnvConfig()->nMaxPlayer;

	m_pSockEvents = new QSOCKET_EVENT[g_snMaxEventAccept];
	memset(m_pSockEvents, 0, sizeof(QSOCKET_EVENT) * g_snMaxEventAccept);
	m_cIdPool.Init(m_nMaxPlayer);

	m_pConnData = new KCONNECTION_DATA[m_nMaxPlayer + 1]; // 索引从1开始所以加1，0保留
	m_piEventProcessor = piEventProcessor;
	m_piEventProcessor->Init(m_nMaxPlayer);

	memset(&m_cSocketInfo, 0, sizeof(m_cSocketInfo));
	return TRUE;
}

BOOL KSocketServer::Send( INT nConnectId, INT nEventId, LPCVOID pData, INT nData )
{
	QCONFIRM_RET_FALSE(nConnectId <= m_nMaxPlayer);
	ISocketStream* piSocket = m_pConnData[nConnectId].piSocketStream;
	QCONFIRM_RET_FALSE(piSocket);
	QCONFIRM_RET_FALSE(pData && nData > 0);

	// 使用SocketPacker，并未立即发送
	INT nRet = m_pConnData[nConnectId].SocketPacker.Send(piSocket, (UINT)nData, (LPCBYTE)pData);
#ifndef _USE_SOCKET_PACKER
	m_pConnData[nConnectId].SocketPacker.FlushSend(piSocket);
#endif // ! _USE_SOCKET_PACKER
	if (nRet <= 0)
	{
		QLogPrintf(LOG_ERR, "Connect %d Send Error %d  Socket %p  IsAlive %d"
			, nConnectId, nRet, piSocket, piSocket->IsAlive());
		// 现在Player可能还在Activate生命周期内，释放玩家资源会造成问题 // TODO Detach
		//CloseConnect(nConnectId, TRUE);
		return FALSE;
	}
	m_cSocketInfo.dwSendPackages++;
	m_cSocketInfo.dwTxBytesDelta += nData;
	return TRUE;
}

// Flush SocketPacker里缓冲的网络包并发送
BOOL KSocketServer::RealSend( INT nConnectId )
{
	BOOL bResult = FALSE;
	if (nConnectId > 0)
	{
		if (!m_pConnData[nConnectId].piSocketStream)
			return TRUE;
		bResult = m_pConnData[nConnectId].SocketPacker.FlushSend(m_pConnData[nConnectId].piSocketStream);
		if (!bResult)
		{
			CloseConnect(nConnectId, TRUE);
		}
	}
	return bResult;
}

BOOL KSocketServer::MessageLoop()
{
	UINT uCurElapse = KSysService::GetTickCount();
#ifdef _USE_SOCKET_PACKER
	// 判断时间间隔，发送SocketPacker缓冲的数据包
	if (uCurElapse - m_uLastLoopElapse > (UINT)g_snSendInterval)
	{
		for (INT i = 1; i <= m_nMaxPlayer; ++i)
		{
			RealSend(i);
		}
		m_uLastLoopElapse = uCurElapse;
	}
#endif // _USE_SOCKET_PACKER

	// 统计网络流量
	static UINT uLastNetworkStatTime = uCurElapse;
	UINT uStatTimeDiff = uCurElapse - uLastNetworkStatTime;
	if (uStatTimeDiff > 1000)
	{
		m_cSocketInfo.dwSendBytes += m_cSocketInfo.dwTxBytesDelta;
		m_cSocketInfo.dwSendBytesPerSecond = m_cSocketInfo.dwTxBytesDelta * 1000 / uStatTimeDiff;
		m_cSocketInfo.dwRecvBytes += m_cSocketInfo.dwRxBytesDelta;
		m_cSocketInfo.dwRecvBytesPerSecond = m_cSocketInfo.dwRxBytesDelta * 1000 / uStatTimeDiff;

		m_cSocketInfo.dwRxBytesDelta = 0;
		m_cSocketInfo.dwTxBytesDelta = 0;

		uLastNetworkStatTime = uCurElapse;
	}

	INT nEventCount = 0;
	// 监听网络事件
	if (!m_cSockServer.Wait(g_snMaxEventAccept, m_pSockEvents, &nEventCount))
		return FALSE;
	QSOCKET_EVENT* pEvent = NULL;
	ISocketStream* pStream = NULL;
	for (INT i = 0; i < nEventCount; i++)
	{
		pEvent = &m_pSockEvents[i];
		pStream = pEvent->piSocket;
		pStream->AddRef();
		SAFE_RELEASE(pEvent->piSocket);

		// 假如是客户端连上
		if ((QSOCKET_EVENT_ACCEPT & pEvent->uEventFlag) != 0)
		{
			// 获取空闲连接号
			in_addr sIp;
			USHORT uPort = 0;
			INT nConnectId = m_cIdPool.New();
			memset(&sIp, 0, sizeof(sIp));
			pStream->GetRemoteAddress(&sIp, &uPort);
			uPort = ntohs(uPort); // 本来是GetRemoteAddress应该做的

			Q_Printl("Incoming connection: remote[" << inet_ntoa(sIp) << ":" << uPort << "]");
			m_cSocketInfo.dwTotalConnection++;
			m_cSocketInfo.dwCurrentConnection++;

			// 达到最大连接数，拒绝连接
			if (0 == nConnectId)
			{
				Q_Error("No ConnIndex to allocate, disconnecting...");
				SAFE_RELEASE(pStream);
				continue;
			}

			if (m_pConnData[nConnectId].nConnectId != 0 ||
				m_pConnData[nConnectId].piSocketStream != NULL)
				Q_Error("Pre Connect Release Error ?");

			m_pConnData[nConnectId].nConnectId = nConnectId;
			m_pConnData[nConnectId].piSocketStream = pStream;
			m_pConnData[nConnectId].piSocketStream->AddRef();
			m_pConnData[nConnectId].SocketPacker.Reset();
			pStream->SetUserData(&m_pConnData[nConnectId].nConnectId);			
			SAFE_RELEASE(pStream);
			// 连接建立事件
			m_piEventProcessor->ProcessEvent(nConnectId, emKSOCKET_EVENT_CREATE,
				inet_ntoa(sIp), uPort);

			m_cSocketInfo.dwProcessedConnection++;
			continue;
		}

		// 如果不是收到网络包事件
		if ((QSOCKET_EVENT_IN & pEvent->uEventFlag) == 0)
		{
			Q_Error("[KSocketServer] unknown event flag: " << pEvent->uEventFlag);
			SAFE_RELEASE(pStream);
			continue;
		}

		// 注意：当网络连接号满时，pStream没有SetUerData，但仍会收到KG_SOCKET_EVENT_IN事件！
		VOID* pUserData = pStream->GetUserData();	// pUserData指向连接索引
		INT nConnectId = 0;
		if (NULL == pUserData)
		{
			SAFE_RELEASE(pStream);
			continue;
		}

		INT nProcessCount	= 0;

		nConnectId = *(INT*)pUserData;
		// 循环中不断的去尝试接收数据
		// !!!有可能piSocketStream在此消息循环中被主动Release，所以使用m_pConnData[nConnectId].piSocketStream != NULL作为循环条件
		while (m_pConnData[nConnectId].piSocketStream != NULL)
		{
			nProcessCount++;
			if (nProcessCount > 1000)	// 消息过多，断开连接
			{
				m_piEventProcessor->ProcessEvent(nConnectId, emKSOCKET_EVENT_MASS, NULL, nProcessCount);

				CloseConnect(nConnectId, FALSE);
				break;
			}

			IMemBlock *pBuffer = NULL;
			INT nRetCode = pStream->Recv(&pBuffer);
			if (nRetCode <= 0)
			{
				// 如果收包出错，关闭连接
				if (nRetCode == -1)
				{
					m_piEventProcessor->ProcessEvent(nConnectId, emKSOCKET_EVENT_CLOSE, NULL, 0);
					CloseConnect(nConnectId, TRUE);
				}
				break;
			}

			LPVOID pData = pBuffer->GetData();
			UINT nLen = pBuffer->GetSize();

			m_cSocketInfo.dwRecvPackages++;
			m_cSocketInfo.dwRxBytesDelta += nLen;

			BOOL bProcessed = m_piEventProcessor->ProcessPacket(nConnectId, (LPCBYTE)pData, nLen);
			// 如果处理失败，断开该链接
			if (!bProcessed)
			{
				KPTC_HEADER_BASE* pHeader = (KPTC_HEADER_BASE*)pData;
				QLogPrintf(LOG_ERR, "Failed to process [PF %u, Protocol %u], disconnecting [%d]",
					pHeader->byProtocolFamily, pHeader->byProtocol, nConnectId);
				CloseConnect(nConnectId, FALSE);
				break;
			}

			SAFE_RELEASE(pBuffer);
		}

		SAFE_RELEASE(pStream);
	}

	m_piEventProcessor->Activate();
	return nEventCount;
}

BOOL KSocketServer::GetHostIP(KHOST_ADDRESS& rcAddress)
{
	rcAddress = m_cParam;
	return TRUE;
}

BOOL KSocketServer::CloseConnect( INT nConnectId,BOOL bLost )
{
	QCONFIRM_RET_FALSE(nConnectId > 0 && nConnectId <= m_nMaxPlayer);
	QCONFIRM_RET_FALSE(m_pConnData[nConnectId].piSocketStream);
	Q_Printl("[SocketServer] disconnecting [" << nConnectId << "]...");
	// 回收连接号
	m_cIdPool.Delete(nConnectId);
	// 释放资源
	m_pConnData[nConnectId].piSocketStream->SetUserData(NULL);
	SAFE_RELEASE(m_pConnData[nConnectId].piSocketStream);
	m_pConnData[nConnectId].SocketPacker.Reset(); // 清空网络包缓存
	m_pConnData[nConnectId].nConnectId = 0;
	// 抛出连接关闭事件
	m_piEventProcessor->ProcessEvent(nConnectId, emKSOCKET_EVENT_CLOSE, NULL, 0);

	if (bLost)
		m_cSocketInfo.dwTotalLostConnection++;
	else
		m_cSocketInfo.dwTotalCloseConnection++;
	m_cSocketInfo.dwCurrentConnection--;
		
	return TRUE;
}

BOOL KSocketServer::Disconnect( INT nConnectId )
{
	QCONFIRM_RET_FALSE(nConnectId != -1);
	return CloseConnect(nConnectId, FALSE);
}

BOOL KSocketServer::GetRemoteAddress( INT nConnectId, DWORD& rdwIp, USHORT& ruPort )
{
	QCONFIRM_RET_FALSE(nConnectId > 0 && nConnectId <= m_nMaxPlayer);
	QCONFIRM_RET_FALSE(m_pConnData[nConnectId].piSocketStream);
	m_pConnData[nConnectId].piSocketStream->GetRemoteAddress((struct in_addr*)&rdwIp, &ruPort);
	ruPort = ntohs(ruPort); // sh1t! should have been done by GetRemoteAddress
	return TRUE;
}

BOOL KSocketServer::SocketClose()
{
	for (int i = 1; i <= m_nMaxPlayer && m_pConnData; ++i)
	{
		if (m_pConnData[i].piSocketStream)
			CloseConnect(i, FALSE);
	}
	m_cSockServer.UnInit(NULL);
	m_cIdPool.Uninit();
	SAFE_DELETE_ARRAY(m_pSockEvents);
	SAFE_DELETE_ARRAY(m_pConnData);

	m_nMaxPlayer = 0;
	m_piEventProcessor = NULL;
	m_uLastLoopElapse = 0;
	return TRUE;
}
// -------------------------------------------------------------------------
