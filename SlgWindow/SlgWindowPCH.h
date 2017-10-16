#pragma once

//Windows
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


//Standard
#include <mutex>
#include <stdexcept>
#include <cassert>


//My own
#include "Slg3DScannerHelperPCH.h"
#include "LoggerPCH.h"


//Lib inclusion
#ifdef _DEBUG
#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#   pragma comment(lib, "SlgLogger_d.lib")
#else
#   pragma comment(lib, "Slg3DScannerHelper.lib")
#   pragma comment(lib, "SlgLogger.lib")
#endif
