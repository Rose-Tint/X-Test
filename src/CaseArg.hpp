#include "../include/CaseArg.hpp"


using namespace xtst;

/**
* generates an argument for a test case based off of whether the value held is and ArgGenerator instance or a value_type instance
*/
template < std::size_t I, class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
Rtn CaseArg<I, Rtn(*)(ArgTypes...), Func>::generate( void ) const &
{
    return Rtn();
}
