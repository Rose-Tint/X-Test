#ifndef X_TEST_TESTBASE_HPP
#define X_TEST_TESTBASE_HPP

#include <initializer_list>
#include <utility>
#include <vector>
#include <memory>

#include "./TypeTraits.hpp"


namespace xtst
{
    template < class T > using ilist = const std::initializer_list<T>&;

    template < class Fn, Fn > struct FunctionTraits;
    template < class Traits > struct Formatter;

    enum struct CaseResult : bool;
    enum struct ExpResult : bool;

    template < class T >
    using generator_f = T(*)( void );

    template < class Traits > class UnitTest
    {
      public:
        using typename Traits::return_type;
        using typename Traits::arg_types;
        using typename Traits::input_types;
        typedef std::pair<return_type, input_types> trusted_t;

        UnitTest() = delete;

        template < std::size_t, class T >
        constexpr static void SetArgGen( generator_f<T> );
        constexpr static void SetArgGens( TransformTuple_t<generator_f, arg_types> );
        static void Trust( return_type, input_types );
        static void Trust( ilist<trusted_t> );
        static void Doubt( input_types );
        static void Doubt( ilist<input_types> );
        static void RunTests( void );
        static std::vector<CaseResult> Results( void );

      private:
        using typename Traits::error_types;
        typedef std::pair<std::shared_ptr<return_type>, ExpResult> expectation;
        typedef Formatter<Traits> format_type;
        typedef std::pair<expectation, input_types> case_type;
        typedef dtl::InheritAll<error_types> AllErrors;
        typedef TransformTuple_t<generator_f, arg_types> gen_types;

        static constexpr return_type run_case( arg_types );
        static constexpr arg_types gen_from_input( input_types );
        static constexpr bool is_exp_error( void );

        static gen_types arg_gens;
        static std::vector<CaseResult> results;
        static std::vector<trusted_t> cases;
        static const format_type formatter = format_type();
        static constexpr auto function = Traits::function;
    };

    template < class Fn, Fn > struct MakeUnitTestHelper;
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    struct MakeUnitTestHelper<Rtn(*)(ArgTypes...), Func>
    {
        typedef FunctionTraits<decltype(&Func), Func> traits;
        typedef UnitTest<traits> test;
    };
}

#endif
