#pragma once
#include <mpl/list.h>
#include <mpl/list_traits.h>
#include <tuple>
#include <type_traits>

using namespace mpl;
using std::tuple;

/*
equals (done first as this is used in other tests)
*/

namespace equals_test {
using v1 = tuple<int, double, float>;
using v2 = tuple<int, double, float>;
using v3 = tuple<int, double, float, int>;

static_assert(list_traits<tuple>::equals<v1, v2>::value);
static_assert(list_traits<tuple>::equals<v1, v1>::value);
static_assert(!list_traits<tuple>::equals<v1, v3>::value);
} // namespace equals_test

/*
Set equals
*/
namespace set_equals_test {

using w1 = tuple<int, double, double, float>;
using w2 = tuple<float, int, int, double>;
using w3 = tuple<float, double>;

static_assert(list_traits<tuple>::set_equals<w1, w2>::value);
static_assert(!list_traits<tuple>::set_equals<w1, w3>::value);
static_assert(!list_traits<tuple>::set_equals<w2, w3>::value);
} // namespace set_equals_test

namespace append_test {
/*
append
*/

using v1 = tuple<int, double, float>;
using v3 = tuple<int, double, float, int>;
static_assert(list_traits<tuple>::equals<
              list_traits<tuple>::push_back<v1, int>::type, v3>::value);

} // namespace append_test

namespace transform_test {
/*
transform
*/
template <typename Integer> struct AddOne {
  using type = std::integral_constant<int, Integer::value + 1>;
};

template <int N> using bint = std::integral_constant<int, N>;

using nums = tuple<bint<1>, bint<2>, bint<3>>;
using expected_transform = tuple<bint<2>, bint<3>, bint<4>>;
static_assert(list_traits<tuple>::equals<
              expected_transform,
              list_traits<tuple>::transform<nums, AddOne>::type>::value);

} // namespace transform_test

namespace enumerate_test {
/*
Enumerate
*/

using v1 = tuple<int, double, float>;
using expected_enumerate = tuple<pair<int, std::integral_constant<long, 0>>,
                                 pair<double, std::integral_constant<long, 1>>,
                                 pair<float, std::integral_constant<long, 2>>>;

static_assert(
    list_traits<tuple>::equals<list_traits<tuple>::enumerate<v1>::type,
                               expected_enumerate>::value);
} // namespace enumerate_test

namespace prefix_predicate_test {
/*
prefix_predicate

*/

using l1 = list<int, int, double, float>;
using l2 = list<int, int, double>;
using l3 = list<int, double, float>;

static_assert(list_traits<list>::prefix_predicate<std::is_same, l2, l1>::value);
static_assert(
    !list_traits<list>::prefix_predicate<std::is_same, l1, l2>::value);
static_assert(list_traits<list>::prefix_predicate<std::is_same, l1, l1>::value);
static_assert(
    !list_traits<list>::prefix_predicate<std::is_same, l3, l1>::value);
static_assert(
    list_traits<list>::prefix_predicate<std::is_same, list<>, l1>::value);
} // namespace prefix_predicate_test

/*
All of
*/

namespace all_of_test {

using lst = mpl::list_traits<mpl::list>;

using l1 = list<int, int, int>;
using l2 = list<int, int, int, char>;
using l3 = list<char, int, int, int>;

template <typename T> using is_int = std::is_same<T, int>;

static_assert(lst::all_of<l1, is_int>::value);
static_assert(!lst::all_of<l2, is_int>::value);
static_assert(lst::all_of<mpl::list<>, is_int>::value);
static_assert(!lst::all_of<l3, is_int>::value);

} // namespace all_of_test
