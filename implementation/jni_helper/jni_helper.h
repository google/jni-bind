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

#ifndef JNI_BIND_JNI_HELPER_JNI_HELPER_H_
#define JNI_BIND_JNI_HELPER_JNI_HELPER_H_

#include "jni_env.h"
#include "jni_dep.h"
#include "metaprogramming/lambda_string.h"
#include "trace.h"

namespace jni {

// Implemented in "find_class_fallback.h".
extern jclass FindClassFallback(const char* class_name);

// Helper JNI shim for object, method, class, etc. lookup.
class JniHelper {
 public:
  // Finds a class with "name".  Note, the classloader used is whatever is
  // present on the stack when this is called.  No caching is performed,
  // returned `jclass` is a local.
  static jclass FindClass(const char* name);

  // Returns a local ref jclass for the given jobject.
  // Note, if the object is polymorphic it may be a sub or superclass.
  static jclass GetObjectClass(jobject object);

  // Gets a method for a signature (no caching is performed).
  static inline jmethodID GetMethodID(jclass clazz, const char* method_name,
                                      const char* method_signature);

  // Gets a static method for a signature (no caching is performed).
  static inline jmethodID GetStaticMethodID(jclass clazz,
                                            const char* method_name,
                                            const char* method_signature);

  // FieldIds follow the same semantics as MethodIds.
  static inline jfieldID GetFieldID(jclass clazz, const char* name,
                                    const char* signature);

  // Gets a static field for a signature (no caching is performed).
  static inline jfieldID GetStaticFieldID(jclass clazz, const char* field_name,
                                          const char* field_signature);

  // Strings.
  static const char* GetStringUTFChars(jstring str);

  static void ReleaseStringUTFChars(jstring str, const char* chars);
};

//==============================================================================

// This object shall be `null` or a global of a classloader (set by `JvmRef`).
// This is a hack for idiosyncracies with Android.
inline jobject& FallbackLoader() {
  static jobject loader{};

  return loader;
}

inline jclass JniHelper::FindClass(const char* name) {
  Trace(metaprogramming::LambdaToStr(STR("FindClass")), name);

#ifdef DRY_RUN
  return Fake<jclass>();
#else
  jclass jclass_from_thread_loader = jni::JniEnv::GetEnv()->FindClass(name);
  if (!jclass_from_thread_loader && FallbackLoader() != nullptr) {
    jni::JniEnv::GetEnv()->ExceptionClear();

    return FindClassFallback(name);
  }

  return jclass_from_thread_loader;
#endif  // DRY_RUN
}

inline jclass JniHelper::GetObjectClass(jobject object) {
  Trace(metaprogramming::LambdaToStr(STR("GetObjectClass")), object);

#ifdef DRY_RUN
  return Fake<jclass>();
#else
  return jni::JniEnv::GetEnv()->GetObjectClass(object);
#endif  // DRY_RUN
}

jmethodID JniHelper::GetMethodID(jclass clazz, const char* method_name,
                                 const char* method_signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetMethodID")), clazz, method_name,
        method_signature);

#ifdef DRY_RUN
  return Fake<jmethodID>();
#else
  return jni::JniEnv::GetEnv()->GetMethodID(clazz, method_name,
                                            method_signature);
#endif  // DRY_RUN
}

jmethodID JniHelper::GetStaticMethodID(jclass clazz, const char* method_name,
                                       const char* method_signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetStaticMethodID")), clazz,
        method_name, method_signature);

#ifdef DRY_RUN
  return Fake<jmethodID>();
#else
  return jni::JniEnv::GetEnv()->GetStaticMethodID(clazz, method_name,
                                                  method_signature);
#endif  // DRY_RUN
}

jfieldID JniHelper::GetFieldID(jclass clazz, const char* name,
                               const char* signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetFieldID")), clazz, name,
        signature);

#ifdef DRY_RUN
  return Fake<jfieldID>();
#else
  return jni::JniEnv::GetEnv()->GetFieldID(clazz, name, signature);
#endif  // DRY_RUN
}

jfieldID JniHelper::GetStaticFieldID(jclass clazz, const char* name,
                                     const char* signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetStaticFieldID")), clazz, name,
        signature);

#ifdef DRY_RUN
  return Fake<jfieldID>();
#else
  return jni::JniEnv::GetEnv()->GetStaticFieldID(clazz, name, signature);
#endif  // DRY_RUN
}

inline const char* JniHelper::GetStringUTFChars(jstring str) {
  Trace(metaprogramming::LambdaToStr(STR("GetStringUTFChars")), str);

#ifdef DRY_RUN
  return "DEAD_BEEF";
#else
  // If is_copy is an address of bool it will be set to true or false if a copy
  // is made.  That said, this seems to be of no consequence, as the API still
  // requires you to release the string at the end. There's no discernible
  // reason you would ever be able to meaningfully act differently based on
  // this parameter of the API (except to do the wrong thing).
  return jni::JniEnv::GetEnv()->GetStringUTFChars(str, /*isCopy=*/nullptr);
#endif  // DRY_RUN
}

inline void JniHelper::ReleaseStringUTFChars(jstring str, const char* chars) {
  Trace(metaprogramming::LambdaToStr(STR("ReleaseStringUTFChars")), str, chars);

#ifdef DRY_RUN
#else
  jni::JniEnv::GetEnv()->ReleaseStringUTFChars(str, chars);
#endif  // DRY_RUN
}

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_HELPER_H_
