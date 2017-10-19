#pragma once

//Standard
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>
#include <cassert>


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
