#pragma once

#include <mpl/functional.h>

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

static_assert(is_unary_auto<decltype(auto1)>::value);
static_assert(!is_unary_auto<decltype(not_auto)>::value);


/*
Unary arg
*/
auto fn1 = [](int)
{};
auto fn2 = [](int&)
{};
auto fn3 = [](const int*)
{};
auto fn4 = [](const volatile int&)
{};

static_assert(std::is_same_v<
        unary_arg<decltype(fn1)>::type,
        int>);
static_assert(std::is_same_v<
        unary_arg<decltype(fn2)>::type,
        int&>);
static_assert(std::is_same_v<
        unary_arg<decltype(fn2)>::type,
        int&>);
static_assert(std::is_same_v<
        unary_arg<decltype(fn3)>::type,
        const int*>);
static_assert(std::is_same_v<
        unary_arg<decltype(fn4)>::type,
        const volatile int&>);

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
