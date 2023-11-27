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

#include <array>
#include <cstring>
#include <memory>

#include "object_test_helper_jni.h"
#include "jni_bind.h"
#include "metaprogramming/lambda_string.h"

using ::jni::Array;
using ::jni::ArrayView;
using ::jni::Class;
using ::jni::Field;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kArrayTestFieldRank1 {
    "com/jnibind/test/ArrayTestFieldRank1",

    // Validator functions.
    Method {"booleanArray", Return<void>{}, Params{jboolean{}, jboolean{}, Array{jboolean{}}}},
    Method {"byteArray", Return<void>{}, Params{jbyte{},jbyte{}, Array{jbyte{}}}},
    Method {"charArray", Return<void>{}, Params{jchar{}, jchar{}, Array{jchar{}}}},
    Method {"shortArray", Return<void>{}, Params{jshort{},jshort{}, Array{jshort{}}}},
    Method {"intArray", Return<void>{}, Params{int{},int{}, Array{jint{}}}},
    Method {"longArray", Return<void>{}, Params{jlong{},jlong{}, Array{jlong{}}}},
    Method {"floatArray", Return<void>{}, Params{float{},float{}, Array{jfloat{}}}},
    Method {"doubleArray", Return<void>{}, Params{jdouble{},jdouble{}, Array{jdouble{}}}},

    Method {"stringArray", Return<void>{}, Params{Array{jstring{}}, jboolean{}}},
    Method {"stringEqual", Return<void>{}, Params{jboolean{}, jstring{}, jstring{}}},

    Method {"object", Return<void>{}, Params{int{}, kObjectTestHelperClass}},
    Method {"objectArray", Return<void>{}, Params{int{}, Array{kObjectTestHelperClass}}},

    // Fields under test: Rank 1 Primitives.
    Field {"booleanArrayField", Array{jboolean{}}},
    Field {"byteArrayField", Array{jbyte{}}},
    Field {"charArrayField", Array{jchar{}}},
    Field {"shortArrayField", Array{jshort{}}},
    Field {"intArrayField", Array{jint{}}},
    Field {"longArrayField", Array{jlong{}}},
    Field {"floatArrayField", Array{jfloat{}}},
    Field {"doubleArrayField", Array{jdouble{}}},
    Field {"stringArrayField", Array{jstring{}}},

    // Fields under test: Rank 1 Objects.
    Field {"objectArrayField", Array{kObjectTestHelperClass}},
};
// clang-format on

// Generic field test suitable for simple primitive types.
// Strings are passed through lambdas as field indexing is compile time.
template <typename SpanType, typename FieldNameLambda,
          typename MethodNameLambda>
void GenericFieldTest(LocalObject<kArrayTestFieldRank1> fixture,
                      SpanType max_val, SpanType base, SpanType stride,
                      FieldNameLambda field_name_lambda,
                      MethodNameLambda method_name_lambda) {
  // Field starts in default state.
  LocalArray<SpanType> arr{fixture[field_name_lambda()].Get()};
  fixture(method_name_lambda(), SpanType{0}, SpanType{0},
          fixture[field_name_lambda()].Get());

  // Updating the field manually works.
  {
    ArrayView<SpanType> pin = arr.Pin();
    for (int i = 0; i < arr.Length(); ++i) {
      pin.ptr()[i] = static_cast<SpanType>(i * stride);
    }
  }

  fixture(method_name_lambda(), SpanType{0}, stride,
          fixture[field_name_lambda()].Get());

  // Updating the field repeatedly works.
  {
    ArrayView<SpanType> pin = arr.Pin();
    for (int i = 0; i < arr.Length(); ++i) {
      pin.ptr()[i] = base + static_cast<SpanType>(stride * i);
    }
  }
  fixture(method_name_lambda(), base, stride,
          fixture[field_name_lambda()].Get());

  // Not pinning the value has no effect.
  { std::memset(arr.Pin(false).ptr(), 0, arr.Length() * sizeof(SpanType)); }
  fixture(method_name_lambda(), base, stride,
          fixture[field_name_lambda()].Get());
}

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestFieldRank1_jniTearDown(
    JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeBooleanTests(
    JNIEnv* env, jclass, jobject test_fixture, jbooleanArray boolean_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   jboolean{true}, jboolean{0}, jboolean{true},
                   STR("booleanArrayField"), STR("booleanArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture, jbyteArray byte_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jbyte>::max(), jbyte{10}, jbyte{1},
                   STR("byteArrayField"), STR("byteArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture, jcharArray char_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jchar>::max(), jchar{10}, jchar{1},
                   STR("charArrayField"), STR("charArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture, jshortArray short_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jshort>::max(), jshort{10}, jshort{1},
                   STR("shortArrayField"), STR("shortArray"));
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestFieldRank1_nativeIntTests(
    JNIEnv* env, jclass, jobject test_fixture, jintArray int_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jint>::max(), jint{10}, jint{1},
                   STR("intArrayField"), STR("intArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture, jlongArray long_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jlong>::max(), jlong{10}, jlong{1},
                   STR("longArrayField"), STR("longArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture, jfloatArray float_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jfloat>::max(), jfloat{10.f},
                   jfloat{1.f}, STR("floatArrayField"), STR("floatArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture, jdoubleArray double_array) {
  GenericFieldTest(LocalObject<kArrayTestFieldRank1>{test_fixture},
                   std::numeric_limits<jdouble>::max(), jdouble{10}, jdouble{1},
                   STR("doubleArrayField"), STR("doubleArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeStringTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray object_array) {
  LocalObject<kArrayTestFieldRank1> fixture{test_fixture};

  // Field starts in default state.
  fixture("stringArray", fixture["stringArrayField"].Get(), jboolean{false});

  // Updating the field manually works.
  LocalArray<jstring> arr = fixture["stringArrayField"].Get();
  arr.Set(0, LocalString{"Foo"});
  arr.Set(1, LocalString{"Baz"});
  arr.Set(2, LocalString{"Bar"});
  fixture("stringArray", arr, true);

  // Updating the field repeatedly works.
  arr.Set(0, LocalString{"FAKE"});
  arr.Set(1, LocalString{"DEAD"});
  arr.Set(2, LocalString{"BEEF"});
  fixture("stringArray", arr, false);

  // Updating the field repeatedly works.
  arr.Set(0, "Foo");
  const char* kBaz = "Baz";
  arr.Set(1, kBaz);
  arr.Set(2, std::string{"Bar"});
  fixture("stringArray", arr, true);

  // Iterating over values works.
  static constexpr std::array vals{"Foo", "Baz", "Bar"};
  int i = 0;
  for (LocalString local_string : arr.Pin()) {
    fixture("stringEqual", jboolean{true}, local_string, vals[i]);
    ++i;
  }
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeObjectTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray) {
  LocalObject<kArrayTestFieldRank1> fixture{test_fixture};
  LocalArray<jobject, 1, kObjectTestHelperClass> arr =
      fixture["objectArrayField"].Get();

  int i = 0;
  for (LocalObject<kObjectTestHelperClass> new_obj :
       fixture["objectArrayField"].Get().Pin()) {
    fixture("object", i, new_obj);
    i++;
  }
}

}  // extern "C"
