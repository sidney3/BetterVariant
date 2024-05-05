#include <mpl/map.h>
#include <mpl/list.h>
#include <tuple>

/*
   A type-indexed wrapper of std::tuple. 
   (i.e. for a tuple MyTuple of type mpl::tuple<T,U>, we can call
   std::get<U>(MyTuple))
   
   As we index into the tuple by type we cannot have repeated types.

   As this is only used internally, we continue to use the type_list
   interface. That said, as this certainly has applications outside of
   this library, we include an implementation in comments at the bottom
   of this file
*/
namespace mpl
{
    template<typename TypeList>
    struct make_indexed_map;

    template<typename ... Ts>
    struct make_indexed_map<type_list<Ts...>>
    {

    };


    template<typename TypeList>
    struct ti_tuple;

    template<typename ... Ts>
    struct ti_tuple<type_list<Ts...>>
    {
        template<typename T>
        T get()
        {
        }
    private:
        std::tuple<Ts...> internal_tuple;

    };
}
