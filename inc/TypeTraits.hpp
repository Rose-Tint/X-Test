#ifndef X_TEST_TYPETRAITS_HPP
#define X_TEST_TYPETRAITS_HPP

#include <tuple>
#include <type_traits>


namespace xtst
{
    inline namespace typetraits
    {
        // convenience
        template < class T >
        using Decay = typename std::decay<T>::type;
        template < class T >
        using RemoveRef = typename std::remove_reference<T>::type;
        template < bool Cond >
        using EnableIf = typename std::enable_if<Cond, std::true_type>::type;
        template < class T1, class T2 >
        using AreSame = std::is_same<T1, T2>;
        template < class T1, class T2 >
        using SameNaked = AreSame<Decay<T1>, Decay<T2>>;

        // transformers
        namespace dtl
        {
            template < std::size_t I, class Tuple > struct TypeAtHelper;
            template < std::size_t I, class...Types >
            struct TypeAtHelper<I, std::tuple<Types...>>
            { typedef typename std::tuple_element<I, std::tuple<Types...>>::type type; };
        }

        template < std::size_t I, class...Types >
        using TypeAt = typename dtl::TypeAtHelper<I, std::tuple<Types...>>::type;

        // traits
        template < class To, class...Froms >
        constexpr bool are_convertible( void );
        template < class T, class...Ts >
        constexpr bool in_param_pack( void );

        // enablers
        template < class To, class...Froms >
        using EnableIfCvt = EnableIf<are_convertible<To, Froms...>()>;
        template < class...Args >
        using EnableIfStrCvt = EnableIfCvt<std::string, Args...>;
        template < class F, class...A >
        using EnableIfCallable = EnableIf<std::is_invocable<F, A...>::value>;

        template < std::size_t... > struct IntegerSeq { };

        namespace dtl
        {
            // see https://stackoverflow.com/questions/49669958/details-of-stdmake-index-sequence-and-stdindex-sequence
            template < std::size_t N, std::size_t...Next >
            struct IntegerSeqHelper : public IntegerSeqHelper<N-1U, N-1U, Next...> { };

            // 0U template specialization acts as a base case
            template < std::size_t...Next > struct IntegerSeqHelper<0U, Next...>
            { using type = IntegerSeq<Next...>; };
        }

        template < std::size_t N >
        using IndexSeq = typename dtl::IntegerSeqHelper<N>::type;
    }
    namespace dtl { using namespace typetraits::dtl; }
}

#endif
