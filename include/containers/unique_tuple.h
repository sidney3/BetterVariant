#pragma once
#include <mpl/util.h>
#include <variant>
#include <mpl/template_union.h>

namespace containers 
{
/* template<typename T> */
/* struct _wrapper_class */
/* { */
/*     T value; */
/* }; */
/* template<typename ... Ts> */
/* struct unique_tuple : _wrapper_class<Ts>... */
/* { */
/*     static_assert(is_unique<Ts...>::value, "unique_tuple must take unique input types"); */
/*  */
/*     template<typename ... Us> */
/*         requires ( std::is_constructible_v<Us,Ts>&&... ) */
/*     unique_tuple(Us&&... us)  */
/*         : _wrapper_class<Ts>{us}... */
/*     {} */
/* }; */
/* template<typename T, typename ... Us> */
/* T &get(unique_tuple<Us...>& tup) */
/* { */
/*     return static_cast<_wrapper_class<T>&>(tup).value; */
/* } */


template<typename ... Ts>
struct variable_vector : std::vector<std::variant<Ts...>>
{
    using BaseT = std::vector<std::variant<Ts...>>;

    template<typename Head, typename ... Rest>
    void init_parent(Head&& head, Rest&&... rest)
    {
        BaseT::emplace_back(std::forward<Head>(head));
        init_parent(rest...);
    }

    // no-op for the base case of initialization
    void init_parent()
    {}

    template<typename ... Us>
        requires ( std::is_same_v<Ts,Us>&&... )
    variable_vector(Us&&... us)
    {
        init_parent(std::forward<Us>(us)...);
    }
};


template<typename ... Ts>
struct unique_tuple
{

    template<typename ... Us>
        requires (std::is_same_v<Ts,Us>&&...)
    explicit unique_tuple(Ts&&... fns)
    {}


};

template<typename T, typename ... Us>
T &get(unique_tuple<Us...>& tup)
{
    static constexpr long idx = mpl::index_of<T, Us...>::value;
    return tup._data[idx];
}
template<typename UniqueTuple, typename ... Ts>
struct UniqueTupleFactory;

template<typename ... Us, typename ... Ts>
struct UniqueTupleFactory<unique_tuple<Us...>, Ts...>
{
    unique_tuple<Us..., Ts...>
    create(unique_tuple<Us...>&& moved_tuple,
            Ts&&... extra_types)
    {
        
    }
};

} // namespace mpl
