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
using jni::Field;
using jni::LocalArray;
using jni::LocalObject;
using jni::Params;
using jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
static constexpr Class kArrayFieldTest {
    "com/jnibind/test/ArrayFieldTest",

    // Rank 1 Fields.
    Field {"booleanFieldRank1", Array{jboolean{}}},
    Field {"byteFieldRank1", Array{jbyte{}}},
    Field {"charFieldRank1", Array{jchar{}}},
    Field {"shortFieldRank1", Array{jshort{}}},
    Field {"intFieldRank1", Array{jint{}}},
    Field {"longFieldRank1", Array{jlong{}}},
    Field {"floatFieldRank1", Array{jfloat{}}},
    Field {"doubleFieldRank1", Array{jdouble{}}},
    Field {"objectFieldRank1", Array{kObjectTestHelperClass}},

    // Rank 2 Fields.
    Field {"booleanFieldRank2", Array{jboolean{}}},
    Field {"byteFieldRank2", Array{jbyte{}}},
    Field {"charFieldRank2", Array{jchar{}}},
    Field {"shortFieldRank2", Array{jshort{}}},
    Field {"intFieldRank2", Array{jint{}}},
    Field {"longFieldRank2", Array{jlong{}}},
    Field {"floatFieldRank2", Array{jfloat{}}},
    Field {"doubleFieldRank2", Array{jdouble{}}},
    Field {"objectFieldRank2", Array{kObjectTestHelperClass}},

    // Rank 3 Fields.
    Field {"booleanFieldRank3", Array{jboolean{}}},
    Field {"byteFieldRank3", Array{jbyte{}}},
    Field {"charFieldRank3", Array{jchar{}}},
    Field {"shortFieldRank3", Array{jshort{}}},
    Field {"intFieldRank3", Array{jint{}}},
    Field {"longFieldRank3", Array{jlong{}}},
    Field {"floatFieldRank3", Array{jfloat{}}},
    Field {"doubleFieldRank3", Array{jdouble{}}},
    Field {"objectFieldRank3", Array{kObjectTestHelperClass}},

};
// clang-format on

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ArrayFieldTest_jniTearDown(JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

// Rank 1 Field Tests.
JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeBooleanTests(
    JNIEnv* env, jclass, jobject test_fixture, jbooleanArray boolean_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
  rjni_test_helper["booleanFieldRank1"].Set(
      // LocalArray<jboolean>{boolean_array});
      boolean_array);
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeByteTests(
    JNIEnv* env, jclass, jobject test_fixture, jbyteArray byte_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeCharTests(
    JNIEnv* env, jclass, jobject test_fixture, jcharArray char_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeShortTests(
    JNIEnv* env, jclass, jobject test_fixture, jshortArray short_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeIntTests(
    JNIEnv* env, jclass, jobject test_fixture, jintArray int_array) {}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeLongTests(
    JNIEnv* env, jclass, jobject test_fixture, jlongArray long_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeFloatTests(
    JNIEnv* env, jclass, jobject test_fixture, jfloatArray float_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeDoubleTests(
    JNIEnv* env, jclass, jobject test_fixture, jdoubleArray double_array) {
  LocalObject<kArrayFieldTest> rjni_test_helper{test_fixture};
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ArrayFieldTest_nativeObjectTests(
    JNIEnv* env, jclass, jobject test_fixture, jobjectArray object_array) {}

}  // extern "C"
