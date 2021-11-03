#include "../include/TypeTraits.hpp"

using namespace xtst;


template < class To, class...Froms >
constexpr bool xtst::are_convertible( void )
{
    return AndTraits<std::is_convertible<Froms, To>...>::value;
}

/**
* checks whether a parameter pack contains a given type
*
* @temp-param T  type to check for
* @temp-param ...Types  parameter pack to check
* @return  if T is found in parameter pack
*/
template < class T, class...Types >
constexpr bool xtst::in_param_pack( void )
{
    return OrTraits<AreSame<T, Types>...>::value;
}

/**
* checks whether a tuple contains a given type
*
* @temp-param T  type to check for
* @temp-param Tuple  tuple to check
* @return  if T is a type in Tuple
*/
template < class T, class Tuple >
constexpr bool xtst::in_tuple( void )
{
    return dtl::in_tuple_helper<T, Tuple>::value;
}
