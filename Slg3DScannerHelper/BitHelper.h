#pragma once

namespace Slg3DScanner
{
    template<unsigned int shiftValue>
    struct BitFill
    {
        enum
        {
            value = (BitFill<shiftValue - 1>::value << 1) | 0x1
        };
    };

    template<>
    struct BitFill<1>
    {
        enum
        {
            value = 0x1
        };
    };

    template<>
    struct BitFill<0>
    {
        enum
        {
            value = 0x0
        };
    };

    template<unsigned int bit>
    struct BitMask
    {
        enum
        {
            value = 0x1 << bit
        };
    };
}
