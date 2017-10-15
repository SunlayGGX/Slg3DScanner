#pragma once

namespace Slg3DScanner
{
    template<class Singleton> class SlgSingletonAllocatorHelper;

    class DllManager
    {
    private:
        friend class SlgSingletonAllocatorHelper<DllManager>;


    private:
        static DllManager* s_instance;


    private:
        std::unique_ptr<class DllManagerPImpl> m_dllMgrPImpl;
        std::mutex m_mutex;


    private:
        DllManager();
        ~DllManager();


    public:
        static DllManager& instance();

        static void bindToExistant(DllManager* instance);


    public:
        void initialize();
        void destroy();

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

