#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <initializer_list>
#include <regex>

#include "FunctionTraits.hpp"


namespace xtst
{
    template < class T >
    using ilist = const std::initializer_list<T>&;

    typedef std::unordered_map<std::string, std::string> string_map_t;

    template< class Tr >
    class Formatter final : public AssertTraits<Tr>
    {
        using typename Tr::return_type;
        using typename Tr::arg_types;

        explicit constexpr Formatter( void ) = default;

        std::string Format( bool, std::shared_ptr<return_type>,const return_type&, const arg_types& );

        static void SetFormat ( const std::string& );
        static void SetArgPattern ( const std::string& );

      private:
        static const std::regex pattern = std::regex( R"\$\{([a-z\-]+)?}" );
        static std::string format;
        static std::string arg_pattern;
        std::pair<string_map_t, std::array<std::string, Tr::argc>> get_args( const return_type&, const arg_types& ) const;
    };
}

#endif
