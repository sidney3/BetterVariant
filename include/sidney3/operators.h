#include <sidney3/functor.h>
#include <sidney3/variant.h>
#include <sidney3/visit.h>

#include <mpl/list_traits.h>


using lst = mpl::list_traits<mpl::list>;

template<typename ... Ts, typename Functor>
sidney3::FunctorTypeTraits<Functor>::return_type
operator|| (sidney3::variant<Ts...>& variant, Functor& func)
{
    using decayed_args =
            typename lst::transform<
                typename sidney3::FunctorTypeTraits<Functor>::arg_types,
                std::decay>::type;

    static_assert(lst::set_equals<

            decayed_args,
            mpl::list<std::decay_t<Ts>...>

            >::value 
            
            || lst::contains<
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
            typename lst::transform<
                typename sidney3::FunctorTypeTraits<Functor>::arg_types,
                std::decay>::type;

    static_assert(lst::set_equals<

            decayed_args,
            mpl::list<std::decay_t<Ts>...>

            >::value 
            
            || lst::contains<
                decayed_args, 
                mpl::types::auto_arg>::value);

    return visit(variant, func);
}

template<typename T, typename V>
sidney3::Functor<T,V> operator|(T&& t, V&& v)
{
    return sidney3::Functor<T,V>{std::forward<T>(t), std::forward<V>(v)};
}

