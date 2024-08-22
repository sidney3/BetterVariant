#pragma once

#include <utility>

#include <mpl/functional.h>
#include <mpl/list.h>
#include <mpl/list_traits.h>
#include <mpl/unary_function_traits.h>

#include <sidney3/guarded_return.h>
#include <sidney3/tags.h>

namespace sidney3 {

/*
   A function with return := GuardedReturn<T> still gets counted as being of
   return type T (because we will never return GuardedReturn<T> to a client)
*/
template <typename Unary> struct UnaryReturnType {
private:
  using raw_return_type = mpl::unary_traits<Unary>::return_type;

public:
  using type = stripGuardedReturn<raw_return_type>::type;
};

template <typename FunctorType> struct FunctorTypeTraits;

using lst = mpl::list_traits<mpl::list>;

/*
  Handle the shared logic: each functor is defined only in terms of
  its child lambdas, and so we let a single class handle this logic
  and then the functors can get recursively generated.
*/
template <typename Child>
struct FunctorImpl : FunctorTypeTraits<Child>::my_type,
                     FunctorTypeTraits<Child>::child_type {

  using child_type = FunctorTypeTraits<Child>::child_type;
  using base_type = FunctorTypeTraits<Child>::my_type;
  using return_type = FunctorTypeTraits<Child>::return_type;

  using child_type::operator();
  using base_type::operator();

  template <typename X> return_type operator()(X &&x) {

    static constexpr bool childInvocable =
        FunctorTypeTraits<child_type>::template is_exact_invocable<X>::value;
    static constexpr bool baseInvocable =
        FunctorTypeTraits<base_type>::template is_exact_invocable<X>::value;

    static_assert(childInvocable || baseInvocable);

    if constexpr (FunctorTypeTraits<child_type>::template is_exact_invocable<
                      X>::value) {
      auto res = child_type::operator()(x);

      if (hasValue(res)) {
        return getValue(res);
      }
    }

    if constexpr (FunctorTypeTraits<base_type>::template is_exact_invocable<
                      X>::value) {
      return base_type::operator()(std::forward<X>(x));
    }

    std::unreachable();
  }

  template <typename Arg1, typename Arg2>
    requires(std::is_constructible_v<child_type, Arg1> &&
             std::is_constructible_v<base_type, Arg2>)
  FunctorImpl(Arg1 &&arg1, Arg2 &&arg2)
      : base_type{std::forward<Arg2>(arg2)},
        child_type{std::forward<Arg1>(arg1)} {}
};

// base case
template <typename T, typename U> struct Functor : FunctorImpl<Functor<T, U>> {
  static_assert(std::is_same_v<typename UnaryReturnType<T>::type,
                               typename UnaryReturnType<U>::type>);

  using self = Functor<T, U>;
  using tag = functor_tag;
  using FunctorImpl<self>::operator();

  template <typename... Args>
  Functor(Args &&...args) : FunctorImpl<self>{std::forward<Args>(args)...} {}
};

template <typename T, typename V, typename U>
struct Functor<Functor<T, V>, U> : FunctorImpl<Functor<Functor<T, V>, U>> {
  using self = Functor<Functor<T, V>, U>;

  static_assert(std::is_same_v<typename UnaryReturnType<T>::type,
                               typename UnaryReturnType<V>::type>);
  static_assert(std::is_same_v<typename UnaryReturnType<V>::type,
                               typename UnaryReturnType<U>::type>);

  using tag = functor_tag;
  using FunctorImpl<self>::operator();

  template <typename... Args>
  Functor(Args &&...args) : FunctorImpl<self>{std::forward<Args>(args)...} {}
};

template <typename Child> struct FunctorTraitsImpl {
public:
  /*
    Returns true if the functor can be applied to a variant including the type T
  */
  template <typename T> struct CoversInput {
  private:
    template <typename Fn> struct isGuardedFunction {
      static constexpr bool value =
          isGuardedFunction<typename mpl::unary_traits<Fn>::return_type>::value;
    };

    template <typename Fn>
    using isInvocableWithT = mpl::is_exact_invocable<Fn, T>;

    template <typename Fn> struct functionCoversT {
      static constexpr bool value = isInvocableWithT<Fn>::value;
      //! isGuardedFunction<Fn>::value;
    };
    /* template<typename Fn> */
    /* using functionCoversT = mpl::And< */
    /*   isInvocableWithT, mpl::Negation<isGuardedFunction>:: template type */
    /* >::type; */
  public:
    static constexpr bool value =
        lst::any_of<typename Child::raw_functions, functionCoversT>::value;
  };

  template <typename K> struct is_exact_invocable {
    template <typename T> using is_invokable = mpl::is_exact_invocable<T, K>;

    static constexpr bool value =
        lst::any_of<typename Child::raw_functions, is_invokable>::value;
  };
};

/*
  Each FunctorTypeTraits takes one of the different types of functors:
  lambda, binary functor, and recursive binary functor. Because each
  is layed out roughly the same way, with some number of functions that
  correspond to it, and some number of input arguments, we factor out the
  actual functionality to a seperate FunctorTraitsImpl
*/
template <typename T>
struct FunctorTypeTraits : FunctorTraitsImpl<FunctorTypeTraits<T>> {
  using arg_types = mpl::unary_traits<T>::arg_type;
  using return_type = UnaryReturnType<T>::type;
  using raw_functions = mpl::list<T>;
  using my_type = T;
};

template <typename T, typename U>
struct FunctorTypeTraits<Functor<T, U>>
    : FunctorTraitsImpl<FunctorTypeTraits<Functor<T, U>>> {
  using arg_types = mpl::list<typename mpl::unary_traits<T>::arg_type,
                              typename mpl::unary_traits<U>::arg_type>;
  using return_type = UnaryReturnType<T>::type;
  using raw_functions = mpl::list<T, U>;
  using my_type = U;
  using child_type = T;
};

template <typename T, typename V, typename U>
struct FunctorTypeTraits<Functor<Functor<T, V>, U>>
    : FunctorTraitsImpl<FunctorTypeTraits<Functor<Functor<T, V>, U>>> {
private:
  using child_functor = Functor<T, V>;

public:
  using my_type = U;
  using child_type = Functor<T, V>;
  using arg_types = mpl::list_traits<mpl::list>::push_back<
      typename FunctorTypeTraits<child_functor>::arg_types,
      typename mpl::unary_traits<U>::arg_type>::type;
  using return_type = FunctorTypeTraits<child_functor>::return_type;
  using raw_functions =
      lst::push_back<typename FunctorTypeTraits<child_functor>::raw_functions,
                     U>::type;
};

} // namespace sidney3
