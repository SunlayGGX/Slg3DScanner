#pragma once

namespace Slg3DScanner
{
    struct KeyboardElement
    {
    public:
        std::int64_t m_array[4] = {};


    public:
        constexpr KeyboardElement() = default;

        bool getKey(const char key) const;
    };
}
