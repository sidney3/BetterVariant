#pragma once
#include <mpl/map.h>
#include <mpl/pair.h>
#include <type_traits>
#include <mpl/types.h>
#include <string>

using namespace mpl;

template<long N>
using nint = std::integral_constant<long, N>;

using mp = map::map<
            pair<int, nint<4>>, 
            pair<double, nint<5>>,
            pair<std::string, nint<9>>>;
/*****
AT
*****/

static_assert(std::is_same_v<map::at<mp,int>::type, nint<4>>);
static_assert(std::is_same_v<map::at<mp,double>::type, nint<5>>);
static_assert(std::is_same_v<map::at<mp,std::string>::type, nint<9>>);
static_assert(std::is_same_v<map::at<mp,char>::type, types::type_not_found>);

/*****
CONTAINS
*****/

static_assert(map::contains<mp, int>::value);
static_assert(map::contains<mp, double>::value);
static_assert(map::contains<mp, std::string>::value);
static_assert(!map::contains<mp, float>::value);
static_assert(!map::contains<mp, long>::value);
static_assert(!map::contains<mp, char>::value);
static_assert(!map::contains<mp, int*>::value);

/*****
INSERT
*****/

using mp2 = map::insert<mp, char, nint<10>>::type;
static_assert(std::is_same_v<map::at<mp2, char>::type, nint<10>>);


using mp3 = map::map<pair<int,int>, pair<int,double>>;
