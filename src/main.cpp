// compile time tests - just including is fine
#include <mpl/test_list_traits.h>
#include <mpl/test_unary_function_traits.h>

#include <sidney3/test_functor.h>
#include <sidney3/test_variant.h>
#include <sidney3/test_visit.h>

int main() {
  test_functor();
  test_visit();

  // this is the important test suite that verifies functionality of our variant
  test_variant();
}
