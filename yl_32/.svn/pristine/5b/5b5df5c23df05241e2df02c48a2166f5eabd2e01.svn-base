
#pragma once

#include "CoreDeclspec.h"

#ifndef KD_MAX_MACADDRESS_LEN
	#define KD_MAX_MACADDRESS_LEN      8
#endif // KD_MAX_MACADDRESS_LEN

//此函数会确保两个地址均有
//返回值为网卡数量
CORE_API int gGetMacAndIPAddress(
    unsigned char  *pMacAddressIn,  unsigned long *pIPAddressIn, 
    unsigned char  *pMacAddressOut, unsigned long *pIPAddressOut, 
    unsigned int uMask = 0x0000a8c0, 
    int nMacLength = 6
);

//获取内外网IP列表
//返回值为网卡数量
struct KADAPTER_MACADDRESS
{
	CHAR szMacAddress[KD_MAX_MACADDRESS_LEN];
};

CORE_API int gGetIPAddressList(
					KADAPTER_MACADDRESS* pMacAddressIns, DWORD* pdwIPAddressIns, DWORD& dwInIPCount, 
					KADAPTER_MACADDRESS* pMacAddressOuts, DWORD* pdwIPAddressOuts, DWORD& dwOutIPCount, 
					DWORD dwListCount, DWORD dwMask = 0x0000a8c0);

