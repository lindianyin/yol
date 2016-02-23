
#pragma once

#include <deque>
#include "Core/QLock.h"

struct IMemBlock;

////////////////////////////////////////////////////////////////////////////////

struct KG_PACKAGE
{
    unsigned uConnectionID;
    IMemBlock *piDataBuffer;
};

typedef std::deque<KG_PACKAGE> KG_PACKAGE_QUEUE;

struct KG_PACKAGE_LOCK_QUEUE 
{
    KG_PACKAGE_QUEUE  Queue;
    QLock            Lock;
};

const unsigned MAX_REQUEST_QUEUE_SIZE = 100000;

////////////////////////////////////////////////////////////////////////////////


