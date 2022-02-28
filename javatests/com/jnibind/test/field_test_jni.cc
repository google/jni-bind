// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <memory>

#include "object_test_helper_jni.h"
#include "jni_bind.h"

using jni::Class;
using jni::Constructor;
using jni::Field;
using jni::LocalObject;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
constexpr Class kFieldTestHelper {
    "com/jnibind/test/FieldTestHelper",
    Constructor{},
    Constructor{int{}, float{}, double{}},

    Field{"intField", jint{}},
    Field{"floatField", jfloat{}},
    Field{"doubleField", jdouble{}},
};

constexpr Class kFieldTest {
    "com/jnibind/test/FieldTest",
    Field{"fieldTestHelper", kFieldTestHelper},
};

// clang-format on

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_FieldTest_jniTearDown(JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT jint JNICALL Java_com_jnibind_test_FieldTest_jniIntField(
    JNIEnv* env, jclass, jobject object, jint val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["intField"].Set(jint{val});
  return rjni_test_helper["intField"].Get();
}

JNIEXPORT jfloat JNICALL Java_com_jnibind_test_FieldTest_jniFloatField(
    JNIEnv* env, jclass, jobject object, jfloat val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["floatField"].Set(jfloat{val});
  return rjni_test_helper["floatField"].Get();
}

JNIEXPORT jdouble JNICALL Java_com_jnibind_test_FieldTest_jniDoubleField(
    JNIEnv* env, jclass, jobject object, jdouble val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["doubleField"].Set(jdouble{val});
  return rjni_test_helper["doubleField"].Get();
}

JNIEXPORT void JNICALL Java_com_jnibind_test_FieldTest_jniObjectFieldSet(
    JNIEnv* env, jclass, jobject test_class, jint intVal, jfloat floatVal,
    jdouble doubleVal) {
  LocalObject<kFieldTest> field_test{test_class};
  LocalObject<kFieldTestHelper> obj{intVal, floatVal, doubleVal};

  field_test["fieldTestHelper"].Set(obj);
}

JNIEXPORT jobject JNICALL Java_com_jnibind_test_FieldTest_jniObjectFieldGet(
    JNIEnv* env, jclass, jobject test_class) {
  return LocalObject<kFieldTest>{test_class}["fieldTestHelper"].Get().Release();
}

}  // extern "C"
