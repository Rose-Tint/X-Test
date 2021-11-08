#include "../include/UnitTest.hpp"

#include <iostream>
#include <sstream>


/**
* registers the given values as a case that should run without errors, and return a value equivilant to the specified value
*
* @param rtn  expected return value
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void xtst::UnitTest<Traits>::Trust( return_type rtn, arg_types input )
{
    cases.emplace_back( std::move(rtn), std::move(input));
}

/**
* registers each of the given values in a list as a case that should run without errors, and return a value equivilant to the specified value
*
* @param inputs  an initializer list of pairs of the expected return value and a tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void xtst::UnitTest<Traits>::Trust( ilist<std::pair<return_type, arg_types>> inputs )
{
    for (std::pair<return_type, arg_types> icase : inputs)
        cases.emplace_back(icase.first, icase.second);
}

/**
* registers the given values as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void xtst::UnitTest<Traits>::Doubt( arg_types input )
{
    // TODO: add checks on the input?
    cases.emplace_back(nullptr, std::move(input));
}

/**
* registers each of the given values in a list as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param inputs  an initializer list of tuples of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void xtst::UnitTest<Traits>::Doubt( ilist<arg_types> input )
{
    for (const arg_types& iargs : input)
        cases.emplace_back(iargs.first, iargs.second);
}

/**
* runs registered tests and prints the formatted results to the console.
*/
template < class Traits >
void xtst::UnitTest<Traits>::RunTests()
{
    bool pass;
    std::stringstream stream;
    std::unique_ptr<return_type> rtn_ptr = nullptr;
    result_type result = nullptr;
    for (auto test_case : cases)
    {
        const arg_types& args = test_case.second;
        result = test_case.first;

        try { rtn_ptr = std::make_unique(apply(Traits::function, args)); }
        catch (const all_errors& err) { rtn_ptr = nullptr; }

        if (rtn_ptr == nullptr)
            pass = (result == nullptr);
        else if (result == nullptr)
            pass = false;
        else pass = (*rtn_ptr == *result);

        formatter.Format(stream, pass, result, std::move(rtn_ptr), args);
    }
}
