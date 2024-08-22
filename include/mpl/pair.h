#pragma once

namespace mpl {
template <typename T, typename U> struct pair {
  using first = T;
  using second = U;
};
} // namespace mpl
