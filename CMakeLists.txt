cmake_minimum_required(VERSION 3.6)

project(Variant VERSION 1.0 LANGUAGES CXX)

include_directories(include)

set(HEADER_FILES include/sidney3/variant.h
                 include/sidney3/test_variant.h
                 include/sidney3/test_functor.h
                 include/sidney3/test_variant.h

                include/mpl/list.h
                include/mpl/list_traits.h
                include/mpl/pair.h
                include/mpl/test_list_traits.h
                include/mpl/types.h
                include/mpl/util.h
                include/mpl/map.h
                include/mpl/test_map.h
                include/mpl/test_util.h
                include/mpl/functional.h
                include/mpl/test_functional.h

                include/containers/unique_tuple.h
                include/containers/test_unique_tuple.h
                include/containers/variable_vector.h
                include/containers/test_variable_vector.h
                )
set(SOURCE_FILES    src/sidney3/test_functor.cpp
                    src/sidney3/test_variant.cpp
                    
                    src/containers/test_unique_tuple.cpp
                    src/containers/test_variable_vector.cpp

                    )

add_compile_options(-Wall -Wextra -Wpedantic -std=c++20 -g)

add_executable(Main src/main.cpp ${HEADER_FILES} ${SOURCE_FILES})

