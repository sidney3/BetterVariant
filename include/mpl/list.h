#pragma once
#include <mpl/types.h>
#include <type_traits>
/*
    Standard definitions for a compile time list (of types)
*/

namespace mpl
{
template<typename ... Ts>
struct list
{};
template<typename TypeList, typename T>
struct push_front;

template<typename ... Ts, typename T>
struct push_front<list<Ts...>, T>
{
    using type = list<T, Ts...>;
};

template<typename TypeList, typename T>
struct push_back;

template<typename ... Ts, typename T>
struct push_back<list<Ts...>, T>
{
    using type = list<Ts..., T>;
};

template<typename TypeList>
struct pop_back;

template<typename Head, typename ... Rest>
struct pop_back<list<Head, Rest...>>
{
    using type = push_front<
        typename pop_back<list<Rest...>>::type, 
        Head>;
};
template<typename Head>
struct pop_back<list<Head>>
{
    using type = list<>;
};

template<typename TypeList>
struct size;

template<typename Head, typename ... Rest>
struct size<list<Head, Rest...>>
{
    static constexpr size_t value = 
        1 + size<list<Rest...>>::value + 1;
};

template<>
struct size<list<>>
{
    static constexpr size_t value = 0;
};

template<typename T, typename TypeList>
struct contains;

template<typename T, typename Head, typename ... Rest>
struct contains<T, list<Head, Rest...>>
{
    static constexpr bool value =
        std::is_same_v<T, Head> || contains<T, list<Rest...>>::value;
};

template<typename T>
struct contains<T, list<>>
{
    static constexpr bool value = false;
};

template<typename List1, typename List2>
struct equals;

template<typename ... Ts, typename ... Us>
struct equals<list<Ts...>, list<Us...>>
{
    static constexpr bool value =
        size<list<Ts...>>::value == size<list<Us...>>::value
        && (contains<Ts, list<Us...>>::value &&...);
};

template<typename TypeList, typename Comparator>
struct find_if;

template<typename Head, typename ... Rest, typename Comparator>
struct find_if<list<Head, Rest...>, Comparator>
{
    using type = 
        std::conditional<
            Comparator::template apply<Head>::type,
            Head,
            typename find_if<
                list<Rest...>, 
                Comparator>::type
            >::type;
};

template<typename Comparator>
struct find_if<list<>, Comparator>
{
    using type = type_not_found;
};

template<typename TypeList>
struct head;

template<typename Head, typename ... Rest>
struct head<list<Head, Rest...>>
{
    using type = Head;
};

template<typename TypeList, typename Wrapper>
struct apply;

template<typename ... Ts, typename Wrapper>
struct apply<list<Ts...>, Wrapper>
{
    using type = Wrapper::template apply<Ts...>;
};

}
