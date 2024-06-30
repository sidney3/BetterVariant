#pragma once
#include <type_traits>
#include <mpl/types.h>
#include <mpl/pair.h>

/*
    Standard definitions for a compile time List (of types)
*/

namespace mpl
{
template<template<typename...> typename List>
struct list_traits 
{
template<typename TypeList, typename T>
struct push_front;

template<typename ... Ts, typename T>
struct push_front<List<Ts...>, T>
{
    using type = List<T, Ts...>;
};

template<typename TypeList, typename T>
struct push_back;

template<typename ... Ts, typename T>
struct push_back<List<Ts...>, T>
{
    using type = List<Ts..., T>;
};

template<typename TypeList>
struct pop_back;

template<typename Head, typename ... Rest>
struct pop_back<List<Head, Rest...>>
{
    using type = push_front<
        typename pop_back<List<Rest...>>::type, 
        Head>;
};
template<typename Head>
struct pop_back<List<Head>>
{
    using type = List<>;
};

template<typename TypeList>
struct size;

template<typename Head, typename ... Rest>
struct size<List<Head, Rest...>>
{
    static constexpr size_t value = 
        size<List<Rest...>>::value + 1;
};

template<>
struct size<List<>>
{
    static constexpr size_t value = 0;
};

template<typename TypeList, typename T>
struct contains;

template<typename T, typename Head, typename ... Rest>
struct contains<List<Head, Rest...>, T>
{
    static constexpr bool value =
        std::is_same_v<T, Head> || contains<List<Rest...>, T>::value;
};

template<typename T>
struct contains<List<>, T>
{
    static constexpr bool value = false;
};

template<typename List1, typename List2>
struct equals;

template<typename ... Ts, typename ... Us>
struct equals<List<Ts...>, List<Us...>>
{
    static constexpr bool value =
        size<List<Ts...>>::value == size<List<Us...>>::value
        && (contains<List<Us...>, Ts>::value &&...);
};

template<typename TypeList, typename Comparator>
struct find_if;

template<typename Head, typename ... Rest, typename Comparator>
struct find_if<List<Head, Rest...>, Comparator>
{
    using type = 
        std::conditional<
            Comparator::template apply<Head>::type,
            Head,
            typename find_if<
                List<Rest...>, 
                Comparator>::type
            >::type;
};

template<typename Comparator>
struct find_if<List<>, Comparator>
{
    using type = types::type_not_found;
};

template<typename TypeList, long I>
struct at;

template<typename Head, typename ... Rest, long I>
struct at<List<Head, Rest...>, I>
{
    static_assert(I < size<List<Head,Rest...>>::value);
    using type = at<List<Rest...>, I - 1>::type;
};

template<typename Head, typename ... Rest>
struct at<List<Head, Rest...>, 0>
{
    using type = Head;
};


template<typename TypeList>
struct head;

template<typename Head, typename ... Rest>
struct head<List<Head, Rest...>>
{
    using type = Head;
};

template<typename TypeList, template<typename> typename Unary>
struct transform;

template<typename ... Ts, template<typename> typename Unary>
struct transform<List<Ts...>, Unary>
{
    using type = List<typename Unary<Ts>::type...>;
};

template<typename TypeList1, typename TypeList2>
struct set_equals;

template<typename ... Ts, typename ... Us>
struct set_equals<List<Ts...>, List<Us...>>
{
    static constexpr bool value = 
        (contains<List<Ts...>, Us>::value&&...) 
        && (contains<List<Us...>, Ts>::value&&...);
};


template<typename TypeList>
struct enumerate;

template<typename ... Ts>
struct enumerate<List<Ts...>>
{
    template<long currPos, typename ... Us>
    struct enumerate_impl;

    template<long currPos, typename Head, typename ... Rest>
    struct enumerate_impl<currPos, Head, Rest...>
    {
        using type = 
            push_front<
                typename enumerate_impl<
                    currPos + 1, Rest...>::type,
                pair<Head, std::integral_constant<long, currPos>>
            >::type;
    };

    template<long currPos>
    struct enumerate_impl<currPos>
    {
        using type = List<>;
    };
    using type = enumerate_impl<0, Ts...>::type;
};

/*
Adapter between different type lists
*/
template<typename TypeList, template <typename...> typename Other>
struct apply;

template<typename ... Ts, template <typename ...> typename Other>
struct apply<List<Ts...>, Other>
{
    using type = Other<Ts...>;
};


}; // struct list_traits 
}// namespace mpl
