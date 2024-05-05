#include <iostream>
#include <string>
#include <sidney3/xvariant.h>

int main()
{
    sidney3::variant<int, std::string> v = "Hello World!";

    auto fn = v >> 
        [](const int& obj)
        {
            std::cout << "contains an integer " << obj << "\n";
        }
        || [](const std::string& obj)
        {
            std::cout << "contains a string " << obj << "\n";
        };

    fn();
}
