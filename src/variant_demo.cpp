#include <iostream>
#include <string>
#include <sidney3/xvariant.h>
#include <mpl/tuple.h>
#include <utility>
#include <tuple>

int main()
{
    sidney3::variant<int, std::string> v = 2;
    auto m = v >>
        [](int obj)
        {
            std::cout << "contains int: " << obj << "\n";
        };

}
