#include <containers/template_union.h>
#include <cassert>

using namespace containers;

void test_template_union()
{
    template_union<int, char, double> tu;
    
    get<int>(tu) = 5;
    assert(get<int>(tu) == 5);
    get<int>(tu) = 8;
    assert(get<int>(tu) == 8);
    get<char>(tu) = 'a';
    assert(get<char>(tu) == 'a');
}
