#pragma once

#include <mpl/function_traits.h>
#include <mpl/list.h>
#include <mpl/list_traits.h>

namespace {

using lst = mpl::list_traits<mpl::list>;

auto fn1 = [](int, double, const int) -> int * { return nullptr; };

using fn1_traits = mpl::function_traits<decltype(fn1)>;

static_assert(std::is_same_v<fn1_traits::return_type, int *>);
static_assert(lst::equals<fn1_traits::arg_type,
                          mpl::list<int, double, const int>>::value);

struct fn2 {
  static bool operator()(const char *, float) { return false; }
};

using fn2_traits = mpl::function_traits<fn2>;

static_assert(std::is_same_v<fn2_traits::return_type, bool>);
static_assert(lst::equals<fn1_traits::arg_type,
                          mpl::list<int, double, const int>>::value);

} // namespace
