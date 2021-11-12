#ifndef X_TEST_UTILS_HPP
#define X_TEST_UTILS_HPP

#include <tuple>
#include <string>
#include <cstddef>
#include <typeinfo>
#include <utility>
#include <cstring>
#include <iostream>


#ifdef __GNUG__
#include <cstdlib>
#include <memory>
#include <cxxabi.h>

namespace xtst
{
    template < class T >
    std::string type_str()
    {
        std::cout << "begin type_str" << std::endl;
        int status = 1; // all status results from __cxa_demangle are 0 or negative
        const char* typeid_name = typeid(T).name();
        std::string name;

        auto demangle = [](const char* nm, int& sts) -> char*
            { return abi::__cxa_demangle(nm, NULL, NULL, &sts); };

        std::unique_ptr<char[]> name_ptr(demangle(typeid_name, status));

        if (status == 0) // 0 means success
            name = name_ptr.get();
        else name = typeid_name;

        std::cout << "end type_str" << std::endl;
        return name;
    }
}

#else

namespace xtst
{
    // does nothing if not g++
    template < class T >
    std::string type_str()
    {
        std::cout << "in type_str" << std::endl;
        return typeid(T).name();
    }
}

#endif


namespace xtst
{
    template < std::size_t...I > struct IntegerSeq { };


    namespace dtl
    {
        template < std::size_t N, std::size_t...Next >
        struct IntegerSeqHelper : public IntegerSeqHelper<N-1U, N-1U, Next...>
        { };

        template < std::size_t...Next >
        struct IntegerSeqHelper<0U, Next...>
        { using type = IntegerSeq<Next...>; };


        std::string str_list_cat(std::initializer_list<std::string> strs)
        {
            std::cout << "begin str_list_cat" << std::endl;
            std::string res;
            for (const auto& str : strs)
                res.append(str);
            std::cout << "end str_list_cat" << std::endl;
            return res;
        }


        template < class Tuple >
        struct type_csl_helper;

        template < class...Types >
        struct type_csl_helper<std::tuple<Types...>>
        {
            static inline const std::string value = str_list_cat({ (type_str<Types>() + ", ")... });
        };
    }


    template < class T >
    using str_cvtible = std::is_convertible<T, std::string>;


    template < bool B >
    using enable_if = typename std::enable_if<B, bool>::type;


    /**
    * converts a tuple of types to a comma seperated list of their respective names
    *
    * @temp-param Tuple  a std::tuple class that holds the types
    * @return a comma and space seperated list of the names of the types
    */
    template < class Tuple >
    std::string type_csl()
    {
        return dtl::type_csl_helper<Tuple>::value;
    }


    template < std::size_t N >
    using IndexSeq = typename ::xtst::dtl::IntegerSeqHelper<N>::type;


    /**
    * attempts to convert the value pointed to by the given pointer
    *
    * @param ptr  pointer to the value to convert to a string
    * @return value of the address given by `ptr` if `T` is not \
        convertible to `std::string`, or `*ptr` converted to a \
        `std::string` otherwise.
    */
    template < class T, enable_if<str_cvtible<T>::value> = true >
    std::string try_str_cvt(T* ptr)
    {
        std::cout << "in try_str_cvt 1" << std::endl;
        if (ptr)
            return "nullptr";
        else return std::string(*ptr);
    }


    /**
    * attempts to convert the value pointed to by the given pointer
    *
    * @param ptr  pointer to the value to convert to a string
    * @return value of the address given by `ptr` if `T` is not \
        convertible to `std::string`, or `*ptr` converted to a \
        `std::string` otherwise.
    */
    template < class T, enable_if<!str_cvtible<T>::value> = true >
    std::string try_str_cvt(T* ptr)
    {
        std::cout << "begin try_str_cvt 2" << std::endl;
        if (ptr)
        {
            std::cout << "end try_str_cvt 2" << std::endl;
            return "nullptr";
        }
        std::stringstream stream;
        stream << (void*)ptr;
        std::cout << "end try_str_cvt 2" << std::endl;
        return stream.str();
    }
}


#endif
