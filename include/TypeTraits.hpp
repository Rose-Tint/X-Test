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
        // transformers
        template < bool, class = bool > struct EnableIf { };
        template < class Cls = bool >
        struct EnableIf<true, Cls>
        { typedef Cls type; };
        template < bool B, class C = bool >
        using EnableIf_t = typename EnableIf<B, C>::type;

        template < std::size_t I, class...Types > struct TypeAt
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
            : Conditional_t<std::true_type, std::false_type, And<AreSame<T, Ts>...>> { };

        template < class, class > struct AreSame : std::false_type { };
        template < class T > struct AreSame<T, T> : std::true_type { };

        template < class T1, class T2 > struct AreSameNaked
            : AreSame<typename std::decay<T1>::type, typename std::decay<T2>::type> { };

        // enablers
        template < class To, class From >
        using EnableIfAllCvt = EnableIf_t<std::convertible<To, From>>()>;
        template < class...Args >
        using EnableIfStrCvt = EnableIfCvt<std::string, Args...>;
        template < class F, class...A >
        using EnableIfCallable = EnableIf<std::is_invocable<F, A...>::value>;

        // operations
        // AND
        template < class... > struct AndTraits // default case
        { static constexpr bool value = true; };
        template < class Curr, class...Next > struct AndTraits<Curr, Next...> // recurring case
        { static constexpr bool value = Curr::value && AndTraits<Next...>::value; };
        template < class Curr > struct AndTraits<Curr> // base case
        { static constexpr bool value = Curr::value; };
        // OR
        template < class... > struct OrTraits // default case
        { static constexpr bool value = true; };
        template < class Curr, class...Next > struct OrTraits<Curr, Next...> // recurring case
        { static constexpr bool value = Curr::value || OrTraits<Next...>::value; };
        template < class Curr > struct OrTraits<Curr> // base case
        { static constexpr bool value = Curr::value; };

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
