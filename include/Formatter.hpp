#ifndef X_TEST_FORMATTER_HPP
#define X_TEST_FORMATTER_HPP

#include <string>
#include <array>
#include <memory>
#include <unordered_map>
#include <regex>
#include <initializer_list>
#include <ostream>

#include "./utils.hpp"


namespace xtst
{
    using fmt_args_t = std::unordered_map<std::string, std::string>;


    template < class T >
    using ilist = const std::initializer_list<T>&;


    template < class Fn, Fn f > class Formatter;

    template < class Rtn, class...ArgTs, Rtn(*Func)(ArgTs...) >
    class Formatter<Rtn(*)(ArgTs...), Func> final
    {
      public:
        typedef std::tuple<ArgTs...> arg_tuple_t;
        typedef std::shared_ptr<Rtn> return_ptr_t;
        typedef std::shared_ptr<Rtn> result_ptr_t;

        static void Format(std::ostream&, bool, result_ptr_t, return_ptr_t, const arg_tuple_t&);

        static void SetFormat(const std::string&);
        static void SetArgPattern(const std::string&);

        Formatter(void) = default;

      private:
        static constexpr auto ArgIdxs = utl::IndexSeq<sizeof...(ArgTs)>{};
        static inline const std::regex pattern = std::regex(R"(\$\{([a-z\-]+)?})");
        static inline std::string format =
            "${func-name} -> ${result}\n"
              "\treturn value: ${return}\n"
              "\targs:${args}\n";
        static inline std::string arg_pattern = "\n\t\t${type} ${value}";

        static fmt_args_t get_fmt_args(const return_ptr_t&, const arg_tuple_t&);
        template < std::size_t...I >
        static std::string format_args(const arg_tuple_t&, utl::IntegerSeq<I...>);
        template < class T >
        static std::string format_arg(const T&);
    };


    /**
    * List of main formatting arguments
    * * ${func-name}   : name of the function being tested
    * * ${result}      : "Pass" if the the test case follows the expected behavior, else "Fail"
    * * ${return}      : return value of the case being tested
    * * ${return-type} : type of the return value
    * * ${signature}   : function signature of the function being tested
    * * ${args}        : values of the given args, formatted using arg_pattern
    *
    * List of arg pattern formatting args
    * * ${index}   : type of the arg
    * * ${type}    : type of the arg
    * * ${value}   : string value of the arg, or the address if not convertible to `std::string`, \
                       else "nullptr"
    * * ${address} : address of the arg
    *
    * @param test_passed  whether or not the test being formatted passed
    * @param exp  value that was expected from the test case, or nullptr if an error was expected
    * @param rtn  return value of the tested case
    * @param args  arguments used in the tested case
    * @return  a formatted string
    */
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    void Formatter<Rtn(*)(ArgTypes...), Func>::Format(std::ostream& stream,
                                    bool test_passed,
                                    result_ptr_t exp,
                                    return_ptr_t rtn,
                                    const arg_tuple_t& args)
    {
        // readability > memory.
        std::size_t end_pos = 0, pos = 0;
        std::string param, fmt_arg, match_str;
        std::smatch match;
        auto fmt_args = get_fmt_args(rtn, args);

        std::regex_search(format, match, pattern);

        for (int i = 0; i < match.size(); match_str = match.str(i++))
        {
            // get what is between "${" and "}"
            param = match_str.substr(2, match_str.size() - 1);

            // if the parameter exists in the argument map, use the value with
            // `param` as it's key
            if (fmt_args.count(param) != 0)
                fmt_arg = fmt_args.at(param);
            else fmt_arg = match_str;

            end_pos = pos + fmt_arg.size();
            pos = match.position(i);
            stream << format.substr(end_pos, pos) << fmt_arg;
        }
    }


    /**
    * @param rtn  return value of the tested case
    * @param args  arguments used in the tested case
    * @return  a pair consisting of a map with format args as keys,
    *     and an array of string values of args if possible, else "unavailable"
    */
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    fmt_args_t Formatter<Rtn(*)(ArgTypes...), Func>::get_fmt_args(const return_ptr_t& rtn, const arg_tuple_t& args)
    {
        std::string rtn_t_str = type_str<Rtn>();
        std::string sig_str = rtn_t_str+"(*)("+type_csl<arg_tuple_t>()+")";
        return fmt_args_t {
            { "return"      , utl::try_str_cvt(&rtn)      },
            { "return-type" , rtn_t_str                   },
            { "signature"   , sig_str                     },
            { "func-name"   , type_str<decltype(Func)>()  },
            { "args"        , format_args(args, ArgIdxs)  },
        };
    }


    /**
    * formats args according to `arg_pattern`
    *
    * @param args  tuple of args to format
    */
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    template < std::size_t...I >
    std::string Formatter<Rtn(*)(ArgTypes...), Func>::format_args(const arg_tuple_t& args, IntegerSeq<I...>)
    {
        std::string arg_str;
        ilist<std::string> arg_list { format_arg(std::get<I>(args))... };
        for (const std::string& str : arg_list)
            arg_str.append(str);
        return arg_str;
    }


    template < class T >
    std::string addr_to_str(T* v_ptr)
    {
        std::stringstream ss;
        ss << (void*)v_ptr;
        return ss.str();
    }


    /**
    * @param arg  argument to return a formating map
    * @return a map containing formatting params as keys, and their respective values based on properties of `arg`
    */
    template < class Rtn, class...ArgTypes, Rtn(*Func)(ArgTypes...) >
    template < class T >
    std::string Formatter<Rtn(*)(ArgTypes...), Func>::format_arg(const T& arg)
    {
        const fmt_args_t fmt_args
        {
            { "index"   , 0                      }, // TODO
            { "type"    , type_str<T>()         },
            { "value"   , utl::try_str_cvt(&arg) },
            { "address" , addr_to_str(&arg)      },
        };
        std::string fstr = arg_pattern, param;
        std::size_t pos = 0, end_pos = 0;

        while (pos != std::string::npos)
        {
            pos = fstr.find("${", end_pos);
            end_pos = fstr.find("}", pos) + 1;
            param = fstr.substr(pos, end_pos - pos);
            if (fmt_args.count(param) == 0)
                continue;
            fstr.replace(pos, pos - end_pos, fmt_args.at(param));
        }

        return fstr;
    }
}


#endif
