/*
 * Copyright 2021 Google LLC
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
#include <gtest/gtest.h>
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::kClassNotInLoaderSetIdx;
using ::jni::kDefaultClassLoader;
using ::jni::kNullClassLoader;
using ::jni::ParentLoaderForClass;
using ::jni::SupportedClassSet;

// Base custom loader.
static constexpr Class kFoo{"Foo"};
static constexpr Class kBar{"Bar"};
static constexpr Class kBaz{"Baz"};

static constexpr ClassLoader kLoaderWithNoClassesDefaultParent{
    kDefaultClassLoader, SupportedClassSet{}};
static constexpr ClassLoader kLoaderWithDefaultParent{kDefaultClassLoader,
                                                      SupportedClassSet{kFoo}};

static constexpr ClassLoader kLoaderWithNullParent{kNullClassLoader,
                                                   SupportedClassSet{kFoo}};

// Doesn't compile (good)
//
// static_assert(
//   kLoaderWithNullParent == ParentLoaderForClass<kLoaderWithNullParent,
//   kBar>());

// Index of classes not in the set.
static_assert(kDefaultClassLoader.IdxOfClass<kFoo>() ==
              kClassNotInLoaderSetIdx);
static_assert(kDefaultClassLoader ==
              ParentLoaderForClass<kLoaderWithNoClassesDefaultParent, kFoo>());
static_assert(kLoaderWithDefaultParent ==
              ParentLoaderForClass<kLoaderWithDefaultParent, kFoo>());
static_assert(kDefaultClassLoader ==
              ParentLoaderForClass<kLoaderWithDefaultParent, kBar>());
static_assert(kLoaderWithNullParent ==
              ParentLoaderForClass<kLoaderWithNullParent, kFoo>());

////////////////////////////////////////////////////////////////////////////////
// Null Class Loader Tests
////////////////////////////////////////////////////////////////////////////////
static constexpr ClassLoader kLoader{kNullClassLoader,
                                     SupportedClassSet{kFoo, kBar, kBaz}};

// Child loader only.
static constexpr Class kBazNar{"BazNar"};
static constexpr ClassLoader kChildLoader{"com/google/kClassLoader", kLoader,
                                          SupportedClassSet{kBazNar}};

// Child-child loader only.
static constexpr Class kGnar{"Gnar"};
static constexpr ClassLoader kChildChildLoader{
    "com/google/kChildLoader", kChildLoader, SupportedClassSet{kGnar}};

// No loaders (fulfilled by default, but not null).
static constexpr Class kUnusedClass{"UnusedClass"};

// Simple (i.e. no inheritance) classloader support.
static_assert(kLoader.SupportedByThisSet<kFoo>());
static_assert(kLoader.SupportedByThisSet<kBaz>());
static_assert(kLoader.SupportedByThisSet<kBar>());
static_assert(!kLoader.SupportedByThisSet<kBazNar>());

static_assert(!kChildLoader.SupportedByThisSet<kFoo>());
static_assert(!kChildLoader.SupportedByThisSet<kFoo>());
static_assert(kChildLoader.SupportedByThisSet<kBazNar>());

// Direct and Indirect Class Support.
static_assert(kChildLoader.SupportedDirectlyOrIndirectly<kBar>());

static_assert(kLoader.SupportedDirectlyOrIndirectly<kFoo>());
static_assert(kLoader.SupportedDirectlyOrIndirectly<kBaz>());
static_assert(kLoader.SupportedDirectlyOrIndirectly<kBar>());

static_assert(kChildLoader.SupportedDirectlyOrIndirectly<kFoo>());
static_assert(kChildLoader.SupportedDirectlyOrIndirectly<kBaz>());
static_assert(kChildLoader.SupportedDirectlyOrIndirectly<kBar>());

static_assert(!kChildLoader.SupportedDirectlyOrIndirectly<kUnusedClass>());

// Index for Class.
static_assert(kLoader.IdxOfClass<kFoo>() == 0);
static_assert(kLoader.IdxOfClass<kBar>() == 1);
static_assert(kLoader.IdxOfClass<kBaz>() == 2);
static_assert(kLoader.IdxOfClass<kBazNar>() == kClassNotInLoaderSetIdx);
static_assert(kLoader.IdxOfClass<kGnar>() == kClassNotInLoaderSetIdx);

// Index for Ancestor.
static_assert(kLoader.IdxOfAncestor<kFoo>() == 0);
static_assert(kLoader.IdxOfAncestor<kBaz>() == 0);
static_assert(kLoader.IdxOfAncestor<kBar>() == 0);

static_assert(kChildLoader.IdxOfAncestor<kFoo>() == 1);
static_assert(kChildLoader.IdxOfAncestor<kBazNar>() == 0);

static_assert(kChildChildLoader.IdxOfAncestor<kFoo>() == 2);
static_assert(kChildChildLoader.IdxOfAncestor<kBazNar>() == 1);
static_assert(kChildChildLoader.IdxOfAncestor<kGnar>() == 0);

// ClassLoader Selection.

// For kLoader:
static_assert(kLoader == ParentLoaderForClass<kLoader, kFoo>());
static_assert(kLoader == ParentLoaderForClass<kLoader, kBaz>());
static_assert(kLoader == ParentLoaderForClass<kLoader, kBar>());

// For kChildLoader:
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kFoo>());
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kBaz>());
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kBar>());
static_assert(kChildLoader == ParentLoaderForClass<kChildLoader, kBazNar>());

// For kChildChildLoader:
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kFoo>());
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kBaz>());
static_assert(kLoader == ParentLoaderForClass<kChildLoader, kBar>());
static_assert(kChildLoader == ParentLoaderForClass<kChildLoader, kBazNar>());
static_assert(kChildChildLoader ==
              ParentLoaderForClass<kChildChildLoader, kGnar>());

}  // namespace
