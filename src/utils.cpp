#include "../include/utils.hpp"
#include "../include/TypeTraits.hpp"
#include "../include/macrodef.hpp"
#include <typeinfo>
#include <cxxabi.h>
#include <utility>
#include <cstring>

using namespace xtst;


/**
* @temp-param T  the type to get the name of
* @return  the demangled name of the type
*/
template < class T >
constexpr const char* xtst::type_name( void )
{
    return abi::__cxa_demangle(typeid(T).name(),0,0,0);
}


/**
* @temp-param Tuple  a std::tuple class that holds the types
* @return a comma and space seperated list of the names of the types
*/
template < class Tuple >
constexpr const char* xtst::type_name_csl( void )
{
    return dtl::type_name_csl_helper<Tuple>::value;
}

/**
* @param func  the function to be called
* @param tup  the tuple of args to be used in the function call
* @param seq only used to get a sequence of indices for std::get
* @return  return value of function call
*/
template < class R, class F, class Tuple, size_t...I >
constexpr R dtl::apply_impl(F&& func, Tuple&& tup, IntegerSeq<I...> seq)
{
    return func(std::get<I>(std::forward<Tuple>(tup))...);
}

/**
* calls a given function using the values in a given tuple as arguments
*
* @param func  the function to be called
* @param tup  the tuple of args to be used in the function call
* @return  return value of function call
*/
template < class R, class F, class Tuple >
constexpr R xtst::apply( F&& func, Tuple&& tup )
{
    using namespace std;
    return apply_impl(forward<F>(func), forward<Tuple>(tup),
        IndexSeq<tuple_size<Decay<Tuple>>::value>{});
}

template < class T, class Tuple, std::size_t...I >
constexpr std::array<T, sizeof...(I)> dtl::to_array_of_impl(const Tuple& tup, IntegerSeq<I...>, const T& def )
{
    return std::array<T, sizeof...(I)> { if_cvt<TypeAt<I, Tuple>, T>(static_cast<T>(std::get<I>(tup)), def)... };
}

/**
* @temp-param T  type to convert values of tup to
* @param tup  tuple of values to be converted
* @param def  default value to fallback on when a tuple type cannot be converted
*/
template < class T, class...Types >
constexpr std::array<T, sizeof...(Types)> xtst::to_array_of(const std::tuple<Types...>& tup, T def)
{
    return dtl::to_array_of_impl(tup, IndexSeq<sizeof...(Types)>{}, def);
}

/**
* @param from_v  value that attempt conversion on
* @param def  value to fallback on if Fr cannot be converted to To
* @return from_v converted to To if possible, else def
*/
template < class Fr, class To >
constexpr To xtst::if_cvt(const Fr& from_v, const To& def)
{
    if_constexpr (std::is_convertible<Fr, To>::value)
        return To(from_v);
    else return def;
}
