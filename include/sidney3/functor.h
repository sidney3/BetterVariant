#pragma once
#include <type_traits>
#include <containers/variable_vector.h>
#include <mpl/util.h>
#include <mpl/functional.h>

namespace sidney3
{
template<typename ... Fs>
struct functor
{
private:
    /*
        we don't have std::bind for template parameters
        And so this is a stand in for binding an arg
        to mpl::is_exact_invocable 
    */
    template<typename T>
    struct make_is_invocable_predicate
    {
        template<typename F>
        struct type 
        {
            static constexpr bool value = mpl::is_exact_invocable<F,T>::value;
        };
    };
public:
    // construct from functions
    template<typename ... Fns>
        requires (std::is_same_v<std::decay_t<Fns>, Fs>&&...)
    functor(Fns&&... fns)
        : _data{std::forward<Fns>(fns)...}
    {}

    // construct directly from data
    template<typename FunctorData>
        requires 
            (std::is_same_v<std::decay_t<FunctorData>,
                containers::variable_vector<Fs...>>)
    functor(FunctorData&& data)
        : _data(std::forward<FunctorData>(data))
    {}
    template<typename T>
    auto operator()(T&& arg)
    {
        // find the first function in Fs... that is invocable with T
        using fn = mpl::find_if<
            make_is_invocable_predicate<T>::template type, 
            Fs...>::type;

        static_assert(!std::is_same_v<fn, mpl::types::type_not_found>,
                "Called functor with unconsidered type");

        return containers::get<fn>(_data)(std::forward<T>(arg));
    }
    containers::variable_vector<Fs...> _data;
};
} // namespace s3
