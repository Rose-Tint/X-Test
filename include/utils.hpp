#ifndef X_TEST_UTILS_HPP
#define X_TEST_UTILS_HPP

#include <tuple>
#include <cstddef>
#include <cxxabi.h>


namespace xtst
{
    // things that need to declared before namespace dtl
    inline namespace utl
    {
        template < class T >
        constexpr const char* type_name( void )
         { return abi::__cxa_demangle(typeid(T).name(),0,0,0); }

        template < std::size_t... > struct IntegerSeq { };
    }

    namespace dtl
    {
        template < class R, class F, class Tuple, std::size_t...I >
        constexpr R apply_impl(F&& func, Tuple&& tup, IntegerSeq<I...>);

        // see https://stackoverflow.com/questions/49669958/details-of-stdmake-index-sequence-and-stdindex-sequence
        template < std::size_t N, std::size_t...Next > struct IntegerSeqHelper { };

        template < std::size_t...Next > struct IntegerSeqHelper<0U, Next...>
        { using type = IntegerSeq<Next...>; };

        template < > struct IntegerSeqHelper<1>
        { using type = IntegerSeq<0>; };

        template < class T, class Tuple, std::size_t...I >
        constexpr std::array<T, sizeof...(I)> to_array_of_impl(const Tuple&, utl::IntegerSeq<I...>, const T&);

        template < class > struct type_name_csl_helper;

        template < class...Types > struct type_name_csl_helper<std::tuple<Types...>>
        {
            static constexpr const char* value = strcat(strcat(type_name<Types>(), ", ")...);
        };
    }

    inline namespace utl
    {
        template < class Tuple >
        constexpr const char* type_name_csl( void );

        // std::apply is needed, but not supported until c++17, so i defined my own
        template < class R, class F, class Tuple >
        constexpr R apply( F&& func, Tuple&& tup );

        template < std::size_t N >
        using IndexSeq = typename ::xtst::dtl::IntegerSeqHelper<N>::type;

        template < class T, class...Types >
        constexpr std::array<T, sizeof...(Types)> to_array_of(const std::tuple<Types...>&, T = T());

        template < class Fr, class To >
        constexpr To if_cvt(const Fr&, const To&);
    }
}

#endif