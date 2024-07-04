#include <cassert>
#include <string>

#include <sidney3/variant.h>
#include <sidney3/operators.h>

void base_functionality_test()
{
    struct a{};
    struct b{};
    struct c{};
    
    sidney3::variant<a,b,c> v = a{};
    
    auto res = (v || 
            [](a){return 1;} |
            [](b){return 2;} |
            [](c){return 3;});
    
    assert(res == 1);
    
    res = (v || 
            [](b){return 2;} |
            [](a){return 1;} |
            [](c){return 3;});
    
    assert(res == 1);
}
void test_variant()
{
    base_functionality_test();
}

