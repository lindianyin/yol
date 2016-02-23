#include "CorePrivate.h"

#include <time.h>
#include <stdlib.h>

#if defined(__FreeBSD__) || defined(__APPLE__)
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <net/if_dl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <net/if_types.h>
#include <errno.h>
#include <sys/sockio.h>
#include <sys/sysctl.h>
#include <net/ethernet.h>
#endif // __FreeBSD__/OSX

#include "IpMacAddress.h"

#ifdef WIN32

#define MAX_ADAPTER_DESCRIPTION_LENGTH  128 // arb.
#define MAX_ADAPTER_NAME_LENGTH         256 // arb.
#define MAX_ADAPTER_ADDRESS_LENGTH      8   // arb.


typedef struct {
	char String[4 * 4];
} K_IP_ADDRESS_STRING, *K_PIP_ADDRESS_STRING, K_IP_MASK_STRING, *K_PIP_MASK_STRING;

typedef struct _K_IP_ADDR_STRING {
	struct _K_IP_ADDR_STRING* Next;
	K_IP_ADDRESS_STRING IpAddress;
	K_IP_MASK_STRING IpMask;
	DWORD Context;
} K_IP_ADDR_STRING, *K_PIP_ADDR_STRING;

typedef struct _K_IP_ADAPTER_INFO {
	struct _K_IP_ADAPTER_INFO* Next;
	DWORD ComboIndex;
	char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
	char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
	UINT AddressLength;
	BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
	DWORD Index;
	UINT Type;
	UINT DhcpEnabled;
	K_PIP_ADDR_STRING CurrentIpAddress;
	K_IP_ADDR_STRING IpAddressList;
	K_IP_ADDR_STRING GatewayList;
	K_IP_ADDR_STRING DhcpServer;
	BOOL HaveWins;
	K_IP_ADDR_STRING PrimaryWinsServer;
	K_IP_ADDR_STRING SecondaryWinsServer;
	time_t LeaseObtained;
	time_t LeaseExpires;
} K_IP_ADAPTER_INFO, *K_PIP_ADAPTER_INFO;



//此函数会确保两个地址均有
//返回值为网卡数量
int gGetMacAndIPAddress(
						unsigned char *pMacAddressIn,   unsigned long *pIPAddressIn, 
						unsigned char  *pMacAddressOut, unsigned long *pIPAddressOut, 
						unsigned int uMask, 
						int nMacLength
						)
{
	typedef DWORD(CALLBACK * PGAINFO)(K_PIP_ADAPTER_INFO, PULONG);//GetAdaptersInfo

	//加载IP Helper API 所需的库文件
	HINSTANCE hInst = LoadLibraryA("iphlpapi.dll");
	if(!hInst)
		return -1;
	PGAINFO pGAInfo = (PGAINFO)GetProcAddress(hInst,"GetAdaptersInfo");
	if (!pGAInfo)
		return -1;
	K_PIP_ADAPTER_INFO pInfo = NULL;

	ULONG ulSize = 0;

	pGAInfo(pInfo, &ulSize);//第一次调用，获取缓冲区大小

	pInfo = (K_PIP_ADAPTER_INFO)alloca(ulSize);

	pGAInfo(pInfo, &ulSize);

	bool bIn = false;
	char* pMacAddressInX = (char*)alloca(nMacLength + 1);
	DWORD nIPAddressInX = 0;
	bool bOut = false;
	char* pMacAddressOutX = (char*)alloca(nMacLength + 1);
	DWORD nIPAddressOutX = 0;

	DWORD nMAcNum = 0;
	//遍历每一张网卡

	while (pInfo && (!bIn || !bOut))
	{
		K_PIP_ADDR_STRING pAddTemp = &(pInfo->IpAddressList);       

		while (pAddTemp)/*遍历IP列表中的每一个元素*/
		{
			DWORD nAddress = inet_addr(pAddTemp->IpAddress.String);
			if (!bIn &&
				((nAddress & 0x0000FFFF) == uMask)
				)
			{
				bIn = true;
				nIPAddressInX = nAddress;
				//物理地址的长度
				if (pInfo->AddressLength == nMacLength)
					memcpy(pMacAddressInX, pInfo->Address, nMacLength);
				else
					memset(pMacAddressInX, 0, nMacLength);
			}

			if (!bOut &&
				((nAddress & 0x0000FFFF) != uMask)
				)
			{
				bOut = true;
				nIPAddressOutX = nAddress;
				//物理地址的长度
				if (pInfo->AddressLength == nMacLength)
					memcpy(pMacAddressOutX, pInfo->Address, nMacLength);
				else
					memset(pMacAddressOutX, 0, nMacLength);
			}
			pAddTemp = pAddTemp->Next;
		}
		//将当前指针移向下一个
		pInfo = pInfo->Next;

		nMAcNum++;
	}

	if (bIn)
	{
		if (pMacAddressIn)
			memcpy(pMacAddressIn, pMacAddressInX, nMacLength);
		if (pIPAddressIn)
			*pIPAddressIn = nIPAddressInX;

		if (!bOut)
		{
			if (pMacAddressOut)
				memcpy(pMacAddressOut, pMacAddressInX, nMacLength);
			if (pIPAddressOut)
				*pIPAddressOut = nIPAddressInX;
		}
	}

	if (bOut)
	{
		if (pMacAddressOut)
			memcpy(pMacAddressOut, pMacAddressOutX, nMacLength);
		if (pIPAddressOut)
			*pIPAddressOut = nIPAddressOutX;

		if (!bIn)
		{
			if (pMacAddressIn)
				memcpy(pMacAddressIn, pMacAddressOutX, nMacLength);
			if (pIPAddressIn)
				*pIPAddressIn = nIPAddressOutX;
		}
	}

	FreeLibrary(hInst);

	return nMAcNum;
}

//获取内外网IP列表
//返回值为网卡数量
int _gGetIPAddressList(
				KADAPTER_MACADDRESS* pMacAddressIns, DWORD* pdwIPAddressIns, DWORD& dwInIPCount, 
				KADAPTER_MACADDRESS* pMacAddressOuts, DWORD* pdwIPAddressOuts, DWORD& dwOutIPCount, 
				DWORD dwListCount, DWORD dwMask)
{
	typedef DWORD(CALLBACK * PGAINFO)(K_PIP_ADAPTER_INFO, PULONG);//GetAdaptersInfo

	//加载IP Helper API 所需的库文件
	HINSTANCE hInst = LoadLibraryA("iphlpapi.dll");
	if(!hInst)
		return -1;
	PGAINFO pGAInfo = (PGAINFO)GetProcAddress(hInst,"GetAdaptersInfo");
	if (!pGAInfo)
		return -1;
	K_PIP_ADAPTER_INFO pInfo = NULL;

	ULONG ulSize = 0;

	pGAInfo(pInfo, &ulSize);//第一次调用，获取缓冲区大小

	pInfo = (K_PIP_ADAPTER_INFO)alloca(ulSize);

	pGAInfo(pInfo, &ulSize);

	if (pMacAddressIns)
	{
		memset(pMacAddressIns, 0, sizeof(KADAPTER_MACADDRESS) * dwListCount);
	}
	if (pMacAddressOuts)
	{
		memset(pMacAddressOuts, 0, sizeof(KADAPTER_MACADDRESS) * dwListCount);
	}

	DWORD nMAcNum = 0;
	//遍历每一张网卡
	dwInIPCount = 0;
	dwOutIPCount = 0;
	while (pInfo)
	{
		K_PIP_ADDR_STRING pAddTemp = &(pInfo->IpAddressList);       

		while (pAddTemp)/*遍历IP列表中的每一个元素*/
		{
			DWORD nAddress = inet_addr(pAddTemp->IpAddress.String);

			// IN
			if ((nAddress & 0x0000FFFF) == dwMask && dwInIPCount <= dwListCount)
			{
				if (pMacAddressIns && pInfo->AddressLength <= KD_MAX_MACADDRESS_LEN)
				{
					memcpy(pMacAddressIns[dwInIPCount].szMacAddress, pInfo->Address, pInfo->AddressLength);
				}
				if (pdwIPAddressIns)
					pdwIPAddressIns[dwInIPCount] = nAddress;

				dwInIPCount ++;
			}

			if ((nAddress & 0x0000FFFF) != dwMask && dwOutIPCount <= dwListCount)
			{
				if (pMacAddressOuts && pInfo->AddressLength <= KD_MAX_MACADDRESS_LEN)
				{
					memcpy(pMacAddressOuts[dwOutIPCount].szMacAddress, pInfo->Address, pInfo->AddressLength);
				}
				if (pdwIPAddressOuts)
					pdwIPAddressOuts[dwOutIPCount] = nAddress;
				
				dwOutIPCount ++;
			}
			pAddTemp = pAddTemp->Next;
		}
		//将当前指针移向下一个
		pInfo = pInfo->Next;

		nMAcNum++;
	}

	FreeLibrary(hInst);

	return nMAcNum;
}

#else
// Linux

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>

//此函数会确保两个地址均有
//返回值为网卡数量
int gGetMacAndIPAddress(
    unsigned char  *pMacAddressIn,  unsigned long *pIPAddressIn, 
    unsigned char  *pMacAddressOut, unsigned long *pIPAddressOut, 
    unsigned int uMask, 
    int nMacLength)
{
#if defined(__FreeBSD__) || defined(__APPLE__)
return 1;
#else
	int nResult  = false;
	int nRetCode = false;

	int nSocket = -1;
	int nSize   = -1;
	struct ifconf  ifc;
	struct ifreq  *pifreq    = NULL;

	struct sockaddr_in InAddress;
	unsigned long ulAddress = 0;
	bool bIn = false;
	char *pMacAddressInX = (char *)alloca(nMacLength + 1);
	unsigned long ulIPAddressInX = 0;
	bool bOut = false;
	char *pMacAddressOutX = (char *)alloca(nMacLength + 1);
	unsigned ulIPAddressOutX = 0;


	memset(&ifc, 0, sizeof(ifc));

	nSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (nSocket < 0)
		goto EXIT0;

	ifc.ifc_req = NULL;
	ifc.ifc_len = 0;
	nRetCode = ioctl(nSocket, SIOCGIFCONF, &ifc);
	if (nRetCode)  // success when nRetCode == 0
		goto EXIT0;

	nSize = ifc.ifc_len / sizeof(struct ifreq);
	if (!nSize)
		goto EXIT0;

	ifc.ifc_req = (struct ifreq *)malloc(ifc.ifc_len + sizeof(struct ifreq));   // add for last check node
	if (!ifc.ifc_req)
		goto EXIT0;

	memset(ifc.ifc_req, 0, sizeof(ifc.ifc_len + sizeof(struct ifreq)));

	nRetCode = ioctl(nSocket, SIOCGIFCONF, &ifc);
	if (nRetCode)  // success when nRetCode == 0
		goto EXIT0;

	for (pifreq = ifc.ifc_req; pifreq < (ifc.ifc_req + nSize); ++pifreq)
	{
		if (pifreq[0].ifr_addr.sa_data == pifreq[1].ifr_addr.sa_data)
			continue;  // duplicate, skip it

		//printf( "Interface:  %s\n", ifr->ifr_name );
		nRetCode = ioctl(nSocket, SIOCGIFFLAGS, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;   // failed to get flags, skip it

		nRetCode = ioctl(nSocket, SIOCGIFADDR, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;

		InAddress = *((struct sockaddr_in *)&(pifreq->ifr_addr));
		ulAddress = InAddress.sin_addr.s_addr; 

		if (ulAddress == 0x100007f) // 127.0.0.1    skip
			continue;

		nRetCode = ioctl(nSocket, SIOCGIFHWADDR, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;

		if (
			(!bIn) &&
			((ulAddress & 0x0000FFFF) == uMask)
			)
		{
			bIn = true;
			ulIPAddressInX = ulAddress;
			memcpy(pMacAddressInX, &pifreq->ifr_hwaddr.sa_data, IFHWADDRLEN);
		}

		if (
			(!bOut) &&
			((ulAddress & 0x0000FFFF) != uMask)
			)
		{
			bOut = true;
			ulIPAddressOutX = ulAddress;
			memcpy(pMacAddressOutX, &pifreq->ifr_hwaddr.sa_data, IFHWADDRLEN);
		}
	}


	if (bIn)
	{
		if (pMacAddressIn)
			memcpy(pMacAddressIn, pMacAddressInX, nMacLength);

		if (pIPAddressIn)
			*pIPAddressIn = ulIPAddressInX;

		if (!bOut)
		{
			if (pMacAddressOut)
				memcpy(pMacAddressOut, pMacAddressInX, nMacLength);
			if (pIPAddressOut)
				*pIPAddressOut = ulIPAddressInX;
		}
	}

	if (bOut)
	{
		if (pMacAddressOut)
			memcpy(pMacAddressOut, pMacAddressOutX, nMacLength);
		if (pIPAddressOut)
			*pIPAddressOut = ulIPAddressOutX;

		if (!bIn)
		{
			if (pMacAddressIn)
				memcpy(pMacAddressIn, pMacAddressOutX, nMacLength);
			if (pIPAddressIn)
				*pIPAddressIn = ulIPAddressOutX;
		}
	}


	nResult = true;    
EXIT0:

	if (ifc.ifc_req)
	{
		free(ifc.ifc_req);
		ifc.ifc_req = NULL;
	}

	if (nSocket != -1)
	{
		close(nSocket);
		nSocket = -1;
	}

	if (!nResult)
		nSize = -1;

	return nSize;
#endif
}

//获取内外网IP列表
//返回值为网卡数量
int _gGetIPAddressList(
					KADAPTER_MACADDRESS* pMacAddressIns, DWORD* pdwIPAddressIns, DWORD& dwInIPCount, 
					KADAPTER_MACADDRESS* pMacAddressOuts, DWORD* pdwIPAddressOuts, DWORD& dwOutIPCount, 
					DWORD dwListCount, DWORD dwMask)
{
	int nResult  = false;
	int nRetCode = false;

	int nSocket = -1;
	int nSize   = -1;
	struct ifconf  ifc;
	struct ifreq  *pifreq    = NULL;
	int			mib[6];
	size_t		len = 0;
	char			*buf = NULL;
	char		ifcbuf[720];
	unsigned char		*ptr;
	struct if_msghdr	*ifm;
	struct sockaddr_dl	*sdl;

	struct sockaddr_in InAddress;
	unsigned long ulAddress = 0;

	memset(&ifc, 0, sizeof(ifc));

	nSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (nSocket < 0)
		goto EXIT0;

	ifc.ifc_req = (ifreq*)ifcbuf;
	ifc.ifc_len = sizeof(ifcbuf);
	nRetCode = ioctl(nSocket, SIOCGIFCONF, &ifc);
	if (nRetCode && errno != EINVAL)  // success when nRetCode == 0
		goto EXIT0;

	nSize = ifc.ifc_len / sizeof(struct ifreq);
	if (!nSize)
		goto EXIT0;

	ifc.ifc_req = (struct ifreq *)malloc(ifc.ifc_len + sizeof(struct ifreq));   // add for last check node
	if (!ifc.ifc_req)
		goto EXIT0;

	memset(ifc.ifc_req, 0, sizeof(ifc.ifc_len + sizeof(struct ifreq)));

	nRetCode = ioctl(nSocket, SIOCGIFCONF, &ifc);
	if (nRetCode)  // success when nRetCode == 0
		goto EXIT0;


	if (pMacAddressIns)
	{
		memset(pMacAddressIns, 0, sizeof(KADAPTER_MACADDRESS) * dwListCount);
	}
	if (pMacAddressOuts)
	{
		memset(pMacAddressOuts, 0, sizeof(KADAPTER_MACADDRESS) * dwListCount);
	}

	dwInIPCount = 0;
	dwOutIPCount = 0;
	for (pifreq = ifc.ifc_req; pifreq < (ifc.ifc_req + nSize); ++pifreq)
	{
		if (pifreq[0].ifr_addr.sa_data == pifreq[1].ifr_addr.sa_data)
			continue;  // duplicate, skip it

		//printf( "Interface:  %s\n", ifr->ifr_name );
		nRetCode = ioctl(nSocket, SIOCGIFFLAGS, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;   // failed to get flags, skip it

		nRetCode = ioctl(nSocket, SIOCGIFADDR, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;

		InAddress = *((struct sockaddr_in *)&(pifreq->ifr_addr));
		ulAddress = InAddress.sin_addr.s_addr; 

		if (ulAddress == 0x100007f) // 127.0.0.1    skip
			continue;

#ifdef SIOCGIFHWADDR
		nRetCode = ioctl(nSocket, SIOCGIFHWADDR, pifreq);
		if (nRetCode)   // success when nRetCode == 0
			continue;

		// IN
		if ((ulAddress & 0x0000FFFF) == dwMask && dwInIPCount <= dwListCount)
		{
			if (pMacAddressIns)
			{
				memcpy(pMacAddressIns[dwInIPCount].szMacAddress, &pifreq->ifr_hwaddr.sa_data, IFHWADDRLEN);
			}
			if (pdwIPAddressIns)
				pdwIPAddressIns[dwInIPCount] = ulAddress;

			dwInIPCount ++;
		}

		if ((ulAddress & 0x0000FFFF) != dwMask && dwOutIPCount <= dwListCount)
		{
			if (pMacAddressOuts)
			{
				memcpy(pMacAddressOuts[dwOutIPCount].szMacAddress, &pifreq->ifr_hwaddr.sa_data, IFHWADDRLEN);
			}
			if (pdwIPAddressOuts)
				pdwIPAddressOuts[dwOutIPCount] = ulAddress;

			dwOutIPCount ++;
		}
#else
		mib[0] = CTL_NET;
		mib[1] = PF_ROUTE;
		mib[2] = 0;
		mib[3] = PF_LINK;
		mib[4] = NET_RT_IFLIST;
		mib[5] = if_nametoindex(pifreq->ifr_name);

		if (mib[5] == 0)
			continue;

		nRetCode = sysctl(mib, 6, NULL, &len, NULL, 0);
		if (nRetCode < 0)
			continue;

		if (buf)
			free(buf);

		buf = (char*)malloc(len);

		if (buf == NULL)
			continue;

		nRetCode = sysctl(mib, 6, buf, &len, NULL, 0);
		if (nRetCode < 0)
			continue;

		ifm = (struct if_msghdr *)buf;
		sdl = (struct sockaddr_dl *)(ifm + 1);
		ptr = (unsigned char *)LLADDR(sdl);
		//printf("%02x:%02x:%02x:%02x:%02x:%02x\n", *ptr, *(ptr+1), *(ptr+2),
		//	*(ptr+3), *(ptr+4), *(ptr+5));

		// IN
		if ((ulAddress & 0x0000FFFF) == dwMask && dwInIPCount <= dwListCount)
		{
			if (pMacAddressIns)
			{
				strcpy(pMacAddressIns[dwInIPCount].szMacAddress, ether_ntoa((struct ether_addr *)ptr));
			}
			if (pdwIPAddressIns)
				pdwIPAddressIns[dwInIPCount] = ulAddress;

			dwInIPCount ++;
		}

		if ((ulAddress & 0x0000FFFF) != dwMask && dwOutIPCount <= dwListCount)
		{
			if (pMacAddressOuts)
			{
				strcpy(pMacAddressOuts[dwOutIPCount].szMacAddress, ether_ntoa((struct ether_addr *)ptr));
			}
			if (pdwIPAddressOuts)
				pdwIPAddressOuts[dwOutIPCount] = ulAddress;

			dwOutIPCount ++;
		}	
#endif // SIOCGIFHWADDR
	}

	nResult = true;    
EXIT0:

	if (buf)
	{
		free(buf);
		buf = NULL;
	}

	if (ifc.ifc_req)
	{
		free(ifc.ifc_req);
		ifc.ifc_req = NULL;
	}

	if (nSocket != -1)
	{
		close(nSocket);
		nSocket = -1;
	}

	if (!nResult)
		nSize = -1;

	return nSize;
}
#endif // WIN32

//获取内外网IP列表
//返回值为网卡数量
int gGetIPAddressList(
								  KADAPTER_MACADDRESS* pMacAddressIns, DWORD* pdwIPAddressIns, DWORD& dwInIPCount, 
								  KADAPTER_MACADDRESS* pMacAddressOuts, DWORD* pdwIPAddressOuts, DWORD& dwOutIPCount, 
								  DWORD dwListCount, DWORD dwMask)
{
	int nAdapterCount = _gGetIPAddressList(pMacAddressIns, pdwIPAddressIns, dwInIPCount, 
						pMacAddressOuts, pdwIPAddressOuts, dwOutIPCount, 
						dwListCount, dwMask);

	DWORD dwNewInCount		= dwInIPCount;
	DWORD dwNewOutCount		= dwOutIPCount;
	if (0 == dwInIPCount && dwOutIPCount > 0)
	{
		if (pMacAddressOuts && pMacAddressIns)
			memcpy(pMacAddressIns, pMacAddressOuts, sizeof(KADAPTER_MACADDRESS) * dwOutIPCount);

		if (pMacAddressOuts && pMacAddressIns)
			memcpy(pdwIPAddressIns, pdwIPAddressOuts, sizeof(DWORD) * dwOutIPCount);

		dwNewInCount = dwOutIPCount;
	}
	if (0 == dwOutIPCount && dwInIPCount > 0)
	{
		if (pMacAddressOuts && pMacAddressIns)
			memcpy(pMacAddressOuts, pMacAddressIns, sizeof(KADAPTER_MACADDRESS) * dwInIPCount);

		if (pdwIPAddressOuts && pdwIPAddressIns)
			memcpy(pdwIPAddressOuts, pdwIPAddressIns, sizeof(DWORD) * dwInIPCount);

		dwNewOutCount = dwInIPCount;
	}

	dwInIPCount		= dwNewInCount;
	dwOutIPCount	= dwNewOutCount;

	return nAdapterCount;
}
