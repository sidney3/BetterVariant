#pragma once
#include <variant>
#include <type_traits>
#include <mpl/list_traits.h>
#include <mpl/map.h>

template<typename ... Fs>
struct function_container
{
public:
    using any_func = std::variant<Fs...>;
    template<typename ... Ts>
        requires (std::is_same_v<Ts,Fs>&&...)
    explicit function_container(Ts&&... fns)
        : _functions({fns...})
    {}
    template<typename ... Ts>
        requires (std::is_same_v<Fs,Ts>&&...)
    explicit function_container(function_container<Ts...>&& rhs) 
        : _functions(std::move(rhs._functions))
    {}
    template<typename T>
    T &get()
    {
    }

    std::vector<any_func> _functions;

    using enumerated_fs =
        mpl::list_traits<list>::enumerate<list<Fs...>>::type;
};
