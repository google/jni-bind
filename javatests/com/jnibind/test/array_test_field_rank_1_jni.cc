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
#include <limits>
#include <memory>
#include <string>

#include "array_test_helpers_native.h"
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
using ::jni::StaticRef;
using ::jni::metaprogramming::StringLiteral;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kArrayTestFieldRank1 {
    "com/jnibind/test/ArrayTestFieldRank1",

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
template <typename SpanType, StringLiteral field_name_literal,
          StringLiteral method_name_literal>
void GenericFieldTest(LocalObject<kArrayTestFieldRank1> fixture,
                      SpanType max_val, SpanType base, SpanType stride) {
  // Field starts in default state.
  LocalArray<SpanType> arr{fixture.Access<field_name_literal>().Get()};
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      SpanType{0}, SpanType{0}, fixture.Access<field_name_literal>().Get());

  // Updating the field manually works.
  {
    ArrayView<SpanType> pin = arr.Pin();
    for (int i = 0; i < arr.Length(); ++i) {
      pin.ptr()[i] = static_cast<SpanType>(i * SpanType{1});
    }
  }

  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      SpanType{0}, SpanType{1}, fixture.Access<field_name_literal>().Get());

  // Updating the field repeatedly works.
  {
    ArrayView<SpanType> pin = arr.Pin();
    for (int i = 0; i < arr.Length(); ++i) {
      pin.ptr()[i] = base + static_cast<SpanType>(stride * i);
    }
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, stride, fixture.Access<field_name_literal>().Get());

  // Not pinning the value has no effect.
  {
    std::memset(arr.Pin(false).ptr(), 0, arr.Length() * sizeof(SpanType));
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, stride, fixture.Access<field_name_literal>().Get());
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
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jboolean, "booleanArrayField", "assertBoolean1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture}, jboolean{true},
      jboolean{0}, jboolean{true});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jbyte, "byteArrayField", "assertByte1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jbyte>::max(), jbyte{10}, jbyte{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jchar, "charArrayField", "assertChar1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jchar>::max(), jchar{10}, jchar{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jshort, "shortArrayField", "assertShort1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jshort>::max(), jshort{10}, jshort{1});
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestFieldRank1_nativeIntTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jint, "intArrayField", "assertInt1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jint>::max(), jint{10}, jint{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jlong, "longArrayField", "assertLong1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jlong>::max(), jlong{10}, jlong{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jfloat, "floatArrayField", "assertFloat1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jfloat>::max(), jfloat{10.f}, jfloat{1.f});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jdouble, "doubleArrayField", "assertDouble1D">(
      LocalObject<kArrayTestFieldRank1>{test_fixture},
      std::numeric_limits<jdouble>::max(), jdouble{10}, jdouble{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeStringTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray object_array) {
  LocalObject<kArrayTestFieldRank1> fixture{test_fixture};

  // Field starts in default state.
  StaticRef<kArrayTestHelperClass>{}("assertString1D",
                                     fixture.Access<"stringArrayField">().Get(),
                                     jboolean{false});

  // Updating the field manually works.
  LocalArray<jstring> arr = fixture.Access<"stringArrayField">().Get();
  arr.Set(0, LocalString{"Foo"});
  arr.Set(1, LocalString{"Baz"});
  arr.Set(2, LocalString{"Bar"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(arr, true);

  // Updating the field repeatedly works.
  arr.Set(0, LocalString{"FAKE"});
  arr.Set(1, LocalString{"DEAD"});
  arr.Set(2, LocalString{"BEEF"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(arr, false);

  // Updating the field repeatedly works.
  arr.Set(0, "Foo");
  const char* kBaz = "Baz";
  arr.Set(1, kBaz);
  arr.Set(2, std::string{"Bar"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(arr, true);

  // Iterating over values works.
  static constexpr std::array vals{"Foo", "Baz", "Bar"};
  int i = 0;
  for (LocalString local_string : arr.Pin()) {
    StaticRef<kArrayTestHelperClass>{}.Call<"assertString">(local_string,
                                                            vals[i]);
    ++i;
  }

  // Iterating over passed values works.
  i = 0;
  for (LocalString local_string : LocalArray<jstring>{object_array}.Pin()) {
    StaticRef<kArrayTestHelperClass>{}.Call<"assertString">(local_string,
                                                            vals[i]);
    ++i;
  }
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank1_nativeObjectTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray) {
  LocalObject<kArrayTestFieldRank1> fixture{test_fixture};
  LocalArray<jobject, 1, kObjectTestHelperClass> arr =
      fixture.Access<"objectArrayField">().Get();

  int i = 0;
  for (LocalObject<kObjectTestHelperClass> new_obj :
       fixture.Access<"objectArrayField">().Get().Pin()) {
    StaticRef<kArrayTestHelperClass>{}.Call<"assertObject">(i, new_obj);
    i++;
  }
}

}  // extern "C"
