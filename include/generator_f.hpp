#ifndef X_UTILS_GENERATOR_F_HPP
#define X_UTILS_GENERATOR_F_HPP

#include "./TypeTraits.hpp"


namespace xutl
{
    template < class T >
    using generator_f = T(*)( void );

    template < class Fn, Fn f > struct FuncPair;
    template < class Rtn, class...Args, Rtn(*Func)(Args...) >
    struct FuncPair<Rtn(*)(Args...), Func>
    {
        using type = Rtn(*)(Args...);
        using func = Func;
    };

    template < class T, generator_f<T> Fn >
    using GenPair = FuncPair<generator_f<T>, Fn>;

    template < class...genTs > struct GenTuple; // = delete
    {
        using type = std::tuple<generator_f<genTs>...>;
        static type gens;
    };
    template < class...genTs, generator_f<genTs>...genFs >
    struct GenTuple<GenPair<genTs, genFs>...>
    {
        using type = std::tuple<generator_f<genTs>...>;
        static type gens = { genFs... };
    };

    template < class, class > struct SetGenTuple;
    template < class...lTs, class...lFs, class...rTs, class...rFs >
    struct SetGenTuple<
        GenTuple<GenPair<lTs, lFs>...>,
        GenTuple<GenPair<rTs, rFs>...>
    >{
        static_assert( sizeof...(lTs) == sizeof...(rTs) );
        using type = GenTuple<
            CondValue<IsNullFunction<generator_f<rTs>, rFs>,
                GenPair<lTs, lFs>,
                GenPair<rTs, rFs>
            >...
        >;
    };
    template < class lFPair, class rFPair >
    using SetGenTuple_t = typename SetGenTuple<lFPair, rFPair>::type;
}

#endif