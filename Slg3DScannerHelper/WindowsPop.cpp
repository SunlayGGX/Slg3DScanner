#include "Slg3DScannerHelperPCH.h"
#include "WindowsPop.h"

using Slg3DScanner::WindowsPop;

void WindowsPop::popErrorWindow(const char* errorMessage)
{
    ::MessageBoxA(
        nullptr,
        static_cast<LPCSTR>(errorMessage),
        static_cast<LPCSTR>("Error"),
        MB_ICONERROR
    );
}

