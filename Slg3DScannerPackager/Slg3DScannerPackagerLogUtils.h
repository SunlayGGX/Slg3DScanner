#pragma once

#define ENABLE_LOG_PRINT

#ifdef ENABLE_LOG_PRINT
#   define LOG(message) std::cout << message << std::endl
#else
#   define LOG(message)
#endif
