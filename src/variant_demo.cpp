#include <iostream>
#include <string>
#include <sidney3/xvariant.h>
#include <mpl/tuple.h>
#include <utility>
#include <tuple>

int main()
{
    sidney3::variant<int, std::string> v = 2;
    auto fn = [](int obj)
    {
        std::cout << "contains int: " << obj << "\n";
    };
    static_assert(mpl::takes_T<decltype(fn), int>::value);

}
