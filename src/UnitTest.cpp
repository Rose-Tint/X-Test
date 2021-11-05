#include "../include/UnitTest.hpp"
#include "../include/CaseArg.hpp"

#include <iostream>

using namespace xtst;


/**
* sets an argument generator for a given index
*
* @temp-param I  index of the argument generator to set
* @temp-param T  type of the argument and the given index
* @param gen  generator to be put at index I
*/
template < class Traits >
template < std::size_t I, class T >
constexpr void UnitTest<Traits>::SetArgGen( generator_f<T> gen )
{
    
}

/**
* sets argument generators
*
* @param gens  pointers to functions that act as argument generators
*/
template < class Traits >
constexpr void UnitTest<Traits>::SetArgGens( gen_types gens )
{
    arg_gens = TransformTuple_t<EnableIfNotNullptr, class Tuple>
}


/**
* registers the given values as a case that should run without errors, and return a value equivilant to the specified value
*
* @param rtn  expected return value
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void UnitTest<Traits>::Trust( return_type rtn, input_types input )
{
    cases.emplace_back({ std::move(rtn), ExpResult::Return }, std::move(input));
}

/**
* registers each of the given values in a list as a case that should run without errors, and return a value equivilant to the specified value
*
* @param inputs  an initializer list of pairs of the expected return value and a tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void UnitTest<Traits>::Trust( ilist<trusted_t> inputs )
{
    for (trusted_t icase : inputs)
        cases.emplace_back({ icase.first, ExpResult::Return }, icase.second);
}

/**
* registers the given values as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param input  tuple of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void UnitTest<Traits>::Doubt( input_types input )
{
    cases.emplace_back({ nullptr, ExpResult::Error }, std::move(input));
}

/**
* registers each of the given values in a list as a case that should throw one of any of the errors given in the given FunctionTraits class
*
* @param inputs  an initializer list of tuples of arguments or instances of an ArgGenerator to use as the test case's arguments
*/
template < class Traits >
void UnitTest<Traits>::Doubt( ilist<input_types> input )
{
    for (const input_types& iargs : input)
        cases.emplace_back({ iargs.first, ExpResult::Return }, iargs.second);
}

/**
* runs registered tests and prints the formatted results to the console.
*/
template < class Traits >
void UnitTest<Traits>::RunTests( void )
{
    bool pass;
    std::string fstring;
    // pointer to the return value, or nullptr if there was an error
    std::unique_ptr<return_type> rtn_ptr = nullptr;
    // pointer to the expexted return value, or nullptr for error
    std::shared_ptr<return_type> result = nullptr;

    for (auto test_case : cases)
    {
        const arg_types& args = gen_from_input(test_case.second);
        result = test_case.first.first;
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
        // {uses short circuiting to prevent seg-faults}
        // if both the return pointer and expected result pointer can
        // be dereferenced, test their equality
        else pass = !(result == nullptr && (*rtn_ptr != *result));

        fstring = formatter.format(pass, result, rtn_ptr, args);
        std::cout << fstring << std::endl;
    }
}
