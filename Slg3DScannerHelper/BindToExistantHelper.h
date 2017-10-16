#pragma once

#include "NonInstanciable.h"

namespace Slg3DScanner
{
    struct BindToExistantSlgSingletonHelper : private NonInstanciable
    {
    public:
        template<class SlgSingleton>
        static void bind(SlgSingleton* instance)
        {
            if(SlgSingleton::s_instance != instance)
            {
                assert(SlgSingleton::s_instance == nullptr);

                if(instance != nullptr)
                {
                    SlgSingleton::s_instance = instance;
                }
            }
        }
    };
    
}