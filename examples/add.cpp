#include <iostream>
#include <string>
#include "../include/UnitTest.hpp"


int add(int a, int b) { return (a + b); }


typedef xtst::UnitTest<decltype(&add), add> test_add;


int main()
{
    test_add::Trust( 4, 3, 1 );
    test_add::RunTests();
}
