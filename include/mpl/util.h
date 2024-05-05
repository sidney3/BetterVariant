#pragma once
#include <mpl/types.h>
#include <mpl/list.h>
namespace mpl
{

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
    using return_type = ReturnType;
    using type = list<Args...>;
};
}
