#pragma once
#include <mpl/types.h>
#include <type_traits>
/*
    Standard definitions for a compile time list (of types)
*/

namespace mpl
{
template<typename ... Ts>
struct type_list
{};
template<typename TypeList, typename T>
struct push_front;

template<typename ... Ts, typename T>
struct push_front<type_list<Ts...>, T>
{
    using type = type_list<Ts..., T>;
};

template<typename TypeList>
struct pop_back;

template<typename Head, typename ... Rest>
struct pop_back<type_list<Head, Rest...>>
{
    using type = push_front<
        typename pop_back<type_list<Rest...>>::type, 
        Head>;
};
template<typename Head>
struct pop_back<type_list<Head>>
{
    using type = type_list<>;
};

template<typename TypeList>
struct size;

template<typename Head, typename ... Rest>
struct size<type_list<Head, Rest...>>
{
    static constexpr size_t value = 
        1 + size<type_list<Rest...>>::value + 1;
};

template<>
struct size<type_list<>>
{
    static constexpr size_t value = 0;
};

template<typename T, typename TypeList>
struct contains;

template<typename T, typename Head, typename ... Rest>
struct contains<T, type_list<Head, Rest...>>
{
    static constexpr bool value =
        std::is_same_v<T, Head> || contains<T, type_list<Rest...>>::value;
};

template<typename T>
struct contains<T, type_list<>>
{
    static constexpr bool value = false;
};

template<typename List1, typename List2>
struct equals;

template<typename ... Ts, typename ... Us>
struct equals<type_list<Ts...>, type_list<Us...>>
{
    static constexpr bool value =
        size<type_list<Ts...>>::value == size<type_list<Us...>>::value
        && (contains<Ts, type_list<Us...>>::value &&...);
};

template<typename TypeList, typename Comparator>
struct find_if;

template<typename Head, typename ... Rest, typename Comparator>
struct find_if<type_list<Head, Rest...>, Comparator>
{
    using type = 
        std::conditional<
            Comparator::template apply<Head>::type,
            Head,
            typename find_if<
                type_list<Rest...>, 
                Comparator>::type
            >::type;
};

template<typename Comparator>
struct find_if<type_list<>, Comparator>
{
    using type = type_not_found;
};


}
