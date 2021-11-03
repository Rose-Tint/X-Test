#ifndef X_TEST_CASEARG_HPP
#define X_TEST_CASEARG_HPP

#include <cstddef>


namespace xtst
{
    template < std::size_t I, class Fn, Fn fn > class ArgGenerator;

    template < std::size_t, class Sig, Sig Func > struct CaseArg;
    template < std::size_t I, class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    struct CaseArg<I, Rtn(*)(ArgTypes...), Func>
    {
      public:
        typedef ArgGenerator<I, decltype(&Func), Func> gen_type;
        typedef typename gen_type::value_type value_type;

        constexpr CaseArg( void ) noexcept(gen_type()) : gen(gen_type()) { }
        constexpr CaseArg( value_type&& val ) : arg(val), method(Fwd) { }
        constexpr CaseArg( gen_type&& g ) : gen(g), method(Gen) { }

        constexpr value_type generate( void ) const & { return (method == Gen) ? gen() : arg; }

      private:
        union
        {
            gen_type gen;
            value_type arg;
        };
        enum : bool { Gen, Fwd } method;
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
        { typedef std::tuple<CaseArg<I, Rtn(*)(ArgTypes...), Fn>...> type; };

      public:
        template < std::size_t I >
        using case_arg_type = CaseArg<I, Rtn(*)(ArgTypes...), Fn>;
        typedef typename MakeCaseArgTuple<IndexSeq<sizeof...(ArgTypes)>>::type type;
    };
}

#endif
