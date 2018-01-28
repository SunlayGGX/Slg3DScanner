#pragma once

#include "ITask.h"

namespace Slg3DScanner
{
    class InputTask : public ITask
    {
    private:
        float m_translationMove;
        float m_rotationMove;


    public:
        InputTask();
        ~InputTask() = default;


    public:
        void initialize() override;
        void destroy() override;
        void update() override;
    };
}
