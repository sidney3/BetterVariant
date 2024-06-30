#pragma once
#include <type_traits>
#include <mpl/list.h>

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

/*
   Given a unary function, returns its argument type
*/
template <typename T>
struct unary_arg_impl
    : public unary_arg_impl<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename Args>
struct unary_arg_impl<ReturnType(ClassType::*)(Args) const>
    : public unary_arg_impl<ReturnType(Args)>
{};

template<typename ReturnType, typename Arg>
struct unary_arg_impl<ReturnType(*)(Arg)>
    : public unary_arg_impl<ReturnType(Arg)>
{};

template<typename ReturnType, typename Arg>
struct unary_arg_impl<ReturnType(Arg)>
{
    using type = Arg;
};

template<typename F>
struct unary_arg
{
    static_assert(!is_unary_auto<F>::value);
    using type = unary_arg_impl<F>::type;
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
                std::decay_t<typename unary_arg<F>::type>, 
                std::decay_t<T>>);
};

} // namespace mpl
