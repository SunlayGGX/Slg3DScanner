#pragma once

#pragma warning(disable:4624)

namespace Slg3DScanner
{
    struct NonInstanciable
    {
    private:
        template<class ... Args>
        NonInstanciable(Args& ...) = delete;
        ~NonInstanciable() = delete;
    };
}