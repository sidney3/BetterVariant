#include <cassert>

#include <sidney3/guarded_return.h>
#include <sidney3/operators.h>
#include <sidney3/variant.h>

struct a {};
struct b {};
struct c {};
struct d {};
static constexpr int A_RESPONSE = 1, B_RESPONSE = 2, C_RESPONSE = 3,
                     D_RESPONSE = 4, AUTO_RESPONSE = 5;

//    Passing an auto lambda will catch the default
//    cases for our variant

void auto_test() {
  sidney3::variant<a, b, c, d> v = d{};

  auto res = v || [](a) { return A_RESPONSE; } | [](b) { return B_RESPONSE; } |
                      [](auto) { return AUTO_RESPONSE; };

  assert(res == AUTO_RESPONSE);

  // interesting case -> chronological sequencing
  // matters for auto I.E. it catches the D case
  // before it can get to that lambda
  auto res2 = v || [](a) { return A_RESPONSE; } | [](b) { return B_RESPONSE; } |
                       [](auto) { return AUTO_RESPONSE; } |
                       [](d) { return D_RESPONSE; };
  assert(res2 == AUTO_RESPONSE);
}
void base_functionality_test() {
  // Base use:
  // make a variant and spread it across
  // multiple lambdas

  sidney3::variant<a, b, c> v = a{};

  auto res = v || [](a) { return A_RESPONSE; } | [](b) { return B_RESPONSE; } |
                      [](c) { return C_RESPONSE; };

  assert(res == A_RESPONSE);

  res = v || [](b) { return B_RESPONSE; } | [](a) { return A_RESPONSE; } |
                 [](const c &) { return C_RESPONSE; };

  assert(res == A_RESPONSE);

  // alternate use!
  // create one functor and have it apply across
  // multiple variants

  auto functor = [](a) { return A_RESPONSE; } | [](b) { return B_RESPONSE; } |
                 [](c) { return C_RESPONSE; };

  sidney3::variant<a, b, c> holds_a = a{};
  sidney3::variant<a, b, c> holds_b = b{};
  sidney3::variant<a, b, c> holds_c = c{};

  assert((holds_a || functor) == A_RESPONSE);
  assert((holds_b || functor) == B_RESPONSE);
  assert((holds_c || functor) == C_RESPONSE);
}

void nested_test() {
  sidney3::variant<a, sidney3::variant<b, c>> nested = b{};

  auto res = nested || [](a) {
    return A_RESPONSE;
  } | [](sidney3::variant<b, c> inner) {
    return inner || [](b) { return B_RESPONSE; } | [](c) { return C_RESPONSE; };
  };
  assert(res == B_RESPONSE);
}

void guarded_return_test() {
  sidney3::variant<int, char> v = 0;

  static constexpr int INT_RETURN = 0, AUTO_RETURN = 1;

  int res = v || [](int x) -> sidney3::GuardedReturn<int> {
    if (x == 0) {
      return std::nullopt;
    }
    return INT_RETURN;
  } | [](auto &&) { return AUTO_RETURN; };

  assert(res == AUTO_RETURN);
}

/* void no_compile_test() { */
/*    */
/*   sidney3::variant<a, b, c> nested = b{}; */
/*  */
/*   nested || [](b){} | [](a){}; */
/* } */

void test_variant() {
  base_functionality_test();
  nested_test();
  auto_test();
  guarded_return_test();
}
