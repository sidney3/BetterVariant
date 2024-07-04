#include <iostream>
#include <string>
// compile time tests - just including is fine
#include <mpl/test_list_traits.h>
#include <mpl/test_util.h>
#include <mpl/test_map.h>

#include <containers/test_template_union.h>

#include <sidney3/test_visit.h>
#include <sidney3/test_functor.h>
#include <sidney3/test_variant.h>

int main()
{
    test_template_union();
    test_functor();
    test_visit();
    test_variant();

}
