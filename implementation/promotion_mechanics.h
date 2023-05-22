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

#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jni_type.h"
#include "implementation/object_ref.h"
#include "implementation/ref_base.h"
#include "jni_dep.h"
#include "metaprogramming/deep_equal.h"
#include "metaprogramming/pack_discriminator.h"

namespace jni {

// Creates an additional reference to the underlying object.
// When used for local, presumes local, for global, presumes global.
struct CreateCopy {};

// This tag allows the constructor to promote underlying jobject for you.
struct PromoteToGlobal {};

// CAUTION: This tag assume the underlying jobject has been pinned as a global.
// This is atypical when solely using JNI Bind, use with caution.
struct AdoptGlobal {};

// Augments a a local constructor of type |Span|.
// Inheritance and ctor inheritance will continue through |CrtpBase|.
template <LifecycleType lifecycleType, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct ScopedEntry : public ScopedEntry<lifecycleType, JniT, ViableSpans...> {
  using Base = ScopedEntry<lifecycleType, JniT, ViableSpans...>;
  using Base::Base;
  using Span = typename JniT::SpanType;
  using LifecycleT = LifecycleHelper<Span, lifecycleType>;

  // "Copy" constructor: Additional reference to object will be created.
  ScopedEntry(CreateCopy, ViableSpan object)
      : Base(static_cast<Span>(
            LifecycleT::NewReference(static_cast<Span>(object)))) {}

  // "Wrap" constructor: Object released at end of scope.
  ScopedEntry(ViableSpan object) : Base(static_cast<Span>(object)) {}

  template <typename T,
            typename = std::enable_if_t<
                ::jni::metaprogramming::DeepEqual_v<ScopedEntry, T> ||
                std::is_base_of_v<RefBaseTag<Span>, T>>>
  ScopedEntry(T&& rhs) : Base(rhs.Release()) {}
};

struct ScopedTerminalTag {};

template <LifecycleType lifecycleType, typename JniT>
struct ScopedEntry<lifecycleType, JniT, ScopedTerminalTag>
    : public ValidatorProxy<JniT> {
  using Base = ValidatorProxy<JniT>;
  using Base::Base;
};

template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
struct Scoped : public ScopedEntry<lifecycleType, JniT, ViableSpans...,
                                   ScopedTerminalTag> {
  using Base =
      ScopedEntry<lifecycleType, JniT, ViableSpans..., ScopedTerminalTag>;
  using Base::Base;

  ~Scoped() {
    if (Base::object_ref_) {
      LifecycleHelper<typename JniT::SpanType, lifecycleType>::Delete(
          Base::object_ref_);
    }
  }
};

template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
Scoped(Scoped<lifecycleType, JniT, ViableSpans...>)
    -> Scoped<lifecycleType, JniT, ViableSpans...>;

template <LifecycleType val, typename Self, typename CrtpBase, typename JniT,
          typename... ViableSpans>
struct GlobalCtor : public CrtpBase {
  using CrtpBase::CrtpBase;
};

// Augments a a local constructor of type |Span| (created by |LoadedBy|).
// Inheritance and ctor inheritance will continue through |Base|.
template <typename Self, typename CrtpBase, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct GlobalCtor<LifecycleType::GLOBAL, Self, CrtpBase, JniT, ViableSpan,
                  ViableSpans...>
    : public GlobalCtor<LifecycleType::GLOBAL, Self, CrtpBase, JniT,
                        ViableSpans...> {
  using Base =
      GlobalCtor<LifecycleType::GLOBAL, Self, CrtpBase, JniT, ViableSpans...>;
  using Base::Base;
  using Span = typename JniT::SpanType;
  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  // "Copy" constructor: Additional reference to object will be created.
  GlobalCtor(CreateCopy, ViableSpan object)
      : Base(static_cast<Span>(LifecycleT::NewReference(object))) {}

  // "Promote" constructor: Creates new global, frees |obj| (standard).
  explicit GlobalCtor(PromoteToGlobal, ViableSpan obj)
      : Base(LifecycleT::Promote(obj)) {}

  // "Adopts" a global (non-standard).
  explicit GlobalCtor(AdoptGlobal, ViableSpan obj) : Base(obj) {}

  template <typename T, typename = std::enable_if_t<
                            ::jni::metaprogramming::DeepEqual_v<Self, T> ||
                            std::is_base_of_v<RefBaseTag<Span>, T>>>
  GlobalCtor(T&& rhs) : Base(rhs.Release()) {}
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_H_
