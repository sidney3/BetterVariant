#pragma once
#include <mpl/types.h>

/*
Compile time pair of values
*/
namespace mpl
{
template<typename T, typename U>
struct pair
{
    using tag = pair_tag;
    using first = T;
    using second = U;
};
}
