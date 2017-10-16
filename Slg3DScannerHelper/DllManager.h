#pragma once

#include "SlgSingleton.h"

namespace Slg3DScanner
{
    template<class Singleton> class SlgSingletonAllocatorHelper;

    class DllManager : private Slg3DScanner::SlgSingleton<DllManager>
    {
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(DllManager);


    private:
        std::unique_ptr<class DllManagerPImpl> m_dllMgrPImpl;
        std::mutex m_mutex;


    public:
        virtual void initialize() override;
        virtual void destroy() override;

        //Load an external Dll. External means not a SlgEngine dll. Specify the exact name of what you want to load
        HMODULE loadExternDll(const std::string& dllName);

        //Unload a already loaded external Dll. External means not a SlgEngine dll. Specify the exact name of what you want to load
        void unloadExternDll(const std::string& dllName);

        //Load a SlgEngine Dll. Just Specify the name of the dll without minding the debug and the dll part.
        //For example : if the name is 'blabla.dll' on release and 'blabla_d.dll' on debug, just pass 'blabla' on the method
        HMODULE loadSlgEngineDll(const std::string& dllName);

        //Unload a already loaded SlgEngine Dll. Just Specify the name of the dll without minding the debug and the dll part.
        //For example : if the name is 'blabla.dll' on release and 'blabla_d.dll' on debug, just pass 'blabla' on the method
        void unloadSlgEngineDll(const std::string& dllName);
    };
}

