#include "../inc/FunctionTraits.hpp"

using namespace xtst;


template < class Traits > constexpr bool assert_traits( void )
{
    using return_type = typename Traits::return_type;
    using arg_types = typename Traits::arg_types;
    using signature = typename Traits::signature;
    using size_type = typename Traits::size_type;
    return std::is_same< decltype(Traits::get_result) ,   const return_type&(void) >::value
        && std::is_same< decltype(Traits::get_args)   ,     const arg_types&(void) >::value
        && std::is_same< decltype(Traits::exhausted)  ,                 bool(void) >::value
        && std::is_same< decltype(Traits::argc)       ,                  size_type >::value
        && std::is_same< decltype(Traits::pass_clr)   ,           const AnsiOption >::value
        && std::is_same< decltype(Traits::fail_clr)   ,           const AnsiOption >::value
        && std::is_same< decltype(Traits::function)   ,            const signature >::value;
}
