#ifndef X_TEST_GENERATOR_HPP
#define X_TEST_GENERATOR_HPP

#include "TypeTraits.hpp"


namespace xtst
{
    template < std::size_t, class Fn, Fn Func > struct ArgGenerator;
    template < std::size_t I, class Rtn, class...Args, Rtn(*Func)(Args...) >
    struct ArgGenerator<I, Rtn(*)(Args...), Func>
    {
        // ctr -> gens ->
        typedef TypeAt<I, Args...> value_type;
        typedef Rtn(*signature_f)(Args...);

        ArgGenerator();

        value_type operator ()( void );

        static constexpr signature_f function = Func;
    };

    template < std::size_t, class Sig, Sig Func > auto generate_arg( void );
    template < std::size_t I, class Rtn, class...Args, Rtn(*Func)(Args...) >
    TypeAt<I, Args...>
}

#endif
