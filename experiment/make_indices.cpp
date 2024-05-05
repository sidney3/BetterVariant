#include <utility>
#include <iostream>

template<long ... Indices>
void foo()
{
    ((std::cout << Indices) << ...);
}

int main()
{

}
