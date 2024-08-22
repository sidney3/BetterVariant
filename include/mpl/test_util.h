#pragma once

#include <mpl/list.h>
#include <mpl/util.h>

using namespace mpl;

/*
Is_unique
*/
static_assert(is_unique<int, double, float>::value);
static_assert(!is_unique<int, int>::value);
static_assert(!is_unique<int, double, int>::value);
static_assert(!is_unique<int, int, int>::value);

/*
Indexof
*/

static_assert(index_of<double, int, double, float>::value == 1);
static_assert(index_of<int, int, double, float>::value == 0);
static_assert(index_of<float, int, double, float>::value == 2);

/*
Contains
*/

static_assert(mpl::contains<int, int, double, float>::value);
static_assert(!mpl::contains<char, int, double, float>::value);
static_assert(!mpl::contains<char, char *>::value);
static_assert(mpl::contains<char, char>::value);
static_assert(!mpl::contains<char>::value);

/*
Find if
*/

template <long N> using nint = std::integral_constant<long, N>;
template <typename Int> struct is_even {
  static constexpr bool value = Int::value % 2 == 0;
};

static_assert(std::is_same_v<
              mpl::find_if<is_even, nint<1>, nint<3>, nint<8>, nint<9>>::type,
              nint<8>>);

static_assert(
    std::is_same_v<mpl::find_if<is_even, nint<1>, nint<9>, nint<11>>::type,
                   types::type_not_found>);
