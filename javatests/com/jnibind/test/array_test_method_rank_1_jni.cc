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

using jni::Array;
using jni::ArrayView;
using jni::Class;
using jni::LocalArray;
using jni::LocalObject;
using jni::LocalString;
using jni::Method;
using jni::Params;
using jni::Return;

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
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jboolean> local_arr{boolean_array};
  fixture("booleanArray", false, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("booleanArray", false, LocalArray<jboolean>{boolean_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jboolean> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = true;
    }
  }
  fixture("booleanArray", true, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = true;
    }
  }

  // And it can be iterated over.
  for (jboolean& val : new_array.Pin(true)) {
    // Below we modify the value 2 times but with a bool there's no way,
    // so we instead just set it back.
    val = false;
  }

  fixture("booleanArray", false, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture, jbyteArray byte_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jbyte> local_arr{byte_array};
  fixture("byteArray", jbyte{0}, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("byteArray", jbyte{0}, LocalArray<jbyte>{byte_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jbyte> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = static_cast<jbyte>(i);
    }
  }
  fixture("byteArray", jbyte{0}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (jbyte& val : new_array.Pin(true)) {
    val += 5;
  }
  fixture("byteArray", jbyte{5 + 5}, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture, jcharArray char_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jchar> local_arr{char_array};
  fixture("charArray", jchar{0}, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("charArray", jchar{0}, LocalArray<jchar>{char_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jchar> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = static_cast<jchar>(i);
    }
  }
  fixture("charArray", jchar{0}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (jchar& val : new_array.Pin(true)) {
    val += 5;
  }

  fixture("charArray", jchar{5 + 5}, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture, jshortArray short_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jshort> local_arr{short_array};
  fixture("shortArray", jshort{0}, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("shortArray", jshort{0}, LocalArray<jshort>{short_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jshort> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = static_cast<jshort>(i);
    }
  }
  fixture("shortArray", jshort{0}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (jshort& val : new_array.Pin(true)) {
    val += 5;
  }

  fixture("shortArray", jshort{5 + 5}, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeIntTests(JNIEnv* env, jclass,
                                                          jobject test_fixture,
                                                          jintArray int_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jint> local_arr{int_array};
  fixture("intArray", 0, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("intArray", 0, LocalArray<jint>{int_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jint> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = i;
    }
  }
  fixture("intArray", 0, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (int& val : new_array.Pin(true)) {
    val += 5;
  }

  fixture("intArray", 5 * 2, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture, jlongArray long_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jlong> local_arr{long_array};
  fixture("longArray", jlong{0}, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("longArray", jlong{0}, LocalArray<jlong>{long_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jlong> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = static_cast<jlong>(i);
    }
  }
  fixture("longArray", jlong{0}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (jlong& val : new_array.Pin(true)) {
    val += 5;
  }

  fixture("longArray", jlong{5 * 2}, new_array);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture, jfloatArray float_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  LocalArray<jfloat> local_arr{float_array};
  fixture("floatArray", 0.f, local_arr);

  {
    ArrayView array_view{local_arr.Pin(true)};
    for (int i = 0; i < local_arr.Length(); ++i) {
      array_view.ptr()[i] += 2.5f;
    }
  }
  fixture("floatArray", 2.5f, local_arr);
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayTestMethodRank1_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture, jdoubleArray double_array) {
  LocalObject<kArrayTestMethodRank1> fixture{test_fixture};

  // Simple lvalue pass through works as expected.
  LocalArray<jdouble> local_arr{double_array};
  fixture("doubleArray", jdouble{0}, local_arr);

  // Simple rvalue pass through works as expected.
  fixture("doubleArray", jdouble{0}, LocalArray<jdouble>{double_array});

  // Building a new array, and setting all the values by hand works.
  LocalArray<jdouble> new_array{8};
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] = static_cast<jdouble>(i);
    }
  }
  fixture("doubleArray", jdouble{0}, new_array);

  // You can pull the view multiple times.
  {
    ArrayView array_view{new_array.Pin(true)};
    for (int i = 0; i < new_array.Length(); ++i) {
      array_view.ptr()[i] += 5;
    }
  }

  // And it can be iterated over.
  for (jdouble& val : new_array.Pin(true)) {
    val += 5;
  }
  fixture("doubleArray", jdouble{5 * 2}, new_array);
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
