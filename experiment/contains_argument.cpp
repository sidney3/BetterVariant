#include <utility>
#include <type_traits>

struct type_not_found {};

template<
    typename F, 
    typename T, 
    typename = decltype(std::declval<F>()(std::declval<T>()))>
struct invoke_f
{
    using type = std::true_type;
};


template<typename F, typename T>
struct invoke_f<F,T,void>
{
    using type = std::false_type;
};

template<typename F, typename ... Ts>
struct extract_input_type;

template<typename F, typename Head, typename ... Rest>
struct extract_input_type<F, Head, Rest...>
{
    using type = 
        std::conditional<invoke_f<F, Head>::type::value,
            Head,
            typename extract_input_type<F, Rest...>::type>::type;
};

template<typename F>
struct extract_input_type<F>
{
    using type = type_not_found;
};


int main()
{
    auto f = [](int i)
    {
        return 2;
    };

    using arg = extract_input_type<decltype(f), int, double, char>::type;

    static_assert(std::is_same_v<arg, int>);

}

