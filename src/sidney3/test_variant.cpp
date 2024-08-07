#include <cassert>
#include <string>

#include <sidney3/variant.h>
#include <sidney3/operators.h>

struct a{};
struct b{};
struct c{};
struct d{};
    static constexpr int
        A_RESPONSE = 1,
        B_RESPONSE = 2,
        C_RESPONSE = 3,
        D_RESPONSE = 4,
        AUTO_RESPONSE = 5;


//    Passing an auto lambda will catch the default
//    cases for our variant

void auto_test()
{
    sidney3::variant<a,b,c,d> v = d{};
    
    auto res = v || 
            [](a){return A_RESPONSE;} |
            [](b){return B_RESPONSE;} |
            [](auto){return AUTO_RESPONSE;};

    assert(res == AUTO_RESPONSE);

    // interesting case -> chronological sequencing
    // matters for auto I.E. it catches the D case
    // before it can get to that lambda
    auto res2 = v || 
            [](a){return A_RESPONSE;} |
            [](b){return B_RESPONSE;} |
            [](auto){return AUTO_RESPONSE;} |
            [](d){return D_RESPONSE;};
    assert(res2 == AUTO_RESPONSE);

}
void base_functionality_test()
{
    // Base use:
    // make a variant and spread it across
    // multiple lambdas


    sidney3::variant<a,b,c> v = a{};
    
    auto res = v || 
            [](a){return A_RESPONSE;} |
            [](b){return B_RESPONSE;} |
            [](c){return C_RESPONSE;};
    
    assert(res == A_RESPONSE);
    
    res = v || 
            [](b){return B_RESPONSE;} |
            [](a){return A_RESPONSE;} |
            [](c&){return C_RESPONSE;};
    
    
    assert(res == A_RESPONSE);

    // alternate use!
    // create one functor and have it apply across 
    // multiple variants

    auto functor = 
            [](a){return A_RESPONSE;} |
            [](b){return B_RESPONSE;} |
            [](c){return C_RESPONSE;};


    sidney3::variant<a,b,c> holds_a = a{};
    sidney3::variant<a,b,c> holds_b = b{};
    sidney3::variant<a,b,c> holds_c = c{};

    assert((holds_a || functor) == A_RESPONSE);
    assert((holds_b || functor) == B_RESPONSE);
    assert((holds_c || functor) == C_RESPONSE);

}
void test_variant()
{
    base_functionality_test();
    auto_test();
}

