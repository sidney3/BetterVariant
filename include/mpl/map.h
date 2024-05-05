#pragma once
#include <mpl/types.h>
#include <mpl/pair.h>
#include <mpl/list.h>
#include <type_traits>

/*
    Utility functions for a compile time map

    This is implemented using a list of pairs
*/
namespace mpl
{
template<typename ... KVPairs>
struct map
{};

template<typename Map, typename T>
struct at;

template<typename T, typename  HeadPair, typename ... Rest>
struct at<map<HeadPair, Rest...>, T>
{
    using type = std::conditional<
        std::is_same_v<typename HeadPair::first, T>,
        typename HeadPair::second,
        typename at<map<Rest...>, T>::type
    >::type;
};

template<typename T>
struct at<map<>, T>
{
    using type = type_not_found;
};


template<typename Map, typename T>
struct contains
{
    static constexpr bool value =
        !std::is_same_v<
            typename at<Map, T>::type, 
            type_not_found>;
};
template<typename Map, typename K, typename V>
struct insert;

template<typename ... Pairs, typename K, typename V>
struct insert<map<Pairs...>, K, V>
{
    static_assert(!contains<map<Pairs...>, K>::value, 
            "no repeat values allowed in compile time map");

    using type = map<Pairs..., pair<K,V>>;
};

}

