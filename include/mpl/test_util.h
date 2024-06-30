#pragma once

#include <mpl/util.h>
#include <mpl/list.h>

using namespace mpl;

/*
Enumerate
*/

/*
Is_unique
*/
static_assert(is_unique<int,double,float>::value);
static_assert(!is_unique<int,int>::value);
static_assert(!is_unique<int,double,int>::value);
static_assert(!is_unique<int,int,int>::value);

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
static_assert(!mpl::contains<char, char*>::value);
static_assert(mpl::contains<char, char>::value);
static_assert(!mpl::contains<char>::value);
