#include <iostream>
#include <type_traits>

template <typename Head, typename... Tail>
struct TemplateUnion {
    union {
        Head head;
        TemplateUnion<Tail...> tail;
    };
};

template <typename T>
struct TemplateUnion<T> {
  T head;
};

template <typename T, typename Head, typename... Tail>
T& get(TemplateUnion<Head, Tail...>& tu) {
    if constexpr(std::is_same_v<T, Head>) {
      return tu.head;
    } else {
        return get<T>(tu.tail);
    }
}
