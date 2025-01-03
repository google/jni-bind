/*
 * Copyright 2025 Google LLC
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

#include <cstddef>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

#if __clang__

using ::jni::AdoptLocal;
using ::jni::Class;
using ::jni::Fake;
using ::jni::LocalArray;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Construction.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, BuildsFromSizeForMultiDimensional_no_value) {
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint, 2>{std::size_t{10}, Fake<jintArray>()};
}

TEST_F(JniTest, BuildsFromSizeForMultiDimensional_primitive_xref) {
  EXPECT_CALL(*env_, FindClass(StrEq("[I")));
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint, 2>{std::size_t{10},
                      LocalArray<jint>{AdoptLocal{}, Fake<jintArray>()}};
}

TEST_F(JniTest, BuildsFromSizeForMultiDimensional_primitive_lvalue) {
  EXPECT_CALL(*env_, NewObjectArray(10, _, Fake<jintArray>()));

  LocalArray<jint> arr{AdoptLocal{}, Fake<jintArray>()};
  LocalArray<jint, 2>{std::size_t{10}, arr};
}

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, GetsIntValues) {
  EXPECT_CALL(*env_, GetObjectArrayElement(Fake<jobjectArray>(), 0))
      .WillOnce(::testing::Return(Fake<jintArray>(0)));
  EXPECT_CALL(*env_, GetObjectArrayElement(Fake<jobjectArray>(), 1))
      .WillOnce(::testing::Return(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, GetObjectArrayElement(Fake<jobjectArray>(), 2))
      .WillOnce(::testing::Return(Fake<jintArray>(2)));
  LocalArray<jint, 2> arr{std::size_t{10}, Fake<jobjectArray>()};

  EXPECT_EQ((static_cast<jintArray>(arr.Get(0))), (Fake<jintArray>(0)));
  EXPECT_EQ((static_cast<jintArray>(arr.Get(1))), (Fake<jintArray>(1)));
  EXPECT_EQ((static_cast<jintArray>(arr.Get(2))), (Fake<jintArray>(2)));
}

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, SetsIntValues) {
  EXPECT_CALL(
      *env_, SetObjectArrayElement(Fake<jobjectArray>(), 0, Fake<jintArray>()));
  EXPECT_CALL(
      *env_, SetObjectArrayElement(Fake<jobjectArray>(), 1, Fake<jintArray>()));
  EXPECT_CALL(
      *env_, SetObjectArrayElement(Fake<jobjectArray>(), 2, Fake<jintArray>()));

  LocalArray<jint, 1> array_arg{AdoptLocal{}, Fake<jintArray>()};
  LocalArray<jint, 2> arr{std::size_t{10}, Fake<jobjectArray>()};
  arr.Set(0, array_arg);
  arr.Set(1, array_arg);
  arr.Set(2, std::move(array_arg));
}

TEST_F(JniTest, SetsObjectValues) {
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(1), 0,
                                           Fake<jobjectArray>(2)));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(1), 1,
                                           Fake<jobjectArray>(2)));
  EXPECT_CALL(*env_, SetObjectArrayElement(Fake<jobjectArray>(1), 2,
                                           Fake<jobjectArray>(2)));

  LocalArray<jobject, 1, kClass> array_arg{AdoptLocal{}, Fake<jobjectArray>(2)};
  LocalArray<jint, 2> arr{AdoptLocal{}, Fake<jobjectArray>(1)};
  arr.Set(0, array_arg);
  arr.Set(1, array_arg);
  arr.Set(2, std::move(array_arg));
}

#endif  // __clang__

}  // namespace
