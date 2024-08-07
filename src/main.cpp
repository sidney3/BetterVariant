// compile time tests - just including is fine
#include <mpl/test_list_traits.h>
#include <mpl/test_util.h>
#include <mpl/test_functional.h>

#include <sidney3/test_visit.h>
#include <sidney3/test_functor.h>
#include <sidney3/test_variant.h>

int main()
{
    test_functor();
    test_visit();

    // this is the important test suite that verifies functionality
    test_variant();
}
