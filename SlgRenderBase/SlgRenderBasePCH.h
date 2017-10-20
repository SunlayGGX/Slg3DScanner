#pragma once



/* Windows */
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


/* DirectX */
#include <DirectXMath.h>
#include <D3D11.h>


/* Standard */
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include <cassert>
#include <cstdint>


//My own
#include "Slg3DScannerHelperPCH.h"
#include "LoggerPCH.h"


//Lib inclusion
#pragma comment(lib, "d3d11.lib")

#ifdef _DEBUG
#   pragma comment(lib, "d3dx11d.lib")

#   pragma comment(lib, "Slg3DScannerHelper_d.lib")
#   pragma comment(lib, "SlgLogger_d.lib")
#else
#   pragma comment(lib, "d3dx11.lib")

#   pragma comment(lib, "Slg3DScannerHelper.lib")
#   pragma comment(lib, "SlgLogger.lib")
#endif
