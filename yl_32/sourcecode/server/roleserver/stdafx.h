
#pragma once

#define NOMINMAX

// Windows下定义以下宏
#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#endif // _WIN32

#include "Core.h"
#include "Network/QSocket.h"

#include "Misc/utilities.h"

#include <list>
#include "roledb_def.h"
// -------------------------------------------------------------------------

IMemBlock* KAlloc2(unsigned uSize);

//#define RS_MEM_ALLOCATOR QCreateMemBlock
#define RS_MEM_ALLOCATOR KAlloc2

