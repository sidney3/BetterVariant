#include <sidney3/functor.h>
#include <cassert>

using namespace sidney3;

void test_base_functionality()
{
    static constexpr int INT_RES = 0, CHAR_RES = 1;

    auto int_pred = [](int)
    {
        return INT_RES;
    };

    auto char_pred = [](char)
    {
        return CHAR_RES;
    };

    functor<decltype(int_pred), decltype(char_pred)> f{int_pred, char_pred};
    assert(f(5) == INT_RES);
    assert(f('a') == CHAR_RES);

}

void test_weird_signatures()
{

    auto int_pred = [](const int&)
    {
        return true;
    };
    functor<decltype(int_pred)> f(int_pred);
    // will still bind even though non-exact signature match
    assert(f(2));
}
void test_functor()
{
    test_base_functionality();
    test_weird_signatures();
}
