#pragma once
#include <mpl/list.h>
#include <mpl/util.h>
#include <mpl/tuple.h>
#include <variant>


/*
How do we want to store the functions? We want to be able
to index them by type:


*/
namespace sidney3{
    
template<typename XVariant, typename FunctionsList, typename TypesList>
struct variant_functor;

template<typename XVariant, typename ... Fs, typename ... Ts>
struct variant_functor<XVariant, mpl::list<Fs...>, mpl::list<Ts...>>
{
    using self = variant_functor<XVariant, mpl::list<Fs...>, mpl::list<Ts...>>;     
    
    using predecessor = variant_functor<XVariant, 
          typename mpl::pop_back<mpl::list<Fs...>>::type, 
          typename mpl::pop_back<mpl::list<Ts...>>::type>;

    // return type of our functor will be the return type of the first function in the list
    using return_type = decltype(std::declval<typename mpl::head<mpl::list<Fs...>>::type>()
                                (std::declval<typename mpl::head<mpl::list<Ts...>>::type>()));

    template<typename ... Fns>
    explicit variant_functor(const XVariant* variant, Fns&&... fns)
        : variant(variant), functions(std::make_tuple(fns...))
    {}

    template<typename Fns, typename F, long ... Indices>
    static self make_variant_functor(const XVariant* variant, Fns&& fns, F&& f, std::index_sequence<Indices...>)
    {
        return self{variant, 
                    std::forward<Fns>(std::get<Indices>(fns))..., 
                    std::forward<F>(f)};
    }

    template<typename TypeList>
    struct call_operator_impl;

    template<typename Head, typename ... Rest>
    struct call_operator_impl<mpl::list<Head, Rest...>>
    {
        return_type apply(const self* _this)
        {
            if constexpr (std::holds_alternative<Head>(_this->variant))
            {
                return std::get<Head>(_this->functions)
                        (std::get<Head>(_this->variant));
            }
            else
            {
                return call_operator_impl<mpl::list<Rest...>>::apply(_this->variant);
            }
            
        }
    };
    return_type operator()()
    {
        static_assert(mpl::equals<typename XVariant::types, mpl::list<Ts...>>::value, "all cases must be met");
        return call_operator_impl<mpl::list<Ts...>>::apply(this);
    }

    template<typename F>
    variant_functor<
        XVariant, 
        mpl::list<Fs..., F>, 
        mpl::list<
            Ts..., 
            typename mpl::extract_input_type<
                F, 
                typename XVariant::types>::type
            >
        >
    operator||(F&& func)
    {
        using F_type = mpl::extract_input_type<F, typename XVariant::types>::type;
        using F_return = decltype(std::declval<F>(std::declval<F_type>()));

        static_assert(std::is_same_v<F_return, return_type>,
                "return type of inputted function must match that of other functions");
        static_assert(!std::is_same_v<F_type, mpl::type_not_found>, 
            "input to pattern matching statement should be a single argument\
            function taking one of the variant types");
        static_assert(!mpl::contains<F_type, mpl::list<Ts...>>::value,
                "this case is already covered by a function");

        return make_variant_functor(variant, std::move(functions), std::forward<F>(func));
    }
private:
    const XVariant* variant; 
    mpl::ti_tuple<Fs...> functions;
};

struct VariantWrapper
{
    template<typename ... Ts>
    using apply = std::variant<Ts...>;
};

template<
    typename TypesList,
    typename BaseVariantWrapper = mpl::apply<TypesList, VariantWrapper>::type
    >
struct xvariant : BaseVariantWrapper
{
    using self = xvariant<BaseVariantWrapper, TypesList>;
    using types = TypesList; 

    // maintain compatability with base variant impl
    using BaseVariantWrapper::BaseVariantWrapper;
    xvariant(BaseVariantWrapper&& v)
        : BaseVariantWrapper(std::move(v))
    {}
    xvariant(const BaseVariantWrapper& v)
        : BaseVariantWrapper(v)
    {}


    template<typename F>
    variant_functor<
        self, 
        mpl::list<F>, 
        mpl::list<typename mpl::extract_input_type<F,TypesList>::type>
    >
    operator>>(F&& fn)
    {
        using return_type = 
            variant_functor<
                self, 
                mpl::list<F>, 
                mpl::list<typename mpl::extract_input_type<F,TypesList>::type>
            >;
        static_assert(std::is_same_v<decltype(this), self>);
        return return_type{this, std::forward<F>(fn)};
    }
};


template<typename ... Ts>
using variant = xvariant<mpl::list<Ts...>>;
}
