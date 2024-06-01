#pragma once
#include <mpl/list.h>
#include <mpl/util.h>
#include <mpl/tuple.h>
#include <variant>

namespace sidney3{
    
template<typename XVariant, typename FunctionsList, typename TypesList>
struct variant_functor;

template<typename XVariant, typename ... Fs, typename ... Ts>
struct variant_functor<XVariant, mpl::list<Fs...>, mpl::list<Ts...>>
{
    using self = variant_functor<XVariant, mpl::list<Fs...>, mpl::list<Ts...>>;     

    // return type of our functor will be the return type of the first function in the list
    // after adding each new function, we check that its return type is the same as this
    using return_type = mpl::function_traits<typename mpl::head<mpl::list<Fs...>>::type>::return_type;

    template<typename ... Fns>
    explicit variant_functor(const XVariant* variant, Fns&&... fns)
        : variant(variant), functions(std::forward<Fns>(fns)...)
    {}

    /*
        Use template argument deduction to traverse our Types / Functions list when our
        metafunctor gets invoked. This is because we can only know at runtime which
        type our variant holds.
    */
    template<typename TypeList, typename FunctionsList>
    struct call_operator_impl;

    template<typename THead, typename ... TRest,
             typename FHead, typename ... FRest>
    struct call_operator_impl<
        mpl::list<THead, TRest...>, 
        mpl::list<FHead, FRest...>
        >
    {
        static decltype(auto) apply(self* _this)
        {
            if(std::holds_alternative<THead>(*_this->variant))
            {


                auto obj = std::get<THead>(*_this->variant);
                return (mpl::get<FHead>(_this->functions))(obj);
            }
            else
            {
                return call_operator_impl<mpl::list<TRest...>, mpl::list<FRest...>>::apply(_this);
            }
            
        }
    };

    /*
        Although we will never reach this point (as we assert in our call operator that we have all types met),
        this struct definition will still get created by the compiler, and so it must be defined (even though
        it doesn't make sense).
    */
    template<>
    struct call_operator_impl<mpl::list<>, mpl::list<>>
    {
        static return_type apply(self*) {
            __builtin_unreachable();
        }
    };

    return_type operator()()
    {
        static_assert(mpl::equals<typename XVariant::types, mpl::list<std::decay_t<Ts>...>>::value, "all cases must be met");
        return call_operator_impl<mpl::list<Ts...>, mpl::list<Fs...>>::apply(this);
    }

    /*
    Takes an already existing metafunctor and add another lambda to it. Most of the bulk here is just assertions
    */
    template<typename F>
    decltype(auto)
    operator||(F&& func)
    {
        using F_args = mpl::function_traits<F>::type;
        static_assert(mpl::size<F_args>::value == 1, "lambda inputted must take a single argument");

        using raw_F_type = typename mpl::head<F_args>::type;
        using F_type = std::decay_t<raw_F_type>;

        using F_return = decltype(std::declval<F>()(std::declval<raw_F_type>()));
        
        static_assert(std::is_same_v<F_return, return_type>,
                "return type of inputted function must match that of other functions");
        static_assert(!std::is_same_v<F_type, mpl::type_not_found>, 
            "input to pattern matching statement should be a single argument\
            function taking one of the variant types");
        static_assert(!mpl::list_contains<mpl::list<Ts...>, F_type>::value,
                "this case is already covered by a function");

        return variant_functor
            <XVariant, mpl::list<Fs..., F>, mpl::list<Ts..., F_type>>
        {
            variant,
            mpl::get<Fs>(functions)...,
            std::forward<F>(func)
        };
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

    /*
    takes a variant and a single lambda and spits out the initial metafunctor

    Note that as >> is higher precedence than ||, we know that this will
    always get evaluated first (as || operator is not defined between lambdas)
    */
    template<typename F>
    decltype(auto)
    operator>>(F&& fn)
    {
        using F_ = std::decay_t<F>;
        using F_args = mpl::function_traits<F_>::type;
        static_assert(mpl::size<F_args>::value == 1,
                "lambda inputted must take a single argument");
        using F_type = std::remove_reference_t<typename mpl::head<F_args>::type>;
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
