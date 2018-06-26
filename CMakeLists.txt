project(rose-slice CXX)
cmake_minimum_required(VERSION 3.3)
add_definitions("-std=c++11")

# Everything below is for tests

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(rose_slice_test
               test/slice_test.cpp
               test/mut_slice_test.cpp)

target_link_libraries(rose_slice_test ${CONAN_LIBS})
include_directories(include)

enable_testing()
include(GoogleTest)
gtest_add_tests(TARGET rose_slice_test)
