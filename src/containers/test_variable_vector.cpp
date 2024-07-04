#include <containers/variable_vector.h>
#include <string>
#include <cassert>

using namespace containers;

struct copy_tracker
{
    std::shared_ptr<long> copy_count ;
    std::shared_ptr<long> move_count ;
    copy_tracker()
        : copy_count(std::make_shared<long>(0))
          , move_count(std::make_shared<long>(0))
    {}
    copy_tracker(const copy_tracker& rhs)
        : copy_count(rhs.copy_count)
          , move_count(rhs.move_count)
    {
        (*move_count)++;
    }
    copy_tracker(copy_tracker&& rhs)
        : copy_count(std::move(rhs.copy_count))
          , move_count(std::move(rhs.move_count))
    {
        (*copy_count)++;
    }
};

void test_variable_vector()
{
}
