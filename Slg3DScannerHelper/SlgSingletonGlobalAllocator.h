#pragma once

#include "UtilsMacros.h"
#include "SizeAccumulator.h"

#define SLG_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT


#ifdef SLG_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
#   define SLG_ALIGN_SINGLETON_GLOBAL_ALLOCATOR alignas(SYSTEM_CACHE_ALIGNMENT_SIZE)
#   define SLG_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeAccumulator<Singletons>::cache_optimized_value
#else
#   define SLG_ALIGN_SINGLETON_GLOBAL_ALLOCATOR 
#   define SLG_RETRIEVE_ALLOCATED_SIZE_OF(Singletons) SizeAccumulator<Singletons>::value
#endif

namespace Slg3DScanner
{
    template<class SingletonType>
    class SlgSingletonAllocatorHelper
    {
        static_assert(
            sizeof(SingletonType) != 0,
            SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The singleton object must be correctly declared (no prototype). Have you forgotten an include somewhere?")
            );


    public:
        static void allocate(char* &where)
        {
            assert(SingletonType::s_instance == nullptr);
            SingletonType::s_instance = new(static_cast<void*>(where)) SingletonType;
            where += SLG_RETRIEVE_ALLOCATED_SIZE_OF(SingletonType);
        }

        static void deallocate()
        {
            SingletonType::s_instance->~SingletonType();
            SingletonType::s_instance = nullptr;
        }
    };


    template<class ... Args>
    class SLG_ALIGN_SINGLETON_GLOBAL_ALLOCATOR SlgSingletonGlobalAllocator
    {
    public:
        enum
        {
            MEMORY_SIZE_IN_BYTES = SLG_RETRIEVE_ALLOCATED_SIZE_OF(Args...)
        };


    public:
        static_assert(
            sizeof...(Args) > 0, 
            SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The argument count must be superior or equal to 1")
            );

        static_assert(
            Slg3DScanner::SlgSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES > 0,
            SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("The memory size allocated must be superior to 0")
            );

#ifdef SLG_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT
        static_assert(
            ((Slg3DScanner::SlgSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES % SYSTEM_CACHE_ALIGNMENT_SIZE) == 0) &&
            (SizeAccumulator<Args...>::value <= SlgSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES),
            SLG_NORMALIZE_STATIC_ASSERT_MESSAGE_OUTSIDE_FUNCTION("Error in cach optimized algorithm. Comment the macro SLG_ALLOCATE_SINGLETON_ON_CACHE_ALIGNMENT and give a feed back at sunlay.g@free.fr. Tell me that you found a case where it doesn't work. Thanks a lot ^^")
            );
#endif


    private:
        char m_bufferMemory[Slg3DScanner::SlgSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES];


    private:
        template<class SlgSingleton, class ... RemainingSingletons>
        class SlgSingletonGlobalAllocatorImpl
        {
        public:
            static void allocate(char* &where)
            {
                SlgSingletonGlobalAllocatorImpl<SlgSingleton>::allocate(where);
                SlgSingletonGlobalAllocatorImpl<RemainingSingletons...>::allocate(where);
            }

            static void deallocate()
            {
                SlgSingletonGlobalAllocatorImpl<RemainingSingletons...>::deallocate();
                SlgSingletonGlobalAllocatorImpl<SlgSingleton>::deallocate();
            }
        };

        template<class SlgSingleton>
        class SlgSingletonGlobalAllocatorImpl<SlgSingleton>
        {
        public:
            static void allocate(char* &where)
            {
                SlgSingletonAllocatorHelper<SlgSingleton>::allocate(where);
            }

            static void deallocate()
            {
                SlgSingletonAllocatorHelper<SlgSingleton>::deallocate();
            }
        };


    public:
        SlgSingletonGlobalAllocator()
        {
            char* begin = m_bufferMemory;
            SlgSingletonGlobalAllocatorImpl<Args...>::allocate(begin);

            assert(begin == (m_bufferMemory + Slg3DScanner::SlgSingletonGlobalAllocator<Args...>::MEMORY_SIZE_IN_BYTES));
        }

        ~SlgSingletonGlobalAllocator()
        {
            SlgSingletonGlobalAllocatorImpl<Args...>::deallocate();
        }
    };
}

