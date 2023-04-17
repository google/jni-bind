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

using ::jni::Class;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Overload;
using ::jni::Params;
using ::jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
constexpr Class kMethodTestHelper {
    "com/jnibind/test/MethodTestHelper",

    // Void Method Tests.
    Method{"voidMethod", Return<void>{}, Params<>{}},
    Method{"voidMethodTakesOneInt", Return<void>{}, Params<jint>{}},
    Method{"voidMethodTakesFiveInts", Return<void>{}, Params<jint,jint,jint,jint,jint>{}},

    // Boolean Method Tests.
    Method{"booleanMethod", Return<jboolean>{}, Params<>{}},
    Method{"booleanMethodTakesOneBoolean", Return<jboolean>{}, Params<jboolean>{}},
    Method{"booleanMethodTakesFiveBooleans", Return<jboolean>{}, Params<jboolean,jboolean,jboolean,jboolean,jboolean>{}},

    // Int Method Tests.
    Method{"intMethod", Return<jint>{}, Params<>{}},
    Method{"intMethodTakesOneInt", Return<jint>{}, Params<jint>{}},
    Method{"intMethodTakesFiveInts", Return<jint>{}, Params<jint,jint,jint,jint,jint>{}},

    // Long Method Tests.
    Method{"longMethod", Return<jlong>{}, Params<>{}},
    Method{"longMethodTakesOneLong", Return<jlong>{}, Params<jlong>{}},
    Method{"longMethodTakesFiveLongs", Return<jlong>{}, Params<jlong,jlong,jlong,jlong,jlong>{}},

    // Float Method Tests.
    Method{"floatMethod", Return<jfloat>{}, Params<>{}},
    Method{"floatMethodTakesOneFloat", Return<jfloat>{}, Params<jfloat>{}},
    Method{"floatMethodTakesFiveFloats", Return<jfloat>{}, Params<jfloat,jfloat,jfloat,jfloat,jfloat>{}},

    // Double Method Tests.
    Method{"doubleMethod", Return<jdouble>{}, Params<>{}},
    Method{"doubleMethodTakesOneDouble", Return<jdouble>{}, Params<jdouble>{}},
    Method{"doubleMethodTakesFiveDoubles", Return<jdouble>{}, Params<jdouble,jdouble,jdouble,jdouble,jdouble>{}},

    // Overloaded Method Tests.
    Method{"foo",
      Overload { Return<int>{}, Params<>{}},
      Overload { Return<int>{}, Params<jfloat>{}},
      Overload { Return<int>{}, Params<jfloat,jfloat>{}}
    },
};
// clang-format on

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_MethodTest_jniTearDown(JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

/** Void Method Tests. */
JNIEXPORT void JNICALL Java_com_jnibind_test_MethodTest_jniVoidMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  gtest_helper("voidMethod");
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_MethodTest_jniVoidMethodTakesOneInt(JNIEnv* env, jclass,
                                                          jobject object,
                                                          jint i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  gtest_helper("voidMethodTakesOneInt", jint{i});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_MethodTest_jniVoidMethodTakesFiveInts(JNIEnv* env, jclass,
                                                            jobject object,
                                                            jint i1, jint i2,
                                                            jint i3, jint i4,
                                                            jint i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  gtest_helper("voidMethodTakesFiveInts", jint{i1}, jint{i2}, jint{i3},
               jint{i4}, jint{i5});
}

/** Boolean Method Tests. */
JNIEXPORT jboolean JNICALL Java_com_jnibind_test_MethodTest_jniBooleanMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("booleanMethod");
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_MethodTest_jniBooleanMethodTakesOneBoolean(JNIEnv* env,
                                                                 jclass,
                                                                 jobject object,
                                                                 jboolean i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("booleanMethodTakesOneBoolean", jboolean{i});
}

JNIEXPORT jboolean JNICALL
Java_com_jnibind_test_MethodTest_jniBooleanMethodTakesFiveBooleans(
    JNIEnv* env, jclass, jobject object, jboolean i1, jboolean i2, jboolean i3,
    jboolean i4, jboolean i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("booleanMethodTakesFiveBooleans", jboolean{i1},
                      jboolean{i2}, jboolean{i3}, jboolean{i4}, jboolean{i5});
}

/** Int Method Tests. */
JNIEXPORT jint JNICALL Java_com_jnibind_test_MethodTest_jniIntMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("intMethod");
}

JNIEXPORT jint JNICALL Java_com_jnibind_test_MethodTest_jniIntMethodTakesOneInt(
    JNIEnv* env, jclass, jobject object, jint i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("intMethodTakesOneInt", jint{i});
}

JNIEXPORT jint JNICALL
Java_com_jnibind_test_MethodTest_jniIntMethodTakesFiveInts(JNIEnv* env, jclass,
                                                           jobject object,
                                                           jint i1, jint i2,
                                                           jint i3, jint i4,
                                                           jint i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("intMethodTakesFiveInts", jint{i1}, jint{i2}, jint{i3},
                      jint{i4}, jint{i5});
}

/** Long Method Tests. */
JNIEXPORT jlong JNICALL Java_com_jnibind_test_MethodTest_jniLongMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("longMethod");
}

JNIEXPORT jlong JNICALL
Java_com_jnibind_test_MethodTest_jniLongMethodTakesOneLong(JNIEnv* env, jclass,
                                                           jobject object,
                                                           jlong i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("longMethodTakesOneLong", jlong{i});
}

JNIEXPORT jlong JNICALL
Java_com_jnibind_test_MethodTest_jniLongMethodTakesFiveLongs(
    JNIEnv* env, jclass, jobject object, jlong i1, jlong i2, jlong i3, jlong i4,
    jlong i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("longMethodTakesFiveLongs", jlong{i1}, jlong{i2},
                      jlong{i3}, jlong{i4}, jlong{i5});
}

/** Float Method Tests. */
JNIEXPORT jfloat JNICALL Java_com_jnibind_test_MethodTest_jniFloatMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("floatMethod");
}

JNIEXPORT jfloat JNICALL
Java_com_jnibind_test_MethodTest_jniFloatMethodTakesOneFloat(JNIEnv* env,
                                                             jclass,
                                                             jobject object,
                                                             jfloat i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("floatMethodTakesOneFloat", jfloat{i});
}

JNIEXPORT jfloat JNICALL
Java_com_jnibind_test_MethodTest_jniFloatMethodTakesFiveFloats(
    JNIEnv* env, jclass, jobject object, jfloat i1, jfloat i2, jfloat i3,
    jfloat i4, jfloat i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("floatMethodTakesFiveFloats", jfloat{i1}, jfloat{i2},
                      jfloat{i3}, jfloat{i4}, jfloat{i5});
}

/** Double Method Tests. */
JNIEXPORT jdouble JNICALL Java_com_jnibind_test_MethodTest_jniDoubleMethod(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("doubleMethod");
}

JNIEXPORT jdouble JNICALL
Java_com_jnibind_test_MethodTest_jniDoubleMethodTakesOneDouble(JNIEnv* env,
                                                               jclass,
                                                               jobject object,
                                                               jdouble i) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("doubleMethodTakesOneDouble", jdouble{i});
}

JNIEXPORT jdouble JNICALL
Java_com_jnibind_test_MethodTest_jniDoubleMethodTakesFiveDoubles(
    JNIEnv* env, jclass, jobject object, jdouble i1, jdouble i2, jdouble i3,
    jdouble i4, jdouble i5) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("doubleMethodTakesFiveDoubles", jdouble{i1}, jdouble{i2},
                      jdouble{i3}, jdouble{i4}, jdouble{i5});
}

/** Overload Method Tests. */
JNIEXPORT int JNICALL Java_com_jnibind_test_MethodTest_jniCallFooOverload1(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("foo");
}

JNIEXPORT int JNICALL Java_com_jnibind_test_MethodTest_jniCallFooOverload2(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("foo", 123.f);
}

JNIEXPORT int JNICALL Java_com_jnibind_test_MethodTest_jniCallFooOverload3(
    JNIEnv* env, jclass, jobject object) {
  LocalObject<kMethodTestHelper> gtest_helper{object};
  return gtest_helper("foo", 123.f, 456.f);
}

}  // extern "C"
