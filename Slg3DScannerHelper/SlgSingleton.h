#pragma once

#include "UtilsMacros.h"
#include "Uncopyable.h"


namespace Slg3DScanner
{
    DECLARE_SLG_SINGLETON_ALLOCATOR_HELPER_FOR(Type);

    template<class Type>
    class SlgSingleton : private Slg3DScanner::Uncopyable
    {
    private:
        friend Type;
        friend SlgSingletonAllocatorHelper<Type>;


    private:
        static Type* s_instance;


    private:
        SlgSingleton() = default;
        virtual ~SlgSingleton() = default;


    public:
        static Type& instance()
        {
            return *SlgSingleton<Type>::s_instance;
        }

        virtual void initialize() = 0;
        virtual void destroy() = 0;
    };


    template<class Type> Type* SlgSingleton<Type>::s_instance = nullptr;
}


#define SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(Type)     \
private:                                                    \
    friend SlgSingleton<Type>;                              \
    friend SlgSingletonAllocatorHelper<Type>;               \
    friend struct BindToExistantSlgSingletonHelper;         \
public:                                                     \
    using SlgSingleton<Type>::instance;                     \
private:                                                    \
    Type();                                                 \
    ~Type();                                                \
public:                                                     \
    void initialize() override;                             \
    void destroy() override