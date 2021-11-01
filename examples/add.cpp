///// add.hpp /////
#ifndef ADD_HPP
#define ADD_HPP

int add( int a, int b )
{
    if (a < b) // intentional mistake
        return 0;
    else return (a + b);
}

#endif

///// test.cpp ///// 

#include "../X-Test.hpp"

using add_traits = FunctionTraits<decltype(&add), add>;
using test_add = UnitTest<add_traits>;

test_add::Trust( 4, 3, 1 ); // add tests individually
test_add::Trust({
    {   7, {  4,   3} },
    {   4, {  6, -10} },
    { -18, { -7, -11} },
    {   6, {-23,  29} }, // will fail, but is not supposed to
}); // or using initializer lists
// no Doubts because add is nothrow

int main()
{
    test_add::RunTests();
    return 0;
}
