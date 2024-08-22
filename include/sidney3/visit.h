#pragma once

#include <sidney3/variant.h>
#include <mpl/list.h>
#include <sidney3/tags.h>

#include <stdexcept>

template<typename VariantList, typename CurrList, typename Functor>
struct visit_impl;


template<
    typename ... Ts,
    typename Head, typename ... Rest,
    typename Functor
    >
struct visit_impl<mpl::list<Ts...>, mpl::list<Head, Rest...>, Functor>
{
    static typename 
    sidney3::FunctorTypeTraits<Functor>::return_type
    apply(
            sidney3::variant<Ts...>& variant, Functor fn)
    {
        if(std::holds_alternative<Head>(variant))
        {
            return fn(std::get<Head>(variant));
        }
        else
        {
            return visit_impl<mpl::list<Ts...>, mpl::list<Rest...>, Functor>::apply
                (variant, std::forward<decltype(fn)>(fn));
        }
    }
};

template<typename ... Ts, typename Functor>
struct visit_impl<mpl::list<Ts...>, mpl::list<>, Functor>
{
    static 
    sidney3::FunctorTypeTraits<Functor>::return_type
    apply(...)
    {
        throw std::runtime_error{"FATAL: case not matched"};
    }
};



template<typename ... Ts, typename Functor>
    requires std::is_same_v<
            typename Functor::tag, 
            sidney3::functor_tag>
auto visit(sidney3::variant<Ts...>& variant, Functor&& fn)
{
    return visit_impl<mpl::list<Ts...>, mpl::list<Ts...>, Functor>::apply(variant, std::forward<Functor>(fn));
}

template<typename ... Ts, typename Functor>
    requires std::is_same_v<
            typename Functor::tag, 
            sidney3::functor_tag>
auto visit(sidney3::variant<Ts...>& variant, Functor& fn)
{
    return visit_impl<mpl::list<Ts...>, mpl::list<Ts...>, Functor>::apply(variant, std::forward<Functor>(fn));
}

