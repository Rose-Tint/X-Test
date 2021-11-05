#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <string>
#include <memory>
#include <unordered_map>
#include <initializer_list>
#include <regex>

#include "./FunctionTraits.hpp"
#include "./TypeTraits.hpp"


namespace xtst
{
    template < class T >
    using ilist = const std::initializer_list<T>&;

    typedef std::unordered_map<std::string, std::string> string_map_t;

    template < std::size_t N >
    using fmt_args_t = std::pair<string_map_t, std::array<std::string, N>>;

    template< class Traits >
    class Formatter final : Traits
    {
        using typename Traits::return_type;
        using typename Traits::arg_types;

        explicit constexpr Formatter( void ) = default;

        std::string Format( bool, std::shared_ptr<return_type>, const return_type*, const arg_types& );

        static void SetFormat ( const std::string& );
        static void SetArgPattern ( const std::string& );

      private:
        // matches dash-seperated words enclosed in ${}, and groups whats inside.
        static const std::regex pattern = std::regex( R"\$\{([a-z\-]+)?}" );
        static std::string format;
        static std::string arg_pattern;

        static fmt_args_t<Traits::argc> get_args( const return_type*, const arg_types& );
        template < class T >
        static std::string try_str_cvt( T* );
        template < class T, EnableIf_t<std::is_convertible<T, std::string>::value> = true >
        static std::string try_str_cvt( T* v_ptr ) { return (v_ptr == nullptr) ? "nullptr" : std::string(*v_ptr); }
    };
}

#endif
