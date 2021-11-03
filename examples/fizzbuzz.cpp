///// fizzbuzz.hpp /////
#ifndef FIZZBUZZ_HPP
#define FIZZBUZZ_HPP

#include <string>
#include <iostream>
#include <string.h>


std::string fizzbuzz_logic( long long n )
{
    std::string result;
    if (n % 15 == 0)
        result = "FizzBuzz";
    else if (n % 5 == 0)
        result = "Buzz";
    else if (n % 3 == 0)
        result = "Fizz";
    else result = std::to_string(n);
    return result;
}

constexpr const char* ce_fizzbuzz_logic( long long n )
{
    return
      (n % 15 == 0)
        ? "FizzBuzz"
        : (n % 5 == 0)
          ? "Buzz"
          : (n % 3 == 0)
            ? "Fizz"
            : "N"; // TODO: figure out how to make this right
}

void FizzBuzz( long long start = 1, long long max = 100 )
{
    for (long long n = start; n < max; n++)
        std::cout << fizzbuzz_logic(n) << std::endl;
}

constexpr void ce_FizzBuzz( long long start = 1, long long max = 100 )
{
    for (long long n = start; n < max; n++)
        std::cout << ce_fizzbuzz_logic(n) << std::endl;
}

#endif

///// test.hpp ////
#ifndef TEST_HPP
#define TEST_HPP

#include "../X-Test.hpp"

using namespace xtst;

using test_fizzbuzz_logic = UnitTest<FunctionTraits<decltype(&fizzbuzz_logic), fizzbuzz_logic>>;
using test_ce_fizzbuzz_logic = UnitTest<FunctionTraits<decltype(&ce_fizzbuzz_logic), ce_fizzbuzz_logic>>;


test_fizzbuzz_logic::Trust({
    {"Fizz"    ,{      6 }},
    {"Buzz"    ,{     10 }},
    {"FizzBuzz",{  10860 }},
    {"16"      ,{     16 }},
    {"349324"  ,{ 349324 }},
    {"FizzBuzz",{  64470 }},
});

test_ce_fizzbuzz_logic::Trust({
    {"Fizz"    ,{      6 }},
    {"Buzz"    ,{     10 }},
    {"FizzBuzz",{  10860 }},
    {"N"       ,{     16 }},
    {"N"       ,{ 349324 }},
    {"FizzBuzz",{  64470 }},
});

#endif
