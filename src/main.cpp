#include <iostream>
#include <string>
// compile time tests - just including is fine
#include <mpl/test_list_traits.h>
#include <mpl/test_util.h>
#include <mpl/test_map.h>

#include <containers/test_variable_vector.h>
/* #include <mpl/test_unique_tuple.h> */

int main()
{
    test_variable_vector();

    /* sidney3::variant<int, std::string> v = "Hello World!"; */
    /*  */
    /* auto fn = v >>  */
    /*     [](const int& obj) */
    /*     { */
    /*         std::cout << "contains an integer " << obj << "\n"; */
    /*     } */
    /*     || [](const std::string& obj) */
    /*     { */
    /*         std::cout << "contains a string " << obj << "\n"; */
    /*     }; */
    /*  */
    /* fn(); */
}
