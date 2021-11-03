#include "../include/Formatter.hpp"
#include "../include/utils.hpp"
#include "../include/macrodef.hpp"
#include <unordered_map>

using namespace xtst;

/**
* @param test_passed  whether or not the test being formatted passed
* @param exp  value that was expected from the test case, or nullptr if an error was expected
* @param rtn  return value of the tested case
* @param args  arguments used in the tested case
* @return  a formatted string
*/
template<class Tr>
std::string Formatter<Tr>::Format( bool test_passed, std::shared_ptr<return_type> exp, const return_type& rtn, const arg_types& args )
{
    std::size_t pos = 0;
    std::size_t offset = 0;
    std::string fstr = format; // formatted string
    std::string param;
    std::string arg;
    std::smatch match;
    auto _args = get_args();
    auto fmtargs = _args.first;
    auto args_arr = _args.first;
    std::regex_search(format, match, pattern);

    for (int i = 0; i < match.size(); i++)
    {
        pos = match.position(i);
        param = match.str(i);
        if (fmtargs.count(param) != 0)
        {
            arg = fmtargs.at(param);
            fstr.replace(pos + offset - 2, param.size() + 3, arg);
            offset += param.size() - arg.size() + 3;
        }
    }
}

/**
* @param rtn  return value of the tested case
* @param args  arguments used in the tested case
* @return  a pair consisting of a map with format arguments as keys and an array of string representations of args, or "unavailable" if the arg is not convertible to a string
*/
template < class Tr >
std::pair<string_map_t, std::array<std::string, Tr::argc>> Formatter<Tr>::get_args(const return_type& rtn, const arg_types& args) const
{
    std::string rtn_str = if_cvt(rtn, "unavailable");
    std::string rtn_t_str = utl::type_name<Tr::return_type>();
    std::string sig_str = rtn_t_str+"(*)("+type_name_csl<arg_types>()+")";
    auto arg_strs = utl::to_array_of(args, std::string("unavailable"));
    return{{
        {"return"     , rtn_str  },
        {"return-type", rtn_t_str},
        {"signature"  , sig_str  },
    }, arg_strs };
}


















{
    for (unsigned short i = 0; i < args.size(); i++)
    {
        set_arg(args.at(i), i);
        fmtargs.insert({ info.name, fmt_arg(info) });
    }
    // set the rest of the args' values to DEFAULT_ARG_VALUE
    for (unsigned short i = args.size(); i < args.size(); i++)
        args.at(i).value = DEFAULT_ARG_VALUE;

    while (sk.end < format.size())
    {
        sk.start = format.find("${", sk.end);
        sk.end = format.find('}', sk.start);
        param = fmt_param(format, result, seek);
        if (param == "args")
        {
            result.erase(sk.start + sk.offset, sk.start - sk.end);
            for (unsigned short i = 0; i < argc; i++)
            {
                sk.param = args.at(i).name = args.at(i);
                result.insert(sk.start + sk.offset, fmtargs.at(sk.param));
                sk.offset += sk.param.size();
            }
            continue;
        }
        else if (param == "result")
        {
            if (test_passed)
                sk.param = ansi_format(PASS_MESSAGE, pass_clr);
            else sk.param = ansi_format(FAIL_MESSAGE, fail_clr);
        }
        else sk.param = fmtargs.at(param);
        result.replace(sk.start + sk.offset, param.size(), sk.param);
        sk.offset += sk.param.size() - param.size();
    }
    return result;
}
