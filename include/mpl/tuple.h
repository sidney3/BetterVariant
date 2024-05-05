#pragma once
#include <mpl/map.h>
#include <mpl/list.h>
#include <tuple>

/*
   A type-indexed wrapper of std::tuple. 
   (i.e. for a tuple MyTuple of type mpl::ti_tuple<T,U>, we can call
   std::get<U>(MyTuple))
   
   As we index into the tuple by type we cannot have repeated types.

   We also add an "append" member where we can generate a new tuple from an 
   existing tuple (with move construction)
*/
namespace mpl
{
template<long currIndex, typename TypeList>
struct make_indexed_map_impl;

template<long currIndex, typename Head, typename ... Rest>
struct make_indexed_map_impl<currIndex, list<Head, Rest...>>
{
    using type = insert<
        typename make_indexed_map_impl<currIndex + 1, list<Rest...>>::type,
        Head,
        std::integral_constant<long, currIndex>
        >::type;
};

template<long currIndex>
struct make_indexed_map_impl<currIndex, list<>>
{
    using type = map<>;
};

template<typename TypeList>
struct make_indexed_map
{
    using type = make_indexed_map_impl<0, TypeList>::type;
};

template<typename TypeList>
struct ti_tuple;

template<typename ... Ts>
struct ti_tuple<list<Ts...>> : std::tuple<Ts...>
{
    using parent = std::tuple<Ts...>;
    using parent::parent;
    using types = list<Ts...>;
    using tag = tuple_tag;

    // explicitly declare copy and move constructors,
    // as these do not get inherited
    ti_tuple(const parent& t) : parent(t) {}
    ti_tuple(parent&& t) : parent(std::move(t)) {}

    using IndexMap = make_indexed_map<list<Ts...>>::type;
};

template<typename Tuple, typename T, size_t ... Indices>
ti_tuple<typename push_back<typename Tuple::types, T>::type>
append_impl(Tuple&& tuple, T&& new_elt, std::index_sequence<Indices...>)
{
    using return_type = 
        ti_tuple<typename push_back<typename Tuple::types, T>::type>;

    return return_type{
        std::forward<Tuple>(std::get<Indices>(tuple))...,
        std::forward<T>(new_elt)};
}

template<typename Tuple, typename T>
ti_tuple<typename push_back<typename Tuple::types, T>::type>
append(Tuple&& tuple, T&& new_elt)
{
    static_assert(std::is_same_v<typename Tuple::tag, tuple_tag>);
    return append_impl(
            std::forward<Tuple>(tuple),
            std::forward<T>(new_elt), 
            std::make_index_sequence<size<typename Tuple::types>::value>());
}


template<typename T, typename TiTuple>
decltype(auto)
get(TiTuple&& t)
{
    using Tuple = std::decay_t<TiTuple>;
    static_assert(std::is_same_v<typename Tuple::tag, tuple_tag>);
    static_assert(contains<typename Tuple::IndexMap, T>::value, "value not found");
    constexpr size_t index = at<typename Tuple::IndexMap, T>::type::value;
    static_assert(index < 2);
    return std::get<index>(t);
}

template<typename Tuple>
struct tuple_size;

template<typename ... Ts>
struct tuple_size<ti_tuple<Ts...>>
{
    static constexpr size_t value = size<list<Ts...>>::value;
};
}
