#pragma once

namespace mpl {

template <template <typename> typename Pred> struct Negation {
  template <typename T> struct type {
    static constexpr bool value = !Pred<T>::value;
  };
};

template <template <typename> typename Pred1,
          template <typename> typename Pred2>
struct And {
  template <typename T> struct type {
    static constexpr bool value = Pred1<T>::value && Pred2<T>::value;
  };
  template <typename T>
  static constexpr bool value = Pred1<T>::value && Pred2<T>::value;
};

template <template <typename> typename Pred1,
          template <typename> typename Pred2>
struct Or {
  template <typename T> struct type {
    static constexpr bool value = Pred1<T>::value || Pred2<T>::value;
  };
};

}; // namespace mpl
