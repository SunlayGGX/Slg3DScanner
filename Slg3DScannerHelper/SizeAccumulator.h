#pragma once


namespace Slg3DScanner
{
    template<class Type, class ... Args>
    struct SizeAccumulator
    {
    public:
        enum
        {
            value = sizeof(Type) + SizeAccumulator<Args...>::value
        };
    };

    template<class Type>
    struct SizeAccumulator<Type>
    {
    public:
        enum
        {
            value = sizeof(Type)
        };
    };
}

