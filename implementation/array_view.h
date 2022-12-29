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

#include <iterator>

#include "implementation/jni_helper/jni_array_helper.h"
#include "jni_dep.h"

namespace jni {

template <typename SpanType, typename ClassId = void>
class ArrayView {
 public:
  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = SpanType;
    using pointer = SpanType*;
    using reference = SpanType&;

    Iterator(SpanType* ptr, std::size_t size, std::size_t idx)
        : ptr_(ptr), size_(size), idx_(idx) {}

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
            JniArrayHelper<SpanType>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion),
        size_(size) {}

  ~ArrayView() {
    JniArrayHelper<SpanType>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  SpanType* ptr() { return get_array_elements_result_.ptr_; }

  Iterator begin() { return Iterator{ptr(), size_, 0}; }
  Iterator end() { return Iterator{ptr(), size_, size_}; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
  const std::size_t size_;
};

template <typename ClassId>
class ArrayView<jobject, ClassId> {
 public:
  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = jobject;
    using pointer = jobject*;
    using reference = jobject&;

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

    jobject operator*() const {
      return JniArrayHelper<jobject>::GetArrayElement(arr_, idx_);
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

  ArrayView(jobjectArray array, bool, std::size_t size)
      : array_(array), size_(size) {}

  Iterator begin() { return Iterator(array_, size_, 0); }
  Iterator end() { return Iterator(array_, size_, size_); }

 protected:
  const jobjectArray array_;
  const std::size_t size_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType>
ArrayView(ArrayView<SpanType>&&) -> ArrayView<SpanType>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ARRAY_VIEW_H_
