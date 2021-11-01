// #include "Formatter.hpp"
#include <cxxabi.h>
#include <regex>

using demangle = abi::__cxa_demangle;



using namespace xtst;

template<class Traits>
Formatter<Traits>::Formatter(const string_t& fmt)
    : format(fmt) { }

template<class Traits>
Formatter<Traits>::Formatter(const string_t& fmt, const string_t& pattern)
    : Formatter(fmt), arg_pattern(pattern) { }

template<class Traits>
string_t Formatter<Traits>::fmt_arg(const ArgInfo& info) const
{
    std::size_t start = 0;
    std::size_t end = 0;
    std::size_t offset = 0;
    string_t curr_arg;
    string_t param;
    string_t result = arg_pattern;
    const arg_map_t map {
        { "type" , info.type  },
        { "name" , info.name  },
        { "value", info.value },
    };

    // TODO: consider regex, but this is pretty good already
    while (end < arg_pattern.size())
    {
        start = arg_pattern.find("${", end);
        end = arg_pattern.find('}', start);
        param = arg_pattern.substr(start + 2, end + 1);
        curr_arg = map.find(param);
        result.replace(start + offset, param.size(), curr_arg);
        offset += curr_arg.size() - param.size();
    }
    return result;
}

template<class Traits>
string_t Formatter<Traits>::fmt_param(string_t& refer, string_t& msg, Seek& p_seek) const
{
    Seek sk { p_seek.start, p_seek.start, 0 };
    const ins_idx = start - 2;
    string_t ansi_str;
    string_t param;
    string_t opt_str;

    while (end < p_seek.end)
    {
        start = refer.find(':', end);
        end = refer.find_first_of(":}", start);
        if (start == string_t::npos || end == string_t::npos)
            break;
        sk.curr = refer.substr(start, end - start);
        // god, i hate this.
        switch (std::hash(sk.curr))
        {
            case ("end"_hash):            opt_str += ansi_format(AnsiOption::end           ); break;
            case ("reset"_hash):          opt_str += ansi_format(AnsiOption::reset         ); break;
            case ("bold_on"_hash):        opt_str += ansi_format(AnsiOption::bold_on       ); break;
            case ("faint"_hash):          opt_str += ansi_format(AnsiOption::faint         ); break;
            case ("italic"_hash):         opt_str += ansi_format(AnsiOption::italic        ); break;
            case ("uline"_hash):          opt_str += ansi_format(AnsiOption::uline         ); break;
            case ("slow_blink"_hash):     opt_str += ansi_format(AnsiOption::slow_blink    ); break;
            case ("rapid_blink"_hash):    opt_str += ansi_format(AnsiOption::rapid_blink   ); break;
            case ("invert"_hash):         opt_str += ansi_format(AnsiOption::invert        ); break;
            case ("hide"_hash):           opt_str += ansi_format(AnsiOption::hide          ); break;
            case ("strike"_hash):         opt_str += ansi_format(AnsiOption::strike        ); break;
            case ("def_font"_hash):       opt_str += ansi_format(AnsiOption::def_font      ); break;
            case ("gothic"_hash):         opt_str += ansi_format(AnsiOption::gothic        ); break;
            case ("dbl_uline"_hash):      opt_str += ansi_format(AnsiOption::dbl_uline     ); break;
            case ("norm_intensity"_hash): opt_str += ansi_format(AnsiOption::norm_intensity); break;
            case ("no_italic"_hash):      opt_str += ansi_format(AnsiOption::no_italic     ); break;
            case ("no_uline"_hash):       opt_str += ansi_format(AnsiOption::no_uline      ); break;
            case ("no_blink"_hash):       opt_str += ansi_format(AnsiOption::no_blink      ); break;
            case ("not_reversed"_hash):   opt_str += ansi_format(AnsiOption::not_reversed  ); break;
            case ("reveal"_hash):         opt_str += ansi_format(AnsiOption::reveal        ); break;
            case ("no_strike"_hash):      opt_str += ansi_format(AnsiOption::no_strike     ); break;
            case ("fg_black"_hash):       opt_str += ansi_format(AnsiOption::fg_black      ); break;
            case ("fg_red"_hash):         opt_str += ansi_format(AnsiOption::fg_red        ); break;
            case ("fg_green"_hash):       opt_str += ansi_format(AnsiOption::fg_green      ); break;
            case ("fg_yellow"_hash):      opt_str += ansi_format(AnsiOption::fg_yellow     ); break;
            case ("fg_blue"_hash):        opt_str += ansi_format(AnsiOption::fg_blue       ); break;
            case ("fg_magenta"_hash):     opt_str += ansi_format(AnsiOption::fg_magenta    ); break;
            case ("fg_cyan"_hash):        opt_str += ansi_format(AnsiOption::fg_cyan       ); break;
            case ("fg_white"_hash):       opt_str += ansi_format(AnsiOption::fg_white      ); break;
            case ("def_fg_clr"_hash):     opt_str += ansi_format(AnsiOption::def_fg_clr    ); break;
            case ("bg_black"_hash):       opt_str += ansi_format(AnsiOption::bg_black      ); break;
            case ("bg_red"_hash):         opt_str += ansi_format(AnsiOption::bg_red        ); break;
            case ("bg_green"_hash):       opt_str += ansi_format(AnsiOption::bg_green      ); break;
            case ("bg_yellow"_hash):      opt_str += ansi_format(AnsiOption::bg_yellow     ); break;
            case ("bg_blue"_hash):        opt_str += ansi_format(AnsiOption::bg_blue       ); break;
            case ("bg_magenta"_hash):     opt_str += ansi_format(AnsiOption::bg_magenta    ); break;
            case ("bg_cyan"_hash):        opt_str += ansi_format(AnsiOption::bg_cyan       ); break;
            case ("bg_white"_hash):       opt_str += ansi_format(AnsiOption::bg_white      ); break;
            case ("def_bg_clr"_hash):     opt_str += ansi_format(AnsiOption::def_bg_clr    ); break;
            case ("oline"_hash):          opt_str += ansi_format(AnsiOption::oline         ); break;
            case ("no_oline"_hash):       opt_str += ansi_format(AnsiOption::no_oline      ); break;
            case ("super"_hash):          opt_str += ansi_format(AnsiOption::super         ); break;
            case ("sub"_hash):            opt_str += ansi_format(AnsiOption::sub           ); break;
            case ("no_sup_or_sub"_hash):  opt_str += ansi_format(AnsiOption::no_sup_or_sub ); break;
            default: throw std::invalid_argument(opt);
        }
        if (msg.at(end) == '}')
            break;
    }
    ansi_str = ansi_format(opt_str);
    p_seek.offset += ansi_str.size();
    param = refer.substr(start, p_seek.end - 1);
    msg.insert(ins_idx, ansi_str);
    return param;
}

template<class Traits>
string_t Formatter<Traits>::format(bool test_passed, const string_t& return_str, ilist<string_t> _args) const
{
    if (_args.size() > args.size())
        throw std::out_of_range();
    const unsigned short argc = args.size();
    Seek seek;
    string_t param;
    string_t result = format;
    arg_map_t arg_map {{ "return", return_str }};

    for (unsigned short i = 0; i < _args.size(); i++)
    {
        set_arg(_args.at(i), i);
        arg_map.insert({ info.name, fmt_arg(info) });
    }
    // set the rest of the args' values to DEFAULT_ARG_VALUE
    for (unsigned short i = _args.size(); i < args.size(); i++)
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
                sk.curr = args.at(i).name = args.at(i);
                result.insert(sk.start + sk.offset, arg_map.at(sk.curr));
                sk.offset += sk.curr.size();
            }
            continue;
        }
        else if (param == "result")
        {
            if (test_passed)
                sk.curr = ansi_format(PASS_MESSAGE, pass_clr);
            else sk.curr = ansi_format(FAIL_MESSAGE, fail_clr);
        }
        else sk.curr = arg_map.at(param);
        result.replace(sk.start + sk.offset, param.size(), sk.curr);
        sk.offset += sk.curr.size() - param.size();
    }
    return result;
}

template<class Traits>
template<class R, class... Args, bool>
string_t Formatter<Traits>::format(bool test_passed, const R& return_v, const Args&... _args) const
    { return format(test_passed, string_t(return_v), { string_t(_args)... }); }

template<class Traits>
void Formatter<Traits>::add_args(const ArgInfo& info)
    { args.emplace_back(info); }

template<class Traits>
void Formatter<Traits>::add_args(ilist<ArgInfo> ai_list)
{
    for (const ArgInfo& info : ai_list)
        add_args(info);
}

template<class Traits>
template<class T>
void Formatter<Traits>::add_args(const T& curr_arg, const string_t& name = DEFAULT_ARG_NAME)
{
    int status;
    string_t arg_val;
    string_t type_name = demangle(typeid(curr_arg).name(),0,0,&status);
    if IF_CE (std::is_convertible<const T&, string_t>::value)
        arg_val = (string_t)curr_arg;
    else arg_val = DEFAULT_ARG_VALUE;
    args.emplace_back(type_name, arg_name, arg_val);
}

template<class Traits>
template<class T, bool>
void Formatter<Traits>::set_arg(const T& curr_arg, const string_t& name)
{
    for (ArgInfo& info : args)
    {
        if (info.name == name)
        {
            info.value = (string_t)curr_arg;
            return;
        }
    }
}

template<class Traits>
template<class T, bool>
void Formatter<Traits>::set_arg(const T& curr_arg, std::size_t pos)
{
    ArgInfo& info = args.at(pos);
    info.value = (string_t)curr_arg;
}

template<class Traits>
template<class T>
ArgInfo make_arg_info(const string_t& name)
{
    string_t type_name = demangle(typeid(curr_arg).name(),0,0,&status);
    return { type_name, name, DEFAULT_ARG_VALUE };
}

template<class Traits>
template<class R, class...A>
Formatter make_formatter(const string_t& fmt, const string_t& ptrn, R (*func)(A...))
{
    Formatter fmtr(fmt, ptrn);
    fmtr.add_args(A);
}

template<class Traits>
template<class R, class...A>
Formatter make_formatter(const string_t& fmt, R (*func)(A...))
{
    Formatter fmtr(fmt);
    fmtr.add_args({ make_arg_info<A>()... });
}
