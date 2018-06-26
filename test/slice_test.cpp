#include <type_traits>

#include <gtest/gtest.h>
#include <rose/slice.hpp>

using rose::Slice;

TEST(Slice, IsBothTrivialAndStandardLayout) {
  EXPECT_TRUE(std::is_standard_layout<Slice<int>>::value);
  EXPECT_TRUE(std::is_trivial<Slice<int>>::value);
  EXPECT_TRUE(std::is_trivially_copyable<Slice<int>>::value);
}

TEST(Slice, CanBeAggregateInitialized) {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
  EXPECT_TRUE(slice.length == 3);
  EXPECT_TRUE(slice.pointer[0] == 1);
  EXPECT_TRUE(slice.pointer[1] == 2);
  EXPECT_TRUE(slice.pointer[2] == 3);
}

TEST(Slice, get) {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
  EXPECT_TRUE(slice.get(0) == 1);
  EXPECT_TRUE(slice.get(1) == 2);
  EXPECT_TRUE(slice.get(2) == 3);
  EXPECT_TRUE(slice[0] == 1);
  EXPECT_TRUE(slice[1] == 2);
  EXPECT_TRUE(slice[2] == 3);

  // Abort: out of bounds
  // EXPECT_TRUE(slice[3] == 0);
}

TEST(Slice, last) {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
  EXPECT_TRUE(slice.last() == 3);
  int arr2[] = {5, 4, 3, 2, 1};
  Slice<int> slice2 = {5, arr2};
  EXPECT_TRUE(slice2.last() == 1);
}

TEST(Slice, slice) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Slice<int> slice = {8, arr};
  auto smaller_slice = slice.slice(3, slice.len());
  EXPECT_TRUE(smaller_slice[0] == 4);
  EXPECT_TRUE(smaller_slice[1] == 5);
  EXPECT_TRUE(smaller_slice[2] == 6);
  EXPECT_TRUE(smaller_slice[3] == 7);
  EXPECT_TRUE(smaller_slice[4] == 8);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[5] == 0);
  auto even_smaller_slice = smaller_slice.slice(1, 3);
  EXPECT_TRUE(even_smaller_slice[0] == 5);
  EXPECT_TRUE(even_smaller_slice[1] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[2] == 7);

  // Abort: invalid arguments
  // auto bad_slice1 = slice.slice(3, 2);
  // auto bad_slice1 = slice.slice(2, 2);
  // auto bad_slice1 = slice.slice(2, 9);
}

TEST(Slice, length) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Slice<int> slice = {8, arr};
  EXPECT_TRUE(slice.len() == 8);
  auto smaller_slice = slice.slice(2, slice.len());
  EXPECT_TRUE(smaller_slice.len() == 6);
  auto even_smaller_slice = smaller_slice.slice(2, 4);
  EXPECT_TRUE(even_smaller_slice.len() == 2);
}

TEST(Slice, empty) {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
  EXPECT_TRUE(!slice.empty());
  int empty_arr[0] = {};
  Slice<int> empty_slice = {0, empty_arr};
  EXPECT_TRUE(empty_slice.empty());
}

TEST(Slice, ptr) {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
  EXPECT_TRUE(slice.ptr()[0] == 1);
  EXPECT_TRUE(slice.ptr()[1] == 2);
  EXPECT_TRUE(slice.ptr()[2] == 3);
}

TEST(Slice, slice_from) {
  int arr[] = {1, 2, 3, 4, 5, 6};
  Slice<int> slice = {6, arr};
  auto smaller_slice = slice.slice_from(3);
  EXPECT_TRUE(smaller_slice[0] == 4);
  EXPECT_TRUE(smaller_slice[1] == 5);
  EXPECT_TRUE(smaller_slice[2] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[3] == 0);
  auto even_smaller_slice = smaller_slice.slice_from(2);
  EXPECT_TRUE(even_smaller_slice[0] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[1] == 0);

  // Abort: out of bounds
  // auto bad_slice = slice.slice_from(6);
}

TEST(Slice, slice_to) {
  int arr[] = {1, 2, 3, 4, 5, 6};
  Slice<int> slice = {6, arr};
  auto smaller_slice = slice.slice_to(3);
  EXPECT_TRUE(smaller_slice[0] == 1);
  EXPECT_TRUE(smaller_slice[1] == 2);
  EXPECT_TRUE(smaller_slice[2] == 3);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[3] == 0);
  auto even_smaller_slice = smaller_slice.slice_to(1);
  EXPECT_TRUE(even_smaller_slice[0] == 1);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[1] == 0);
  auto empty_slice = slice.slice_to(0);
  EXPECT_TRUE(empty_slice.empty());
  // Abort: out of bounds
  // EXPECT_TRUE(empty_slice[0] == 0);
  auto same_slice = slice.slice_to(slice.len());
  EXPECT_TRUE(same_slice.len() == 6);

  // Abort: out of bounds
  // slice.slice_to(7);
}

TEST(Slice, MethodsCanBeCalledForConst) {
  int arr[] = {1, 2, 3};
  const Slice<int> slice = {3, arr};
  EXPECT_TRUE(slice.get(0) == 1);
  EXPECT_TRUE(slice[1] == 2);
  EXPECT_TRUE(slice.ptr()[2] == 3);
  EXPECT_TRUE(slice.last() == 3);
  EXPECT_TRUE(slice.len() == 3);
  EXPECT_TRUE(!slice.empty());
  slice.slice(0, 1);
  slice.slice_from(1);
  slice.slice_to(1);
}
