#include <iostream>
#include <string>
#include "../include/FunctionTraits.hpp"
#include "../include/UnitTest.hpp"


using namespace std::string_literals;



std::string fizzbuzz( int n )
{
    if (n % 15 == 0)
        return "FizzBuzz";
    else if (n % 5 == 0)
        return "Buzz";
    else if (n % 3 == 0)
        return "Fizz";
    return std::to_string(n);
}


using fizzbuzz_traits = xtst::FunctionTraits<decltype(&fizzbuzz), fizzbuzz>;
using test_fizzbuzz = xtst::UnitTest<fizzbuzz_traits>;


int main()
{
    test_fizzbuzz::Trust( "Fizz"s     , {      6 } );
    // test_fizzbuzz::Trust( "Buzz"     , {     10 } );
    // test_fizzbuzz::Trust( "FizzBuzz" , {  10860 } );
    // test_fizzbuzz::Trust( "16"       , {     16 } );
    // test_fizzbuzz::Trust( "349324"   , { 349324 } );
    // test_fizzbuzz::Trust( "FizzBuzz" , {  64470 } );

    // test_fizzbuzz::RunTests();
}
