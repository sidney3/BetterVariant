#include <iostream>
#include <sidney3/xvariant.h>

int main()
{
    sidney3::xvariant<std::variant<int, double, const char*>, int, double, const char*> v;

}
