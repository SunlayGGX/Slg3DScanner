#include "WindowManager.h"

//#include "GlobalEngine.h"
//#include "RenderEngine.h"

#include "resource.h"

using namespace Slg3DScanner;


namespace Slg3DScanner
{
    LRESULT CALLBACK wndProcCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT ps;
        HDC hdc;

        switch(message)
        {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            WindowManager::instance().callQuitCallback();
            break;

        case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Analyse menu selection
            switch(wmId)
            {
            case IDCLOSE:
                WindowManager::instance().callQuitCallback();
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
}


WindowManager::WindowManager() :
    m_quitCallback{ []() {} },
    m_finishInitializeCallback{ [](HWND) {} }
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::initialize()
{
    TCHAR szTitle[MAX_TITLE_COUNT];

    HINSTANCE dllInstance = GetModuleHandle(nullptr);

    LoadString(dllInstance, IDS_APP_TITLE, szTitle, MAX_TITLE_COUNT);
    LoadString(dllInstance, IDR_SLG3DSCANNER, m_windowClass, MAX_TITLE_COUNT);

    {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &Slg3DScanner::wndProcCallback;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = dllInstance;
        wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SLG3DSCANNER));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCE(IDR_SLG3DSCANNER);
        wcex.lpszClassName = m_windowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        RegisterClassEx(&wcex);
    }

    m_windowVisuHandle = CreateWindow(
        m_windowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        0,
        CW_USEDEFAULT,
        0,
        nullptr,
        nullptr,
        dllInstance,
        nullptr
    );

    if(m_windowVisuHandle != nullptr)
    {
        m_accelerationTable = LoadAccelerators(dllInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

        this->callFinishInitializeCallback();

        ShowCursor(true);
        ShowWindow(m_windowVisuHandle, SW_SHOWNORMAL);
        UpdateWindow(m_windowVisuHandle);
    }
    else
    {
        throw std::exception{ "Window not created correctly!" };
    }
}

void WindowManager::destroy()
{
    this->unbindCallback();
    DestroyWindow(m_windowVisuHandle);
    PostQuitMessage(0);
}

void WindowManager::update()
{
    MSG msg;

    if(::PeekMessage(&msg, m_windowVisuHandle, 0, 0, PM_REMOVE))
    {
        // Closing message ?
        if(msg.message == WM_QUIT)
        {
            this->destroy();
        }

        // Distribute message
        if(!::TranslateAccelerator(msg.hwnd, m_accelerationTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
}

HWND WindowManager::getWindowHandle() const
{
    return m_windowVisuHandle;
}

const TCHAR* WindowManager::getWindowTitleName() const
{
    return m_windowClass;
}

HACCEL WindowManager::getWindowAccelerationTable() const
{
    return m_accelerationTable;
}

void WindowManager::bindQuitCallback(QuitDelegate callback)
{
    std::lock_guard<std::mutex> autoLocker{ m_callbackMutex };
    m_quitCallback = callback;
}

void WindowManager::bindFinishInitializeCallback(FinishInitializeDelegate callback, bool callNow)
{
    std::lock_guard<std::mutex> autoLocker{ m_callbackMutex };
    m_finishInitializeCallback = callback;

    if(callNow && m_windowVisuHandle != nullptr && m_finishInitializeCallback != nullptr)
    {
        m_finishInitializeCallback(m_windowVisuHandle);
    }
}

void WindowManager::callQuitCallback()
{
    std::lock_guard<std::mutex> autoLocker{ m_callbackMutex };
    m_quitCallback();
}

void WindowManager::callFinishInitializeCallback()
{
    std::lock_guard<std::mutex> autoLocker{ m_callbackMutex };
    m_finishInitializeCallback(m_windowVisuHandle);
}

void WindowManager::unbindCallback()
{
    std::lock_guard<std::mutex> autoLocker{ m_callbackMutex };
    m_quitCallback = []() {};
    m_finishInitializeCallback = [](HWND) {};
}
