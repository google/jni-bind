/*
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_
#define JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <cstddef>
#include <iterator>
#include <type_traits>

#include "implementation/array_type_conversion.h"
#include "implementation/jni_helper/get_array_element_result.h"
#include "implementation/jni_helper/jni_array_helper.h"
#include "implementation/jni_helper/lifecycle.h"
#include "jni_dep.h"

namespace jni {

struct ArrayViewHelperBase {};

template <typename T>
struct ArrayViewHelper : ArrayViewHelperBase {
  using SpanType = T;
  const T val_;
  operator T() const { return val_; }

  ArrayViewHelper(const T& val) : val_(val) {}
};

// Primitive Rank 1 Arrays.
template <typename SpanType_, std::size_t kRank = 1, typename Enable = void>
class ArrayView {
 public:
  using SpanType = SpanType_;

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = SpanType;
    using pointer = SpanType*;
    using reference = SpanType&;

    Iterator(SpanType* ptr, std::size_t size, std::size_t idx)
        : size_(size), ptr_(ptr), idx_(idx) {}

    Iterator& operator++() {
      idx_++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    SpanType& operator*() const { return ptr_[idx_]; }
    const SpanType* operator->() const { return ptr_[idx_]; }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return (lhs.ptr_ == rhs.ptr_) && (lhs.size_ == rhs.size_) &&
             (lhs.idx_ == rhs.idx_);
    };

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return !(lhs == rhs);
    };

    friend std::size_t operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.idx_ - rhs.idx_;
    };

   private:
    const std::size_t size_;
    SpanType* const ptr_;
    std::size_t idx_;
  };

  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  ArrayView(jarray array, bool copy_on_completion, std::size_t size)
      : array_(array),
        get_array_elements_result_(
            JniArrayHelper<SpanType, kRank>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion),
        size_(size) {}

  ~ArrayView() {
    JniArrayHelper<SpanType, kRank>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  // Arrays of rank > 1 are object arrays which are not contiguous.
  std::enable_if_t<kRank == 1, SpanType*> ptr() {
    return get_array_elements_result_.ptr_;
  }

  Iterator begin() { return Iterator{ptr(), size_, 0}; }
  Iterator end() { return Iterator{ptr(), size_, size_}; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
  const std::size_t size_;
};

// Metafunction that returns the type after a single dereference.
template <typename SpanType, std::size_t>
struct PinHelper {
  using type = jobjectArray;
};

template <typename SpanType>
struct PinHelper<SpanType, 2> {
  using type = RegularToArrayTypeMap_t<SpanType>;
};

template <typename SpanType>
struct PinHelper<SpanType, 1> {
  using type = jobject;
};

// Object arrays, or arrays with rank > 1 (which are object arrays), or strings.
template <typename SpanType_, std::size_t kRank>
class ArrayView<
    SpanType_, kRank,
    std::enable_if_t<(kRank > 1) || std::is_same_v<SpanType_, jobject> ||
                     std::is_same_v<SpanType_, jstring>>> {
 public:
  using SpanType = SpanType_;

  using PinHelper_t = typename PinHelper<SpanType_, kRank>::type;

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = PinHelper_t;
    using pointer = PinHelper_t*;
    using reference = PinHelper_t&;

    Iterator(jobjectArray arr, std::size_t size, std::size_t idx)
        : arr_(arr), size_(size), idx_(idx) {}

    Iterator& operator++() {
      idx_++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    ArrayViewHelper<PinHelper_t> operator*() const {
      if constexpr (kRank >= 2) {
        return {static_cast<PinHelper_t>(
            JniArrayHelper<jobject, kRank>::GetArrayElement(arr_, idx_))};
      } else {
        return {JniArrayHelper<SpanType, kRank>::GetArrayElement(arr_, idx_)};
      }
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return (lhs.arr_ == rhs.arr_) && (lhs.size_ == rhs.size_) &&
             (lhs.idx_ == rhs.idx_);
    };

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return !(lhs == rhs);
    };

    friend std::size_t operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.idx_ - rhs.idx_;
    };

   private:
    jobjectArray const arr_;
    const std::size_t size_;
    std::size_t idx_;
  };

  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  // This constructor creates a copy of the parent `jarray` so that the
  // lifetime doesn't end before objects. e.g. `obj["field"].Get().Pin()` is a
  // useful pattern in iterators, but the returned Get() `LocalArray` would
  // be released immediately.
  ArrayView(jobjectArray array, bool, std::size_t size)
      : array_(
            LifecycleHelper<jobjectArray, LifecycleType::LOCAL>::NewReference(
                array)),
        size_(size) {}

  ~ArrayView() {
    LifecycleHelper<jobjectArray, LifecycleType::LOCAL>::Delete(array_);
  }

  Iterator begin() { return Iterator(array_, size_, 0); }
  Iterator end() { return Iterator(array_, size_, size_); }

 protected:
  const jobjectArray array_;
  const std::size_t size_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType, std::size_t kRank>
ArrayView(ArrayView<SpanType, kRank>&&) -> ArrayView<SpanType, kRank>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_
