#ifndef X_TEST_CASEARG_HPP
#define X_TEST_CASEARG_HPP

#include <cstddef>
#include <cstdint>



namespace xtst
{
    inline namespace utl
    {
        template < std::size_t > struct IndexSeq;
        template < std::size_t > struct IntegerSeq;
    }


    template < class T >
    using generator_f = T(*)( void );

    template < template < class > class, class > struct GenTuple;
    template < template < class > class Transform, class...Types >
    struct GenTuple<Transform, std::tuple<Types...>>
    {
        typedef std::tuple<typename Transform<Types>::type...> type;
    }

    template < class T, std::size_t I = SIZE_MAX > struct CaseArg
    {
      public:
        typedef generator_f<T> gen_type;
        typedef T value_type;

        constexpr CaseArg( const value_type& val ) : arg(val), method(Fwd) { }
        constexpr CaseArg( gen_type g ) : gen(g), method(Gen) { }

        constexpr value_type generate( void ) const & { return (method == Gen) ? gen() : arg; }

      private:
        enum : bool { Gen, Fwd } method;
        union { gen_type gen; value_type arg; };
    };

    enum struct ExpResult : bool
    {
        Error  = false, Doubt = Error,
        Return = true, Trust = Return,
    };

    template < class Fn, Fn > struct CaseArgTuple;
    template < class Rtn, class...ArgTypes, Rtn(*Fn)(ArgTypes...) >
    struct CaseArgTuple<Rtn(*)(ArgTypes...), Fn>
    {
      private:
        static constexpr auto AllArgIndices = IndexSeq<sizeof...(ArgTypes)>{};
        template < class > struct MakeCaseArgTuple;
        template < std::size_t...I > struct MakeCaseArgTuple<IntegerSeq<I...>>
        { typedef std::tuple<CaseArg<ArgTypes, I>...> type; };

      public:
        template < std::size_t I >
        using case_arg_type = CaseArg<CaseArg<ArgTypes, I>...>;
        typedef typename MakeCaseArgTuple<IndexSeq<sizeof...(ArgTypes)>>::type type;
    };
}

#endif
