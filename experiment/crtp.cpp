#include <iostream>

struct S
{
    int i;
    S(int k) : i(k)
    {}
};

void foo(S s)
{
    std::cout << s.i << "\n";
}

struct T : S
{
    using S::S;

    T(const S& s) : S(s)
    {}
    T(const S&& s) :S(std::move(s))
    {}

    void call_foo()
    {
        foo(*this);
    }
};

int main()
{
    T t{4};
    t.call_foo();
    foo(t);
}
