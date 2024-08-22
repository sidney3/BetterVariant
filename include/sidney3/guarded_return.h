#pragma once

#include <optional>
#include <sidney3/tags.h>

namespace sidney3 {

template <typename T> struct GuardedReturn : std::optional<T> {
  using std::optional<T>::optional;
  using std::optional<T>::operator*;
  using std::optional<T>::has_value;
  using std::optional<T>::value_type;
};

template <typename T> struct isGuardedReturn {
  static constexpr bool value = false;
};
template <typename T> struct isGuardedReturn<GuardedReturn<T>> {
  static constexpr bool value = true;
};

template <typename T> struct stripGuardedReturn {
  using type = T;
};

template <typename T> struct stripGuardedReturn<GuardedReturn<T>> {
  using type = T;
};

} // namespace sidney3
