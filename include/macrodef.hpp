#ifndef X_TEST_MACRODEF_HPP
#  define X_TEST_MACRODEF_HPP

#  ifndef CPP_STD
#    if __cplusplus == 202002L
#      define CPP_STD 20
#    elif __cplusplus == 201703L
#      define CPP_STD 17
#    elif __cplusplus == 201402L
#      define CPP_STD 14
#    elif __cplusplus == 201103L
#      define CPP_STD 11
#    else
#      error "X-Test: Standard must be c++11 or later"
#  endif

// if `if constexpr` can be used, define if_constexpr as
// constexpr, else define as empty. Where it would be
// necessary for the if to be constexpr, do not use this
// macro.
#  ifndef if_constexpr
#    if CPP_STD >= 17
#      define if_constexpr if constexpr
#    else
#      define if_constexpr if
#    endif
#  endif

#  ifndef PASS_MESSAGE
#    define PASS_MESSAGE "Pass"
#  endif
#  ifndef FAIL_MESSAGE
#    define FAIL_MESSAGE "Fail"
#  endif
#  ifndef DEFAULT_ARG_NAME
#    define DEFAULT_ARG_NAME "N/A"
#  endif
#  ifndef DEFAULT_ARG_VALUE
#    define DEFAULT_ARG_VALUE "unavailable"
#  endif


#endif