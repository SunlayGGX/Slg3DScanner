#pragma once


//Windows
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


//Standard
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <cassert>


//My own
#include "Slg3DScannerHelperPCH.h"
#include "LoggerPCH.h"
#include "SlgInputEnginePCH.h"
#include "SlgWindowPCH.h"
#include "SlgRenderEnginePCH.h"
#include "SlgRessourcePCH.h"
#include "SlgCoreEnginePCH.h"


//Lib inclusion
#ifdef _DEBUG
#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#   pragma comment(lib, "SlgLogger_d.lib")
#   pragma comment(lib, "SlgInputEngine_d.lib")
#   pragma comment(lib, "SlgWindow_d.lib")
#   pragma comment(lib, "SlgRenderEngine_d.lib")
#   pragma comment(lib, "SlgRessource_d.lib")
#   pragma comment(lib, "SlgCoreEngine_d.lib")
#else
#   pragma comment(lib, "Slg3DScannerHelper.lib")
#   pragma comment(lib, "SlgLogger.lib")
#   pragma comment(lib, "SlgInputEngine.lib")
#   pragma comment(lib, "SlgWindow.lib")
#   pragma comment(lib, "SlgRenderEngine.lib")
#   pragma comment(lib, "SlgRessource.lib")
#   pragma comment(lib, "SlgCoreEngine.lib")
#endif
