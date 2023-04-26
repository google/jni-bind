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

#include "implementation/jni_helper/lifecycle_object.h"
#include "jni_dep.h"

namespace jni {

// Creates an additional reference to the underlying object.
// When used for local, presumes local, for global, presumes global.
struct CreateCopy {};

// This tag allows the constructor to promote underlying jobject for you.
struct PromoteToGlobal {};

// CAUTION: This tag assume the underlying jobject has been pinned as a global.
// This is atypical when solely using JNI Bind, use with caution.
struct AdoptGlobal {};

template <typename CrtpBase, typename Span, typename... ViableSpans>
struct LocalCtor : public CrtpBase {
  using CrtpBase::CrtpBase;
};

// Augments a a local constructor of type |Span| (created by |LoadedBy|).
// Inheritance and ctor inheritance will continue through |Base|.
template <typename CrtpBase, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct LocalCtor<CrtpBase, JniT, ViableSpan, ViableSpans...>
    : public LocalCtor<CrtpBase, JniT, ViableSpans...> {
  using Base = LocalCtor<CrtpBase, JniT, ViableSpans...>;
  using Base::Base;
  using Span = typename JniT::SpanType;
  using LifecycleT = LifecycleHelper<Span, LifecycleType::LOCAL>;

  // "Copy" constructor: Additional reference to object will be created.
  LocalCtor(CreateCopy, ViableSpan object)
      : Base(static_cast<Span>(
            LifecycleT::NewReference(static_cast<Span>(object)))) {}

  // "Wrap" constructor: Object released at end of scope.
  LocalCtor(ViableSpan object) : Base(static_cast<Span>(object)) {}
};

template <typename CrtpBase, typename JniT, typename... ViableSpans>
struct GlobalCtor : public CrtpBase {
  using CrtpBase::CrtpBase;
};

// Augments a a local constructor of type |Span| (created by |LoadedBy|).
// Inheritance and ctor inheritance will continue through |Base|.
template <typename CrtpBase, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct GlobalCtor<CrtpBase, JniT, ViableSpan, ViableSpans...>
    : public GlobalCtor<CrtpBase, JniT, ViableSpans...> {
  using Base = GlobalCtor<CrtpBase, JniT, ViableSpans...>;
  using Base::Base;
  using Span = typename JniT::SpanType;
  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  // "Copy" constructor: Additional reference to object will be created.
  GlobalCtor(CreateCopy, ViableSpan object)
      : Base(static_cast<Span>(LifecycleT::NewReference(object))) {}

  // "Promote" constructor: Creates new global, frees |obj| (standard).
  explicit GlobalCtor(PromoteToGlobal, ViableSpan obj)
      : Base(LifecycleT::Promote(obj)) {}

  // "Adopts" a global by wrapping a jstring (non-standard).
  explicit GlobalCtor(AdoptGlobal, ViableSpan obj) : Base(obj) {}
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_H_
