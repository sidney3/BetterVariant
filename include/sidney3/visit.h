#pragma once

#include <mpl/list.h>
#include <optional>
#include <sidney3/tags.h>
#include <sidney3/variant.h>

#include <stdexcept>

template <typename T> struct is_optional {
  static constexpr bool value = false;
};

template <typename T> struct is_optional<std::optional<T>> {
  static constexpr bool value = true;
};

/*
  Decides if T matches with variant. If it does, calls variant and returns
  its result. Otherwise returns std::nullopt.
*/
template <typename T, typename Variant, typename Functor>
static std::optional<typename sidney3::FunctorTypeTraits<Functor>::return_type>
match(Variant &variant, Functor fn) {
  if (!std::holds_alternative<T>(variant)) {
    return std::nullopt;
  }
  return fn(std::get<T>(variant));
  /* if constexpr(is_optional<T>::value) */
  /* { */
  /*   using BaseT = std::optional<T>::value_type; */
  /*  */
  /*  */
  /* } */
}

template <typename VariantList, typename CurrList, typename Functor>
struct visit_impl;

template <typename... Ts, typename Head, typename... Rest, typename Functor>
struct visit_impl<mpl::list<Ts...>, mpl::list<Head, Rest...>, Functor> {
  static typename sidney3::FunctorTypeTraits<Functor>::return_type
  apply(sidney3::variant<Ts...> &variant, Functor fn) {
    auto maybeRes = match<Head>(variant, fn);
    if (maybeRes.has_value()) {
      return *maybeRes;
    }
    return visit_impl<mpl::list<Ts...>, mpl::list<Rest...>, Functor>::apply(
        variant, std::forward<decltype(fn)>(fn));
  }
};

template <typename... Ts, typename Functor>
struct visit_impl<mpl::list<Ts...>, mpl::list<>, Functor> {
  static sidney3::FunctorTypeTraits<Functor>::return_type apply(...) {
    throw std::runtime_error{"FATAL: case not matched"};
  }
};

template <typename... Ts, typename Functor>
  requires std::is_same_v<typename Functor::tag, sidney3::functor_tag>
auto visit(sidney3::variant<Ts...> &variant, Functor &&fn) {
  return visit_impl<mpl::list<Ts...>, mpl::list<Ts...>, Functor>::apply(
      variant, std::forward<Functor>(fn));
}

template <typename... Ts, typename Functor>
  requires std::is_same_v<typename Functor::tag, sidney3::functor_tag>
auto visit(sidney3::variant<Ts...> &variant, Functor &fn) {
  return visit_impl<mpl::list<Ts...>, mpl::list<Ts...>, Functor>::apply(
      variant, std::forward<Functor>(fn));
}
