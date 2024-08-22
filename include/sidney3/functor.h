#pragma once
#include <optional>
#include <utility>

#include <mpl/list.h>
#include <mpl/list_traits.h>
#include <mpl/unary_function_traits.h>

#include <sidney3/tags.h>

namespace sidney3 {
template <typename FunctorType> struct FunctorTypeTraits;

using lst = mpl::list_traits<mpl::list>;

// base case
template <typename T, typename U> struct Functor : T, U {
  static_assert(std::is_same_v<typename mpl::unary_traits<T>::return_type,
                               typename mpl::unary_traits<U>::return_type>);
  template <typename X> mpl::unary_traits<U>::return_type operator()(X &&x) {
    if constexpr (mpl::is_exact_invocable<T, X>::value) {
      return T::operator()(std::forward<X>(x));
    } else {
      static_assert(mpl::is_exact_invocable<U, X>::value);
      return U::operator()(std::forward<X>(x));
    }
  }
  using tag = functor_tag;
};

// recursive case
template <typename T, typename V, typename U>
struct Functor<Functor<T, V>, U> : Functor<T, V>, U {
  using self = Functor<Functor<T, V>, U>;

  static_assert(std::is_same_v<typename mpl::unary_traits<T>::return_type,
                               typename mpl::unary_traits<V>::return_type>);
  static_assert(std::is_same_v<typename mpl::unary_traits<V>::return_type,
                               typename mpl::unary_traits<U>::return_type>);

  template <typename X> mpl::unary_traits<U>::return_type operator()(X &&x) {
    if constexpr (FunctorTypeTraits<Functor<T, V>>::template is_exact_invocable<
                      X>::value) {
      return Functor<T, V>::operator()(std::forward<X>(x));
    } else {
      static_assert(mpl::is_exact_invocable<U, X>::value);
      return U::operator()(std::forward<X>(x));
    }
  }

  using T::operator();
  using V::operator();
  using U::operator();
};

template <typename Base> struct FunctorTraitsImpl {
  template <typename K> struct is_exact_invocable {
    template <typename T> using is_invokable = mpl::is_exact_invocable<T, K>;

    static constexpr bool value =
        lst::any_of<typename Base::raw_functions, is_invokable>::value;
  };
};

template <typename T, typename U>
struct FunctorTypeTraits<Functor<T, U>>
    : FunctorTraitsImpl<FunctorTypeTraits<Functor<T, U>>> {
  using arg_types = mpl::list<typename mpl::unary_traits<T>::arg_type,
                              typename mpl::unary_traits<U>::arg_type>;
  using return_type = mpl::unary_traits<T>::return_type;
  using raw_functions = mpl::list<T, U>;
};

template <typename T, typename V, typename U>
struct FunctorTypeTraits<Functor<Functor<T, V>, U>>
    : FunctorTraitsImpl<FunctorTypeTraits<Functor<Functor<T, V>, U>>> {
private:
  using child_functor = Functor<T, V>;

public:
  using arg_types = mpl::list_traits<mpl::list>::push_back<
      typename FunctorTypeTraits<child_functor>::arg_types,
      typename mpl::unary_traits<U>::arg_type>::type;
  using return_type = FunctorTypeTraits<child_functor>::return_type;
  using raw_functions =
      lst::push_back<typename FunctorTypeTraits<child_functor>::raw_functions,
                     U>::type;
};

} // namespace sidney3
