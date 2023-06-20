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
using ::jni::CreateCopy;
using ::jni::GlobalString;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::PromoteToGlobal;
using ::jni::Return;

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

// clang-format off
constexpr Class kMethodTestHelper {
    "com/jnibind/test/StringTestHelper",

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
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_StringTest_jniTearDown(JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_StringTest_jniPassesStringsInManyWays(
    JNIEnv* env, jclass, jobject test_fixture_object, jstring input) {
  LocalObject<kMethodTestHelper> fixture{test_fixture_object};

  GlobalString global_string_lval{CreateCopy{}, input};
  LocalString string_lval{input};

  const char* kSimpleTestString{"SimpleTestString"};
  fixture("voidMethodTakesString", "SimpleTestString");
  fixture("voidMethodTakesString", kSimpleTestString);
  fixture("voidMethodTakesString", std::string_view{"SimpleTestString"});
  fixture("voidMethodTakesString", std::string{"SimpleTestString"});
  fixture("voidMethodTakesString", input);
  fixture("voidMethodTakesString", string_lval);
  fixture("voidMethodTakesString", global_string_lval);
  fixture("voidMethodTakesString", LocalString{input});
  fixture("voidMethodTakesString", GlobalString{PromoteToGlobal{}, input});
}

/** Void return type tests. */
JNIEXPORT void JNICALL
Java_com_jnibind_test_StringTest_jniVoidMethodTakesString(JNIEnv* env, jclass,
                                                          jobject object,
                                                          jstring string) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  LocalString lValue{string};
  r_jni_string_test_helper("voidMethodTakesString", LocalString{string});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_StringTest_jniVoidMethodTakesTwoStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  r_jni_string_test_helper("voidMethodTakesTwoStrings", LocalString{s1},
                           LocalString{s2});
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_StringTest_jniVoidMethodTakesFiveStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2, jstring s3,
    jstring s4, jstring s5) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  std::string s4_string{LocalString{s4}.Pin().ToString()};

  r_jni_string_test_helper("voidMethodTakesFiveStrings",
                           LocalString{s1}.Pin().ToString(), LocalString{s2},
                           s3, s4_string, LocalString{s5}.Pin().ToString());
}

/** String return type tests. */
JNIEXPORT jstring JNICALL
Java_com_jnibind_test_StringTest_jniStringMethodTakesString(JNIEnv* env, jclass,
                                                            jobject object,
                                                            jstring string) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  return r_jni_string_test_helper("stringMethodTakesString",
                                  LocalString{string})
      .Release();
}

JNIEXPORT jstring JNICALL
Java_com_jnibind_test_StringTest_jniStringMethodTakesTwoStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  return LocalString{
      r_jni_string_test_helper("stringMethodTakesTwoStrings", LocalString{s1},
                               std::string{LocalString{s2}.Pin().ToString()})}
      .Release();
}

JNIEXPORT jstring JNICALL
Java_com_jnibind_test_StringTest_jniStringMethodTakesFiveStrings(
    JNIEnv* env, jclass, jobject object, jstring s1, jstring s2, jstring s3,
    jstring s4, jstring s5) {
  LocalObject<kMethodTestHelper> r_jni_string_test_helper{object};
  return LocalString{r_jni_string_test_helper("stringMethodTakesFiveStrings",
                                              LocalString{s1}, LocalString{s2},
                                              LocalString{s3}, LocalString{s4},
                                              LocalString{s5})}
      .Release();
}

}  // extern "C"
