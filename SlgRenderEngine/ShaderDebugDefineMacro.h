#pragma once


//#define DO_PRINT_SHADER_COMPILE_DEBUG_DETAIL_INSTEAD_OF_THROWING


#if defined(_DEBUG) && defined(DO_PRINT_SHADER_COMPILE_DEBUG_DETAIL_INSTEAD_OF_THROWING)
#   define SHADER_COMPILE_DEBUG_DETAIL
#   include "LoggerCommand.h"
#endif

