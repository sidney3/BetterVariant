#include <iostream>
#include <string>
#include <sidney3/xvariant.h>
#include <mpl/tuple.h>

int main()
{
    sidney3::variant<int, std::string> v = "asf";

    (v >> 
        [](int obj)
        {
            std::cout << "contains an integer " << obj << "\n";
        }
        ||  [](std::string obj)
        {
            std::cout << "contains a string " << obj << "\n";
        })();

}
