#pragma once

#include "Uncopyable.h"

namespace Slg3DScanner
{
    class ITask : private Slg3DScanner::Uncopyable
    {
    protected:
        ITask() = default;

    public:
        virtual ~ITask() = default;

        virtual void initialize() = 0;
        virtual void destroy() = 0;
        virtual void update() = 0;
    };
}
