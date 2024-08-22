#include <sidney3/functor.h>
#include <sidney3/operators.h>

#include <cassert>

void functor_base_functionality_test() {
  static constexpr int INT_RESPONSE = 0, CHAR_RESPONSE = 1, DOUBLE_RESPONSE = 2,
                       AUTO_RESPONSE = 3;

  auto functor = [](int) { return INT_RESPONSE; } |
                 [](char) { return CHAR_RESPONSE; } |
                 [](double) { return DOUBLE_RESPONSE; } |
                 [](auto) { return AUTO_RESPONSE; };

  assert(functor(2) == INT_RESPONSE);
  assert(functor('a') == CHAR_RESPONSE);
  assert(functor(2.2) == DOUBLE_RESPONSE);
  assert(functor("ASBC") == AUTO_RESPONSE);

  struct test_type {};
  test_type ts{};
  assert(functor(ts) == AUTO_RESPONSE);
}

/*
    Passing lambdas with signatures that don't quite
    match the inputted types still works
*/
void reference_args() {
  static constexpr int INT_RESPONSE = 0, CHAR_RESPONSE = 1, DOUBLE_RESPONSE = 2,
                       AUTO_RESPONSE = 3;

  auto functor = [](const int &) { return INT_RESPONSE; } |
                 [](const char &) { return CHAR_RESPONSE; } |
                 [](const double &) { return DOUBLE_RESPONSE; } |
                 [](auto &&) { return AUTO_RESPONSE; };

  assert(functor(2) == INT_RESPONSE);
  assert(functor('a') == CHAR_RESPONSE);
  assert(functor(2.2) == DOUBLE_RESPONSE);
  assert(functor("ASBC") == AUTO_RESPONSE);

  struct test_type {};
  test_type ts{};
  assert(functor(ts) == AUTO_RESPONSE);
}
void test_functor() {
  functor_base_functionality_test();
  reference_args();
}
