#pragma once


//Standard
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>


//My own
#include "ToolHelperPCH.h"


//Lib inclusion
#ifdef _DEBUG
#   pragma comment(lib, "ToolHelper_d.lib")
#else
#   pragma comment(lib, "ToolHelper.lib")
#endif
