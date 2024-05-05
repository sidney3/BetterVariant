#include <utility>
#include <functional>
#include <type_traits>

template<typename T, long U>
struct reduce 
{
    using type = T;
};

template<typename U, typename IndexSequence>
struct FunctionHolderImpl;


template<typename U, long ... Indices>
struct FunctionHolderImpl<U, std::index_sequence<Indices...>>
{
    using value = std::function<void(typename reduce<U, Indices>::type...)>;
};

template<long N>
struct FunctionHolder
{
    using func = FunctionHolderImpl<double, std::make_index_sequence<N>>::value;
};

static_assert(std::is_same_v<FunctionHolder<3>::func, std::function<void(double, double, double)>>);

int main()
{
}
