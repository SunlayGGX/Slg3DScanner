#pragma once

namespace Slg3DScanner
{
    class LoggerEngine;

    class InputEngineMediator
    {
    public:
        InputEngineMediator(LoggerEngine* instance, HWND windowHandle);
        ~InputEngineMediator();

        void update();
    };
}
