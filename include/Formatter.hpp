#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <regex>
#include <sstream>

#include "utils.hpp"


namespace xtst
{
    using fmt_args_t = std::unordered_map<std::string, std::string>;

    template< class Traits > class Formatter
    {
        using return_type = typename Traits::return_type;
        using arg_types = typename Traits::arg_types;

        explicit constexpr Formatter( void ) = default;

        void Format( std::ostringstream&, bool, std::shared_ptr<return_type>, std::unique_ptr<return_type>, const arg_types& ) const &;

        static void SetFormat ( const std::string& );
        static void SetArgPattern ( const std::string& );

      private:
        static constexpr auto ArgIndices = IndexSeq<Traits::argc>{};
        static inline const std::regex pattern = std::regex( R"(\$\{([a-z\-]+)?})" );
        static std::string format;
        static std::string arg_pattern;

        fmt_args_t get_fmt_args( const std::unique_ptr<return_type>&, const arg_types& ) const &;
        template < std::size_t...I >
        std::string format_args( const arg_types&, IntegerSeq<I...> = ArgIndices ) const &;
        template < class T >
        std::string format_arg( const T& ) const &;
        template < class T >
        void try_str_cvt( std::stringstream&, T* );
    };
}

#endif
