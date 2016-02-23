
#pragma once

#define SCRIPT_BINDK

#define  NOMINMAX

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#pragma warning(disable:4200) // C99标准 弹性成员数组
#pragma warning(disable:4102) // 'EXIT0' : unreferenced label

#endif // _WIN32

#include "Core.h"
#include "Network/QSocket.h"
#include "Core/QGUID.h"

#include "config/gamedef.h"
#include "Misc/utilities.h"
#include "onlinegamebase/kjxexunify.h"
#include "ProtocolBasic.h"

#ifdef unix
#include <unistd.h>
#endif

#include <algorithm>

