#ifndef X_TEST_FUNCTIONTRAITS_HPP
#define X_TEST_FUNCTIONTRAITS_HPP

#include <tuple>
#include <cstddef>


namespace xtst
{
    template < class > class CaseArgTuple;
    namespace dtl
    {
        // ONLY for use as the base for specialized FunctionTraits
        inline constexpr void pseudo_function( void ) { return; }
    }
    template < class S, S, class...Errors > struct FunctionTraits final { };
    template < class R, class...ArgTypes, R(*Func)(ArgTypes...), class...Errors >
    struct FunctionTraits<R(*)(ArgTypes...), Func, Errors...>
    {
        typedef std::tuple<Errors...> error_types;
        typedef R return_type;
        typedef CaseArgTuple<ArgTypes...> input_types;
        typedef std::tuple<ArgTypes...> arg_types;
        typedef R(*signature_f)(ArgTypes...);

        FunctionTraits() = delete;

        static constexpr signature_f function = Func;
        static constexpr std::size_t argc = sizeof...( ArgTypes );

        static constexpr return_type Apply(arg_types);
    };

    // specialized FunctionTraits should inherit from this.
    using BaseFuncTraits = FunctionTraits<decltype(&dtl::pseudo_function), dtl::pseudo_function>;

    namespace dtl
    {
        enum TraitAssertion : bool { IsTrait, IsNotTrait };
        template < class Tr, TraitAssertion ta > struct AssertTraitsHelper { };
        template < class Tr > struct AssertTraitsHelper<Tr, IsTrait >
        { typedef Tr traits; };
    }

    // returns whether a given traits class is valid and complete
    template < class Tr > constexpr dtl::TraitAssertion assert_traits( void );

    // acts at the given class if the class is a valid and complete FunctionTraits class
    template < class Tr >
    using AssertTraits = typename dtl::AssertTraitsHelper<Tr, assert_traits<Tr>()>::traits;
}

 
#endif
