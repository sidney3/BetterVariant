#pragma once
#include <mpl/types.h>
#include <mpl/list.h>
namespace mpl
{

/*
   compile time check for if a function type F
   takes a single parameter of type T
*/
template<
    typename F, 
    typename T, 
    typename = decltype(std::declval<F>()(std::declval<T>()))>
struct takes_T 
{
    constexpr static bool value = true;
};

template<typename F, typename T>
struct takes_T<F,T,void>
{
    constexpr static bool value = false;
};
/*
    Utility to, given a function F and a candidate list of
    types TypeList, returns the type T such that F can be
    invoked with a single argument of type T
*/
template<typename F, typename TypeList>
struct extract_input_type;

template<typename F, typename Head, typename ... Rest>
struct extract_input_type<F, mpl::type_list<Head, Rest...>>
{
    using type = 
        std::conditional<takes_T<F, Head>::type::value,
            Head,
            typename extract_input_type<F, Rest...>::type>::type;
};

template<typename F>
struct extract_input_type<F, mpl::type_list<>>
{
    using type = type_not_found;
};
}
