#include <iostream>
#include <string>
#include <sidney3/xvariant.h>

int main()
{
    sidney3::variant<int, std::string> v = "asf";

    (v >> 
        [](int& obj)
        {
            std::cout << "contains an integer " << obj << "\n";
        }
        || [](std::string obj)
        {
            std::cout << "contains a string " << obj << "\n";
        })();


    static_assert(std::is_same_v<std::decay_t<const int&>, int>);
}
