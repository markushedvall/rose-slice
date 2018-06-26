#include <rose/slice.hpp>

using rose::Slice;

int main() {
  int arr[] = {1, 2, 3};
  Slice<int> slice = {3, arr};
}
