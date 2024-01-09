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

#include "object_test_helper_jni.h"
#include "jni_bind.h"

using ::jni::Class;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;

// clang-format off
constexpr Class kMethodTestHelper {
    "com/jnibind/android/StringTestHelper",

    // Void return type tests.
    Method{"voidMethodTakesString", Return<void>{}, Params<jstring>{}},
    Method{"voidMethodTakesTwoStrings", Return<void>{}, Params<jstring, jstring>{}},
    Method{"voidMethodTakesFiveStrings", Return<void>{},
      Params<jstring, jstring, jstring, jstring, jstring>{}},

    // String return type tests.
    Method{"stringMethodTakesString", Return<jstring>{}, Params<jstring>{}},
    Method{"stringMethodTakesTwoStrings", Return<jstring>{}, Params<jstring, jstring>{}},
    Method{"stringMethodTakesFiveStrings", Return<jstring>{},
      Params<jstring, jstring, jstring, jstring, jstring>{}},
};
// clang-format on

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  static jni::JvmRef<jni::kDefaultJvm> jvm{pjvm};
  return JNI_VERSION_1_6;
}

/** Void return type tests. */
JNIEXPORT void JNICALL
Java_com_jnibind_android_StringTest_jniVoidMethodTakesString(JNIEnv* env, jclass,
                                                          jobject object,
                                                          jstring string) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  // TODO(b/175083373):  The following (and below) should compile with overload
  // sets.
  /*
  // rjni_test_helper("voidMethodTakesString",rjni_test_helper);
  r_jni_string_test_helper.GetMethod("voidMethodTakesString",
      std::string{LocalString{string}.Pin().ToString()});
  */
  r_jni_string_test_helper("voidMethodTakesString",
                           std::string{LocalString{string}.Pin().ToString()});
}

JNIEXPORT void JNICALL
Java_com_jnibind_android_StringTest_jniVoidMethodTakesTwoStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  r_jni_string_test_helper("voidMethodTakesTwoStrings",
                           std::string{LocalString{s1}.Pin().ToString()},
                           std::string{LocalString{s2}.Pin().ToString()});
}

JNIEXPORT void JNICALL
Java_com_jnibind_android_StringTest_jniVoidMethodTakesFiveStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2, jstring s3,
    jstring s4, jstring s5) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  r_jni_string_test_helper("voidMethodTakesFiveStrings",
                           std::string{LocalString{s1}.Pin().ToString()},
                           std::string{LocalString{s2}.Pin().ToString()},
                           std::string{LocalString{s3}.Pin().ToString()},
                           std::string{LocalString{s4}.Pin().ToString()},
                           std::string{LocalString{s5}.Pin().ToString()});
}

/** String return type tests. */
JNIEXPORT jstring JNICALL
Java_com_jnibind_android_StringTest_jniStringMethodTakesString(JNIEnv* env, jclass,
                                                            jobject object,
                                                            jstring string) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};

  // TODO(b/174272629):  This declaration is clumsy because Return<std::string> is
  // an ObjectRef and thus not implicitly convertible to jstring.  The following
  // needs to be simpler to express.
  return LocalString{r_jni_string_test_helper(
                         "stringMethodTakesString",
                         std::string{LocalString{string}.Pin().ToString()})}
      .Release();
}

JNIEXPORT jstring JNICALL
Java_com_jnibind_android_StringTest_jniStringMethodTakesTwoStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  return LocalString{
      r_jni_string_test_helper("stringMethodTakesTwoStrings",
                               std::string{LocalString{s1}.Pin().ToString()},
                               std::string{LocalString{s2}.Pin().ToString()})}
      .Release();
}

JNIEXPORT jstring JNICALL
Java_com_jnibind_android_StringTest_jniStringMethodTakesFiveStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2, jstring s3,
    jstring s4, jstring s5) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  return LocalString{
      r_jni_string_test_helper("stringMethodTakesFiveStrings",
                               std::string{LocalString{s1}.Pin().ToString()},
                               std::string{LocalString{s2}.Pin().ToString()},
                               std::string{LocalString{s3}.Pin().ToString()},
                               std::string{LocalString{s4}.Pin().ToString()},
                               std::string{LocalString{s5}.Pin().ToString()})}
      .Release();
}

}  // extern "C"
