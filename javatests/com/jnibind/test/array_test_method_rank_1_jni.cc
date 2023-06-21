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

#include <memory>

#include "object_test_helper_jni.h"
#include "jni_bind.h"
#include "metaprogramming/lambda_string.h"

using ::jni::Array;
using ::jni::ArrayView;
using ::jni::Class;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::RegularToArrayTypeMap_t;
using ::jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kArrayTestMethodRank1 {
    "com/jnibind/test/ArrayTestMethodRank1",
    Method {"booleanArray", Return<void>{}, Params{jboolean{}, Array{jboolean{}}}},
    Method {"byteArray", Return<void>{}, Params{jbyte{}, Array{jbyte{}}}},
    Method {"charArray", Return<void>{}, Params{jchar{}, Array{jchar{}}}},
    Method {"shortArray", Return<void>{}, Params{jshort{}, Array{jshort{}}}},
    Method {"intArray", Return<void>{}, Params{int{}, Array{jint{}}}},
    Method {"longArray", Return<void>{}, Params{jlong{}, Array{jlong{}}}},
    Method {"floatArray", Return<void>{}, Params{float{}, Array{jfloat{}}}},
    Method {"doubleArray", Return<void>{}, Params{jdouble{}, Array{jdouble{}}}},
    Method {"stringArray", Return<void>{}, Params{Array{jstring{}}}},
    Method {"objectArray", Return<void>{}, Params{int{}, Array{kObjectTestHelperClass}}},
};
// clang-format on

// Generic method test suitable for simple primitive types.
// Strings are passed through lambdas as method indexing is compile time.
template <typename SpanType, typename MethodNameLambda>
void GenericMethodTest(LocalObject<kArrayTestMethodRank1> fixture,
                       LocalArray<SpanType> local_arr, SpanType base,
                       MethodNameLambda method_name_lambda) {
  // Simple lvalue pass through works as expected.
  fixture(method_name_lambda(), base, local_arr);

  // Simple rvalue pass through works as expected.
  fixture(method_name_lambda(), base, std::move(local_arr));

  // Building a new array, and setting all the values by hand works.
  LocalArray<SpanType> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = base + static_cast<SpanType>(i);
    }
  }
  fixture(method_name_lambda(), base, new_array);

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

  fixture(method_name_lambda(), base, new_array);
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
    JNIEnv* env, jclass, jobject test_fixture, jbooleanArray boolean_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jboolean>{boolean_array}, jboolean{true},
                    STR("booleanArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture, jbyteArray byte_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jbyte>{byte_array}, jbyte{0}, STR("byteArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture, jcharArray char_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jchar>{char_array}, jchar{0}, STR("charArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture, jshortArray short_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jshort>{short_array}, jshort{0},
                    STR("shortArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeIntTests(JNIEnv* env, jclass,
                                                          jobject test_fixture,
                                                          jintArray int_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jint>{int_array}, jint{0}, STR("intArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture, jlongArray long_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jlong>{long_array}, jlong{0}, STR("longArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture, jfloatArray float_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jfloat>{float_array}, jfloat{0},
                    STR("floatArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture, jdoubleArray double_array) {
  GenericMethodTest(LocalObject<kArrayTestMethodRank1>{test_fixture},
                    LocalArray<jdouble>{double_array}, jdouble{0},
                    STR("doubleArray"));
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeStringTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray object_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jstring> local_arr{object_array};
  fixture("stringArray", local_arr);

  // TODO(b/143908983): Currently not possible to write.
  // Simple rvalue pass through works as expected.
  /*
  fixture("stringArray",
                   LocalArray<jstring>{
                       { "Foo", std::string{"baz"}, LocalString{"Baz"} }
                   });
  */

  // Building a new array, and setting all the values by hand works.
  LocalArray<jstring> new_array{3};
  new_array.Set(0, LocalString{"Foo"});
  new_array.Set(1, LocalString{"Baz"});
  new_array.Set(2, LocalString{"Bar"});
  fixture("stringArray", new_array);

  // And it can be iterated over.
  std::size_t i = 0;
  LocalArray<jstring> validator_array{3};
  for (LocalString val : new_array.Pin()) {
    validator_array.Set(i, std::move(val));
    i++;
  }

  fixture("stringArray", validator_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeObjectTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray object_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jobject, 1, kObjectTestHelperClass> local_arr{object_array};
  fixture("objectArray", 0, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("objectArray", 5,
          LocalArray<jobject, 1, kObjectTestHelperClass>{
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
  fixture("objectArray", 0, new_array);

  // You can pull the view multiple times.
  {
    for (int i = 0; i < new_array.Length(); ++i) {
      new_array.Set(i,
                    LocalObject<kObjectTestHelperClass>{2, 2, 2}(
                        "returnNewObjectWithFieldSetToSum", new_array.Get(i)));
    }
  }

  // And it can be iterated over.
  // Note: LocalObject is not a reference here, instead, the underlying jobject
  //  is being iterated over, and upcast to a LocalObject.
  // Also note: Pin is not to contiguous memory, so no copyback bool either.
  for (LocalObject<kObjectTestHelperClass> val : new_array.Pin()) {
    val["intVal1"].Set(val["intVal1"].Get() + 3);
    val["intVal2"].Set(val["intVal2"].Get() + 3);
    val["intVal3"].Set(val["intVal3"].Get() + 3);
  }

  fixture("objectArray", 2 + 3, new_array);
}

}  // extern "C"
