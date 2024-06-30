#pragma once
#include <mpl/list_traits.h>
#include <type_traits>
#include <tuple>

using namespace mpl;
using std::tuple;

/*
equals (done first as this is used in other tests)
*/

using v1 = tuple<int,double,float>;
using v2 = tuple<int,double,float>;
using v3 = tuple<int,double,float,int>;

static_assert(list_traits<tuple>::equals<v1, v2>::value);
static_assert(list_traits<tuple>::equals<v1, v1>::value);
static_assert(!list_traits<tuple>::equals<v1, v3>::value);

/*
Set equals
*/

using w1 = tuple<int, double, double, float>;
using w2 = tuple<float, int, int, double>;
using w3 = tuple<float, double>;

static_assert(list_traits<tuple>::set_equals<w1,w2>::value);
static_assert(!list_traits<tuple>::set_equals<w1,w3>::value);
static_assert(!list_traits<tuple>::set_equals<w2,w3>::value);

/*
Test append
*/

using v1 = tuple<int,double,float>;
using v3 = tuple<int,double,float,int>;
static_assert(list_traits<tuple>::equals
        <list_traits<tuple>::push_back<v1, int>::type, 
        v3>::value);

/*
transform
*/
template<typename Integer>
struct AddOne
{
    using type = std::integral_constant<int, Integer::value + 1>;
};

template<int N>
using bint = std::integral_constant<int, N>;

using nums = tuple<bint<1>, bint<2>, bint<3>>;
using expected_transform = tuple<bint<2>, bint<3>, bint<4>>;
static_assert(list_traits<tuple>::equals<
        expected_transform, 
        list_traits<tuple>::transform<nums,AddOne>::type>::value);

/*
Enumerate
*/

using v1 = tuple<int,double, float>;
using expected_enumerate = tuple<
        pair<int, std::integral_constant<long,0>>,
        pair<double, std::integral_constant<long,1>>,
        pair<float, std::integral_constant<long,2>>
>;

static_assert(list_traits<tuple>::equals<
       list_traits<tuple>::enumerate<v1>::type,
       expected_enumerate
                >::value);

/*

*/
