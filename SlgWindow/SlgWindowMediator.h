#pragma once

namespace Slg3DScanner
{
    class LoggerEngine;

    class SlgWindowMediator
    {
    public:
        SlgWindowMediator(LoggerEngine* instance);
        ~SlgWindowMediator();

        void update();

        HWND getWindowHandle() const;
    };
}
