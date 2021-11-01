#ifndef X_TEST_TESTBASE_HPP
#define X_TEST_TESTBASE_HPP

#include <initializer_list>
#include <utility>
#include <vector>

#include "./FunctionTraits.hpp"


namespace xtst
{
    template < class T > using ilist = const std::initializer_list<T>&;
    
    template < class Traits > struct Formatter;

    enum struct CaseResult : bool;
    enum struct ExpResult : bool;

    template < class Traits > class UnitTest : public AssertTraits<Traits>
    {
        using typename Traits::return_type;
        using typename Traits::input_types;
        using typename Traits::error_types;
        typedef std::pair<return_type, input_types> trusted_t;

        UnitTest() = delete;

        static constexpr void Trust( return_type&&, input_types&& );
        static constexpr void Trust( ilist<trusted_t> );
        template < class Err, EnableIf<in_tuple<Err, error_types> = true>
        static constexpr void Doubt( input_types&& );
        static void RunTests( void );
        static std::vector<CaseResult> Results( void );

      private:
        using typename Traits::arg_types;
        using typename Traits::error_types;
        typedef std::pair<return_type&&, ExpResult> expected_type;
        typedef Formatter<Traits> format_type;
        typedef std::pair<expected_type, input_types> case_type;

        static constexpr return_type run( arg_types );
        static constexpr arg_types generate_from( input_types );

        static std::vector<CaseResult> results;
        static std::vector<case_type> cases;
        const format_type formatter;
        static constexpr auto function = Traits::function;
    };
}

#endif
