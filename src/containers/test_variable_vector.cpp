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

using variable_vector_t = variable_vector<int, std::string, double>;
inline void test_base()
{
    /*
        Base functionality
    */
    variable_vector_t vec{2, "hello world", 2.2};
    assert(get<std::string>(vec) == "hello world");
    get<std::string>(vec) = "hey world";
    assert(get<std::string>(vec) == "hey world");
}

inline void test_move_construction()
{
    /*
        More interesting stuff: make sure move construction is feasible

        In move construction, we don't make any new copies
        of internal members
    */
    variable_vector<copy_tracker> v{copy_tracker{}};
    auto move_ct = ::get<copy_tracker>(v).move_count;
    auto copy_ct = ::get<copy_tracker>(v).copy_count;
    long moved_before = *move_ct;
    long copy_before = *copy_ct;
    variable_vector<copy_tracker> w{std::move(v)};
    long moved_after = *move_ct;
    long copy_after = *copy_ct;

    assert(moved_after - moved_before == 1);
    assert(copy_before - copy_after == 0);
}

void test_emplace()
{
    variable_vector_t v;
    v.emplace_back(2);
    v.emplace_back("Hello world");
    v.emplace_back(2.2);

    assert(get<int>(v) == 2);
    assert(get<std::string>(v) == "Hello world");
    assert(get<double>(v) == 2.2);

    v.pop_back();
    v.emplace_back(2.9);

    assert(get<double>(v) == 2.9);

}

void test_variable_vector()
{
   test_base();
   test_move_construction();
   test_emplace();
}
