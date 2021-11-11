#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <regex>
#include <ostream>

#include "utils.hpp"


namespace xtst
{
    using fmt_args_t = std::unordered_map<std::string, std::string>;

    template< class Traits > class Formatter
    {
        using return_type = typename Traits::return_type;
        using arg_types = typename Traits::arg_types;

        explicit constexpr Formatter(void) = delete;

        static void Format(std::ostream&, bool, std::shared_ptr<return_type>, std::unique_ptr<return_type>, const arg_types&);

        static void SetFormat(const std::string&);
        static void SetArgPattern(const std::string&);

      private:
        static constexpr auto ArgIndices = IndexSeq<Traits::argc>{};
        static inline const std::regex pattern = std::regex(R"(\$\{([a-z\-]+)?})");
        static std::string format;
        static std::string arg_pattern;

        static fmt_args_t get_fmt_args(const std::unique_ptr<return_type>&, const arg_types&);
        template < std::size_t...I >
        static std::string format_args(const arg_types&, IntegerSeq<I...> = ArgIndices);
        template < class T >
        static std::string format_arg(const T&);
        template < class T >
        static void try_str_cvt(std::stringstream&, T*);
    };
}

#endif
