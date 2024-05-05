#include <iostream>

struct Widget
{
    int i;
    Widget(int i) : i(i)
    {}

    int operator>>(int rhs)
    {
        return rhs + i;
    }
};

int main()
{
    int m = 4;
    Widget w{8};
    
    std::cout << (w >> m) << "\n";
}
