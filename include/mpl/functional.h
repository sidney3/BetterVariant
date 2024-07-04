#pragma once
#include <type_traits>
#include <mpl/list.h>
#include <mpl/types.h>

namespace mpl
{

/*
    Returns true if the given functor has an input type
    that is a template or auto parameter
*/
template<typename F>
struct is_unary_auto
{
    struct non_type{};
    static constexpr bool value =
        std::is_invocable_v<F, non_type>;
};


template<typename T, bool IsAuto>
struct unary_traits_impl;
/*
   Given a unary function, returns its argument type
*/
template<typename T>
struct unary_traits_impl<T, true>
{
    using arg_type = types::auto_arg;
    using return_type = std::invoke_result_t<T, int>;
};
template <typename T>
struct unary_traits_impl<T, false>
    : public unary_traits_impl<decltype(&T::operator()), false>
{};

template <typename ClassType, typename ReturnType, typename Args>
struct unary_traits_impl<ReturnType(ClassType::*)(Args) const, false>
    : public unary_traits_impl<ReturnType(Args), false>
{};

template<typename ReturnType, typename Arg>
struct unary_traits_impl<ReturnType(*)(Arg), false>
    : public unary_traits_impl<ReturnType(Arg), false>
{};

template<typename ReturnType, typename Arg>
struct unary_traits_impl<ReturnType(Arg), false>
{
    using arg_type = Arg;
    using return_type = ReturnType;
};

template<typename F>
struct unary_traits
{
    static constexpr bool is_auto =
        is_unary_auto<F>::value;

    using arg_type = unary_traits_impl<F, is_auto>::arg_type;
    using return_type = unary_traits_impl<F, is_auto>::return_type;
};

/*
    Returns true if F is a unary function that can be invoked on the inputted
    types without implicit conversion

    i.e. char -> int
*/
template<typename F, typename T>
struct is_exact_invocable
{
    static constexpr bool value = 
        is_unary_auto<F>::value
        || (std::is_invocable_v<F,T>
            && std::is_same_v<
                std::decay_t<typename unary_traits<F>::arg_type>, 
                std::decay_t<T>>);
};

} // namespace mpl
