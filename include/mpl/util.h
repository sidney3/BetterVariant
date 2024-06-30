#pragma once
#include <mpl/list.h>
#include <mpl/list_traits.h>
#include <mpl/pair.h>
#include <type_traits>

namespace mpl
{
template<typename T, typename ... Ts>
struct contains;

template<typename T, typename Head, typename ... Rest>
struct contains<T, Head, Rest...>
{
    static constexpr bool value = 
        std::conditional_t<
            std::is_same_v<T,Head>,
            std::true_type,
            contains<T, Rest...>
        >::value;
};

template<typename T>
struct contains<T>
{
    static constexpr bool value = false;
};

/*
    Faster way to do this is to sort the types by typeid and then determine uniqueness in O(n) but this is rarely used for large types like that...
*/
template<typename ... Ts>
struct is_unique;

template<typename Head, typename ... Rest>
struct is_unique<Head, Rest...>
{
    static constexpr bool value = 
        !contains<Head, Rest...>::value
        && is_unique<Rest...>::value;
};

template<>
struct is_unique<>
{
    static constexpr bool value = true;
};


    
template<long N, typename Target, typename ... Ts>
struct index_of_impl;
template<long N, typename Target, typename Head, typename ... Rest>
struct index_of_impl<N, Target, Head, Rest...>
{

    using type = std::conditional_t<
            std::is_same_v<Target, Head>,
            std::integral_constant<long, N>,
            typename index_of_impl<N+1, Target, Rest...>::type
        >;
};

template<long N, typename Target>
struct index_of_impl<N, Target>
{
    using type = types::type_not_found;
};
template<typename T, typename ... Ts>
struct index_of
{
    using impl_res = index_of_impl<0, T, Ts...>::type;
    static_assert(!std::is_same_v<impl_res, types::type_not_found>, 
            "target not found");
    static constexpr long value = impl_res::value;
};
} // namespace mpl
