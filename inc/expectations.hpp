#ifndef X_TEST_GIVENVALUE_HPP
#define X_TEST_GIVENVALUE_HPP

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

        constexpr CaseArg( void ) noexcept(gen_type());
        CaseArg( value_type&& );
        CaseArg( gen_type&& );

        constexpr value_type generate( void ) const &;

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
        template < std::size_t I >
        using case_arg_type = CaseArg<I, Rtn(*)(ArgTypes...), Fn>;
        typedef IndexSeq<sizeof...(ArgTypes)> Size;
        typedef std::tuple<CaseArg<>> type;
    };
}

#endif
