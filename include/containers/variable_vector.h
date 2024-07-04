#pragma once
#include <vector>
#include <mpl/util.h>
#include <mpl/list.h>

namespace containers::igored
{
/*
   A vector that can be indexed by type: think a std::tuple where
   we can use get<int> to access the int member of the tuple.

    Critically, this struct wants to be efficient to move from.
    We expect many of these vectors that (A) contain different
    elements but (B) want to be constructed from each other.

    So we put in the definition of each such vector the FinalVariant
    that we expect it to hold. An example:

    If we have a variable_vector<char,int> and a 
    variable_vector<char,int,long long>, the underlying type for
    these two vectors is fundamentally different 

    (std::variant<char,int> versus std::variant<char,int,long long>)

    So one cannot be constructed from the other. Therefore, to
    allow this construction, we place in the type definition the
    largest possible variant that we expect (in the above case
    std::variant<char,int,long long>) to faciliate move construction.
*/
template<typename FinalVariant, typename ... CurrValues>
struct variable_vector;

/* template<typename ... CurrValues> */
/* struct variable_vector<CurrValues...> : variable_vector<list<CurrValues...>, CurrValues...>  */
/* {}; */

template<typename ... Ts, typename ... CurrValues>
struct variable_vector<list<Ts...>, CurrValues...> : std::vector<std::variant<Ts...>>
{
    static_assert(mpl::is_unique<Ts...>::value, 
            "variable vector can index by type and so type arguments \
            mut be distinct");
    using BaseT = std::vector<std::variant<Ts...>>;
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

    template<typename A, typename C>
    struct binary_is_constructible
    {
        static constexpr bool value = std::is_constructible_v<C,A>;
    };
    template<typename ... Us>
        requires(std::is_constructible_v<CurrValues, Us>&&...)
    variable_vector(Us&&... us)
    {
        init_parent(std::forward<Us>(us)...);
    }
    //@todo add some nice assertion here to check if this
    // is actually a legal construction
    template<typename ... Us, typename ... Rest>
    variable_vector(variable_vector<Us...>&& rhs, Rest&&... rest)
        : BaseT(std::move(static_cast<BaseT>(rhs)))
    {
        init_parent(std::forward<Rest>(rest)...);
    }
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

template<typename T, typename BaseVariant, typename ... Ts>
T &get(variable_vector<BaseVariant, Ts...>& vec)
{
    static_assert(mpl::contains<T, Ts...>::value, 
            "get called on a nonexistent type");
    // get_impl returns a std::variant that we then get from again...
    return std::get<T>(get_impl<T, variable_vector<BaseVariant, Ts...>, 0, Ts...>::apply(vec));
}
} // namespace containers
