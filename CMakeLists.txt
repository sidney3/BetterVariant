cmake_minimum_required(VERSION 3.6)

project(XVariant VERSION 1.0 LANGUAGES CXX)

include_directories(include)

set(HEADER_FILES include/sidney3/xvariant.h
                include/mpl/list.h
                include/mpl/map.h)

add_compile_options(-Wall -Wextra -Wpedantic -std=c++20)

add_executable(Demo src/variant_demo.cpp ${HEADER_FILES})
add_executable(Test src/test.cpp ${HEADER_FILES})

