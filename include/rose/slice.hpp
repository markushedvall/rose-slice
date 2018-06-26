#ifndef ROSE_SLICE_HPP
#define ROSE_SLICE_HPP

#include <cassert>
#include <cstddef>

namespace rose {

  template<class TYPE>
  struct Slice {

    const size_t length;
    const TYPE*const pointer;

    const TYPE& get(size_t index) const noexcept {
      assert(index < length);
      return pointer[index];
    }

    const TYPE& operator[](size_t index) const noexcept {
      return get(index);
    }

    const TYPE& last() const noexcept {
      assert(!empty());
      return pointer[length - 1];
    }

    size_t len() const noexcept {
      return length;
    }

    bool empty() const noexcept {
       return (length == 0);
    }

    const TYPE* ptr() const noexcept {
      return pointer;
    }

    Slice<TYPE> slice(size_t from, size_t to) const noexcept {
      assert(from < to);
      assert(to <= length);
      return {to - from, pointer + from};
    }

    Slice<TYPE> slice_from(size_t from) const noexcept {
      assert(from < length);
      return {length - from, pointer + from};
    }

    Slice<TYPE> slice_to(size_t to) const noexcept {
      assert(to <= length);
      return {to, pointer};
    }

  };


  template<class TYPE>
  struct MutSlice {

    const size_t length;
    TYPE*const pointer;

    TYPE& get(size_t index) const noexcept {
      assert(index < length);
      return pointer[index];
    }

    TYPE& operator[](size_t index) const noexcept {
      return get(index);
    }

    TYPE& last() const noexcept {
      assert(!empty());
      return pointer[length - 1];
    }

    size_t len() const noexcept {
      return length;
    }

    bool empty() const noexcept {
       return (length == 0);
    }

    TYPE* ptr() const noexcept {
      return pointer;
    }

    Slice<TYPE> slice() const noexcept {
      return {length, pointer};
    }

    Slice<TYPE> slice(size_t from, size_t to) const noexcept {
      assert(from < to);
      assert(to <= length);
      return {to - from, pointer + from};
    }

    Slice<TYPE> slice_from(size_t from) const noexcept {
      assert(from < length);
      return {length - from, pointer + from};
    }

    Slice<TYPE> slice_to(size_t to) const noexcept {
      assert(to <= length);
      return {to, pointer};
    }

    MutSlice<TYPE> mut_slice(size_t from, size_t to) const noexcept {
      assert(from < to);
      assert(to <= length);
      return {to - from, pointer + from};
    }

    MutSlice<TYPE> mut_slice_from(size_t from) const noexcept {
      assert(from < length);
      return {length - from, pointer + from};
    }

    MutSlice<TYPE> mut_slice_to(size_t to) const noexcept {
      assert(to <= length);
      return {to, pointer};
    }

  };

}

#endif
