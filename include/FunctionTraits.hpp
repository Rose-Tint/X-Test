#ifndef X_TEST_FUNCTIONTRAITS_HPP
#define X_TEST_FUNCTIONTRAITS_HPP

#include <tuple>
#include <cstddef>


namespace xtst
{
    template < class T >
    using generator_f = T(*)(void);

    template < class S, S, class...Errors > struct FunctionTraits { };
    template < class R, class...ArgTypes, R(*Func)(ArgTypes...), class...Errors >
    struct FunctionTraits<R(*)(ArgTypes...), Func, Errors...>
    {
        typedef std::tuple<Errors...> error_types;
        typedef R return_type;
        typedef std::tuple<ArgTypes...> arg_types;
        typedef R(*signature_f)(ArgTypes...);

        FunctionTraits() = delete;

        static constexpr signature_f function = Func;
        static constexpr std::size_t argc = sizeof...( ArgTypes );

        static constexpr return_type Apply( const arg_types& args ) { return apply(function, args); }
    };

    namespace dtl
    {
        // ONLY for use as the base for specialized FunctionTraits
        inline constexpr void pseudo_function( void ) { return; }
    }

    // specialized FunctionTraits should inherit from this. aliased as `BaseFuncTraits`.
    template < >
    struct FunctionTraits<decltype(&dtl::pseudo_function), dtl::pseudo_function>
    {
        typedef std::tuple<> error_types;
        typedef void return_type;
        typedef std::tuple<> arg_types;
        typedef void(*signature_f)( void );

        FunctionTraits() = delete;

        static constexpr signature_f function = dtl::pseudo_function;
        static constexpr std::size_t argc = 0;
    };

    using BaseFuncTraits = FunctionTraits<decltype(&dtl::pseudo_function), dtl::pseudo_function>;
}

 
#endif
