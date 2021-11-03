#ifndef X_TEST_ARGGENERATOR_HPP
#define X_TEST_ARGGENERATOR_HPP

#include "TypeTraits.hpp"


namespace xtst
{
    template < std::size_t, class Fn, Fn Func > struct ArgGenerator;
    template < std::size_t I, class Rtn, class...Args, Rtn(*Func)(Args...) >
    struct ArgGenerator<I, Rtn(*)(Args...), Func>
    {
        typedef TypeAt<I, Args...> value_type;
        typedef Rtn(*signature_f)(Args...);

        static constexpr signature_f function = Func;

        constexpr ArgGenerator() = default;

        value_type operator ()( void ) const & { return value_type(); }
    };
}

#endif
