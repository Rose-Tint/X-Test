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

    template < class Fn, Fn, class...Es > struct FunctionTraits;
    template < class Traits > class Formatter;

    template < class Traits > class UnitTest final : public Traits
    {
      public:
        typedef typename Traits::return_type return_type;
        typedef typename Traits::arg_types arg_types;

        UnitTest() = delete;

        static void Trust( return_type, arg_types );
        static void Trust( ilist<std::pair<return_type, arg_types>> );
        static void Doubt( arg_types );
        static void Doubt( ilist<arg_types> );
        static void RunTests( void );

      private:
        typedef typename Traits::error_types error_types;
        typedef std::shared_ptr<return_type> result_type;
        typedef Formatter<Traits> formatter;
        typedef std::unique_ptr<result_type> case_rtn_type;
        typedef InheritAll<error_types> all_errors;

        static constexpr return_type run_case( arg_types );
        static constexpr bool is_exp_error( void );

        static inline std::vector<std::pair<return_type, arg_types>> cases { };
        static constexpr auto function = Traits::function;
    };

    template < class Fn, Fn > struct MakeUnitTest;
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    struct MakeUnitTest<Rtn(*)(ArgTypes...), Func>
    {
        typedef FunctionTraits<decltype(&Func), Func> traits;
        typedef UnitTest<traits> test;
    };
}

#endif
