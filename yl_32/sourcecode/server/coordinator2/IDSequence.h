
#pragma once

#include "Misc/MSUnknown.h"
#include "mysql.h"

struct IKIDSequence : public IUnknown 
{
    virtual BOOL GenerateID(const char cszSequenceName[], int nPrefetch, UINT64* puRetID) = 0;
};

IKIDSequence* CreateIDSequence(
    const char cszDBAddr[], const char cszDBName[], const char cszUserName[], const char cszPassword[]
);


