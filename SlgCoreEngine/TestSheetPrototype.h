#pragma once

#define SLG_DECLARE_TEST_ORDER(TestName) namespace Slg3DScanner{ namespace TestSheet{ struct TestName; }}
#define SLG_TEST_ORDER(TestName) Slg3DScanner::TestSheet::TestName
#define SLG_TEST_ORDER_MAKE(TestName) const SLG_TEST_ORDER(TestName)& testOrder

#define SLG_BEGIN_TEST_ORDER_DEFINITION(TestName)   \
namespace Slg3DScanner {                            \
namespace TestSheet {                               \
struct TestName                                     \
{                                                   \
public:                                             \
TestName() = default;                               \
TestName(TestName&) = delete;                       \
~TestName() = default;                              \
TestName& operator=(TestName&) = delete;            \

#define SLG_END_TEST_ORDER_DEFINITION };}}

#define SLG_END_TEST_MESSAGE(TestName) "END TEST "#TestName
#define SLG_PRINT_END_TEST(TestName) SLGENGINE_LOG_DEBUG(SLG_END_TEST_MESSAGE(TestName))
