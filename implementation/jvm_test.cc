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
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/jni_env.h"
#include "implementation/jni_helper/jni_helper.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Jvm;
using ::jni::kNullClassLoader;
using ::jni::SupportedClassSet;

TEST(Jni, JvmStaticClassLoaderLookupsWork) {
  static constexpr Class kClass1{"com/google/Class1"};
  static constexpr Class kClass2{"com/google/Class2"};
  static constexpr Class kClass3{"com/google/Class3"};
  static constexpr ClassLoader kClassLoader1{kNullClassLoader,
                                             SupportedClassSet{kClass1}};
  static constexpr ClassLoader kClassLoader2{
      kNullClassLoader, SupportedClassSet{kClass1, kClass2}};
  static constexpr ClassLoader kClassLoader3{
      kNullClassLoader, SupportedClassSet{kClass1, kClass2, kClass3}};
  static constexpr ClassLoader kUnusedLoader{kNullClassLoader,
                                             SupportedClassSet{}};

  static constexpr Jvm kJvm1{kClassLoader1};
  static_assert(kJvm1.IdxOfClassLoader<kUnusedLoader>() == -1);
  static_assert(kJvm1.IdxOfClassLoader<kClassLoader1>() == 0);

  static constexpr Jvm kJvm2{kClassLoader1, kClassLoader2};
  static_assert(kJvm2.IdxOfClassLoader<kUnusedLoader>() == -1);
  static_assert(kJvm2.IdxOfClassLoader<kClassLoader1>() == 0);
  static_assert(kJvm2.IdxOfClassLoader<kClassLoader2>() == 1);

  static constexpr Jvm kJvm3{kClassLoader1, kClassLoader2, kClassLoader3};
  static_assert(kJvm3.IdxOfClassLoader<kUnusedLoader>() == -1);
  static_assert(kJvm3.IdxOfClassLoader<kClassLoader1>() == 0);
  static_assert(kJvm3.IdxOfClassLoader<kClassLoader2>() == 1);
  static_assert(kJvm3.IdxOfClassLoader<kClassLoader3>() == 2);
}

}  // namespace
