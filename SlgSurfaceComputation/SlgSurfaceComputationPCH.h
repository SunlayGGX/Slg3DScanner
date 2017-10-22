#pragma once

/* Windows */
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


/* DirectX */
#include <DirectXMath.h>


/* Standard */
#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <stdexcept>
#include <cassert>
#include <cstdint>
#include <limits>
#include <type_traits>


//My own
#include "Slg3DScannerHelperPCH.h"
#include "LoggerPCH.h"
#include "SlgRenderBasePCH.h"


//Lib inclusion
#ifdef _DEBUG
#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#   pragma comment(lib, "SlgLogger_d.lib")
#   pragma comment(lib, "SlgRenderBase_d.lib")
#else
#   pragma comment(lib, "Slg3DScannerHelper.lib")
#   pragma comment(lib, "SlgLogger.lib")
#   pragma comment(lib, "SlgRenderBase.lib")
#endif


//undef
#undef min
#undef max