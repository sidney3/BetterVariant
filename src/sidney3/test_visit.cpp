#include <sidney3/variant.h>

#include <cassert>

void test_visit() {
  static constexpr char exp_char = 'a';
  sidney3::variant<int, double, char> v = 'a';

  auto fn = [](int const) { return false; } | [](double &) { return false; } |
            [](char &c) { return c == exp_char; };

  assert(visit(fn, v));
}
