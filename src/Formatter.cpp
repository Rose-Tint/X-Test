#include "../include/Formatter.hpp"
#include "../include/utils.hpp"
#include "../include/macrodef.hpp"
#include <unordered_map>
#include <sstream>

using namespace xtst;

/**
* @param test_passed  whether or not the test being formatted passed
* @param exp  value that was expected from the test case, or nullptr if an error was expected
* @param rtn  return value of the tested case
* @param args  arguments used in the tested case
* @return  a formatted string
*/
template<class Traits>
std::string Formatter<Traits>::Format( bool test_passed,
                                       std::shared_ptr<return_type> exp,
                                       const return_type* rtn,
                                       const arg_types& args )
{
    std::size_t pos = 0, offset = 0;
    std::string fstr = format, param, arg;
    std::smatch match;
    auto _args = get_args();
    auto fmtargs = _args.first;
    auto args_arr = _args.second;
    std::regex_search(format, match, pattern);

    for (int i = 0; i < match.size(); i++)
    {
        pos = match.position(i) + offset;
        param = match.str(i);
        if (fmtargs.count(param) != 0)
        {
            arg = fmtargs.at(param);
            // the `- 2` is `pos - 2` accounts for `${`,
            // and the `- 3` in `param.size() - 3` accounts for all three chars (`${}`).
            fstr.replace(pos - 2, param.size() + 3, arg);
            offset += param.size() - arg.size() + 3;
            // the best way to explain the above line is through a visual example:
            // the format is "foobar ${ex-param} quuxbaz", and'some-param' maps to "example argument"
            // replace from          ^ to      ^ with
            //                       example argument
            // fstring will be offset by       ^    ^ (5), which is the difference between the
            // size of the parameter (plus 3 to account for the `${}`), and the size of the arg
        }
    }
}

/**
* @param rtn  return value of the tested case
* @param args  arguments used in the tested case
* @return  a pair consisting of a map with format args as keys,
*     and an array of string values of args if possible, else "unavailable"
*/
template < class Traits >
fmt_args_t<Traits::argc> Formatter<Traits>::get_args(const return_type* rtn, const arg_types& args)
{
    std::string rtn_str = try_str_cvt(rtn);
    std::string rtn_t_str = utl::type_name<Traits::return_type>();
    std::string sig_str = rtn_t_str+"(*)("+type_name_csl<arg_types>()+")";
    auto arg_str_array = to_array_of(args, std::string("unavailable"));
    return{{
        {"return"     , rtn_str  },
        {"return-type", rtn_t_str},
        {"signature"  , sig_str  },
    }, arg_str_array };
}

/**
* @param v_ptr  pointer to the value to convert to a string
* @return  the given value converted to a string, or the address if the deduced type cannot be
*     converted to a string, or "nullptr" if the pointer is `nullptr`
*/
template < class Traits >
template < class T >
std::string Formatter<Traits>::try_str_cvt( T* v_ptr )
{
    if (v_ptr == nullptr)
        return "nullptr";
    std::stringstream ss;
    ss << (const void*)v_ptr;
    return ss.str();
}
