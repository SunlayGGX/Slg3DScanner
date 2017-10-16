#pragma once

//windows
#define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#undef WIN32_LEAN_AND_MEAN


//Standard
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <cassert>
#include <cstddef>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>
#include <timeapi.h>


//Lib inclusion
#pragma comment(lib, "Winmm.lib")
