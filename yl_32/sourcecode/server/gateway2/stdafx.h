
#pragma once

#define NOMINMAX
#define SCRIPT_BINDK

#ifdef _WIN32

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

#pragma warning(disable:4200) // C99 弹性成员数组
#pragma warning(disable:4102) // 'EXIT0' : unreferenced label

#endif // _WIN32

#include "Core.h"
#include "Core/QThread.h"
#include "Network/QSocket.h"
#include "Core/QTime.h"

#include <stdio.h>
#include <map>
#include <list>
#include <string>
#include <string.h>

#include "ProtocolBasic.h"

