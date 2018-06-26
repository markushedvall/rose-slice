#include <type_traits>

#include <gtest/gtest.h>
#include <rose/slice.hpp>

using rose::MutSlice;
using rose::Slice;

TEST(MutSlice, IsBothTrivialAndStandardLayout) {
  EXPECT_TRUE(std::is_standard_layout<MutSlice<int>>::value);
  EXPECT_TRUE(std::is_trivial<MutSlice<int>>::value);
  EXPECT_TRUE(std::is_trivially_copyable<MutSlice<int>>::value);
}

TEST(MutSlice, CanBeAggregateInitialized) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  EXPECT_TRUE(slice.length == 3);
  EXPECT_TRUE(slice.pointer[0] == 1);
  EXPECT_TRUE(slice.pointer[1] == 2);
  EXPECT_TRUE(slice.pointer[2] == 3);
}

TEST(MutSlice, get) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  EXPECT_TRUE(slice.get(0) == 1);
  EXPECT_TRUE(slice.get(1) == 2);
  EXPECT_TRUE(slice.get(2) == 3);
  EXPECT_TRUE(slice[0] == 1);
  EXPECT_TRUE(slice[1] == 2);
  EXPECT_TRUE(slice[2] == 3);

  // Abort: out of bounds
  // EXPECT_TRUE(slice[3] == 0);
}

TEST(MutSlice, last) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  EXPECT_TRUE(slice.last() == 3);
  int arr2[] = {5, 4, 3, 2, 1};
  MutSlice<int> slice2 = {5, arr2};
  EXPECT_TRUE(slice2.last() == 1);
}

TEST(MutSlice, mut_slice) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  MutSlice<int> slice = {8, arr};
  auto smaller_slice = slice.mut_slice(3, slice.len());
  EXPECT_TRUE(smaller_slice[0] == 4);
  EXPECT_TRUE(smaller_slice[1] == 5);
  EXPECT_TRUE(smaller_slice[2] == 6);
  EXPECT_TRUE(smaller_slice[3] == 7);
  EXPECT_TRUE(smaller_slice[4] == 8);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[5] == 0);
  auto even_smaller_slice = smaller_slice.mut_slice(1, 3);
  EXPECT_TRUE(even_smaller_slice[0] == 5);
  EXPECT_TRUE(even_smaller_slice[1] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[2] == 7);

  // Abort: invalid arguments
  // auto bad_slice1 = slice.mut_slice(3, 2);
  // auto bad_slice1 = slice.mut_slice(2, 2);
  // auto bad_slice1 = slice.mut_slice(2, 9);
}

TEST(MutSlice, length) {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  MutSlice<int> slice = {8, arr};
  EXPECT_TRUE(slice.len() == 8);
  auto smaller_slice = slice.mut_slice(2, slice.len());
  EXPECT_TRUE(smaller_slice.len() == 6);
  auto even_smaller_slice = smaller_slice.mut_slice(2, 4);
  EXPECT_TRUE(even_smaller_slice.len() == 2);
}

TEST(MutSlice, empty) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  EXPECT_TRUE(!slice.empty());
  int empty_arr[0] = {};
  MutSlice<int> empty_slice = {0, empty_arr};
  EXPECT_TRUE(empty_slice.empty());
}

TEST(MutSlice, ptr) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  EXPECT_TRUE(slice.ptr()[0] == 1);
  EXPECT_TRUE(slice.ptr()[1] == 2);
  EXPECT_TRUE(slice.ptr()[2] == 3);
}

TEST(MutSlice, mut_slice_from) {
  int arr[] = {1, 2, 3, 4, 5, 6};
  MutSlice<int> slice = {6, arr};
  auto smaller_slice = slice.mut_slice_from(3);
  EXPECT_TRUE(smaller_slice[0] == 4);
  EXPECT_TRUE(smaller_slice[1] == 5);
  EXPECT_TRUE(smaller_slice[2] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[3] == 0);
  auto even_smaller_slice = smaller_slice.mut_slice_from(2);
  EXPECT_TRUE(even_smaller_slice[0] == 6);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[1] == 0);

  // Abort: out of bounds
  // auto bad_slice = slice.mut_slice_from(6);
}

TEST(MutSlice, mut_slice_to) {
  int arr[] = {1, 2, 3, 4, 5, 6};
  MutSlice<int> slice = {6, arr};
  auto smaller_slice = slice.mut_slice_to(3);
  EXPECT_TRUE(smaller_slice[0] == 1);
  EXPECT_TRUE(smaller_slice[1] == 2);
  EXPECT_TRUE(smaller_slice[2] == 3);
  // Abort: out of bounds
  // EXPECT_TRUE(smaller_slice[3] == 0);
  auto even_smaller_slice = smaller_slice.mut_slice_to(1);
  EXPECT_TRUE(even_smaller_slice[0] == 1);
  // Abort: out of bounds
  // EXPECT_TRUE(even_smaller_slice[1] == 0);
  auto empty_slice = slice.mut_slice_to(0);
  EXPECT_TRUE(empty_slice.empty());
  // Abort: out of bounds
  // EXPECT_TRUE(empty_slice[0] == 0);
  auto same_slice = slice.mut_slice_to(slice.len());
  EXPECT_TRUE(same_slice.len() == 6);

  // Abort: out of bounds
  // slice.mut_slice_to(7);
}

TEST(MutSlice, CanBeModified) {
  int arr[] = {1, 2, 3};
  MutSlice<int> slice = {3, arr};
  slice[0] = 3;
  EXPECT_TRUE(slice[0] == 3);
  slice.ptr()[1] = 4;
  EXPECT_TRUE(slice[1] == 4);
  slice.last() = 1;
  EXPECT_TRUE(slice[2] == 1);
}

TEST(MutSlice, CanBeSlicedIntoSlice) {
  int arr[] = {1, 2, 3, 4, 5, 6};
  MutSlice<int> mut_slice = {6, arr};
  { // slice()
    Slice<int> slice = mut_slice.slice();
    EXPECT_TRUE(slice.len() == mut_slice.len());
    EXPECT_TRUE(slice.ptr() == mut_slice.ptr());
  }
  { // slice(from, to)
    Slice<int> slice = mut_slice.slice(2, 4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 3);
    EXPECT_TRUE(slice[1] == 4);
  }
  { // slice_from()
    Slice<int> slice = mut_slice.slice_from(4);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 5);
    EXPECT_TRUE(slice[1] == 6);
  }
  { // slice_to()
    Slice<int> slice = mut_slice.slice_to(2);
    EXPECT_TRUE(slice.len() == 2);
    EXPECT_TRUE(slice[0] == 1);
    EXPECT_TRUE(slice[1] == 2);
  }
}
