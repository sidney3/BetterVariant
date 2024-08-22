#pragma once
#include <mpl/function_traits.h>
#include <mpl/list.h>
#include <mpl/list_traits.h>
#include <mpl/types.h>
#include <type_traits>

namespace mpl {

/*
    Returns true if the given functor has an input type
    that is a template or auto parameter
*/
template <typename F> struct is_unary_auto {
  struct non_type {};
  static constexpr bool value = std::is_invocable_v<F, non_type>;
};

/*
   Given a unary function, returns its argument type
*/
template <typename T> struct generic_unary_traits {
  using arg_type = types::auto_arg;
  using return_type = std::invoke_result_t<T, int>;
};

template <typename T> struct non_generic_unary_traits {
  using lst = mpl::list_traits<mpl::list>;

  using traits = mpl::function_traits<T>;

  static_assert(lst::size<typename traits::arg_type>::value == 1);
  using arg_type = lst::at<typename traits::arg_type, 0>::type;
  using return_type = traits::return_type;
};

template <typename F> struct unary_traits {
  struct non_generic_unary_traits_wrapper {
    using type = non_generic_unary_traits<F>;
  };
  struct generic_unary_traits_wrapper {
    using type = generic_unary_traits<F>;
  };
  static constexpr bool is_auto = is_unary_auto<F>::value;

  using traits = std::conditional_t<is_auto, generic_unary_traits_wrapper,
                                    non_generic_unary_traits_wrapper>::type;

  using arg_type = traits::arg_type;
  using return_type = traits::return_type;
};

/*
    Returns true if F is a unary function that can be invoked on the inputted
    types without implicit conversion

    i.e. char -> int
*/
template <typename F, typename T> struct is_exact_invocable {
  static constexpr bool value =
      is_unary_auto<F>::value ||
      (std::is_invocable_v<F, T> &&
       std::is_same_v<std::remove_cvref_t<typename unary_traits<F>::arg_type>,
                      std::remove_cvref_t<T>>);
};

} // namespace mpl
