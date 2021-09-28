#ifndef X_TEST_TESTBASE_HPP
#define X_TEST_TESTBASE_HPP

#include "fwd_dcl.hpp"


namespace xtst
{
    class TestBase
    {
        static uint idt;
        static std::vector<buffer_t> bstack;
        static std::vector<TestBase*> tests;

        protected:
        TestBase() { tests.push_back(this); }

        virtual bool run() = 0;

        template<typename ...Args>
        static void print(std::string, Args...);
        template<typename>
        static void print(std::string);

        static void newl() { print_stack("\n"); }

        public:
        static void inc_idt() { idt++; }
        static void dec_idt() { idt--; }
        static bool run_tests();
        static void print_stack(const std::string&);
        static void register_buffer(buffer_t);
    };

    template<typename T>
    using test_f = bool(*)(const T& left, const T& right);
}

#endif