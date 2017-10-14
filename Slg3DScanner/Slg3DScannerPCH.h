#pragma once


//Windows
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


//Standard
#include <string>
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <cassert>


//My own
#include "Slg3DScannerHelperPCH.h"


//Lib inclusion
#ifdef _DEBUG
#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#else
#   pragma comment(lib, "Slg3DScannerHelper.lib")
#endif
