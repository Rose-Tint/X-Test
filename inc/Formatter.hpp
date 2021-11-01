#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <initializer_list>

#include "FunctionTraits.hpp"


namespace xtst
{
    template < class T >
    using ilist = const std::initializer_list<T>&;

    template< class Tr >
    class Formatter final : public AssertTraits<Tr>
    {
        using Tr::return_type;
        using Tr::arg_types;

        explicit Formatter( bool, const return_type&, const arg_types& );

        operator std::string() const;

        static void SetFormat ( const std::string& );
        static void SetArgPattern ( const std::string& );

      private:
        struct Seek
        {
            std::size_t start = 0;
            std::size_t end = 0;
            std::size_t offset = 0;
            std::string curr;
        };

        static std::string format;
        static std::string arg_pattern;

        Seek main_sk;
    };

    template < class Tr >
    std::string format( const Formatter<Tr>& );

    template < class Fmt, class Rtn, class Args... >
    std::string format( bool, const Rtn&, const std::tuple<Args...>& );
}

#endif
