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

#include <cstddef>
#include <memory>
#include <utility>

#include "javatests/com/jnibind/test/array_test_helpers_native.h"
#include "javatests/com/jnibind/test/object_test_helper_jni.h"
#include "jni_bind.h"

using ::jni::ArrayView;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::StaticRef;
using ::jni::metaprogramming::StringLiteral;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// Generic method test suitable for simple primitive types.
// Strings are passed through lambdas as method indexing is compile time.
template <typename SpanType, StringLiteral method_name_literal>
void GenericMethodTest(LocalArray<SpanType> local_arr, SpanType base) {
  // Simple lvalue pass through works as expected.
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, SpanType{1}, local_arr);

  // Simple rvalue pass through works as expected.
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, SpanType{1}, std::move(local_arr));

  // Building a new array, and setting all the values by hand works.
  LocalArray<SpanType> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = base + static_cast<SpanType>(i);
    }
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, SpanType{1}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = base + static_cast<SpanType>(i);
    }
  }

  // And it can be iterated over.
  SpanType i{0};
  for (SpanType& val : new_array.Pin(true)) {
    // Below we modify the value 2 times but with a bool there's no way,
    // so we instead just set it back.
    val = base + i;
    i++;
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, SpanType{1}, new_array);

  // You can build an array of null values and set the values manually.
  LocalArray<SpanType> arr_built_from_null{5};
  SpanType j{0};
  for (SpanType& val : new_array.Pin(true)) {
    // Below we modify the value 2 times but with a bool there's no way,
    // so we instead just set it back.
    val = base + j;
    j++;
  }
  StaticRef<kArrayTestHelperClass>{}.Call<method_name_literal>(
      base, SpanType{1}, new_array);
}

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayTestMethodRank1_jniTearDown(
    JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeBooleanTests(
    JNIEnv* env, jclass, jbooleanArray boolean_array) {
  GenericMethodTest<jboolean, "assertBoolean1D">(
      LocalArray<jboolean>{boolean_array}, jboolean{true});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeByteTests(
    JNIEnv* env, jclass, jbyteArray byte_array) {
  GenericMethodTest<jbyte, "assertByte1D">(LocalArray<jbyte>{byte_array},
                                           jbyte{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeCharTests(
    JNIEnv* env, jclass, jcharArray char_array) {
  GenericMethodTest<jchar, "assertChar1D">(LocalArray<jchar>{char_array},
                                           jchar{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeShortTests(
    JNIEnv* env, jclass, jshortArray short_array) {
  GenericMethodTest<jshort, "assertShort1D">(LocalArray<jshort>{short_array},
                                             jshort{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeIntTests(JNIEnv* env, jclass,

                                                          jintArray int_array) {
  GenericMethodTest<jint, "assertInt1D">(LocalArray<jint>{int_array}, jint{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeLongTests(
    JNIEnv* env, jclass, jlongArray long_array) {
  GenericMethodTest<jlong, "assertLong1D">(LocalArray<jlong>{long_array},
                                           jlong{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeFloatTests(
    JNIEnv* env, jclass, jfloatArray float_array) {
  GenericMethodTest<jfloat, "assertFloat1D">(LocalArray<jfloat>{float_array},
                                             jfloat{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeDoubleTests(
    JNIEnv* env, jclass, jdoubleArray double_array) {
  GenericMethodTest<jdouble, "assertDouble1D">(
      LocalArray<jdouble>{double_array}, jdouble{0});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeStringTests(
    JNIEnv* env, jclass, jobjectArray object_array) {
  // Simple lvalue pass through works as expected.
  LocalArray<jstring> local_arr{object_array};
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(local_arr, true);

  // TODO(b/143908983): Currently not possible to write.
  // Simple rvalue pass through works as expected.
  /*
  StaticRef<kArrayTestHelperClass>{}("assertString1D",
                   LocalArray<jstring>{
                       { "Foo", std::string{"baz"}, LocalString{"Baz"} }
                   });
  */

  // Building a new array, and setting all the values by hand works.
  LocalArray<jstring> new_array{3};
  new_array.Set(0, LocalString{"Foo"});
  new_array.Set(1, LocalString{"Baz"});
  new_array.Set(2, LocalString{"Bar"});
  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(new_array, true);

  // And it can be iterated over.
  std::size_t i = 0;
  LocalArray<jstring> validator_array{3};
  for (LocalString val : new_array.Pin()) {
    validator_array.Set(i, std::move(val));
    i++;
  }

  StaticRef<kArrayTestHelperClass>{}.Call<"assertString1D">(validator_array,
                                                            true);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeObjectTests(
    JNIEnv* env, jclass, jobjectArray object_array) {
  // Creating arrays of nulls with just size works.
  LocalArray<jobject, 1, kObjectTestHelperClass> local_arr_nulls{5};
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObjectArrayOfNulls1D">(
      local_arr_nulls);

  // Simple lvalue pass through works as expected.
  LocalArray<jobject, 1, kObjectTestHelperClass> local_arr{object_array};
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject1D">(0, local_arr);

  // Simple rvalue pass through works as expected.
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject1D">(
      5, LocalArray<jobject, 1, kObjectTestHelperClass>{
             1, LocalObject<kObjectTestHelperClass>{5, 5, 5}});

  // Building a new array, and setting all the values by hand works.
  LocalObject<kObjectTestHelperClass> obj{0, 0, 0};
  LocalArray<jobject, 1, kObjectTestHelperClass> new_array{8, obj};
  {
    for (int i = 0; i < new_array.Length(); ++i) {
      new_array.Set(
          i, LocalObject<kObjectTestHelperClass>{jint{i}, jint{i}, jint{i}});
    }
  }
  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject1D">(0, new_array);

  // You can pull the view multiple times.
  {
    for (int i = 0; i < new_array.Length(); ++i) {
      new_array.Set(
          i, LocalObject<kObjectTestHelperClass>{2, 2, 2}
                 .Call<"returnNewObjectWithFieldSetToSum">(new_array.Get(i)));
    }
  }

  // And it can be iterated over.
  // Note: LocalObject is not a reference here, instead, the underlying jobject
  //  is being iterated over, and upcast to a LocalObject.
  // Also note: Pin is not to contiguous memory, so no copyback bool either.
  for (LocalObject<kObjectTestHelperClass> val : new_array.Pin()) {
    val.Access<"intVal1">().Set(val.Access<"intVal1">().Get() + 3);
    val.Access<"intVal2">().Set(val.Access<"intVal2">().Get() + 3);
    val.Access<"intVal3">().Set(val.Access<"intVal3">().Get() + 3);
  }

  StaticRef<kArrayTestHelperClass>{}.Call<"assertObject1D">(2 + 3, new_array);
}

}  // extern "C"
