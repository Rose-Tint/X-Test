#include "../include/Formatter.hpp"
#include "../include/macrodef.hpp"
#include <ostream>
#include <unordered_map>


using namespace xtst;


/**
* @param test_passed  whether or not the test being formatted passed
* @param exp  value that was expected from the test case, or nullptr if an error was expected
* @param rtn  return value of the tested case
* @param args  arguments used in the tested case
* @return  a formatted string
*/
template < class Traits >
void Formatter<Traits>::Format(std::ostream& stream,
                                bool test_passed,
                                std::shared_ptr<return_type> exp,
                                std::unique_ptr<return_type> rtn,
                                const arg_types& args)
{
    // readability > memory.
    std::size_t end_pos = 0, pos = 0;
    std::string param, fmt_arg, match_str;
    std::smatch match;
    auto fmt_args = get_fmt_args();

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
template < class Traits >
fmt_args_t Formatter<Traits>::get_fmt_args(const std::unique_ptr<return_type>& rtn, const arg_types& args)
{
    std::string rtn_t_str = type_name<return_type>();
    std::string sig_str = rtn_t_str+"(*)("+type_name_csl<arg_types>()+")";
    return {
        { "return"      , try_str_cvt(rtn)  },
        { "return-type" , rtn_t_str         },
        { "signature"   , sig_str           },
        { "args"        , format_args(args) },
    };
}


/**
* outputs the address of a pointer, or nullptr. this function gets overloaded only if type T
* cannot be converted to `std::string`
*
* @param stream  an ostream to send the result to
* @param v_ptr  pointer to the value to convert to a string
*/
template < class Traits >
template < class T >
void Formatter<Traits>::try_str_cvt(std::stringstream& stream, T* v_ptr)
{
    if (v_ptr == nullptr)
        stream << "nullptr";
    stream << (void*)v_ptr;
}


/**
* formats args according to `arg_pattern`
*
* @param args  tuple of args to format
*/
template < class Traits >
template < std::size_t...I >
std::string Formatter<Traits>::format_args(const arg_types& args, IntegerSeq<I...>)
{
    std::string arg_str;
    std::array<std::string, sizeof...(I)> arg_arr { format_arg(std::get<I>(args))... };
    for (std::string str : arg_arr)
        arg_str.append(str + ", ");
    // remove final space and comma
    return arg_str.substr(0, arg_str.size() - 2);
}


template < class T >
static inline std::string addr_to_str(T* v_ptr)
{
    std::stringstream ss;
    ss << (void*)v_ptr;
    return ss.str();
}


/**
* @param arg  argument to return a formating map
* @return a map containing formatting params as keys, and their respective values based on properties of `arg`
*/
template < class Traits >
template < class T >
std::string Formatter<Traits>::format_arg(const T& arg)
{
    const fmt_args_t fmt_args
    {
        { "type"    , type_name<T>()    },
        { "value"   , try_str_cvt(&arg) },
        { "address" , addr_to_str(&arg) },
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
