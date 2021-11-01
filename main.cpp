#ifndef X_TEST_TESTBASE_HPP
#define X_TEST_TESTBASE_HPP

#include "inc/FunctionTraits.hpp"


namespace xtst
{
    template < class T > using ilist = const std::initializer_list<T>&;
    template < class Traits > struct Formatter;

    enum struct CaseResult : bool { Pass, Fail };

    template < class Traits > class UnitTest : public AssertTraits<Traits>
    {
        using Traits::Expectation;
        using Traits::return_type;
        using Traits::InputTypes;
        typedef std::pair<Expectation, InputTypes> TestCase;

        UnitTest() = delete;

        template < ExpResult >
        static constexpr void Append( return_type, InputTypes );
        static constexpr void Append( Expectation, InputTypes );
        static constexpr void Append( const TestCase& );
        static constexpr void Append( ilist<TestCase> );
        static void RunTests( void );

      private:
        using Traits::arg_types;
        typedef Formatter<Traits> format_type;

        static constexpr return_type run(arg_types);
        static constexpr arg_types norm_to_args(InputTypes);

        static std::vector<CaseResult> results;
        static std::vector<TestCase> cases;
        const format_type formatter;
        static constexpr auto function = Traits::function;
    };
}

#endif
