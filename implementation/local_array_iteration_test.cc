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
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::ArrayView;
using ::jni::Class;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

static constexpr Class kClass{"kClass"};

////////////////////////////////////////////////////////////////////////////////
// Rank 0.
////////////////////////////////////////////////////////////////////////////////
TEST_F(JniTest, IteratesOver1DRange) {
  std::array expected{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(10));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillRepeatedly(Return(expected.data()));

  LocalArray<jint, 1> new_array{AdoptLocal{}, Fake<jintArray>()};

  int i{0};
  for (jint val : new_array.Pin()) {
    EXPECT_EQ(val, expected[i]);
    ++i;
  }
  EXPECT_EQ(i, 10);
}

TEST_F(JniTest, WorksWithSTLComparison) {
  std::array expected{10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(10));
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillRepeatedly(Return(expected.data()));

  LocalArray<jint, 1> new_array{AdoptLocal{}, Fake<jintArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, WorksWithSTLComparisonOfObjects) {
  std::array expected{Fake<jobject>(1), Fake<jobject>(2), Fake<jobject>(3)};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jobject>(1)))
      .WillOnce(Return(Fake<jobject>(2)))
      .WillOnce(Return(Fake<jobject>(3)));

  LocalArray<jobject, 1, kClass> new_array{Fake<jobjectArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, WorksWithSTLComparisonOfRichlyDecoratedObjects) {
  std::array expected{LocalObject<kClass>{AdoptLocal{}, Fake<jobject>(1)},
                      LocalObject<kClass>{AdoptLocal{}, Fake<jobject>(2)},
                      LocalObject<kClass>{AdoptLocal{}, Fake<jobject>(3)}};

  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(3));
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jobject>(1)))
      .WillOnce(Return(Fake<jobject>(2)))
      .WillOnce(Return(Fake<jobject>(3)));

  LocalArray<jobject, 1, kClass> new_array{AdoptLocal{}, Fake<jobjectArray>()};
  ArrayView array_view = new_array.Pin();
  EXPECT_TRUE(
      std::equal(array_view.begin(), array_view.end(), expected.begin()));
}

TEST_F(JniTest, 2D_Iterates) {
  int a[5] = {1, 2, 3, 4, 5};
  std::array expected{1, 2, 3, 4, 5};

  EXPECT_CALL(*env_, FindClass(StrEq("[I")));
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobjectArray>()));
  EXPECT_CALL(*env_, GetArrayLength)
      .WillOnce(Return(5))   // outer
      .WillOnce(Return(1))   // inner: 1, running sum: 1
      .WillOnce(Return(2))   // inner: 1, running sum: 3
      .WillOnce(Return(3))   // inner: 1, running sum: 6
      .WillOnce(Return(4))   // inner: 1, running sum: 10
      .WillOnce(Return(5));  // inner: 1, running sum: 15

  // 5: Once per outer array element.
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jintArray>(1)))
      .WillOnce(Return(Fake<jintArray>(2)))
      .WillOnce(Return(Fake<jintArray>(3)))
      .WillOnce(Return(Fake<jintArray>(4)))
      .WillOnce(Return(Fake<jintArray>(5)));

  // All the returned intArrays are deleted.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(3)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(4)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(5)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>()));

  // 5 (outer array length) * 2 (pins per) = 10
  EXPECT_CALL(*env_, GetIntArrayElements).Times(10).WillRepeatedly(Return(a));
  EXPECT_CALL(*env_, ReleaseIntArrayElements).Times(10);

  LocalArray<jint, 2> new_array{5, Fake<jobjectArray>()};

  int sum = 0;
  for (LocalArray<jint> arr_1d : new_array.Pin()) {
    // Note: Each `Pin` below triggers a separate `GetIntArrayElements`.
    {
      auto pinn = arr_1d.Pin();
      EXPECT_TRUE(std::equal(pinn.begin(), pinn.end(), expected.begin()));
    }

    // Note: GetArrayLength is not called again (it's cached).
    {
      for (jint val : arr_1d.Pin()) {
        sum += val;
      }
    }
  }

  // 1 + 3 + 6 + 10 + 15 = 35
  EXPECT_EQ(sum, 35);
}

// Identical to above except with raw loops.
TEST_F(JniTest, 2D_Iterates_Raw_loops) {
  int a[5] = {1, 2, 3, 4, 5};
  std::array expected{1, 2, 3, 4, 5};

  EXPECT_CALL(*env_, FindClass(StrEq("[I")));
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobjectArray>()));
  EXPECT_CALL(*env_, GetArrayLength)
      .WillOnce(Return(5))   // outer
      .WillOnce(Return(1))   // inner: 1, running sum: 1
      .WillOnce(Return(2))   // inner: 1, running sum: 3
      .WillOnce(Return(3))   // inner: 1, running sum: 6
      .WillOnce(Return(4))   // inner: 1, running sum: 10
      .WillOnce(Return(5));  // inner: 1, running sum: 15

  // 5: Once per outer array element.
  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jintArray>(1)))
      .WillOnce(Return(Fake<jintArray>(2)))
      .WillOnce(Return(Fake<jintArray>(3)))
      .WillOnce(Return(Fake<jintArray>(4)))
      .WillOnce(Return(Fake<jintArray>(5)));

  // All the returned intArrays are deleted.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(3)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(4)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jintArray>(5)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>()));

  // 5 (outer array length) * 2 (pins per) = 10
  EXPECT_CALL(*env_, GetIntArrayElements).Times(10).WillRepeatedly(Return(a));
  EXPECT_CALL(*env_, ReleaseIntArrayElements).Times(10);

  LocalArray<jint, 2> new_array{5, Fake<jobjectArray>()};

  int sum = 0;
  int loop_size = new_array.Length();
  for (int i = 0; i < loop_size; ++i) {
    LocalArray<jint, 1> arr_1d{new_array.Get(i)};

    // Note: Each `Pin` below triggers a separate `GetIntArrayElements`.
    {
      auto pinn = arr_1d.Pin();
      EXPECT_TRUE(std::equal(pinn.begin(), pinn.end(), expected.begin()));
    }

    // Note: GetArrayLength is not called again (it's cached).
    {
      for (jint val : arr_1d.Pin()) {
        sum += val;
      }
    }
  }

  // 1 + 3 + 6 + 10 + 15 = 35
  EXPECT_EQ(sum, 35);
}

// Identical to above except with object loops.
TEST_F(JniTest, 2D_Iterates_Raw_loops_of_Objects) {
  EXPECT_CALL(*env_, FindClass(StrEq("[LkClass;")));
  EXPECT_CALL(*env_, NewObjectArray(5, _, Fake<jobjectArray>(100)));
  EXPECT_CALL(*env_, GetArrayLength).WillOnce(Return(5));  // outer

  EXPECT_CALL(*env_, GetObjectArrayElement)
      .WillOnce(Return(Fake<jobjectArray>(1)))
      .WillOnce(Return(Fake<jobjectArray>(2)))
      .WillOnce(Return(Fake<jobjectArray>(3)))
      .WillOnce(Return(Fake<jobjectArray>(4)))
      .WillOnce(Return(Fake<jobjectArray>(5)));

  // All the returned objectArrays are deleted.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jclass>()));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>(1)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>(2)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>(3)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>(4)));
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>(5)));

  // Note: This is just 0 (default), not 100. 100 is the sample (i.e. template)
  // object, no arg is the default that is created.
  EXPECT_CALL(*env_, DeleteLocalRef(Fake<jobjectArray>()));

  LocalArray<jobject, 2, kClass> new_array{5, Fake<jobjectArray>(100)};
  EXPECT_EQ(jobject{new_array}, Fake<jobjectArray>());

  int i = 1;
  for (LocalArray<jobject, 1, kClass> arr : new_array.Pin()) {
    EXPECT_EQ(jobject(arr), Fake<jobjectArray>(i));
    ++i;
  }
}

}  // namespace
