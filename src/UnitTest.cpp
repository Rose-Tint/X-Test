#include "../include/UnitTest.hpp"
#include "../include/expectations.hpp"

#include <iostream>

using namespace xtst;


/**
* registers the given values as a case that should run without errors, and return a value equivilant to the specified value
*
* @param rtn  expected return value
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Tr >
void UnitTest<Tr>::Trust( return_type rtn, input_types input )
{
    cases.emplace_back({ std::move(rtn), ExpResult::Return }, std::move(input));
}

/**
* registers each of the given values in a list as a case that should run without errors, and return a value equivilant to the specified value
*
* @param inputs  an initializer list of pairs of the expected return value and a tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Tr >
void UnitTest<Tr>::Trust( ilist<trusted_t> inputs )
{
    for (trusted_t icase : inputs)
        cases.emplace_back({ icase.first, ExpResult::Return }, icase.second);
}

/**
* registers the given values as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Tr >
void UnitTest<Tr>::Doubt( input_types input )
{
    cases.emplace_back({ nullptr, ExpResult::Error }, std::move(input));
}

/**
* registers each of the given values in a list as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param inputs  an initializer list of tuples of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Tr >
void UnitTest<Tr>::Doubt( ilist<input_types> input )
{
    for (const input_types& iargs : input)
        cases.emplace_back({ iargs.first, ExpResult::Return }, iargs.second);
}

/**
* runs registered tests and prints the formatted results to the console.
*/
template < class Tr >
void UnitTest<Tr>::RunTests( void )
{
    bool pass;
    std::string fstring;
    std::unique_ptr<return_type> rtn_ptr = nullptr;
    std::shared_ptr<return_type> result = nullptr;
    for (auto _case : cases)
    {
        const arg_types& args = gen_from_input(_case.second);
        result = _case.first.first;
        try
        {
            rtn_ptr = std::make_unique(Apply(args));
        }
        catch (const AllErrors& err)
        {
            rtn_ptr = nullptr;
        }
        if (rtn_ptr == nullptr)
            pass = (result == nullptr);
        else pass = !(result == nullptr && (*rtn_ptr != *result));
        fstring = formatter.format(pass, result, *rtn_ptr, args);
        std::cout << fstring << std::endl;
    }
}
