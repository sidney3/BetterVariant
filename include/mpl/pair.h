#pragma once

/*
Compile time pair of values
*/
namespace mpl
{
template<typename T, typename U>
struct pair
{
    using first = T;
    using second = U;
};
}
