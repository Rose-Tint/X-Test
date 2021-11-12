#include <iostream>
#include <string>
#include "../X-Test.hpp"


int add(int a, int b) { return (a + b); }


typedef xtst::UnitTest<decltype(&add), &add> test_add;


int main()
{
    std::cout << "in main\n" << std::flush << std::endl;
    test_add::Trust( 4, 3, 1 );
    test_add::RunTests();
}
