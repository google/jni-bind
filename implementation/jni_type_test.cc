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
#include <type_traits>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"

namespace {

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Extends;
using ::jni::JniT;
using ::jni::JniTEqual_v;
using ::jni::Jvm;
using ::jni::kDefaultClassLoader;
using ::jni::kDefaultJvm;
using ::jni::kNoClassLoaderSpecified;
using ::jni::kNoClassSpecified;
using ::jni::kNullClassLoader;
using ::jni::SupportedClassSet;

constexpr Class kClass1{"kClass1"};
constexpr Class kClass1Prime{"kClass1"};
constexpr Class kClass2{"kClass2"};
constexpr Class kClass2Prime{"kClass2"};
constexpr Class kClass3{"kClass3"};
constexpr Class kClass4{"kClass4"};

constexpr ClassLoader kLoader1{kNullClassLoader, SupportedClassSet{kClass1}};
constexpr ClassLoader kLoader2{kNullClassLoader, SupportedClassSet{kClass2}};
constexpr ClassLoader kLoader3{kNullClassLoader,
                               SupportedClassSet{kClass1, kClass2, kClass3}};
constexpr ClassLoader kLoader4{
    kNullClassLoader, SupportedClassSet{kClass1, kClass2, kClass3, kClass4}};

constexpr Jvm kJvm1{kLoader1};
constexpr Jvm kJvm2{kLoader1};
constexpr Jvm kJvm3{kLoader1, kLoader3};
constexpr Jvm kJvm4{kLoader1, kLoader4};

using T1 = JniT<jobject, kClass1, kLoader1, kJvm1>;
using T1_Prime = JniT<jobject, kClass1Prime, kLoader1, kJvm1>;
using T2 = JniT<jobject, kClass2, kLoader2, kJvm2>;
using T2_Prime = JniT<jobject, kClass2Prime, kLoader2, kJvm2>;
using T3 = JniT<jobject, kClass1, kLoader1, kJvm1, 0, 1, 2>;
using T3_Prime = JniT<jobject, kClass1Prime, kLoader1, kJvm1, 1, 2, 3>;

static_assert(JniTEqual_v<T1, T1>);
static_assert(!JniTEqual_v<T1, T2>);
static_assert(JniTEqual_v<T1, T1_Prime>);
static_assert(JniTEqual_v<T2, T2_Prime>);
static_assert(!JniTEqual_v<T1_Prime, T2>);
static_assert(!JniTEqual_v<T1_Prime, T2_Prime>);

static_assert(JniTEqual_v<T3, T3>);
static_assert(!JniTEqual_v<T3, T3_Prime>);
static_assert(!JniTEqual_v<T1, T3>);

////////////////////////////////////////////////////////////////////////////////
// Minimal Span Tests:  Default Classloader.
////////////////////////////////////////////////////////////////////////////////

// Minimal span tests: Partially specified (class, no loader, no jvm).
static_assert(std::is_same_v<JniT<jobject, kClass1>::MinimallySpanningType,
                             JniT<jobject, kClass1>>);

// Minimal span tests: Partially specified (class, loader, no jvm).
static_assert(std::is_same_v<
              JniT<jobject, kClass1, kLoader1>::MinimallySpanningType,
              JniT<jobject, kNoClassSpecified, kLoader1, kDefaultJvm, 0, 0>>);

// Minimal span tests: Partially specified (class, no loader, jvm).
static_assert(
    std::is_same_v<JniT<jobject, kClass1, kDefaultClassLoader,
                        kDefaultJvm>::MinimallySpanningType,
                   JniT<jobject, kClass1, kDefaultClassLoader, kDefaultJvm>>);

// Minimal span tests: Partially specified (no class, loader, no jvm).
static_assert(std::is_same_v<
              JniT<jobject, kNoClassSpecified, kLoader1>::MinimallySpanningType,
              JniT<jobject, kNoClassSpecified, kLoader1>>);

// Minimal span tests: Fully specified (class, loader, jvm through values).
static_assert(std::is_same_v<
              JniT<jobject, kClass1, kLoader1, kJvm1>::MinimallySpanningType,
              JniT<jobject, kNoClassSpecified, kNoClassLoaderSpecified, kJvm1,
                   0, 0, 0>>);

// Minimal span tests: Fully specified (class, loader, jvm through values)
// Rank, classloader idx are non-trivial.
static_assert(
    std::is_same_v<JniT<jobject, kNoClassSpecified, kDefaultClassLoader, kJvm3,
                        1, 2, 1>::MinimallySpanningType,
                   JniT<jobject, kNoClassSpecified, kNoClassLoaderSpecified,
                        kJvm3, 1, 2, 1>>);

// Minimal span tests: Fully specified (class, loader, jvm all through idx).
static_assert(std::is_same_v<JniT<jobject, kNoClassSpecified, kLoader1, kJvm1,
                                  0, 0, 0>::MinimallySpanningType,
                             JniT<jobject, kNoClassSpecified,
                                  kNoClassLoaderSpecified, kJvm1, 0, 0, 0>>);

////////////////////////////////////////////////////////////////////////////////
// Minimal Span Tests:  Null Classloader (same as above where applicable).
////////////////////////////////////////////////////////////////////////////////

// Minimal span tests: Partially specified (class, no loader, no jvm).
static_assert(std::is_same_v<
              JniT<jobject, kClass1, kNullClassLoader>::MinimallySpanningType,
              JniT<jobject, kClass1, kNullClassLoader>>);

// Minimal span tests: Partially specified (class, no loader, jvm).
static_assert(
    std::is_same_v<JniT<jobject, kClass1, kNullClassLoader,
                        kDefaultJvm>::MinimallySpanningType,
                   JniT<jobject, kClass1, kNullClassLoader, kDefaultJvm>>);

// Minimal span tests: Partially specified (class, loader, jvm through values)
// Rank, classloader idx are non-trivial.
static_assert(
    std::is_same_v<JniT<jobject, kClass4, kNullClassLoader, kJvm4,
                        1>::MinimallySpanningType,
                   JniT<jobject, kClass4, kNullClassLoader, kJvm4, 1>>);

////////////////////////////////////////////////////////////////////////////////
// Inheritance Depth Tests.
////////////////////////////////////////////////////////////////////////////////

static constexpr Class kParent{
    "kParent",
};

static constexpr Class kChild{
    "kChild",
    Extends{kParent},
};

static constexpr Class kGrandchild{
    "kGrandchild",
    Extends{kChild},
};

using GrandparentJniT = JniT<jobject, kParent>;
using ParentJniT = JniT<jobject, kChild>;
using ChildJniT = JniT<jobject, kGrandchild>;

static_assert(
    std::is_same_v<decltype(GrandparentJniT::kParent), const jni::RootObject>);

static_assert(
    std::is_same_v<std::decay_t<decltype(jni::kObject)>, jni::RootObject>);
;

static_assert(GrandparentJniT::kDepthInAncestors == 0);
static_assert(ParentJniT::kDepthInAncestors == 1);
static_assert(ChildJniT::kDepthInAncestors == 2);

}  // namespace
