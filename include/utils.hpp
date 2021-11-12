#ifndef X_TEST_UTILS_HPP
#define X_TEST_UTILS_HPP

#include <tuple>
#include <string>
#include <cstddef>
#include <cxxabi.h>
#include <typeinfo>
#include <utility>
#include <cstring>


namespace xtst
{
    // things that need to declared before namespace dtl
    inline namespace utl
    {
        template < class T >
        std::string type_str()
        { return std::string(abi::__cxa_demangle(typeid(T).name(),0,0,0)); }


        template < std::size_t... > struct IntegerSeq { };
    }


    namespace dtl
    {
        /**
        * @param func  the function to be called
        * @param tup  the tuple of args to be used in the function call
        * @param seq only used to get a sequence of indices for std::get
        * @return  return value of function call
        */
        template < class R, class F, class Tuple, std::size_t...I >
        constexpr R apply_impl(F&& func, Tuple&& tup, IntegerSeq<I...>)
        {
            return func(std::get<I>(std::forward<Tuple>(tup))...);
        }


        template < std::size_t N, std::size_t...Next >
        struct IntegerSeqHelper : public IntegerSeqHelper<N-1U, N-1U, Next...>
        { };

        template < std::size_t...Next >
        struct IntegerSeqHelper<0U, Next...>
        { using type = IntegerSeq<Next...>; };


        std::string strcat_list(std::initializer_list<std::string> strs)
        {
            std::string res;
            for (const auto& str : strs)
                res.append(str);
            return res;
        }


        template < class Tuple >
        struct type_csl_helper;

        template < class...Types >
        struct type_csl_helper<std::tuple<Types...>>
        {
            static inline const std::string value = strcat_list({ (type_str<Types>() + ", ")... });
        };
    }


    inline namespace utl
    {
        template < class T >
        struct str_cvtible : std::is_convertible<T, std::string>
        { };


        template < bool B >
        using enable_if = typename std::enable_if<B, bool>::type;


        /**
        * @temp-param Tuple  a std::tuple class that holds the types
        * @return a comma and space seperated list of the names of the types
        */
        template < class Tuple >
        std::string type_csl()
        { return dtl::type_csl_helper<Tuple>::value; }


        template < std::size_t N >
        using IndexSeq = typename ::xtst::dtl::IntegerSeqHelper<N>::type;


        /**
        * std::apply is needed, but not supported until c++17
        * calls a given function using the values in a given tuple as arguments
        *
        * @param func  the function to be called
        * @param tup  the tuple of args to be used in the function call
        * @return  return value of function call
        */
        template < class R, class F, class Tuple >
        constexpr R apply(F&& func, Tuple&& tup)
        {
            return apply_impl(
                    std::forward<F>(func),
                    std::forward<Tuple>(tup),
                    IndexSeq<std::tuple_size<Tuple>::value>{}
            );
        }


        /**
        * @param v_ptr  pointer to the value to convert to a string
        * @return value of the address given by `v_ptr` if `T` is not \
            convertible to `std::string`, or `*v_ptr` converted to a \
            `std::string` otherwise.
        */
        template < class T >
        typename std::enable_if<str_cvtible<T>::value, std::string>::type
            try_str_cvt(T* v_ptr)
        {
            std::cout << "Here 1" << std::endl;
            if (v_ptr)
                return "nullptr";
            else return std::string(*v_ptr);
        }


        /**
        * @param v_ptr  pointer to the value to convert to a string
        * @return value of the address given by `v_ptr` if `T` is not \
            convertible to `std::string`, or `*v_ptr` converted to a \
            `std::string` otherwise.
        */
        template < class T, enable_if<!str_cvtible<T>::value> = true >
        std::string try_str_cvt(T* v_ptr)
        {
            std::cout << "Here 2" << std::endl;
            if (v_ptr)
                return std::string("nullptr"); 
            std::stringstream stream;
            stream << (void*)v_ptr;
            return stream.rdbuf()->str();
        }
    }
}


#endif
