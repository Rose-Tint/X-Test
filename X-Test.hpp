#ifndef X_TEST_HPP
#define X_TEST_HPP

#include "TestBase.hpp"

#ifndef TEST_CLASS
#define TEST_CLASS(name) class name##Tests final : private xtst::TestBase
#endif

#ifndef ADD_TEST_INST
#define ADD_TEST_INST(cls) cls##Tests _test_##cls = cls##Tests();
#endif

#ifndef DECLARE_TEST
#define DECLARE_TEST(func, exp_t, ...) static bool test_##func(const exp_t &, __VA_OPT__);
#endif

#ifndef DEFINE_TEST
#define DEFINE_TEST(cls, func, exp_t, ...) bool cls##Tests::test_##func_name(const exp_t &, __VA_OPT__)
#endif

#endif