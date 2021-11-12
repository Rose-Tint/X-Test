#ifndef X_TEST_UNITTEST_HPP
#define X_TEST_UNITTEST_HPP

#include <initializer_list>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

#include "Formatter.hpp"


namespace xtst
{
    template < class T >
    using ilist = const std::initializer_list<T>&;


    template < class Fn, Fn > class UnitTest;

    template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
    class UnitTest<Rtn(*)(ArgTs...), Func> final
    {
      public:
        static void Trust(Rtn, ArgTs...);
        static void Doubt(ArgTs...);
        static void RunTests();

        UnitTest() = delete;

      private:
        typedef std::shared_ptr<Rtn> return_ptr_t;
        typedef Formatter<Rtn(*)(ArgTs...), Func> formatter;

        static std::vector<std::pair<return_ptr_t, std::tuple<ArgTs...>>> cases;
    };
}


template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
std::vector<std::pair<std::shared_ptr<Rtn>, std::tuple<ArgTs...>>> xtst::UnitTest<Rtn(*)(ArgTs...), Func>::cases = { };


/**
* registers the given values as a case that should run without errors, and return a value equivilant to the specified value
*
* @param rtn  expected return value
* @param input  tuple of arguments to use as the test case's arguments
*/
template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
void xtst::UnitTest<Rtn(*)(ArgTs...), Func>::Trust(Rtn rtn, ArgTs...input)
{
    std::cout << "begin Trust" << std::endl;
    cases.push_back({ std::make_shared<Rtn>(rtn), { input... } });
    std::cout << "end Trust" << std::endl;
}


/**
* registers the given values as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param input  tuple of arguments to use as the test case's arguments
*/
template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
void xtst::UnitTest<Rtn(*)(ArgTs...), Func>::Doubt(ArgTs... input)
{
    std::cout << "begin Doubt" << std::endl;
    cases.push_back({ nullptr, { input... } });
    std::cout << "end Doubt" << std::endl;
}


/**
* runs registered tests and prints the formatted results to the console.
*/
template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
void xtst::UnitTest<Rtn(*)(ArgTs...), Func>::RunTests()
{
    std::cout << "begin RunTests" << std::endl;
    bool pass;
    std::stringstream stream;
    return_ptr_t rtn_ptr = nullptr;
    return_ptr_t result = nullptr;
    for (const auto& test_case : cases)
    {
        const std::tuple<ArgTs...>& args = test_case.second;
        result = test_case.first;

        try { rtn_ptr = std::make_shared<Rtn>(apply(Func, args)); }
        catch (...) { rtn_ptr = nullptr; }

        if (rtn_ptr == nullptr)
            pass = (result == nullptr);
        else if (result == nullptr)
            pass = false;
        else pass = (*rtn_ptr == *result);

        formatter::Format(stream, pass, result, rtn_ptr, args);
    }
    std::cout << "end RunTests" << std::endl;
}


#endif
