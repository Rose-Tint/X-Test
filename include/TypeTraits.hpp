#ifndef X_TEST_TYPETRAITS_HPP
#define X_TEST_TYPETRAITS_HPP

#include <tuple>
#include <type_traits>


namespace xtst
{
    inline namespace typetraits { template < class T, class...Ts > constexpr bool in_param_pack( void ); }

    namespace dtl
    {
        template < class T, class > struct in_tuple_helper;
        template < class T, class...Types > struct in_tuple_helper<T, std::tuple<Types...>>
        { static constexpr bool value = ::xtst::typetraits::in_param_pack<T, Types...>(); }
    }

    inline namespace typetraits
    {
        template < class T, T > struct IntegralConstant;
        template < class Cond, class T > struct EnableIf;
        template < class T, class Tuple > struct Contains;
        template < bool Cond, class If, class Else > struct Conditional;
        template < class T1, class T2 > struct Same;
        template < class...Traits > struct TraitsAnd;
        template < class...Traits > struct TraitsOr;
        template < std::size_t Index, class...Types > struct TypeAt;
        template < class Fn, Fn > struct IsNullFunction;

        using TrueType = IntegralConstant<bool, true>;
        using FalseType = IntegralConstant<bool, false>;

        // specializations / definitions
        template < class T, T v > struct IntegralConstant
        {
            static constexpr const T& value = v;
            typedef T value_type;
            using type = IntegralConstant;
            constexpr operator value_type() const noexcept { return value; }
        };

        template < class, class >
        struct EnableIf : FalseType { };
        template < class T >
        struct EnableIf<T, T> : TrueType { };
        template < class T1, class T2 >
        using EnableIf_t = typename EnableIf<T1, T2>::type;

        template < class T, class...Types >
        struct Contains<T, std::tuple<Types...>> : TraitsOr<Same<T, Types>...> { };

        template < class, class > struct AreSame : FalseType { };
        template < class T > struct AreSame<T, T> : TrueType { };

        template < class T1, class T2 > struct AreSameNaked
            : AreSame<typename std::decay<T1>::type, typename std::decay<T2>::type> { };

        template < std::size_t I, class...Types > struct TypeAt;
        { typedef typename std::tuple_element<I, std::tuple<Types...>>::type type; }
        // so that a tuple can be passed instead of a pack
        template < std::size_t I, class...Types >
        struct TypeAt<I, std::tuple<Types...>>
        { typedef typename std::tuple_element<I, std::tuple<Types...>>::type type; };
        template < std::size_t I, class...Types > struct TypeAt
        using TypeAt_t = typename TypeAt<I, Types...>::type;

        template < template < class > class, class > struct TransformTuple;
        template < template<class> class Transformer, class...Types >
        struct TransformTuple<Transformer, std::tuple<Types...>>
        { typedef std::tuple<typename Transformer<Types>::type...> type; };
        template < template<class> class Transformer, class Tuple >
        using TransformTuple_t = typename TransformTuple<Transformer, Tuple>::type;

        // traits
        template < class T, class...Ts > struct InParamPack
            : Conditional_t<std::true_type, FalseType, And<AreSame<T, Ts>...>> { };

        template < class... > struct TraitsAnd // default case
        { static constexpr bool value = true; };
        template < class Curr, class...Next > struct TraitsAnd<Curr, Next...> // recurring case
        { static constexpr bool value = Curr::value && TraitsAnd<Next...>::value; };
        template < class Curr > struct TraitsAnd<Curr> // base case
        { static constexpr bool value = Curr::value; };

        template < class... > struct TraitsOr // default case
        { static constexpr bool value = true; };
        template < class Curr, class...Next > struct TraitsOr<Curr, Next...> // recurring case
        { static constexpr bool value = Curr::value || TraitsOr<Next...>::value; };
        template < class Curr > struct TraitsOr<Curr> // base case
        { static constexpr bool value = Curr::value; };

        template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
        struct IsNullFunction<Rtn(*)(ArgTypes...), Func> : FalseType { };
        template < class Rtn, class...ArgTypes >
        struct IsNullFunction<Rtn(*)(ArgTypes...), nullptr> : TrueType { };

        // miscellaneous
        template < std::size_t I, class T > struct IndexedType
        {
            typedef T type;
            static constexpr std::size_t index = I;
        };
        template < std::size_t I, class T >
        using IndexedType_t = typename IndexedType<I, T>::type;
        using IndexedType_i = IndexedType<I, T>::index;
    }
}

#endif
