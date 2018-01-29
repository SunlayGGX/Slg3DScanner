#pragma once

#include "SlgSingleton.h"

#include "TestSheetPrototype.h"
#include "TestSheet1.h"


namespace Slg3DScanner
{
    class TesterManager : private SlgSingleton<TesterManager>
    {
    private:
        SLGENGINE_GENERATE_CODE_FROM_SlgSingleton(TesterManager);


    private:
        mutable std::mutex m_mutex;
        std::chrono::microseconds m_waitValueBetweenTests;


    public:
        std::chrono::microseconds getWaitingValueBetweenTestQueue() const;
        void setWaitingValueBetweenTestQueue(const std::chrono::microseconds newValue);
        void disableWaitingValueBetweenTestQueue();
        void executeWaitBetweenTest();


    public:
        template<class TestOrder>
        void test()
        {
            this->test(TestOrder{});
        }

        template<class ... Args>
        void startTestsQueue()
        {
            TestQueue<Args...>::start();
        }

    private:
        template<class TestOrder, class ... Args>
        struct TestQueue
        {
        public:
            static void start()
            {
                TestQueue<TestOrder>::start();
                TestQueue<Args...>::start();
            }
        };

        template<class TestOrder>
        struct TestQueue<TestOrder>
        {
        public:
            static void start()
            {
                TesterManager& testMgr = TesterManager::instance();
                testMgr.test(TestOrder{});
                testMgr.executeWaitBetweenTest();
            }
        };


    private:
        void test(SLG_TEST_ORDER_MAKE(DrawCubes));
        void test(SLG_TEST_ORDER_MAKE(LoadPointCloud));
        void test(SLG_TEST_ORDER_MAKE(LoadPointCloudSimple));
        void test(SLG_TEST_ORDER_MAKE(LoadPointCloudTranslatedSimple));
        void test(SLG_TEST_ORDER_MAKE(WriteAllPointCloudToObj));
    };
}
