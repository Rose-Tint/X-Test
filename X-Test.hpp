#ifndef X_TEST_HPP
#define X_TEST_HPP

#include "include/macrodef.hpp"
#include "include/ansi.hpp"
#include "include/expectations.hpp"
#include "include/Formatter.hpp"
#include "include/FunctionTraits.hpp"
#include "include/ArgGenerator.hpp"
#include "include/TypeTraits.hpp"
#include "include/UnitTest.hpp"

#ifndef AUTO_TEST
#  define AUTO_TEST(Return, name, Args...)\
    Return name(Args);\
    using name##_traits = xtst::TestTraits<Return(*)(Args), name>;\
    using name##_test = xtst::UnitTest< name##_traits >;\
    Return name(Args)
#endif


#ifndef ADD_TEST
# define ADD_TEST(test)\
    using test##_traits = xtst::TestTraits<decltype(&test, name)>;\
    using test##_test = xtst::UnitTest<  >

#endif