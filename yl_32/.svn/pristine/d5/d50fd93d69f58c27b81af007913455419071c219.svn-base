/* -------------------------------------------------------------------------
//	文件名		：	kg_censerver.cpp
//	创建者		：	luobaohang
//	创建时间	：	2006-12-22 10:02:02
//	功能描述	：	中央服务器类（底下连接数个有限并有身份认证的客户端）
//
// -----------------------------------------------------------------------*/

#include "stdafx.h"
#include "serverbase/kg_censerver.h"
#include "Misc/utilities.h"
#include "onlinegamebase/ksysservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static CHAR THIS_FILE[] = __FILE__;
#endif

// -------------------------------------------------------------------------
KG_CenServer::KG_CenServer()
{
	m_szListenIp[0] = '\0';
	m_nListenPort = 0;
	m_bRuningState = FALSE;
}

BOOL KG_CenServer::Init( LPCSTR szListenIp, INT nListenPort ) 
{
	if (GetRuningState())
		ShutDown();
	QStrCpyLen(m_szListenIp, szListenIp, sizeof(m_szListenIp));
	m_nListenPort = nListenPort;
	m_IDGenerator = UINT_MAX;
	return TRUE;
}

BOOL KG_CenServer::StartUp() 
{
	INT nStep = 0;
	Q_Printl("[CenServer] prepare to listen @ " << m_szListenIp << ":" << m_nListenPort);
	if (!m_Acceptor.Open(m_szListenIp, m_nListenPort))
	{
		Q_Printl("[CenServer] Listen Error! LocalIP = " << m_szListenIp << 
			" Port = " << m_nListenPort);
		goto EXIT0;	
	}
	nStep = 1;
	SetRuningState();
	if (!m_WorkThread.Create(_WorkThreadFunction, this))
	{
		Q_Printl("[CenServer] Create WorkThred Error!");
		goto EXIT0;
	}
	nStep = 2;
	return TRUE;
EXIT0:
	switch(nStep)
	{
	case 2:
		m_WorkThread.Wait();		
	case 1:
		m_Acceptor.Close();
	}
	return FALSE;
}

BOOL KG_CenServer::ShutDown()
{	
	ReSetRuningState();
	//等待线程结束
	m_MutexWorkThread.Lock();
	m_MutexWorkThread.Unlock();
	m_WorkThread.Wait();
	m_Acceptor.Close();
	std::map<INT, KCenServerConnect *>::iterator it;
	for (it = m_mpConnect.begin(); it != m_mpConnect.end(); ++it)
	{
		it->second->Destroy();
	}
	m_mpConnect.clear();
	return TRUE;
}

INT KG_CenServer::_ThreadFunction()
{
	m_MutexWorkThread.Lock();
	ISocketStream *piSockStream = NULL;
	timeval TimeVal = {0, 500};
	struct in_addr RemoteAddr = { 0 };
	USHORT usRemotePort = 0;
	//因为要检查Exit，TimeOut不能无限制
	m_Acceptor.SetTimeout(&TimeVal);
	while (GetRuningState())
	{
		piSockStream = m_Acceptor.Accept();
		if (!piSockStream)
		{
			QThread_Sleep(1);
			continue;  // timeout
		}
		
		// 因为Acceptor接收到链接后会把自身的超时间隔copy到新链接，所以要恢复为无超时
		timeval TimeOut = {-1, 0};
		piSockStream->SetTimeout(&TimeOut);
		piSockStream->GetRemoteAddress(&RemoteAddr, &usRemotePort);

		Q_Printl(
			"[CenServer] Accept from " << inet_ntoa(RemoteAddr) << ":" <<  usRemotePort <<
			" " << m_IDGenerator - 1 << "*"
			);
		CreateConnect(piSockStream);
		SAFE_RELEASE(piSockStream);
	}
	m_MutexWorkThread.Unlock();
	return 0;
}

VOID KG_CenServer::_WorkThreadFunction(LPVOID pvParam) 
{
	reinterpret_cast<KG_CenServer *>(pvParam)->_ThreadFunction();
}

BOOL KG_CenServer::CreateConnect(ISocketStream *piSockStream) 
{
	KCenServerConnect *pNewConnect = new KCenServerConnect(piSockStream, TRUE);
	if (!pNewConnect)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_ConnectLock.Lock();
	m_vecConnectWait.push_back(pNewConnect);
	m_ConnectLock.Unlock();
	return TRUE;
}

KCenServerConnect * KG_CenServer::GetConnect(INT nID) 
{
	std::map<INT, KCenServerConnect *>::iterator it;
	if ((it = m_mpConnect.find(nID)) != m_mpConnect.end())
		return it->second;
	return NULL;
}

BOOL KG_CenServer::CheckNewConnect() 
{
	m_ConnectLock.Lock();
	while (!m_vecConnectWait.empty())
	{
		m_mpConnect[--m_IDGenerator] = m_vecConnectWait.back();
		OnConnectCreated(m_IDGenerator, m_vecConnectWait.back());
		m_vecConnectWait.pop_back();
	}
	m_ConnectLock.Unlock();
	return TRUE;
}

BOOL KG_CenClient::Init(QSocketConnector* pConnector)
{
	ASSERT(pConnector);
	m_pConnector = pConnector;
	return TRUE;
}

VOID KG_CenClient::UnInit()
{
	m_pConnector = NULL;
}

//////////////////////////////KG_CenClient///////////////////////////
BOOL KG_CenClient::Connect( CONST CHAR szServerIP[64], INT nListenPort ) 
{
	std::cout << "Connecting GameCenter ["<< szServerIP << ":" << nListenPort << "]... ";

	ISocketStream *piSocketStream = m_pConnector->Connect(szServerIP, nListenPort);
	if (!piSocketStream)
	{
		Q_Printl("Failed.");
		return FALSE; 
	}
	Q_Printl("OK.");

	CreateConnect(piSocketStream);
	SAFE_RELEASE(piSocketStream);
	return TRUE;
}

BOOL KG_CenClient::ConnectSecurity( CONST CHAR cszIPAddress[], 
								   INT nPort, 
								   ENCODE_DECODE_MODE EncodeDecodeMode ) 
{
	ISocketStream *piSocketStream = m_pConnector->ConnectSecurity(cszIPAddress, nPort, EncodeDecodeMode);
	if (!piSocketStream)
	{
		Q_Printl(
			"[CenServer] Connect Failed! IP: " << cszIPAddress << " SocketStream = " <<  piSocketStream
			);
		return FALSE; 
	}
	Q_Printl(
		"[CenServer] Connected ! IP: " << cszIPAddress << " SocketStream = " <<  piSocketStream
		);
	CreateConnect(piSocketStream);
	SAFE_RELEASE(piSocketStream);
	return TRUE;
}

BOOL KG_CenClient::CreateConnect( ISocketStream *piSockStream ) 
{
	if (m_pConnect)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pConnect = new KCenServerConnect(piSockStream, FALSE);
	if (!m_pConnect)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	OnConnectCreated(m_pConnect);
	return TRUE;
}

VOID KG_CenClient::Loop()
{
	if (m_pConnect)
		m_pConnect->Loop();
}

BOOL KG_CenClient::ShutDown() 
{
	if (m_pConnect)
		m_pConnect->Destroy();
	m_pConnect = NULL;
	return TRUE;
}
//////////////////////////////KG_KCenServerConnect///////////////////////////
KCenServerConnect::KCenServerConnect( ISocketStream *piSockStream, BOOL bServer ) :
	m_piSocketStream(piSockStream), m_bIsValid(TRUE), m_bServer(bServer)
{
	piSockStream->AddRef();
	m_bRuningState = TRUE;
	m_bIsGood = TRUE;
	m_uLastReceiveTime = 0x7fffffff;	
	timeval TimeVal = {10, 0};
#ifndef GAME_CENTER
	// 设超时，如果无超时会有问题
	m_piSocketStream->SetTimeout(&TimeVal);
#else
// 	IKGC_Config* pGCConfig = GetGCConfig();
// 	if (pGCConfig)
// 	{
// 		m_nDisConnectTime = pGCConfig->DisconnectTime() * 1000;
// 		TimeVal.tv_sec = pGCConfig->SendTimeout();
// 	}
// 	else
	{
//		Q_Error("GetGCConfig() Error!");
		m_nDisConnectTime = 50 * 1000;
	}
	// 设超时，如果无超时会有问题
	m_piSocketStream->SetTimeout(&TimeVal);
#if defined(_DEBUG) && defined(_WIN32)
	m_nDisConnectTime += 100 * 1000;
#endif
	m_SocketThread.Create(_WorkThreadFunction, this);
#endif
}

VOID KCenServerConnect::Loop()
{
#ifndef GAME_CENTER
	if (!m_bIsValid)
		return;
	IMemBlock *piRequestPacket = NULL;
	timeval TimeVal = {0, 500};
	//static INT nLoopCount = 0;
	do
	{
		// 有包时才Recv，不然会无限等待
		if (m_piSocketStream->CheckCanRecv(&TimeVal) != 1)
			break;
		// TODO: here
		if (m_piSocketStream->Recv(&piRequestPacket) < 0)
		{
			Q_Printl("[CenServer] Receive packet failed!");
			m_bIsValid = FALSE;
			break;
		}
		if(piRequestPacket == NULL)   // if no data arrive
			break;
		//TODO：加超时断连功能
		//m_nLastReceiveTime = GetTickCount();
		m_vecPackagReveive.push_back(piRequestPacket);
		piRequestPacket = NULL;
		//nLoopCount++;
	}
	while (TRUE);
	//m_uLastReceiveTime = KSysService::QIniFile();
#endif
}

INT KCenServerConnect::_SocketThreadFunction() 
{
	m_CloseLock.Lock();
	timeval TimeVal = {0, 1000};
	m_uLastReceiveTime = KSysService::GetTickCount() + m_nDisConnectTime;
	//m_piSocketStream->SetTimeout(&TimeVal);
	while (GetRuningState())
	{
		IMemBlock *piRequestPacket = NULL;
		// 有包时才Recv，不然会无限等待
		if (m_piSocketStream->CheckCanRecv(&TimeVal) != 1)
		{
			QThread_Sleep(1);
		}
		else if (m_piSocketStream->Recv(&piRequestPacket) == -1)
		{
			Q_Printl("[CenServer] Receive packet failed!");
			//因为socket CLOSE_WAIT的bug，接收失败就当断线了，不再判断Alive
			if(TRUE || !m_piSocketStream->IsAlive())
			{
				do
				{
					m_bIsGood = FALSE;
				}while (m_bIsGood);
				//连接丢失，输出信息
				//struct in_addr RemoteAddr = { 0 };
				//USHORT usRemotePort = 0;
				//m_piSocketStream->GetRemoteAddress(&RemoteAddr, &usRemotePort);
				//Q_Printl("[CenServer] Connection Lost! Remote IP = " << inet_ntoa(RemoteAddr));
				//Socket已不可用，进入休眠状态
#ifdef KD_CENCONNECT_RELEASE_SELF
				while (GetRuningState())
				{
					QThread_Sleep(100);
				}				
#endif

				break;
			}
			continue;
		}

		UINT uCurTickCount = KSysService::GetTickCount();
		if(piRequestPacket == NULL)   // if no data arrive
		{
			// 超时断连
			if (m_bServer && uCurTickCount > m_uLastReceiveTime + m_nDisConnectTime)
			{
				do
				{
					m_bIsGood = FALSE;
				} while (m_bIsGood);
				//连接丢失，输出信息
				struct in_addr RemoteAddr = { 0 };
				USHORT usRemotePort = 0;
				m_piSocketStream->GetRemoteAddress(&RemoteAddr, &usRemotePort);
				Q_Printl("[CenServer] Connection Lost For Reveive Timeout! Remote IP = " 
					<< inet_ntoa(RemoteAddr) <<	"Port = " << usRemotePort);
				//Socket已不可用，进入休眠状态
#ifdef KD_CENCONNECT_RELEASE_SELF
				while (GetRuningState())
				{
					QThread_Sleep(100);
				}
#endif
				break;
			}
			continue;
		}

		m_uLastReceiveTime = KSysService::GetTickCount();
		m_PackagLock.Lock();
		m_vecPackagReveive.push_back(piRequestPacket);
		m_PackagLock.Unlock();
	}
	// 本来自身释放是很好的方式，但是申请和释放线程不同影响了内存泄漏检测...
#ifdef KD_CENCONNECT_RELEASE_SELF
	delete this;	//线程结束并消亡
#endif
	do
	{
		m_bIsValid = FALSE;
	} while(m_bIsValid);
	m_CloseLock.Unlock();
	return 0;
}

VOID KCenServerConnect::_WorkThreadFunction( LPVOID pvParam ) 
{
	reinterpret_cast<KCenServerConnect *>(pvParam)->_SocketThreadFunction();
}

KCenServerConnect::~KCenServerConnect() 
{
	std::vector<IMemBlock *>::iterator it;
	m_PackagLock.Lock();
	for (it = m_vecPackagReveive.begin();it!= m_vecPackagReveive.end(); ++it)
	{
		SAFE_RELEASE(*it);
	}
	m_vecPackagReveive.clear();
	m_PackagLock.Unlock();
	SAFE_RELEASE(m_piSocketStream);
}

BOOL KCenServerConnect::SendPacket( IMemBlock *piBuffer ) 
{
	if (!GetRuningState())
		return FALSE;
	INT nRet = m_piSocketStream->Send(piBuffer);
	if (1 != nRet)
	{
		struct in_addr RemoteAddr = { 0 };
		USHORT usRemotePort = 0;
		m_piSocketStream->GetRemoteAddress(&RemoteAddr, &usRemotePort);
		Q_Error("Send Error! Ret Code = " << nRet << " Remote IP = " << inet_ntoa(RemoteAddr)
			 <<	"Port = " << usRemotePort << " Protocol = " << (UINT)*(LPBYTE)piBuffer->GetData() << 
			 " Size = " << piBuffer->GetSize());
		// 发送失败断开
		ReSetRuningState();
		return FALSE;
	}
	return TRUE;
}

VOID KCenServerConnect::Destroy() 
{
	ReSetRuningState();
	m_CloseLock.Lock();
	m_CloseLock.Unlock();
//#ifdef GAME_CENTER
//	m_SocketThread.Destroy(); 
//#endif
	delete this;
}
