#ifndef X_TEST_HPP
#define X_TEST_HPP

#include "inc/macrodef.hpp"
#include "inc/ansi.hpp"
#include "inc/expectations.hpp"
#include "inc/Formatter.hpp"
#include "inc/FunctionTraits.hpp"
#include "inc/ArgGenerator.hpp"
#include "inc/TypeTraits.hpp"
#include "inc/UnitTest.hpp"

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