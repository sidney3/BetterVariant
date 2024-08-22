#include <sidney3/functor.h>
#include <sidney3/variant_impl.h>
#include <sidney3/visit.h>

#include <mpl/list_traits.h>

using lst = mpl::list_traits<mpl::list>;

template <typename... Ts, typename Functor>
auto operator||(sidney3::variant<Ts...> &variant, Functor &&func) {
  static_assert(
      (sidney3::FunctorTypeTraits<Functor>::template CoversInput<Ts>::value &&
       ...));

  return visit(variant, std::forward<Functor>(func));
}

template <typename T, typename V>
sidney3::Functor<T, V> operator|(T &&t, V &&v) {
  return sidney3::Functor<T, V>{std::forward<T>(t), std::forward<V>(v)};
}
