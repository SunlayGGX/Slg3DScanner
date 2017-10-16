#include "DllManager.h"

#include <unordered_map>

#define DLLMacro extern "C" __declspec(dllimport)

#define ERROR_MESSAGE_WHEN_DLL_OPERATION_FAIL(operation, dllName) \
(\
    "Error when "#operation" Dll, Error number : " + std::to_string(GetLastError()) \
    + ", Dll Name was : " + dllName \
).c_str()



namespace Slg3DScanner
{
    class DLL
    {
    private:
        using ImportedFunc = void(*)();


    public:
        HMODULE m_dllModule;


    public:
        DLL() :
            m_dllModule{ nullptr }
        {}

        DLL(DLL&& other) :
            m_dllModule{ other.m_dllModule }
        {
            m_dllModule = nullptr;
        }

        DLL(const std::string& dllName)
        {
            m_dllModule = LoadLibraryA(dllName.c_str());
            
            if(m_dllModule != nullptr)
            {
                ImportedFunc install = reinterpret_cast<ImportedFunc>(GetProcAddress(m_dllModule, "install"));

                if(install != nullptr)
                {
                    install();
                }
                else
                {
                    throw std::exception{ ERROR_MESSAGE_WHEN_DLL_OPERATION_FAIL(installing, dllName) };
                }
            }
            else
            {
                throw std::exception{ ERROR_MESSAGE_WHEN_DLL_OPERATION_FAIL(loading, dllName) };
            }
        }

        DLL(const DLL& other) = delete;
        DLL& operator=(const DLL& other) = delete;

        ~DLL()
        {
            if(m_dllModule != nullptr)
            {
                ImportedFunc uninstall = reinterpret_cast<ImportedFunc>(GetProcAddress(m_dllModule, "uninstall"));
                uninstall();

                FreeLibrary(m_dllModule);
            }
        }

        void swap(DLL& other) noexcept
        {
            std::swap(this->m_dllModule, other.m_dllModule);
        }

        DLL& operator=(DLL&& other)
        {
            this->swap(other);
            return *this;
        }
    };

    class ExternDLL
    {
    public:
        HMODULE m_dllModule;


    public:
        ExternDLL() :
            m_dllModule{ nullptr }
        {
        }

        ExternDLL(ExternDLL&& other) :
            m_dllModule{ other.m_dllModule }
        {
            m_dllModule = nullptr;
        }

        ExternDLL(const std::string& dllName)
        {
            m_dllModule = LoadLibraryA(dllName.c_str());

            if(m_dllModule == nullptr)
            {
                throw std::exception{ ERROR_MESSAGE_WHEN_DLL_OPERATION_FAIL(loading, dllName) };
            }
        }

        ExternDLL(const ExternDLL& other) = delete;
        ExternDLL& operator=(const ExternDLL& other) = delete;

        ~ExternDLL()
        {
            if(m_dllModule != nullptr)
            {
                FreeLibrary(m_dllModule);
            }
        }

        void swap(ExternDLL& other) noexcept
        {
            std::swap(this->m_dllModule, other.m_dllModule);
        }

        ExternDLL& operator=(ExternDLL&& other)
        {
            this->swap(other);
            return *this;
        }
    };

    class DllManagerPImpl
    {
    private:
        std::unordered_map<std::string, DLL> m_dllMap;
        std::unordered_map<std::string, ExternDLL> m_externDllMap;


    public:
        HMODULE loadDll(const std::string& libraryToLoad)
        {
            try
            {
                DLL dll{ libraryToLoad };

                HMODULE dllModule = dll.m_dllModule;

                m_dllMap[libraryToLoad] = std::move(dll);

                return dllModule;
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void unloadDll(const std::string& libraryToLoad)
        {
            m_dllMap.erase(libraryToLoad);
        }

        HMODULE loadExternDll(const std::string& libraryToLoad)
        {
            try
            {
                ExternDLL dll{ libraryToLoad };

                HMODULE dllModule = dll.m_dllModule;

                m_externDllMap[libraryToLoad] = std::move(dll);

                return dllModule;
            }
            catch(const std::exception&)
            {
                throw;
            }
        }

        void unloadExternDll(const std::string& libraryToLoad)
        {
            m_externDllMap.erase(libraryToLoad);
        }

        ~DllManagerPImpl()
        {
            m_dllMap.clear();
            m_externDllMap.clear();
        }
    };
}


using namespace Slg3DScanner;


DllManager::DllManager() :
    m_dllMgrPImpl{ std::make_unique<DllManagerPImpl>() }
{
}

DllManager::~DllManager() = default;

void DllManager::initialize()
{

}

void DllManager::destroy()
{

}

HMODULE DllManager::loadExternDll(const std::string& dllName)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return m_dllMgrPImpl->loadExternDll(dllName);
}

void DllManager::unloadExternDll(const std::string& dllName)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    m_dllMgrPImpl->unloadExternDll(dllName);
}

HMODULE DllManager::loadSlgEngineDll(const std::string& dllName)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
    return 
#ifdef _DEBUG
        m_dllMgrPImpl->loadDll(dllName + "_d.dll");
#else
        m_dllMgrPImpl->loadDll(dllName + ".dll");
#endif
}

void DllManager::unloadSlgEngineDll(const std::string& dllName)
{
    std::lock_guard<std::mutex> autoLocker{ m_mutex };
#ifdef _DEBUG
    m_dllMgrPImpl->unloadDll(dllName + "_d.dll");
#else
    m_dllMgrPImpl->unloadDll(dllName + ".dll");
#endif
}
