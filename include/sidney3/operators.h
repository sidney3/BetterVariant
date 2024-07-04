#include <sidney3/functor.h>
#include <sidney3/variant.h>
#include <sidney3/visit.h>

#include <mpl/list_traits.h>



template<typename ... Ts, typename Functor>
sidney3::FunctorTypeTraits<Functor>::return_type
operator|| (sidney3::variant<Ts...>& variant, Functor& func)
{
    using decayed_args =
            typename mpl::list_traits<list>::transform<
                typename sidney3::FunctorTypeTraits<Functor>::arg_types,
                std::decay>::type;

    static_assert(mpl::list_traits<list>::set_equals<

            decayed_args,
            list<std::decay_t<Ts>...>

            >::value 
            
            || mpl::list_traits<list>::contains<
                decayed_args, 
                mpl::types::auto_arg>::value);

    return visit(variant, func);
}

template<typename ... Ts, typename Functor>
sidney3::FunctorTypeTraits<Functor>::return_type
operator|| (sidney3::variant<Ts...>& variant, Functor&& func)
{
    // input args to all functions in Functor
    using decayed_args =
            typename mpl::list_traits<list>::transform<
                typename sidney3::FunctorTypeTraits<Functor>::arg_types,
                std::decay>::type;

    static_assert(mpl::list_traits<list>::set_equals<

            decayed_args,
            list<std::decay_t<Ts>...>

            >::value 
            
            || mpl::list_traits<list>::contains<
                decayed_args, 
                mpl::types::auto_arg>::value);

    return visit(variant, func);
}

template<typename T, typename V>
sidney3::Functor<T,V> operator|(T&& t, V&& v)
{
    return sidney3::Functor<T,V>{std::forward<T>(t), std::forward<V>(v)};
}

