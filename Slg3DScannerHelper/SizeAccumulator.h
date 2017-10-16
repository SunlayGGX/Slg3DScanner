#pragma once


namespace Slg3DScanner
{
    /*
    Meta object to count the size of all elements.
    */
    template<class FirstType, class ... OtherType>
    struct SizeAccumulator
    {
    public:
        enum
        {
            value = sizeof(FirstType) + SizeAccumulator<OtherType...>::value,

            cache_optimized_value = SizeAccumulator<FirstType>::cache_optimized_value + SizeAccumulator<OtherType...>::cache_optimized_value
        };
    };

    /*
    Version with one element
    */
    template<class LonelyType>
    struct SizeAccumulator<LonelyType>
    {
    public:
        enum
        {
            value = sizeof(LonelyType),

            remainingOnCacheLine = value % SYSTEM_CACHE_ALIGNMENT_SIZE,
            isCacheLineEqual = remainingOnCacheLine == 0,
            toAddToCompleteCacheLine = isCacheLineEqual ? 0 : SYSTEM_CACHE_ALIGNMENT_SIZE - remainingOnCacheLine,

            cache_optimized_value = value + toAddToCompleteCacheLine
        };
    };
}

