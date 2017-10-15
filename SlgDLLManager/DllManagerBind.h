#pragma once

namespace Slg3DScanner
{
    class DllManager;
    class DllManagerBind
    {
    private:
        DllManagerBind() = delete;
        DllManagerBind(const DllManagerBind&) = delete;
        ~DllManagerBind() = delete;
        DllManagerBind& operator=(const DllManagerBind&) = delete;


    public:
        static void bindToExistant(DllManager* instance);
    };
}
