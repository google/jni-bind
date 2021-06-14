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

#include "object_test_helper_jni.h"
#include "class.h"
#include "field.h"
#include "global_object.h"
#include "global_string.h"
#include "jni_dep.h"
#include "jvm_ref.h"
#include "local_object.h"
#include "local_string.h"
#include "object.h"
#include "params.h"
#include "return.h"

using jni::Class;
using jni::Field;
using jni::LocalObject;

// clang-format off
constexpr Class kFieldTestHelper {
    "com/google/jni/FieldTestHelper",

    Field{"intField", jint{}},
    Field{"floatField", jfloat{}},
    Field{"doubleField", jdouble{}},
};

// clang-format on

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  static jni::JvmRef<jni::kDefaultJvm> jvm{pjvm};
  return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL Java_com_google_jni_FieldTest_jniIntField(JNIEnv* env,
                                                                 jclass,
                                                                 jobject object,
                                                                 jint val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["intField"].Set(jint{val});
  return rjni_test_helper["intField"].Get();
}

JNIEXPORT jfloat JNICALL Java_com_google_jni_FieldTest_jniFloatField(
    JNIEnv* env, jclass, jobject object, jfloat val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["floatField"].Set(jfloat{val});
  return rjni_test_helper["floatField"].Get();
}

JNIEXPORT jdouble JNICALL Java_com_google_jni_FieldTest_jniDoubleField(
    JNIEnv* env, jclass, jobject object, jdouble val) {
  LocalObject<kFieldTestHelper> rjni_test_helper{object};
  rjni_test_helper["doubleField"].Set(jdouble{val});
  return rjni_test_helper["doubleField"].Get();
}

}  // extern "C"
