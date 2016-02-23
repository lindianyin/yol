
#ifndef __STDAFX_H__
#define __STDAFX_H__

// -------------------------------------------------------------------------
#define SCRIPT_BINDK
#define  NOMINMAX

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#pragma warning(disable:4200) // C99 弹性成员数组
#pragma warning(disable:4102) // 'EXIT0' : unreferenced label

#endif // _WIN32

#include "Core.h"
#include "Core/QTime.h"
#include "Core/QMemory.h"
#include "Network/QSocket.h"

#include "config/gamedef.h"
#include "Misc/utilities.h"
#include "onlinegamemodule/kmoduledef.h"
#include "ProtocolBasic.h"

#ifdef WIN32
#include <conio.h>
#endif // WIN32

#include <vector>
#include <map>
#include <algorithm>
#pragma warning(disable:4200)
#pragma warning(disable : 4355)
// -------------------------------------------------------------------------

#endif /* __STDAFX_H__ */
