#include <iostream>
#include <string>
#include "../include/FunctionTraits.hpp"
#include "../include/UnitTest.hpp"


using namespace xtst;


int add(int a, int b)
{
    return (a + b);
}


typedef FunctionTraits<int(*)(int, int), add> add_traits;
typedef UnitTest<add_traits> test_add;


int main()
{
    test_add::Trust( 4, { 3, 1 } );
    return 0;
}
