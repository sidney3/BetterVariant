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

    // explicitly declare copy and move constructors,
    // as these do not get inherited
    ti_tuple(const parent& t) : parent(t) {}
    ti_tuple(parent&& t) : parent(std::move(t)) {}

    using IndexMap = make_indexed_map<list<Ts...>>;
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
    return append_impl(
            std::forward<Tuple>(tuple),
            std::forward<T>(new_elt), 
            std::make_index_sequence<size<typename Tuple::types>::value>());
}


template<typename T, typename TiTuple>
T get(TiTuple&& t)
{
    static_assert(contains<T, typename TiTuple::IndexMap>::value, "value not found");
    return std::get<at<typename TiTuple::IndexMap, T>::type::value>(std::forward<TiTuple>(t));
}
}
