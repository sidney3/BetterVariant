#pragma once
#include <mpl/util.h>

namespace containers {
template<typename Head, typename ... Rest>
struct template_union
{
    union {
        Head head;
        template_union<Rest...> rest;
    };
};

template<typename T>
struct template_union<T>
{
    T head;
};

template<typename T, typename Head, typename ... Rest>
T &get_impl(template_union<Head, Rest...>& temp_union)
{
    if constexpr(std::is_same_v<T, Head>)
    {
        return temp_union.head;
    }
    else
    {
        return get_impl<T, Rest...>(temp_union.rest);
    }
}

template<typename T, typename ... Ts>
T &get(template_union<Ts...>& temp_union)
{
    static_assert(mpl::contains<T, Ts...>::value, 
            "can only get type contained in the union");
    return get_impl<T, Ts...>(temp_union);
}

template<typename T, typename ... Ts>
template_union<Ts...>
make_template_union(T&& t)
{
    static_assert(mpl::contains<T, Ts...>::value);

    template_union<Ts...> res;
}
} // namespace containers
