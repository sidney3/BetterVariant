#pragma once

#include <mpl/unary_function_traits.h>

using namespace mpl;

namespace {
/*
Is auto
*/
auto auto1 = [](auto)
{};
// Your function
auto not_auto = [](int) 
{};

static_assert(!is_unary_auto<decltype(not_auto)>::value);
static_assert(is_unary_auto<decltype(auto1)>::value);


/*
Unary arg traits for non-auto
*/
auto fn1 = [](int) -> int*
{
        return nullptr;
};
auto fn2 = [](int&)
{
        return 2;
};
auto fn3 = [](const int*)
{
        return 'a';
};
auto fn4 = [](const volatile int&)
{
        return 'a';
};
auto fn5 = [](auto&&)
{
        return 0;
};

static_assert(std::is_same_v<
        unary_traits<decltype(fn1)>::arg_type,
        int>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn1)>::return_type,
        int*>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn2)>::arg_type,
        int&>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn2)>::return_type,
        int>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn2)>::return_type,
        int>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn3)>::arg_type,
        const int*>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn4)>::arg_type,
        const volatile int&>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn5)>::arg_type,
        types::auto_arg>);
static_assert(std::is_same_v<
        unary_traits<decltype(fn5)>::return_type,
        int>);

/*
Is exact invocable
*/
auto foo = [](int)
{};
using f = decltype(foo);

static_assert(is_exact_invocable<f, int>::value);
// we want to avoid implicit conversion
static_assert(!is_exact_invocable<f, char>::value);
static_assert(is_exact_invocable<f, const int>::value);

auto bar = [](const int&)
{};
using b = decltype(bar);
static_assert(is_exact_invocable<b, int>::value);
static_assert(is_exact_invocable<b, int&>::value);
static_assert(is_exact_invocable<b, int&&>::value);
static_assert(!is_exact_invocable<b, double>::value);




}
