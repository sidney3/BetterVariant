#include <sidney3/functor.h>
#include <sidney3/variant.h>
#include <sidney3/visit.h>



template<typename ... Ts, typename Functor>
sidney3::FunctorTypeTraits<Functor>::return_type
operator|| (const sidney3::variant<Ts...>& variant, Functor&& func)
{
    return visit(variant, func);
}

template<typename T, typename V>
sidney3::Functor<T,V> operator|(T&& t, V&& v)
{
    return sidney3::Functor<T,V>{std::forward<T>(t), std::forward<V>(v)};
}

