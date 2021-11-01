#ifndef X_TEST_UTILS_HPP
#define X_TEST_UTILS_HPP


namespace xtst
{
    template < class T >
    constexpr const char* demangle_type(T* val)
        { return abi::__cxa_demangle(typeid(*val).name(),0,0,0); }


    // needed for `apply`
    namespace dtl {
        template < class R, class F, class Tuple, std::size_t...I >
        constexpr R apply_impl(F&& func, Tuple&& tup, IndexSeq<I...>)
        {
            return func(std::get<I>(fwd_args<Tuple>(tup))...);
        }
    }

    // std::apply is needed, but not supported until c++17, so i defined my own
    template < class R, class F, class Tuple >
    constexpr R apply( F&& func, Tuple&& tup )
    {
        using namespace std; using namespace dtl;
        return apply_impl(fwd_args<F>(func), fwd_args<Tuple>(tup),
            make_idx_seq<tuple_size<rm_ref<Tuple>>::value>{});
    }

    inline constexpr std::size_t operator "" _hash( const char* begin, std::size_t size )
        { return (std::hash<string_t>())(string_t(begin)); }
}

#endif