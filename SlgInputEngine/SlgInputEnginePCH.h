#pragma once




/*Standard*/
#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <atomic>
#include <cassert>


/*Direct X*/
#include <D3D11.h>
#include <Xinput.h>
#include <dinput.h>


//My own
#include "Slg3DScannerHelperPCH.h"
#include "LoggerPCH.h"


//lib inclusion
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "XInput.lib")

#ifdef _DEBUG
#   pragma comment(lib, "d3dx11d.lib")
#else
#   pragma comment(lib, "d3dx11.lib")
#endif


/*Slg3DScanner*/
#ifdef _DEBUG
#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#   pragma comment(lib, "SlgLogger_d.lib")
#else
#   pragma comment(lib, "Slg3DScannerHelper.lib")
#   pragma comment(lib, "SlgLogger.lib")
#endif

