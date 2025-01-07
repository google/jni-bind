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

#include <cstring>
#include <limits>
#include <memory>

#include "array_test_helpers_native.h"
#include "modulo.h"
#include "object_test_helper_jni.h"
#include "jni_bind.h"
#include "metaprogramming/lambda_string.h"
#include "metaprogramming/string_literal.h"

using ::jni::Array;
using ::jni::ArrayView;
using ::jni::Class;
using ::jni::Field;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Modulo;
using ::jni::Rank;
using ::jni::StaticRef;
using ::jni::metaprogramming::StringLiteral;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kArrayTestFieldRank2 {
    "com/jnibind/test/ArrayTestFieldRank2",

    // Fields under test: Rank 1 Primitives.
    Field {"booleanArrayField", Array{jboolean{}, Rank<2>{}}},
    Field {"byteArrayField", Array{jbyte{}, Rank<2>{}}},
    Field {"charArrayField", Array{jchar{}, Rank<2>{}}},
    Field {"shortArrayField", Array{jshort{}, Rank<2>{}}},
    Field {"intArrayField", Array{jint{}, Rank<2>{}}},
    Field {"longArrayField", Array{jlong{}, Rank<2>{}}},
    Field {"floatArrayField", Array{jfloat{}, Rank<2>{}}},
    Field {"doubleArrayField", Array{jdouble{}, Rank<2>{}}},
    Field {"stringArrayField", Array{jstring{}, Rank<2>{}}},

    // Fields under test: Rank 1 Objects.
    Field {"objectArrayField", Array{kObjectTestHelperClass, Rank<2>{}}},
};
// clang-format on

// Generic field test suitable for simple primitive types.
// Strings are passed through lambdas as field indexing is compile time.
template <typename SpanType, StringLiteral field_name_literal,
          StringLiteral method_name_literal>
void GenericFieldTest(LocalObject<kArrayTestFieldRank2> fixture,
                      SpanType max_val, SpanType base, SpanType stride) {
  LocalArray<SpanType, 2> arr{fixture.Access<field_name_literal>().Get()};

  // Field starts in default state.
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      SpanType{0}, SpanType{0}, fixture.Access<field_name_literal>().Get());

  // Updating the field manually works.
  {
    std::size_t i = 0;
    for (LocalArray<SpanType, 1> arr_rank_1 : arr.Pin()) {
      ArrayView<SpanType> pin = arr_rank_1.Pin();
      for (int j = 0; j < arr_rank_1.Length(); j++) {
        pin.ptr()[j] = Modulo(i, base, max_val);
        i++;
      }
    }

    StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
        base, SpanType{1}, fixture.Access<field_name_literal>().Get());
  }

  // Updating the field repeatedly works.
  {
    std::size_t i = 0;
    {
      for (LocalArray<SpanType, 1> arr_rank_1 : arr.Pin()) {
        // Updating the field repeatedly works.
        {
          ArrayView<SpanType> pin = arr_rank_1.Pin();
          for (int j = 0; j < arr_rank_1.Length(); ++j) {
            pin.ptr()[j] =
                Modulo(stride * static_cast<SpanType>(i), base, max_val);
            i++;
          }
        }
      }
      StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
          base, stride, fixture.Access<field_name_literal>().Get());
    }
  }

  // Not pinning the value has no effect.
  {
    for (LocalArray<SpanType, 1> arr_rank_1 : arr.Pin()) {
      std::memset(arr_rank_1.Pin(false).ptr(), 0,
                  arr_rank_1.Length() * sizeof(SpanType));
    }
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, stride, fixture.Access<field_name_literal>().Get());
}

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestFieldRank2_jniTearDown(
    JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeBooleanTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jboolean, "booleanArrayField", "assertBoolean2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture}, jboolean{2}, jboolean{0},
      jboolean{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jbyte, "byteArrayField", "assertByte2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jbyte>::max(), jbyte{10}, jbyte{2});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jchar, "charArrayField", "assertChar2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jchar>::max(), jchar{0}, jchar{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jshort, "shortArrayField", "assertShort2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jshort>::max(), jshort{0}, jshort{1});
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestFieldRank2_nativeIntTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jint, "intArrayField", "assertInt2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jint>::max(), jint{0}, jint{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jlong, "longArrayField", "assertLong2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jlong>::max(), jlong{0}, jlong{1});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jfloat, "floatArrayField", "assertFloat2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jfloat>::max(), jfloat{0.f}, jfloat{1.f});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  GenericFieldTest<jdouble, "doubleArrayField", "assertDouble2D">(
      LocalObject<kArrayTestFieldRank2>{test_fixture},
      std::numeric_limits<jdouble>::max(), jdouble{0}, jdouble{1});
}

// TODO(b/143908983): This is broken, but using regular `jobject` works,
// so there is still an alternative path to be taken.
JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestFieldRank2_nativeStringTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  /*
  LocalObject<kArrayTestFieldRank2> fixture{test_fixture};

  // Field starts in default state.
  StaticRef<kArrayTestHelperClass>{}(
      "assertString1D", fixture.Access<"stringArrayField">().Get(),
  jboolean{false});

  // Updating the field manually works.
  LocalArray<jstring> arr = fixture.Access<"stringArrayField">().Get();
  arr.Set(0, LocalString{"Foo"});
  arr.Set(1, LocalString{"Baz"});
  arr.Set(2, LocalString{"Bar"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">( arr, true);

  // Updating the field repeatedly works.
  arr.Set(0, LocalString{"FAKE"});
  arr.Set(1, LocalString{"DEAD"});
  arr.Set(2, LocalString{"BEEF"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">( arr, false);

  // Updating the field repeatedly works.
  arr.Set(0, "Foo");
  const char* kBaz = "Baz";
  arr.Set(1, kBaz);
  arr.Set(2, std::string{"Bar"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">( arr, true);

  // Iterating over values works.
  static constexpr std::array vals{"Foo", "Baz", "Bar"};
  int i = 0;
  for (LocalString local_string : arr.Pin()) {
    StaticRef<kArrayTestHelperClass>{}.Call<"assertString">( local_string,
  vals[i]);
    ++i;
  }

  // Iterating over passed values works.
  i = 0;
  for (LocalString local_string : LocalArray<jstring>{object_array}.Pin()) {
    StaticRef<kArrayTestHelperClass>{}.Call<"assertString">( local_string,
  vals[i]);
    ++i;
  }
  */
}

void Java_com_jnibind_test_ArrayTestFieldRank2_nativeObjectTests(
    JNIEnv* env, jclass, jobject test_fixture) {
  LocalObject<kArrayTestFieldRank2> fixture{test_fixture};
  LocalArray<jobject, 2, kObjectTestHelperClass> arr =
      fixture.Access<"objectArrayField">().Get();

  int i = 0;
  {
    for (LocalArray<jobject, 1, kObjectTestHelperClass> arr_rank_1 :
         arr.Pin()) {
      for (LocalObject<kObjectTestHelperClass> obj : arr_rank_1.Pin()) {
        obj.Access<"intVal1">().Set(i);
        i++;
        obj.Access<"intVal2">().Set(i);
        i++;
        obj.Access<"intVal3">().Set(i);
        i++;
      }
    }
  }

  // Object on fixture is already updated.
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject2D">(
      0, 1, fixture.Access<"objectArrayField">().Get());

  // But using local reference works too.
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject2D">(0, 1, arr);
}

}  // extern "C"
