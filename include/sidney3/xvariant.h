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
        : variant(variant), functions(std::forward<Fns>(fns)...)
    {}

    template<typename Fns, typename F, size_t ... Indices>
    decltype(auto) make_variant_functor(const XVariant* variant, Fns&& fns, F&& f, std::index_sequence<Indices...>)
    {
        using F_type = mpl::head<
            typename mpl::function_traits<F>::type
            >::type;
        using return_type = variant_functor<XVariant,
              mpl::list<Fs..., F>,
              mpl::list<Ts..., F_type>
              >;
        /* using tl = mpl::list<decltype(std::get<Indices>(fns))...>; */

        /* static_assert(std::is_same_v<tl, mpl::list<int,double>>); */
        return return_type{variant, 
                    std::get<Indices>(fns)..., 
                    std::forward<F>(f)};
    }


    template<typename TypeList, typename FunctionsList>
    struct call_operator_impl;


    template<typename THead, typename ... TRest,
             typename FHead, typename ... FRest>
    struct call_operator_impl<
        mpl::list<THead, TRest...>, 
        mpl::list<FHead, FRest...>
        >
    {
        static return_type apply(self* _this)
        {
            if(std::holds_alternative<THead>(*_this->variant))
            {
                return mpl::get<FHead>(_this->functions)
                        (std::get<THead>(*_this->variant));
            }
            else
            {
                return call_operator_impl<mpl::list<TRest...>, mpl::list<FRest...>>::apply(_this);
            }
            
        }
    };

    template<>
    struct call_operator_impl<mpl::list<>, mpl::list<>>
    {
        static return_type apply(self* _this)
        {
    
        }
    };
    decltype(auto) operator()()
    {
        static_assert(mpl::equals<typename XVariant::types, mpl::list<Ts...>>::value, "all cases must be met");
        return call_operator_impl<mpl::list<Ts...>, mpl::list<Fs...>>::apply(this);
    }

    template<typename F>
    decltype(auto)
    operator||(F&& func)
    {
        using F_args = mpl::function_traits<F>::type;
        static_assert(mpl::size<F_args>::value == 1, "lambda inputted must take a single argument");

        using F_type = mpl::head<F_args>::type;
        /* using F_return = decltype(std::declval<F>(std::declval<F_type>())); */
        /*  */
        /* static_assert(std::is_same_v<F_return, return_type>, */
        /*         "return type of inputted function must match that of other functions"); */
        static_assert(!std::is_same_v<F_type, mpl::type_not_found>, 
            "input to pattern matching statement should be a single argument\
            function taking one of the variant types");
        static_assert(!mpl::contains<F_type, mpl::list<Ts...>>::value,
                "this case is already covered by a function");

        return make_variant_functor(variant, 
                std::move(functions), 
                std::forward<F>(func),
                std::make_index_sequence<mpl::size<mpl::list<Fs...>>::value>());
    }
private:
    const XVariant* variant; 
    mpl::ti_tuple<mpl::list<Fs...>> functions;
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
    using self = xvariant<TypesList, BaseVariantWrapper>;
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
    decltype(auto)
    operator>>(F&& fn)
    {
        using F_ = std::remove_reference_t<F>;
        using F_args = mpl::function_traits<F_>::type;
        static_assert(mpl::size<F_args>::value == 1,
                "lambda inputted must take a single argument");
        using F_type = mpl::head<F_args>::type;
        using return_type = 
            variant_functor<
                self, 
                mpl::list<F>, 
                mpl::list<F_type>
            >;
        return return_type{this, std::forward<F>(fn)};
    }
};


template<typename ... Ts>
using variant = xvariant<mpl::list<Ts...>>;
}
