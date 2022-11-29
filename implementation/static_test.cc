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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::Field;
using jni::Method;
using jni::Params;
using jni::Return;
using jni::Static;
using jni::test::JniTest;

TEST_F(JniTest, MethodSmokeTest) {
  static constexpr Class kClass{
      "c1", Static{Method{"Foo", Return<int>{}, Params<>{}}}};

  // In a follow up CL the following syntax will be enabled.
  // StaticRef<kClass>::Call("Foo");

  static_assert(!std::is_same_v<decltype(kClass), void>);
}

TEST_F(JniTest, FieldSmokeTest) {
  static constexpr Class kClass{"c2", Static{Field{"Foo", int{}}}};

  static_assert(!std::is_same_v<decltype(kClass), void>);
}

}  // namespace
