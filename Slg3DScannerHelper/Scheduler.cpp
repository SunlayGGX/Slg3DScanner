#include "Scheduler.h"

using namespace Slg3DScanner;

LoopScheduler::LoopScheduler(LoopScheduler&& scheduler) :
    m_run{ std::move(scheduler.m_run) }
{

}

LoopScheduler::~LoopScheduler()
{
    this->kill();
}

void LoopScheduler::kill()
{
    if(this->isValid())
    {
        *m_run = false;
    }
}

bool LoopScheduler::isKilled() const
{
    if(this->isValid())
    {
        return *m_run;
    }

    return true;
}

bool LoopScheduler::isValid() const
{
    return static_cast<bool>(m_run);
}
