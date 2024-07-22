/*
 * Copyright 2023 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_H_
#define JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_H_

#include <type_traits>

#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/lifecycle.h"
#include "implementation/object_ref.h"
#include "implementation/promotion_mechanics_tags.h"
#include "implementation/ref_base.h"
#include "metaprogramming/deep_equal_diminished.h"

namespace jni {

// Marks the end of `ScopeEntry` daisy chain.
struct ScopedTerminalTag {};

// Shared implementation common to all *local* `Entry`.
template <typename Base, LifecycleType lifecycleType, typename JniT,
          typename ViableSpan>
struct EntryBase : public Base {
  using Base::Base;
  using Span = typename JniT::SpanType;

  // `RefBaseTag` move constructor for object of same span type.
  template <typename T,
            typename = std::enable_if_t<
                (::jni::metaprogramming::DeepEqualDiminished_v<EntryBase, T> ||
                 std::is_base_of_v<RefBaseTag<Span>, T>)>>
  EntryBase(T&& rhs) : Base(rhs.Release()) {}
  EntryBase(AdoptLocal, ViableSpan object) : Base(object) {}

  // "Copy" constructor: Additional reference to object will be created.
  EntryBase(NewRef, ViableSpan object)
      : EntryBase(AdoptLocal{},
                  object
                      ? static_cast<Span>(
                            LifecycleHelper<Span, lifecycleType>::NewReference(
                                static_cast<Span>(object)))
                      : nullptr) {}

  // Comparison operator for pinned Scoped type (not deep equality).
  template <typename T, typename = std::enable_if_t<
                            (std::is_base_of_v<RefBaseTag<Span>, T> ||
                             std::is_same_v<T, ViableSpan>)>>
  bool operator==(const T& rhs) const {
    if constexpr (std::is_base_of_v<RefBaseTag<Span>, T>) {
      return static_cast<Span>(rhs.object_ref_) == Base::object_ref_;
    } else if constexpr (std::is_same_v<T, ViableSpan>) {
      return rhs == Base::object_ref_;
    }
  }

  // Comparison inequality operator for pinned Scoped type (not deep equality).
  template <typename T, typename = std::enable_if_t<
                            (std::is_base_of_v<RefBaseTag<Span>, T> ||
                             std::is_same_v<T, ViableSpan>)>>
  bool operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

// Local scoped entry augmentation.
template <LifecycleType lifecycleType, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct Entry
    : public EntryBase<Entry<LifecycleType::LOCAL, JniT, ViableSpans...>,
                       LifecycleType::LOCAL, JniT, ViableSpan> {
  using Base = EntryBase<Entry<LifecycleType::LOCAL, JniT, ViableSpans...>,
                         LifecycleType::LOCAL, JniT, ViableSpan>;
  using Base::Base;

  // "Wrap" constructor: Newly created object released at end of scope.
  // Wrap constructors automatically create a new local because objects passed
  // into JNI should not be released, and LocalObject<KClass>(jni_arg) is
  // common.
  Entry(ViableSpan object)
      : Base(AdoptLocal{},
             object ? LifecycleHelper<typename JniT::StorageType,
                                      LifecycleType::LOCAL>::
                          NewReference(
                              static_cast<typename JniT::StorageType>(object))
                    : nullptr) {}

  Entry(AdoptLocal, ViableSpan object)
      : Base(AdoptLocal{}, static_cast<typename JniT::StorageType>(object)) {}
};

// Shared implementation common to all *global* `Entry`.
template <typename Base, typename JniT, typename ViableSpan>
struct EntryBase<Base, LifecycleType::GLOBAL, JniT, ViableSpan> : public Base {
  using Base::Base;
  using Span = typename JniT::SpanType;

  // `RefBaseTag` move constructor for object of same span type.
  template <typename T,
            typename = std::enable_if_t<
                (::jni::metaprogramming::DeepEqualDiminished_v<EntryBase, T> ||
                 std::is_base_of_v<RefBaseTag<Span>, T>)>>
  EntryBase(T&& rhs)
      : Base(LifecycleHelper<typename JniT::StorageType,
                             LifecycleType::GLOBAL>::Promote(rhs.Release())) {}

  // "Copy" constructor: Additional reference to object will be created.
  EntryBase(NewRef, ViableSpan object)
      : Base(static_cast<Span>(
            LifecycleHelper<Span, LifecycleType::GLOBAL>::NewReference(
                static_cast<Span>(object)))) {}
};

// Global scoped entry augmentation.
template <typename JniT, typename ViableSpan, typename... ViableSpans>
struct Entry<LifecycleType::GLOBAL, JniT, ViableSpan, ViableSpans...>
    : public EntryBase<Entry<LifecycleType::GLOBAL, JniT, ViableSpans...>,
                       LifecycleType::GLOBAL, JniT, ViableSpan> {
  using Base = EntryBase<Entry<LifecycleType::GLOBAL, JniT, ViableSpans...>,
                         LifecycleType::GLOBAL, JniT, ViableSpan>;
  using Base::Base;

  // "Promote" constructor: Creates new global, frees |obj| (standard).
  explicit Entry(PromoteToGlobal, ViableSpan obj)
      : Base(LifecycleHelper<typename JniT::StorageType,
                             LifecycleType::GLOBAL>::Promote(obj)) {}

  // "Adopts" a global (non-standard).
  explicit Entry(AdoptGlobal, ViableSpan obj) : Base(obj) {}

 protected:
  // Causes failure for illegal "wrap" like construction.
  explicit Entry(ViableSpan object)
      : Base(reinterpret_cast<typename JniT::SpanType>(object)) {}
};

// Terminal Entry (ends daisy chain).
template <typename JniT>
struct Entry<LifecycleType::LOCAL, JniT, ScopedTerminalTag>
    : public ValidatorProxy<JniT> {
  using Base = ValidatorProxy<JniT>;
  using Base::Base;
};

template <typename JniT>
struct Entry<LifecycleType::GLOBAL, JniT, ScopedTerminalTag>
    : public ValidatorProxy<JniT> {
  using Base = ValidatorProxy<JniT>;
  using Base::Base;
};

// Local augmentation.
template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
struct Scoped
    : public Entry<lifecycleType, JniT, ViableSpans..., ScopedTerminalTag> {
  using Base = Entry<lifecycleType, JniT, ViableSpans..., ScopedTerminalTag>;
  using Base::Base;

 protected:
  void MaybeReleaseUnderlyingObject() {
    if (Base::object_ref_) {
      LifecycleHelper<typename JniT::StorageType, lifecycleType>::Delete(
          Base::object_ref_);
    }
  }

 public:
  ~Scoped() { MaybeReleaseUnderlyingObject(); }
};

template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
Scoped(Scoped<lifecycleType, JniT, ViableSpans...>)
    -> Scoped<lifecycleType, JniT, ViableSpans...>;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_H_
