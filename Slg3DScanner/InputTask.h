#pragma once

#include "ITask.h"

namespace Slg3DScanner
{
    class InputTask : public ITask
    {
    public:
        InputTask() = default;
        ~InputTask() = default;


    public:
        void initialize() override;
        void destroy() override;
        void update() override;
    };
}
