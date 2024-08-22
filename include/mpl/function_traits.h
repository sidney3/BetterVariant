#pragma once

#include <mpl/list.h>

namespace mpl {

/*
@note we need & in front of T::operator() so it isn't interpreted as a function
declaration
*/
template <typename T>
struct function_traits : public function_traits<decltype(&T::operator())> {};

/************************
    Member function land: these can all be qualified with different types and
each of these requires a different temple specialization
***********************/
/*
@note: ReturnType(ClassType::*)(Args...) is the syntax for a member function
pointer.
*/
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...)>
    : public function_traits<ReturnType(Args...)> {};

template <typename ReturnType, typename... Args>
struct function_traits<ReturnType (*)(Args...)>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) volatile>
    : public function_traits<ReturnType(Args...)> {};

/*
@note: ReturnType(ClassType::*)(Args...) const volatile is the syntax for a
const volatile member function pointer. This specialization handles const
volatile member functions.
*/
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const volatile>
    : public function_traits<ReturnType(Args...)> {};
template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) &>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const &>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) volatile &>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const volatile &>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) &&>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const &&>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) volatile &&>
    : public function_traits<ReturnType(Args...)> {};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType (ClassType::*)(Args...) const volatile &&>
    : public function_traits<ReturnType(Args...)> {};

template <typename ReturnType, typename... Args>
struct function_traits<ReturnType(Args...)> {
  using arg_type = mpl::list<Args...>;
  using return_type = ReturnType;
};

} // namespace mpl
