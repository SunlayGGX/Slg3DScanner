#pragma once

//windows
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


//Standard
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstddef>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <timeapi.h>


//My own
#include "UtilsMacros.h" //I cheat a little. Those macros must be known everywhere on the engine


//Lib inclusion
#pragma comment(lib, "Winmm.lib")
