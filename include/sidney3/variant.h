#pragma once
#include <variant>

#include <sidney3/functor.h>
#include <sidney3/tags.h>

namespace sidney3 {
/*
    Just a wrapper over std::variant, as we don't want to add
    functions to namespace std!

    The bulk of the logic is in matcher.h
*/
template <typename... Ts> struct variant : std::variant<Ts...> {
  static_assert(sizeof...(Ts) > 1);
  using BaseT = std::variant<Ts...>;

  using BaseT::BaseT;
  using BaseT::index;

  using tag = variant_tag;
};

template <typename T, typename... Ts>
bool holds_alternative(const variant<Ts...> &var) {
  return std::holds_alternative<T>(
      static_cast<const std::variant<Ts...> &>(var));
}
template <typename T, typename... Ts> T &get(variant<Ts...> &var) {
  return std::get<T>(static_cast<std::variant<Ts...> &>(var));
}

} // namespace sidney3
