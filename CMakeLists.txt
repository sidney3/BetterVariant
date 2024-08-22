cmake_minimum_required(VERSION 3.6)

project(Variant VERSION 1.0 LANGUAGES CXX)

include_directories(include)

set(HEADER_FILES include/sidney3/variant.h
                 include/sidney3/test_variant.h
                 include/sidney3/functor.h
                 include/sidney3/test_functor.h
                 include/sidney3/test_variant.h
                 include/sidney3/visit.h
                 include/sidney3/test_visit.h

                include/mpl/list.h
                include/mpl/list_traits.h
                include/mpl/test_list_traits.h
                include/mpl/function_traits.h
                include/mpl/test_function_traits.h
                include/mpl/types.h
                include/mpl/unary_function_traits.h
                include/mpl/test_unary_function_traits.h
                include/mpl/functional.h
                )

set(SOURCE_FILES    src/sidney3/test_functor.cpp
                    src/sidney3/test_variant.cpp
                    src/sidney3/test_visit.cpp
                    
                    )

add_compile_options(-Wall -Wextra -Wpedantic -std=c++2b -g)

add_executable(Main src/main.cpp ${HEADER_FILES} ${SOURCE_FILES})

