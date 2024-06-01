#include <sidney3/xvariant.h>
#include <cassert>
#include <string>

void base_functionality_test()
{
    struct a{};
    struct b{};
    struct c{};

    sidney3::variant<a,b,c> v = a{};

    auto res = (v >>
            [](a){return 1;} ||
            [](b){return 2;} ||
            [](c){return 3;})();

    assert(res == 1);

    res = (v >>
            [](b){return 2;} ||
            [](a){return 1;} ||
            [](c){return 3;})();

    assert(res == 1);
}
/*
    Difficult to write a test for something that we DON'T want to compile,
    so uncomment the below test to assure that all examples don't compile.
*/
void non_compilation_test()
{
    sidney3::variant<int, double, std::string> v = 5.5;

    /* (v >>  */
    /*  [](int x) */
    /*  {} || */
    /*  [](double y) */
    /*  {})(); */
    /* struct local{}; */
    /* (v >> */
    /*  [](local L) */
    /*  { */
    /*  */
    /*  } || */
    /*  [](int x){} || */
    /*  [](double y) {} || */
    /*  [](std::string S){} */
    /* ); */
}
void run_tests()
{
    non_compilation_test();
    base_functionality_test();
}

int main()
{
    run_tests();
}
