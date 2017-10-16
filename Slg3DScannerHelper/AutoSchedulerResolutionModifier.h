#pragma once


namespace Slg3DScanner
{
    /*
    To Modify the thread scheduler task resolution. Important to wait the right number of frame.

    the default value is the maximum resolution : 1ms
    */
    template<unsigned int newSchedulerResolutionInMillisecond = 1>
    class AutoSchedulerResolutionModifier
    {
    public:
        AutoSchedulerResolutionModifier()
        {
            timeBeginPeriod(newSchedulerResolutionInMillisecond);
        }

        ~AutoSchedulerResolutionModifier()
        {
            timeEndPeriod(newSchedulerResolutionInMillisecond);
        }
    };

    template<>
    class AutoSchedulerResolutionModifier<0>
    {
    public:
        AutoSchedulerResolutionModifier()
        {
            timeBeginPeriod(1);
        }

        ~AutoSchedulerResolutionModifier()
        {
            timeEndPeriod(1);
        }
    };
}

