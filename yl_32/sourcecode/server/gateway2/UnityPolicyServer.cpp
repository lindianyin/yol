/*
  Copyright (C) 2011 Simon<senoutouya@gmail.com>.  All rights reserved.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Unity3D插件policy请求应答程序
 */

#include "stdafx.h"
#include "UnityPolicyServer.h"

#ifdef unix
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>

typedef struct sockaddr SOCKADDR;
#define SOCKET_ERROR            (-1)
#define SOCKET UINT
#define INVALID_SOCKET (SOCKET)-1
#define closesocket close
#endif // unix
// --------------------------------------------------------------------
#define CLIENT_SERVICE_PORT 843

SOCKET g_nRelaySocket = INVALID_SOCKET;
SOCKET g_nRelayListener = INVALID_SOCKET;
SOCKET g_nClientServiceListener = INVALID_SOCKET;

#define RECV_BUFFER_SIZE 1024
#define PING_TIME_OUT 60

BYTE g_abyRecvBuff[RECV_BUFFER_SIZE];

struct CLIENT_SOCKET 
{
	CLIENT_SOCKET()
	{
		nClientServiceSocket = INVALID_SOCKET;
		nReplyTime = 0;
	}

	SOCKET nClientServiceSocket;
	INT nReplyTime;
};
std::map<INT, CLIENT_SOCKET> g_mapClientSocks; // socket clients

const static CHAR POLICY_REPLY[] = "<?xml version='1.0'?><cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\" /></cross-domain-policy>";
// --------------------------------------------------------------------


static inline int _GetSocketErrorCode()
{
#ifdef WIN32
	return WSAGetLastError();
#else
	return errno;
#endif
}

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanSend(int nSocket, const timeval *pcTimeout)
{
	if (nSocket < 0)
		return -1;

	fd_set FDSet;
	FD_ZERO(&FDSet); 
	FD_SET(nSocket, &FDSet);

	timeval  TempTimeout;
	timeval *pTempTimeout = NULL;

	if (pcTimeout)
	{
		TempTimeout  = *pcTimeout;
		pTempTimeout = &TempTimeout;
	}

	int nRetCode = select(nSocket + 1, NULL, &FDSet, NULL, pTempTimeout);

	if (nRetCode == 0)
		return 0;

	if (nRetCode > 0)
		return 1;

	nRetCode = _GetSocketErrorCode();
	QLogPrintf(
		LOG_DEBUG,
		"nSocket = %d, _CheckCanSend(), Error = %d\n",
		nSocket, 
		nRetCode
		);

	return -1;        
}

#ifdef unix

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanRecv(int nSocket, const timeval *pcTimeout)
{
	if (nSocket < 0)
		return -1;

	int nTimeout = -1;
	if (pcTimeout)
	{
		nTimeout = (pcTimeout->tv_sec * 1000) + (pcTimeout->tv_usec / 1000);
	}

	struct pollfd PollFD;
	PollFD.fd = nSocket;
	PollFD.events  = POLLIN;
	PollFD.revents = 0;

	int nRetCode = poll(&PollFD, 1, nTimeout);

	if (nRetCode == 0)
		return 0;

	if (nRetCode > 0)
	{
		ASSERT(
			(PollFD.revents & POLLIN ) ||
			(PollFD.revents & POLLERR) ||
			(PollFD.revents & POLLHUP)
			);
		return 1;
	}

	nRetCode = _GetSocketErrorCode();

	printf(
		"nSocket = %d, _CheckCanRecv(), Error = %d\n",
		nSocket, 
		nRetCode
		);

	return -1;        
}

#else

// return -1: error, 0: timeout, 1: success
static inline int _CheckCanRecv(int nSocket, const timeval *pcTimeout)
{
	if (nSocket < 0)
		return -1;

	fd_set FDSet;
	FD_ZERO(&FDSet);
	FD_SET(nSocket, &FDSet);

	timeval  TempTimeout;
	timeval *pTempTimeout = NULL;

	if (pcTimeout)
	{
		TempTimeout  = *pcTimeout;
		pTempTimeout = &TempTimeout;
	}

	int nRetCode = select(nSocket + 1, &FDSet, NULL, NULL, pTempTimeout);

	if (nRetCode == 0)
		return 0;

	if (nRetCode > 0)
		return 1;

	nRetCode = _GetSocketErrorCode();
	QLogPrintf(
		LOG_DEBUG,
		"nSocket = %d, _CheckCanRecv(), Error = %d\n",
		nSocket, 
		nRetCode
		);

	return -1;
}
#endif // unix

int StartClientServiceListener()
{
	g_nClientServiceListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_nClientServiceListener == INVALID_SOCKET) {
		QLogPrintf(LOG_ERR, "Error at socket(): %ld\n", _GetSocketErrorCode());
		return FALSE;
	}

	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(CLIENT_SERVICE_PORT);

	QLogPrintf(LOG_INFO, "Bind to [%s:%d] ", inet_ntoa(service.sin_addr), ntohs(service.sin_port));

	if (bind( g_nClientServiceListener, (SOCKADDR*) &service, sizeof(service)) == SOCKET_ERROR) {
		QLogPrintf(LOG_ERR, "bind() failed.\n");
		closesocket(g_nClientServiceListener);
		g_nClientServiceListener = INVALID_SOCKET;
		return FALSE;
	}

	if (listen( g_nClientServiceListener, SOMAXCONN ) == SOCKET_ERROR)
	{
		QLogPrintf(LOG_ERR, "Error listening on socket.\n");
		closesocket(g_nClientServiceListener);
		g_nClientServiceListener = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

int CheckNewClient()
{
	ASSERT(g_nClientServiceListener != INVALID_SOCKET);

	timeval tmval = { 0, 0 };

	int nRet = _CheckCanRecv(g_nClientServiceListener, &tmval);
	if (nRet == 1)
	{
		struct sockaddr_in RemoteAddr;
		socklen_t nAddrLen = sizeof(sockaddr_in);
		memset((void *)&RemoteAddr, 0, sizeof(sockaddr_in));

		SOCKET nClientServiceSock = accept(g_nClientServiceListener, (struct sockaddr *)&RemoteAddr, &nAddrLen);
		if (nClientServiceSock == INVALID_SOCKET)
		{
			QLogPrintf(LOG_ERR, "policy client accept failed: %d\n", _GetSocketErrorCode());
		}
		else
		{
			QLogPrintf(LOG_DEBUG, "policy client accepted from [%s:%d].\n", inet_ntoa(RemoteAddr.sin_addr), ntohs(RemoteAddr.sin_port));
			static int nTunnelPairId = 0;
			nTunnelPairId++;
			g_mapClientSocks[nTunnelPairId].nClientServiceSocket = nClientServiceSock;
		}
	}
	else if (nRet < 0)
	{
		QLogPrintf(LOG_ERR, "failed to accept policy client: _CheckCanRecv result %d\n", nRet);
		//ProxyDisconnected();
	}
	return TRUE;
}

int ProcessPacketClientService(int nTunnelId, LPCBYTE pData, int nLen)
{
	ASSERT(pData && nLen);

	printf("policy request %d: %.*s\n", nTunnelId, nLen, pData);
	send(g_mapClientSocks[nTunnelId].nClientServiceSocket, POLICY_REPLY, sizeof(POLICY_REPLY), 0);
	printf("size %d: %s\n", (int)sizeof(POLICY_REPLY), POLICY_REPLY);

	return TRUE;
}

int RecvClientServiceSocket(int nTunnelId, int nSocket)
{
	timeval tmval = { 0, 0 };
	BOOL bSuccess = TRUE;

	int nRet = _CheckCanRecv(nSocket, &tmval);
	if (nRet == 1)
	{
		//time_t timeStart = time(NULL);
		nRet = recv(nSocket, (LPSTR)g_abyRecvBuff, sizeof(g_abyRecvBuff), 0);
		//time_t timeNow = time(NULL);
		//if (timeNow - timeStart > 1)
		//{
		//	printf("WARNING: RecvClientServiceSocket TOO LONG %d \n", timeNow - timeStart);
		//	timeStart = timeNow;
		//}
		if (nRet > 0)
		{
			ProcessPacketClientService(nTunnelId, g_abyRecvBuff, nRet);
		}
		else if (nRet == 0)
		{
			printf("policy client %d disconnected\n", nTunnelId);
			bSuccess = FALSE;
		}
		else
		{
			printf("recv failed: %d\n", _GetSocketErrorCode());
			bSuccess = FALSE;
		}
	}
	else if (nRet < 0)
	{
		printf("_CheckCanRecv result %d\n", nRet);
		bSuccess = FALSE;
	}
	if ( ! bSuccess)
	{
		closesocket(nSocket);

		//printf("disconnect ClientServiceSocket %d, tunnel %d\n", nSocket, nTunnelId);
	}
	return bSuccess;
}

int RecvFromClient()
{
	for (std::map<int, CLIENT_SOCKET>::iterator it = g_mapClientSocks.begin(); it != g_mapClientSocks.end(); )
	{
		INT nResult = RecvClientServiceSocket(it->first, it->second.nClientServiceSocket);

		if (nResult)
			++it;
		else
			g_mapClientSocks.erase(it++);
	}

	return TRUE;
}

UnityPolicyServer::UnityPolicyServer(void)
{
	bInitSuccess = TRUE;
}

UnityPolicyServer::~UnityPolicyServer(void)
{
}

INT UnityPolicyServer::Init(void)
{
	QLogPrintf(LOG_DEBUG, "UnityPolicyServer Starting up...\n");
	INT nResult = StartClientServiceListener();
	if(!nResult)
	{
		bInitSuccess = FALSE;
	}
	return nResult;
}

INT UnityPolicyServer::Activate(void)
{
	if(bInitSuccess)
	{
		CheckNewClient();
		RecvFromClient();
	}
	return 0;
}

INT UnityPolicyServer::Uninit()
{
	return 0;
}
