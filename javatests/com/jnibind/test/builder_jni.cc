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

#include <memory>

#include "object_test_helper_jni.h"
#include "jni_bind.h"

#define JNI_MTHD(x, y, ...) \
  x Java_com_jnibind_test_BuilderTest_##y(JNIEnv*, jclass, ##__VA_ARGS__)

using ::jni::Class;
using ::jni::Constructor;
using ::jni::LocalObject;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::Self;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
constexpr Class kBuilder {
    "com/jnibind/test/BuilderTest$Builder",

    Constructor<>{},

    Method{"setOne", Return{Self{}}, Params<int>{}},
    Method{"setTwo", Return{Self{}}, Params<int>{}},
    Method{"setThree", Return{Self{}}, Params<int>{}},
    Method{"build", Return{kObjectTestHelperClass}},
};
// clang-format on

extern "C" {

JNI_BIND_EXPORT jint JNI_BIND_CALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));

  return JNI_VERSION_1_6;
}

JNI_MTHD(void, nativeJniTeardown) { jvm = nullptr; }

JNI_MTHD(jobject, useBuilderToCreateObject) {
  return LocalObject<kBuilder>{}("setOne", 111)("setTwo", 222)("setThree",
                                                               333)("build")
      .Release();
}

}  // extern "C"
