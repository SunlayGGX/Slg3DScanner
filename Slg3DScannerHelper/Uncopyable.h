#pragma once

namespace Slg3DScanner
{
    struct Uncopyable
    {
    public:
        Uncopyable() = default;


    private:
        Uncopyable(const Uncopyable&) = delete;
        Uncopyable& operator=(const Uncopyable&) = delete;
    };
}