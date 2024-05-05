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
    constexpr static bool value = 
        std::is_same_v<decltype(std::declval<F>()(std::declval<T>())), T>;
};

template<typename F, typename T>
struct takes_T<F,T,none_type>
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
struct extract_input_type<F, mpl::list<Head, Rest...>>
{
    using type = 
        std::conditional<takes_T<F, Head>::value,
            Head,
            typename extract_input_type<F, list<Rest...>>::type>::type;
};

/*
Not 100% complete, but good enough
*/
template <typename T>
struct function_traits
    : public function_traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
    : public function_traits<ReturnType(Args...)>
{};

template<typename ReturnType, typename...Args>
struct function_traits<ReturnType(*)(Args...)>
    : public function_traits<ReturnType(Args...)>
{};

template<typename ReturnType, typename...Args>
struct function_traits<ReturnType(Args...)>
{
    using type = list<Args...>;
};
}
