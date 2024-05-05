#include <variant>
#include <mpl/list.h>
#include <mpl/map.h>
#include <mpl/util.h>


/*
How do we want to store the functions? We want to be able
to index them by type:


*/
namespace sidney3{
    
template<typename XVariant, typename FunctionsList, typename TypesList>
struct variant_functor;

template<typename XVariant, typename ... Fs, typename ... Ts>
struct variant_functor<XVariant, mpl::type_list<Fs...>, mpl::type_list<Ts...>>
{
    using self = variant_functor<XVariant, mpl::type_list<Fs...>, mpl::type_list<Ts...>>;     
    
    using predecessor = variant_functor<XVariant, 
          typename mpl::pop_back<mpl::type_list<Fs...>>::type, 
          typename mpl::pop_back<mpl::type_list<Ts...>>::type>;

    template<typename ... Fns>
    explicit variant_functor(Fns&&... fns)
        : functions(std::make_tuple(fns...))
    {}

    auto operator()()
    {
        static_assert(mpl::equals<typename XVariant::types, mpl::type_list<Ts...>>::value, "all cases must be met");


    }

    template<typename F>
    decltype(auto) operator||(F&& func)
    {
        using F_type = mpl::extract_input_type<F, typename XVariant::types>::type;

        static_assert(!std::is_same_v<F_type, mpl::type_not_found>, 
            "input to pattern matching statement should be a single argument\
            function taking one of the variant types");
        static_assert(!mpl::contains<F_type, mpl::type_list<Ts...>>::value,
                "this case is already covered by a function");


    }
private:
    std::tuple<Fs...> functions;
};



template<typename BaseVariantWrapper, typename TypesList>
struct xvariant
{
    using self = xvariant<BaseVariantWrapper, TypesList>;
    using types = TypesList; 

    template<typename F>
    variant_functor<
        self, 
        mpl::type_list<F>, 
        mpl::type_list<typename mpl::extract_input_type<F,TypesList>::type>
    >
    operator>>(F&& lambda)
    {

    }
};




}
