#pragma once
#include <mpl/pair.h>
#include <mpl/types.h>
#include <type_traits>

namespace mpl {
/*
To avoid forcing a user to use, say, std::tuple as their
type list of choice we template the template library in
terms of some List template.
*/
template <template <typename...> typename List> struct list_traits {

  /*
  Interface of all the operations we support.

  We use template inference to extract the items
  of each Typelist
  */
  template <typename TypeList, typename T> struct push_front;

  template <typename TypeList, typename T> struct push_back;

  template <typename List1, typename List2> struct concat;

  template <typename TypeList> struct size;

  template <typename TypeList, typename Comparator> struct find_if;

  template <typename TypeList, typename T> struct contains;

  // return the front element.
  template <typename TypeList> struct head;

  template <typename TypeList> struct back;

  template <typename TypeList, template <typename> typename Unary>
  struct transform;

  // strict equality between List1 and List2
  // @todo make this take a predicate
  template <typename List1, typename List2> struct equals;

  // set equality between List1 and List2. Informally
  // returns if set(List1) == set(List2). Formally
  // means that for each element in List1 that element
  // is in list2
  template <typename TypeList1, typename TypeList2> struct set_equals;

  template <typename TypeList, long Index> struct at;

  template <typename TypeList> struct enumerate;
  /*
  Given two lists L1 and L2 and a predicate P,
  if for each index i of L1,

  P(L1[i], L2[i])
  */
  template <template <typename, typename> typename Predicate, typename List1,
            typename List2>
  struct prefix_predicate;

  /*
  IMPLEMENTATIONS:
  */

  template <typename... Ts, typename T> struct push_front<List<Ts...>, T> {
    using type = List<T, Ts...>;
  };

  template <typename... Ts, typename... Us>
  struct concat<List<Ts...>, List<Us...>> {
    using type = List<Ts..., Us...>;
  };

  template <typename... Ts, typename T> struct push_back<List<Ts...>, T> {
    using type = List<Ts..., T>;
  };

  template <typename TypeList> struct pop_back;

  template <typename Head, typename... Rest>
  struct pop_back<List<Head, Rest...>> {
    using type = push_front<typename pop_back<List<Rest...>>::type, Head>;
  };
  template <typename Head> struct pop_back<List<Head>> {
    using type = List<>;
  };

  template <typename Head, typename... Rest> struct size<List<Head, Rest...>> {
    static constexpr size_t value = size<List<Rest...>>::value + 1;
  };

  template <> struct size<List<>> {
    static constexpr size_t value = 0;
  };

  template <typename T, typename Head, typename... Rest>
  struct contains<List<Head, Rest...>, T> {
    static constexpr bool value =
        std::is_same_v<T, Head> || contains<List<Rest...>, T>::value;
  };

  template <typename T> struct contains<List<>, T> {
    static constexpr bool value = false;
  };

  template <typename... Ts, typename... Us>
  struct equals<List<Ts...>, List<Us...>> {
    static constexpr bool value =
        size<List<Ts...>>::value == size<List<Us...>>::value &&
        (contains<List<Us...>, Ts>::value && ...);
  };

  template <typename Head, typename... Rest, typename Comparator>
  struct find_if<List<Head, Rest...>, Comparator> {
    using type = std::conditional<
        Comparator::template apply<Head>::type, Head,
        typename find_if<List<Rest...>, Comparator>::type>::type;
  };

  template <typename Comparator> struct find_if<List<>, Comparator> {
    using type = types::type_not_found;
  };

  template <typename Head, typename... Rest, long I>
  struct at<List<Head, Rest...>, I> {
    static_assert(I < size<List<Head, Rest...>>::value);
    using type = at<List<Rest...>, I - 1>::type;
  };

  template <typename Head, typename... Rest> struct at<List<Head, Rest...>, 0> {
    using type = Head;
  };

  template <typename Head, typename... Rest> struct head<List<Head, Rest...>> {
    using type = Head;
  };

  template <typename... Ts, template <typename> typename Unary>
  struct transform<List<Ts...>, Unary> {
    using type = List<typename Unary<Ts>::type...>;
  };

  template <typename... Ts, typename... Us>
  struct set_equals<List<Ts...>, List<Us...>> {
    static constexpr bool value = (contains<List<Ts...>, Us>::value && ...) &&
                                  (contains<List<Us...>, Ts>::value && ...);
  };

  template <typename... Ts> struct enumerate<List<Ts...>> {
    template <long currPos, typename... Us> struct enumerate_impl;

    template <long currPos, typename Head, typename... Rest>
    struct enumerate_impl<currPos, Head, Rest...> {
      using type =
          push_front<typename enumerate_impl<currPos + 1, Rest...>::type,
                     pair<Head, std::integral_constant<long, currPos>>>::type;
    };

    template <long currPos> struct enumerate_impl<currPos> {
      using type = List<>;
    };
    using type = enumerate_impl<0, Ts...>::type;
  };

  /*
  Adapter between different type lists
  */
  template <typename TypeList, template <typename...> typename Other>
  struct apply;

  template <typename... Ts, template <typename...> typename Other>
  struct apply<List<Ts...>, Other> {
    using type = Other<Ts...>;
  };

  template <typename Head, typename... Rest> struct back<List<Head, Rest...>> {
    using type = back<List<Rest...>>::type;
  };

  template <typename Head> struct back<List<Head>> {
    using type = Head;
  };

  /*
  Returns true if for each index i in [0, len(List1)],
  Predicate(List1[i], List2[i]) is true
  */

  template <template <typename, typename> typename Predicate, typename Head1,
            typename... Rest1, typename Head2, typename... Rest2>
  struct prefix_predicate<Predicate, List<Head1, Rest1...>,
                          List<Head2, Rest2...>> {
    static constexpr bool value =
        Predicate<Head1, Head2>::value &&
        prefix_predicate<Predicate, List<Rest1...>, List<Rest2...>>::value;
  };

  template <template <typename, typename> typename Predicate>
  struct prefix_predicate<Predicate, List<>, List<>> {
    static constexpr bool value = true;
  };
  template <template <typename, typename> typename Predicate, typename... Ts>
  struct prefix_predicate<Predicate, List<>, List<Ts...>> {
    static constexpr bool value = true;
  };

  template <template <typename, typename> typename Predicate, typename... Ts>
  struct prefix_predicate<Predicate, List<Ts...>, List<>> {
    static constexpr bool value = false;
  };

}; // struct list_traits
} // namespace mpl
