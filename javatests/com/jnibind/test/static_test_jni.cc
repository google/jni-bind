/*
 * Copyright 2022 Google LLC
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

#include "javatests/com/jnibind/test/object_test_helper_jni.h"
#include "jni_bind.h"

using ::jni::Array;
using ::jni::Class;
using ::jni::Field;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::Static;
using ::jni::StaticRef;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

static constexpr Class kClass{
    "com/jnibind/test/StaticTestHelper",
    Static{
        Method{"voidFunc", Return{}},
        Method{"byteFunc", Return<jbyte>{}},
        Method{"charFunc", Return<jchar>{}},
        Method{"shortFunc", Return<jshort>{}},
        Method{"intFunc", Return<jint>{}},
        Method{"longFunc", Return<jlong>{}},
        Method{"floatFunc", Return<jfloat>{}},
        Method{"doubleFunc", Return<jdouble>{}},
        Method{"complexFunc", Return<jstring>{},
               Params{int{}, float{}, jstring{}, Array<jlong, 2>{}}},
        Method{"objectFunc", Return{kObjectTestHelperClass}},

        Field{"booleanField", jboolean{}},
        Field{"byteField", jbyte{}},
        Field{"charField", jchar{}},
        Field{"shortField", jshort{}},
        Field{"intField", jint{}},
        Field{"longField", jlong{}},
        Field{"floatField", jfloat{}},
        Field{"doubleField", jdouble{}},
        Field{"objectField", kObjectTestHelperClass},
    }};

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_StaticTest_jniTearDown(JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// Method Tests.
////////////////////////////////////////////////////////////////////////////////
JNIEXPORT void JNICALL Java_com_jnibind_test_StaticTest_voidMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  StaticRef<kClass>{}("voidFunc");
}

JNIEXPORT jbyte JNICALL Java_com_jnibind_test_StaticTest_byteMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("byteFunc");
}

JNIEXPORT jchar JNICALL Java_com_jnibind_test_StaticTest_charMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("charFunc");
}

JNIEXPORT jshort JNICALL Java_com_jnibind_test_StaticTest_shortMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("shortFunc");
}

JNIEXPORT jint JNICALL Java_com_jnibind_test_StaticTest_intMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("intFunc");
}

JNIEXPORT jlong JNICALL Java_com_jnibind_test_StaticTest_longMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("longFunc");
}

JNIEXPORT jfloat JNICALL Java_com_jnibind_test_StaticTest_floatMethodTestNative(
    JavaVM* pjvm, void* reserved) {
  return StaticRef<kClass>{}("floatFunc");
}

JNIEXPORT jdouble JNICALL
Java_com_jnibind_test_StaticTest_doubleMethodTestNative(JavaVM* pjvm,
                                                        void* reserved) {
  return StaticRef<kClass>{}("doubleFunc");
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_test_StaticTest_objectMethodTestNative(JavaVM* pjvm,
                                                        void* reserved) {
  return StaticRef<kClass>{}("objectFunc").Release();
}

JNIEXPORT jstring JNICALL
Java_com_jnibind_test_StaticTest_complexMethodTestNative(JavaVM* pjvm,
                                                         void* reserved, jint a,
                                                         jfloat b, jstring c,
                                                         jobjectArray d) {
  return StaticRef<kClass>{}("complexFunc", a, b, c, d).Release();
}

////////////////////////////////////////////////////////////////////////////////
// Field Tests.
////////////////////////////////////////////////////////////////////////////////
JNIEXPORT bool JNICALL Java_com_jnibind_test_StaticTest_booleanFieldTestNative(
    JavaVM* pjvm, void* reserved, bool val) {
  StaticRef<kClass>{}["booleanField"].Set(val);
  return StaticRef<kClass>{}["booleanField"].Get();
}

JNIEXPORT jbyte JNICALL Java_com_jnibind_test_StaticTest_byteFieldTestNative(
    JavaVM* pjvm, void* reserved, jbyte val) {
  StaticRef<kClass>{}["byteField"].Set(val);
  return StaticRef<kClass>{}["byteField"].Get();
}

JNIEXPORT char JNICALL Java_com_jnibind_test_StaticTest_charFieldTestNative(
    JavaVM* pjvm, void* reserved, jchar val) {
  StaticRef<kClass>{}["charField"].Set(val);
  return StaticRef<kClass>{}["charField"].Get();
}

JNIEXPORT short JNICALL Java_com_jnibind_test_StaticTest_shortFieldTestNative(
    JavaVM* pjvm, void* reserved, jshort val) {
  StaticRef<kClass>{}["shortField"].Set(val);
  return StaticRef<kClass>{}["shortField"].Get();
}

JNIEXPORT int JNICALL Java_com_jnibind_test_StaticTest_intFieldTestNative(
    JavaVM* pjvm, void* reserved, jint val) {
  StaticRef<kClass>{}["intField"].Set(val);
  return StaticRef<kClass>{}["intField"].Get();
}

JNIEXPORT long JNICALL Java_com_jnibind_test_StaticTest_longFieldTestNative(
    JavaVM* pjvm, void* reserved, jlong val) {
  StaticRef<kClass>{}["longField"].Set(val);
  return StaticRef<kClass>{}["longField"].Get();
}

JNIEXPORT float JNICALL Java_com_jnibind_test_StaticTest_floatFieldTestNative(
    JavaVM* pjvm, void* reserved, jfloat val) {
  StaticRef<kClass>{}["floatField"].Set(val);
  return StaticRef<kClass>{}["floatField"].Get();
}

JNIEXPORT double JNICALL Java_com_jnibind_test_StaticTest_doubleFieldTestNative(
    JavaVM* pjvm, void* reserved, jdouble val) {
  StaticRef<kClass>{}["doubleField"].Set(val);
  return StaticRef<kClass>{}["doubleField"].Get();
}

JNIEXPORT jobject JNICALL
Java_com_jnibind_test_StaticTest_objectFieldTestNative(JavaVM* pjvm,
                                                       void* reserved,
                                                       jobject val) {
  StaticRef<kClass>{}["objectField"].Set(val);
  return StaticRef<kClass>{}["objectField"].Get().Release();
}
}
