#include <sidney3/functor.h>
#include <sidney3/variant.h>
#include <sidney3/visit.h>

#include <mpl/list_traits.h>

using lst = mpl::list_traits<mpl::list>;

template <typename... Ts, typename Functor>
auto operator||(sidney3::variant<Ts...> &variant, const Functor &func) {
  // input args to all functions in Functor
  /* using functorTraits = sidney3::FunctorTypeTraits<Functor>; */
  /*  */
  /* static_assert((functorTraits::template CoversInput<Ts>::value && ...)); */

  return visit(variant, func);
}

template <typename T, typename V>
sidney3::Functor<T, V> operator|(T &&t, V &&v) {
  return sidney3::Functor<T, V>{std::forward<T>(t), std::forward<V>(v)};
}
