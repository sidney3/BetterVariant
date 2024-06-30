#pragma once
#include <vector>
#include <mpl/util.h>

namespace containers
{
template<typename ... Ts>
struct variable_vector : std::vector<std::variant<Ts...>>
{
    static_assert(mpl::is_unique<Ts...>::value, 
            "variable vector can index by type and so type arguments \
            mut be distinct");
    using BaseT = std::vector<std::variant<Ts...>>;
    using BaseT::emplace_back;
    using BaseT::operator[];

    variable_vector() = default;

    template<typename Head, typename ... Rest>
    void init_parent(Head&& head, Rest&&... rest)
    {
        BaseT::emplace_back(std::forward<Head>(head));
        init_parent(rest...);
    }

    // no-op for the base case of initialization
    void init_parent()
    {}

    template<typename ... Us>
        requires ( std::is_constructible_v<Ts,Us>&&... )
    variable_vector(Us&&... us)
    {
        init_parent(std::forward<Us>(us)...);
    }

    variable_vector(variable_vector<Ts...>&& rhs)
        : BaseT{std::move(static_cast<BaseT>(rhs))}
    {}
};
template<typename Target, 
    typename VariableVector, 
    long CurrIndex,
    typename ... ToTry>
struct get_impl;

template<typename Target, 
    typename ... Ts, 
    long CurrIndex,
    typename Head, 
    typename ... Rest>
struct get_impl<Target, variable_vector<Ts...>, CurrIndex, Head, Rest...>
{
    static auto &apply(variable_vector<Ts...>& vec)
    {
        if constexpr(std::is_same_v<Head, Target>)
        {
            return vec[CurrIndex];
        }
        else
        {
            return get_impl<Target, 
                variable_vector<Ts...>,
                CurrIndex + 1,
                Rest...>::apply(vec);
        }
    }
};


template<typename T, typename ... Ts>
T &get(variable_vector<Ts...>& vec)
{
    static_assert(mpl::contains<T, Ts...>::value, 
            "get called on a nonexistent type");
    // get_impl returns a std::variant that we then get from again...
    return std::get<T>(get_impl<T, variable_vector<Ts...>, 0, Ts...>::apply(vec));
}
} // namespace containers
