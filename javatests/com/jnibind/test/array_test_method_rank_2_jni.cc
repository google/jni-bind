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
#include <limits>
#include <memory>

#include "modulo.h"
#include "javatests/com/jnibind/test/object_test_helper_jni.h"
#include "jni_bind.h"
#include "metaprogramming/lambda_string.h"

using ::jni::Array;
using ::jni::Class;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Modulo;
using ::jni::Params;
using ::jni::Rank;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kFixture {
    "com/jnibind/test/ArrayTestMethodRank2",
    Method {"boolean2D", Return<void>{}, Params{jboolean{}, Array{jboolean{}, Rank<2>{}}}},
    Method {"byte2D", Return<void>{}, Params{jbyte{}, Array{jbyte{}, Rank<2>{}}}},
    Method {"char2D", Return<void>{}, Params{jchar{}, Array{jchar{}, Rank<2>{}}}},
    Method {"short2D", Return<void>{}, Params{jshort{}, Array{jshort{}, Rank<2>{}}}},
    Method {"int2D", Return<void>{}, Params{int{}, Array{jint{}, Rank<2>{}}}},
    Method {"long2D", Return<void>{}, Params{jlong{}, Array{jlong{}, Rank<2>{}}}},
    Method {"float2D", Return<void>{}, Params{float{}, Array{jfloat{}, Rank<2>{}}}},
    Method {"double2D", Return<void>{}, Params{jdouble{}, Array{jdouble{}, Rank<2>{}}}},
    Method {"string2D", Return<void>{}, Params{Array{jstring{}, Rank<2>{}}}},
    Method {"object1D", Return<void>{}, Params{Array{kObjectTestHelperClass, Rank<1>{}}}},
    Method {"object2D", Return<void>{}, Params{int{}, Array{kObjectTestHelperClass, Rank<2>{}}}},
};
// clang-format on

// Generic method test suitable for simple primitive types.
// Strings are passed through lambdas as method indexing is compile time.
template <typename SpanType, typename MethodNameLambda>
void GenericMethodTest(
    LocalObject<kFixture> fixture, MethodNameLambda method_name_lambda,
    LocalArray<SpanType, 2> arr,
    SpanType max_val = std::numeric_limits<SpanType>::max()) {
  // Simple lvalue pass through works as expected.
  fixture(method_name_lambda(), SpanType{0}, arr);

  // Simple rvalue pass through works as expected.
  fixture(method_name_lambda(), SpanType{0}, std::move(arr));

  // Building a new array, and setting all the values by hand works.
  LocalArray<SpanType, 2> new_array{3};

  {
    LocalArray<SpanType, 1> row1{3};
    row1.Pin().ptr()[0] = Modulo(0, {0}, max_val);
    row1.Pin().ptr()[1] = Modulo(1, {0}, max_val);
    row1.Pin().ptr()[2] = Modulo(2, {0}, max_val);

    LocalArray<SpanType, 1> row2{3};
    row2.Pin().ptr()[0] = Modulo(3, {0}, max_val);
    row2.Pin().ptr()[1] = Modulo(4, {0}, max_val);
    row2.Pin().ptr()[2] = Modulo(5, {0}, max_val);

    LocalArray<SpanType, 1> row3{3};
    row3.Pin().ptr()[0] = Modulo(6, {0}, max_val);
    row3.Pin().ptr()[1] = Modulo(7, {0}, max_val);
    row3.Pin().ptr()[2] = Modulo(8, {0}, max_val);

    new_array.Set(0, row1);
    new_array.Set(1, row2);
    new_array.Set(2, row3);

    fixture(method_name_lambda(), SpanType{0}, new_array);
  }

  // You can pull the view multiple times with iterators (each value ticked 1).
  {
    for (LocalArray<SpanType, 1> inner_array : new_array.Pin()) {
      for (SpanType& val : inner_array.Pin()) {
        val = Modulo(1, val, max_val);
      }
    }
    fixture(method_name_lambda(), SpanType{1}, new_array);
  }

  // You can pull the view multiple times with raw loops.
  {
    for (int i = 0; i < new_array.Length(); ++i) {
      LocalArray<SpanType> inn_arr{new_array.Get(i)};
      for (int j = 0; j < inn_arr.Length(); ++j) {
        // Note: You must explicitly hold the pin beyond any ptr.
        auto pin_val = inn_arr.Pin();
        SpanType* val_ptr = &pin_val.ptr()[j];
        *val_ptr = Modulo(1, *val_ptr, max_val);
      }
    }
  }

  // Each variant increments base by 1, so 2 is used here.
  fixture(method_name_lambda(), Modulo(2, {0}, max_val), new_array);
}

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestMethodRank2_jniTearDown(
    JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// 2D Array Tests.
////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeBooleanTests2D(
    JNIEnv*, jclass, jobject fixture, jobjectArray arr) {
  // Boolean has a max of "2" which makes the generic moduloing logic work.
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("boolean2D"),
                    LocalArray<jboolean, 2>{arr}, jboolean{2});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeByteTests2D(JNIEnv*, jclass,
                                                             jobject fixture,
                                                             jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("byte2D"),
                    LocalArray<jbyte, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeCharTests2D(JNIEnv*, jclass,
                                                             jobject fixture,
                                                             jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("char2D"),
                    LocalArray<jchar, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeShortTests2D(
    JNIEnv*, jclass, jobject fixture, jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("short2D"),
                    LocalArray<jshort, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeIntTests2D(JNIEnv*, jclass,
                                                            jobject fixture,
                                                            jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("int2D"),
                    LocalArray<jint, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeLongTests2D(JNIEnv*, jclass,
                                                             jobject fixture,
                                                             jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("long2D"),
                    LocalArray<jlong, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeFloatTests2D(
    JNIEnv*, jclass, jobject fixture, jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("float2D"),
                    LocalArray<jfloat, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeDoubleTests2D(
    JNIEnv*, jclass, jobject fixture, jobjectArray arr) {
  GenericMethodTest(LocalObject<kFixture>{fixture}, STR("double2D"),
                    LocalArray<jdouble, 2>{arr});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank2_nativeObjectTests2D(
    JNIEnv* env, jclass, jobject fixture_jobject,
    jobjectArray arr_jobjectArray) {
  LocalObject<kFixture> fixture{fixture_jobject};

  // Simple lvalue pass through works as expected.
  LocalArray<jobject, 2, kObjectTestHelperClass> arr{arr_jobjectArray};
  fixture("object2D", 0, arr);

  // Simple rvalue pass through works as expected.
  fixture("object2D", 0, std::move(arr));

  // Building a new array, and setting all the values by hand works.
  LocalArray<jobject, 2, kObjectTestHelperClass> new_array{3, nullptr};

  LocalArray<jobject, 1, kObjectTestHelperClass> row1{3};
  row1.Set(0, LocalObject<kObjectTestHelperClass>{0, 0, 0});
  row1.Set(1, LocalObject<kObjectTestHelperClass>{1, 1, 1});
  row1.Set(2, LocalObject<kObjectTestHelperClass>{2, 2, 2});

  LocalArray<jobject, 1, kObjectTestHelperClass> row2{3};
  row2.Set(0, LocalObject<kObjectTestHelperClass>{3, 3, 3});
  row2.Set(1, LocalObject<kObjectTestHelperClass>{4, 4, 4});
  row2.Set(2, LocalObject<kObjectTestHelperClass>{5, 5, 5});

  LocalArray<jobject, 1, kObjectTestHelperClass> row3{3};
  row3.Set(0, LocalObject<kObjectTestHelperClass>{6, 6, 6});
  row3.Set(1, LocalObject<kObjectTestHelperClass>{7, 7, 7});
  row3.Set(2, LocalObject<kObjectTestHelperClass>{8, 8, 8});

  new_array.Set(0, row1);
  new_array.Set(1, row2);
  new_array.Set(2, row3);

  fixture("object2D", 0, new_array);

  // You can pull the view multiple times with iterators (each value ticked 1).
  {
    for (LocalArray<jobject, 1> inner_array : new_array.Pin()) {
      for (LocalObject<kObjectTestHelperClass> obj : inner_array.Pin()) {
        obj("increment", 1);
      }
    }
    fixture("object2D", 1, new_array);
  }

  // You can pull the view multiple times with raw loops.
  {
    for (int i = 0; i < new_array.Length(); ++i) {
      LocalArray<jobject, 1, kObjectTestHelperClass> inn_arr{new_array.Get(i)};
      for (int j = 0; j < inn_arr.Length(); ++j) {
        inn_arr.Get(j)("increment", 1);
      }
    }
  }

  // Each variant increments base by 1, so 2 is used here.
  fixture("object2D", 2, new_array);
}

}  // extern "C"
