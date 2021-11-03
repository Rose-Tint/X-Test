#ifndef X_TEST_TYPETRAITS_HPP
#define X_TEST_TYPETRAITS_HPP

#include <tuple>
#include <type_traits>


namespace xtst
{
    inline namespace typetraits { template < class T, class...Ts > constexpr bool in_param_pack( void ); }

    namespace dtl
    {
        template < std::size_t I, class Tuple > struct TypeAtHelper;
        template < std::size_t I, class...Types >
        struct TypeAtHelper<I, std::tuple<Types...>>
        { typedef typename std::tuple_element<I, std::tuple<Types...>>::type type; };

        template < class T, class > struct in_tuple_helper;
        template < class T, class...Types > struct in_tuple_helper<T, std::tuple<Types...>>
        { static constexpr bool value = ::xtst::typetraits::in_param_pack<T, Types...>(); }
    }

    inline namespace typetraits
    {
        // convenience
        template < class T >
        using Decay = typename std::decay<T>::type;
        template < class T >
        using RemoveRef = typename std::remove_reference<T>::type;
        template < bool Cond >
        using EnableIf = typename std::enable_if<Cond, bool>::type;
        template < class T1, class T2 >
        using AreSame = std::is_same<T1, T2>;
        template < class T1, class T2 >
        using SameNaked = AreSame<Decay<T1>, Decay<T2>>;
        template < bool C, class T, class F >
        struct Condi

        // transformers
        template < std::size_t I, class...Types >
        using TypeAt = typename dtl::TypeAtHelper<I, std::tuple<Types...>>::type;

        // traits
        template < class To, class...Froms >
        constexpr bool are_convertible( void );
        template < class T, class...Ts >
        constexpr bool in_param_pack( void );
        template < class T, class Tuple >
        constexpr bool in_tuple( void );

        // enablers
        template < class To, class...Froms >
        using EnableIfCvt = EnableIf<are_convertible<To, Froms...>()>;
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
    }
}

#endif
