
#include "CorePrivate.h"

#include <time.h>
#include <string.h>

#include "QLock.h"

#include "QTime.h"
#include "QGUID.h"
#include "IpMacAddress.h"

static QLock gs_Lock;
static int gs_nCreateGUIDInitFlag = false;
static unsigned long gs_ulBaseCount = 0;

static unsigned char gs_MacAddress[6] = { 0 };


static unsigned gs_holdrand = 0;

static inline unsigned _Rand()
{
    gs_holdrand = gs_holdrand * 214013L + 2531011L;

    return gs_holdrand;
}


static inline int _InitMacAddress()
{
    return gGetMacAndIPAddress(NULL, NULL, gs_MacAddress, NULL);
}

int QGenerateGUID(GUID *pGuid)
{
    int nResult = false;
    unsigned uValue = 0;
    
    PROCESS_ERROR(pGuid);
    
    if (!gs_nCreateGUIDInitFlag)
    {
        gs_Lock.Lock();
        if (!gs_nCreateGUIDInitFlag)
        {
            _InitMacAddress();
            gs_holdrand = time(NULL);
            
            gs_nCreateGUIDInitFlag = true;
        }
        gs_Lock.Unlock();
    }
    
    memset(pGuid, 0, sizeof(*pGuid));
    
    memcpy(pGuid->Data4, gs_MacAddress, sizeof(gs_MacAddress));
    
    gs_Lock.Lock();
    
    pGuid->Data1 = ++gs_ulBaseCount;
    uValue = _Rand();
    
    gs_Lock.Unlock();
    
    pGuid->Data2 = (unsigned short)(uValue & 0xffff);
    pGuid->Data3 = (unsigned short)((uValue >> 16) & 0xffff);
    
    nResult = true;    
EXIT0:
    return nResult;
}
