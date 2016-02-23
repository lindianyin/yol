
#pragma once

#define NOMINMAX
#define SCRIPT_BINDK

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0502
#endif

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
#include "Misc/utilities.h"

#define KDF_GC_CONFIG	"gamecenter_cfg.ini"

// -------------------------------------------------------------------------
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "lua5d.lib")

